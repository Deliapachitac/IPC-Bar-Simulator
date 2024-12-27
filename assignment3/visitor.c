#include "segment.h"

void log_event(int table_num, SharedMemoryStruct *sharedMemory) {

    sem_wait(&sharedMemory->logging);

    // Open the log file in append mode
    int log_fd = open("bar_log.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (log_fd == -1) {
        perror("Error opening log file");
        exit(1);
    }

    // Write the log message to the file

    char log_message[512];
    char log_occupied[256];

    if(sharedMemory->table[table_num].full){
        snprintf(log_occupied, sizeof(log_occupied), "occupied");
    }else{  
        snprintf(log_occupied, sizeof(log_occupied), "empty");
    }
    snprintf(log_message, sizeof(log_message), "The table %d  is %s\n", table_num,log_occupied);
    write(log_fd, log_message, strlen(log_message));

    snprintf(log_message, sizeof(log_message), "The table %d has %d full chairs\n", table_num, sharedMemory->table[table_num].full_chairs);
    write(log_fd, log_message, strlen(log_message));

    for (int i = 0; i < NUM_CHAIRS; i++) {
        if (sharedMemory->table[table_num].chairs[i] != 0) {
            snprintf(log_message, sizeof(log_message), "    Chair %d is occupied by customer %d\n", i, sharedMemory->table[table_num].chairs[i]);
            write(log_fd, log_message, strlen(log_message));
        } else {
            snprintf(log_message, sizeof(log_message), "    Chair %d is empty\n", i);
            write(log_fd, log_message, strlen(log_message));
        }
    }

    write(log_fd, "\n", strlen("\n"));
    write(log_fd, "\n", strlen("\n"));


    // Close the log file
    close(log_fd);

    sem_post(&sharedMemory->logging);
}

int main(int argc, char *argv[]) {
    
    // Reading and saving the variables from the command line
    int resttime ;
    char shmname[50];
    if(argc!=5){
        printf("Incorrect input command line\n");
        exit(0);
    }  
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            i++;
            resttime = atoi(argv[i]);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            i++;
            strcpy(shmname ,argv[i]);
        } else {
            printf("Unknown flag : %s\n", argv[i]);
            exit(0);
        }
    }

    // Open shared memory
    int shm_fd = shm_open(shmname, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Map shared memory
    SharedMemoryStruct *sharedState = (SharedMemoryStruct *)mmap(NULL, sizeof(SharedMemoryStruct), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (sharedState == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    
  

    //if the monitor wants to use the shared memory it should wait until the visitor is done
    // sem_wait(&sharedState->mutex_access);
    
    int dequeued_customer;

    // Enqueue the visitor into the queue and notify the receptionist
    enqueue(sharedState, getpid());
    dequeue(sharedState, &dequeued_customer);

   

    bool seated = false;
    int i ;

    
    printf("Customer %d: Looking for a table...\n", dequeued_customer);
    
    sem_wait(&sharedState->table_mutex); // Lock the table for modifications      
    while(!seated){
        for (i = 0; i < NUM_TABLES && !seated; i++) {
            for (int j = 0; j < NUM_CHAIRS; j++) {
                
                
                if (sharedState->table[i].chairs[j] == 0) {
                    // Seat customer
                    sem_post(&sharedState->receptionist_access); // Notify the receptionist that the visitor has been seated
                    int slot = enqueueOrder(sharedState, dequeued_customer);

                    sem_wait(&sharedState->receprionist_buffer.order_ready[slot]); // Wait for the orde
                    
                    // sem_wait(&sharedState->table[i].chair_sem[j]);
                    
                    sharedState->table[i].chairs[j] = dequeued_customer;
                    sharedState->table[i].full_chairs++;
                    printf("Customer %d seated at Table %d, Chair %d.\n", dequeued_customer, i, j);
                    
                    seated = true;

                    if (sharedState->table[i].full_chairs == NUM_CHAIRS) {
                        sharedState->table[i].full = true;
                        // printf("Table %d is now full.\n", i);
                    }
                    log_event(i, sharedState);

                    break;

                }

            }
        }
        if (!seated) {
            printf("Customer %d could not find a seat. Waiting for a table reset...\n", dequeued_customer);
            sem_wait(&sharedState->table_reset);
        }
    }
    sem_post(&sharedState->table_mutex); // Unlock the table
            
    
    
    
    
    // Χρόνος διαμονής στο τραπέζι
    srand(time(NULL) ^ getpid());
    int min_dining_time = (int)(0.7 * resttime);
    int random_dining_time = min_dining_time + rand() % (resttime - min_dining_time + 1);
    sleep(random_dining_time);
    
    
    // sem_wait(&sharedState->table_mutex); // Lock the table for modifications         
    // Customer leaves
    for (int i = 0; i < NUM_TABLES; i++) {
        for (int j = 0; j < NUM_CHAIRS; j++) {
            
            
            if (sharedState->table[i].chairs[j] == dequeued_customer) {
                // Mark chair as empty (-1)
                sharedState->table[i].chairs[j] = -1;
                sharedState->table[i].full_chairs--;
                printf("Customer %d left Table %d, Chair %d\n", dequeued_customer, i, j);


                bool all_chairs_empty = true;
                for (int k = 0; k < NUM_CHAIRS; k++) {
                    if (sharedState->table[i].chairs[k] != -1) {
                        all_chairs_empty = false;
                        break;
                    }
                }

                // Reset table if all chairs are empty
                if (all_chairs_empty) {
                    for (int k = 0; k < NUM_CHAIRS; k++) {
                        sharedState->table[i].chairs[k] = 0; // Reset chairs to 0
                    }
                    sharedState->table[i].full = false;    // Mark table as not full
                    printf("Table %d has been reset.\n", i);
                    sem_post(&sharedState->table_reset);  // Signal that a table has been reset
                }
                
                // sem_post(&sharedState->table[i].chair_sem[j]);
                log_event(i, sharedState);
                break;
            }
            
        }
    }
    // sem_post(&sharedState->table_mutex); // Unlock the table

    // Clean up
    if (munmap(sharedState, sizeof(SharedMemoryStruct)) == -1) {
        perror("munmap");
    }

    return 0;
}
