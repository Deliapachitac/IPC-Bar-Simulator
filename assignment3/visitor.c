#include "segment.h"

void log_event(int table_num, SharedMemoryStruct *sharedMemory) {

    // Semaphore for mutual exclusion of the log file
    sem_wait(&sharedMemory->logging);

    // Open the log file in append mode
    int log_fd = open("bar_log.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (log_fd == -1) {
        perror("Error opening log file");
        exit(1);
    }
    char log_message[512];
    char log_occupied[256];
    
    // Write the table status to the log file
    // Find out if the table is occupied or not
    if(sharedMemory->table[table_num].full){
        snprintf(log_occupied, sizeof(log_occupied), "occupied");
    }else{  
        snprintf(log_occupied, sizeof(log_occupied), "empty");
    }
    snprintf(log_message, sizeof(log_message), "The table %d  is %s\n", table_num,log_occupied);
    write(log_fd, log_message, strlen(log_message));

    //Write how many chairs are full
    snprintf(log_message, sizeof(log_message), "The table %d has %d full chairs\n", table_num, sharedMemory->table[table_num].full_chairs);
    write(log_fd, log_message, strlen(log_message));

    // Write the status of each chair
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
   
    // insert the visitor into the waiting buffer which represents the queue of the bar
    int dequeued_customer;
    enqueueWaiting(sharedState, getpid());
    dequeueWaiting(sharedState, &dequeued_customer);
    
    // Start the waiting time and staying time
    struct tms start_waiting_time, end_waiting_time, start_staying_time, end_staying_time;
    clock_t start_wait, end_wait, start_stay, end_stay;
    double ticks_per_sec_wait = (double)sysconf(_SC_CLK_TCK);
    start_wait = (double) times(&start_waiting_time);
    start_stay = (double) times(&start_staying_time);
    
    bool seated = false; // Flag to indicate if the customer has been seated
    sem_wait(&sharedState->table_mutex); // Lock the table for modifications      
    while(!seated){
        for (int i = 0; i < NUM_TABLES && !seated; i++) {
            for (int j = 0; j < NUM_CHAIRS; j++) {

                if (sharedState->table[i].chairs[j] == 0) {
                    
                    sem_post(&sharedState->receptionist_access); // Notify the receptionist that the visitor has found an open seat
                    int slot = enqueueOrder(sharedState, dequeued_customer); // Enqueue the visitor to the receptionist queue
                    sem_wait(&sharedState->receprionist_buffer.order_ready[slot]); // Wait for the receptionist to process the order
                    
                    // Seat the customer to the table
                    sharedState->table[i].chairs[j] = dequeued_customer;
                    sharedState->table[i].full_chairs++;
                    seated = true;

                    printf("Customer %d seated at Table %d, Chair %d.\n", dequeued_customer, i, j);
                    
                    // end the waiting time
                    end_wait = (double) times(&end_waiting_time);
                

                    // If the table is full, mark it as full
                    if (sharedState->table[i].full_chairs == NUM_CHAIRS) {
                        sharedState->table[i].full = true;
                    }

                    // Print the table status to the log file
                    log_event(i, sharedState);
                    break;

                }

            }
        } 
        // If the visitor didnt found a table they should wait until a table is emptyied  
        if (!seated) {
            sem_wait(&sharedState->table_reset);
        }
    }
    sem_post(&sharedState->table_mutex); // Unlock the table for modifications
    
    
    
    // Simulate dining time for a random duration between [0.7 * resttime, resttime]
    srand(time(NULL) ^ getpid());
    int min_dining_time = (int)(0.7 * resttime);
    int random_dining_time = min_dining_time + rand() % (resttime - min_dining_time + 1);
    sleep(random_dining_time);
    
    // Customer leaves the table
    for (int i = 0; i < NUM_TABLES; i++) {
        for (int j = 0; j < NUM_CHAIRS; j++) {
            
            if (sharedState->table[i].chairs[j] == dequeued_customer) {
                
                // Mark chair as empty (-1)
                sharedState->table[i].chairs[j] = -1;
                sharedState->table[i].full_chairs--;
                printf("Customer %d left Table %d, Chair %d\n", dequeued_customer, i, j);

                // Check if all chairs are marked as empty (-1)
                bool all_chairs_empty = true;
                for (int k = 0; k < NUM_CHAIRS; k++) {
                    if (sharedState->table[i].chairs[k] != -1) {
                        all_chairs_empty = false;
                        break;
                    }
                }

                // end the staying time
                end_stay = (double) times(&end_staying_time);
            
                
                // Reset the table if all chairs are empty (-1)
                if (all_chairs_empty) {
                    for (int k = 0; k < NUM_CHAIRS; k++) {
                        sharedState->table[i].chairs[k] = 0; // Reset chairs to 0
                    }
                    sharedState->table[i].full = false;    // Mark table as not full
                    sem_post(&sharedState->table_reset);  // Signal that a table has been reset to allow waiting customers to be seated
                }
                
                // Print the table status to the log file
                log_event(i, sharedState);
                break;
            }
            
        }
    }

    sem_wait(&sharedState->mutex_access);
    sharedState->statistics.total_staying_time += (double)(end_stay - start_stay) / ticks_per_sec_wait;
    sharedState->statistics.avarage_staying_time = sharedState->statistics.total_staying_time / sharedState->statistics.total_visitors;
    sharedState->statistics.total_waiting_time += (double)(end_wait - start_wait) / ticks_per_sec_wait;
    sharedState->statistics.avarage_waiting_time = sharedState->statistics.total_waiting_time / sharedState->statistics.total_visitors;
    sem_post(&sharedState->mutex_access);

    // Clean up
    if (munmap(sharedState, sizeof(SharedMemoryStruct)) == -1) {
        perror("munmap");
    }

    return 0;
}
