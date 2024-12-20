#include "segment.h"

Order generateRandomOrder() {
    Order order;
    order.water = 0;
    order.wine = 0;
    order.cheese = 0;
    order.salad = 0;

    // Initialize random number generator
    srand(time(NULL)^getpid());

    // Randomly generate order
    // Randomly choose water or wine (or both)
    if (rand() % 2) {
        order.water = 1;
    }
    if (rand() % 2) {
        order.wine = 1;
    }
    // Ensure at least one drink is chosen
    if (order.water == 0 && order.wine == 0) {
        if (rand() % 2) {
            order.water = 1;
        } else {
            order.wine = 1;
        }
    }

    // Randomly choose cheese (optional)
    if (rand() % 2) {
        order.cheese = 1;
    }

    // Randomly choose salad (optional)
    if (rand() % 2) {
        order.salad = 1;
    }

    return order;
}


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
    
    // Variable that saves the id of the customer that is dequeued from the waiting buffer
    int dequeued_customer;

    //if the monitor wants to use the shared memory it should wait until the visitor is done
    // sem_wait(&sharedState->mutex_access);
    
    enqueue(sharedState, getpid());
    dequeue(sharedState, &dequeued_customer);

    // notify the monitor that the he can access the shared memory
    // sem_post(&sharedState->mutex_access);
    


    printf( "Customer %d: Looking for a table...\n", dequeued_customer);



   bool table_found = false;

    for (int i = 0; i < NUM_TABLES; i++) {
         
        // Ψάξε για τραπέζι με τουλάχιστον μία θέση μαρκαρισμένη με 0
        bool has_free_chair = false;
        for (int j = 0; j < NUM_CHAIRS; j++) {
            if (sharedState->table[i].chairs[j] == 0) {
                has_free_chair = true;
                break;
            }
        }

        if (has_free_chair) {
            // Δες ποια καρέκλα είναι ελεύθερη και κάτσε εκεί
            int j;
            sem_wait(&sharedState->mutex_access); // Lock shared memory access before updating chairs

            for (j = 0; j < NUM_CHAIRS; j++) {
                
                if (sharedState->table[i].chairs[j] == 0) {
                    
                    // Κράτα θέση για τον πελάτη
                    enqueueOrder(sharedState, dequeued_customer);
                    sem_wait(&sharedState->receptionist_access);

                    
                    sharedState->table[i].chairs[j] = dequeued_customer;

                                      
                    break;
                }
                
            }
             
            log_event(i, sharedState);

            sharedState->table[i].full_chairs++;
            table_found = true;

            // Αν γεμίσουν όλες οι καρέκλες, μαρκάρισε το τραπέζι ως γεμάτο
            if (sharedState->table[i].full_chairs == NUM_CHAIRS) {
                sharedState->table[i].full = true;
            }
            sem_post(&sharedState->mutex_access); // Unlock after updatingprintf("Customer %d sat at table %d in chair %d\n", dequeued_customer,i,j);
                    
            // Χρόνος διαμονής στο τραπέζι
            srand(time(NULL) ^ getpid());
            int min_dining_time = (int)(0.7 * resttime);
            int random_dining_time = min_dining_time + rand() % (resttime - min_dining_time + 1);
            sleep(random_dining_time);

            sem_wait(&sharedState->mutex_access);
            if (sharedState->table[i].full_chairs > 0) {
                sharedState->table[i].full_chairs--;
            }
            sharedState->table[i].chairs[j] = -1;

            // Αν φύγουν όλοι οι πελάτες, άδειασε όλες τις θέσεις
            if (sharedState->table[i].full_chairs == 0) {
                for (int k = 0; k < NUM_CHAIRS; k++) {
                    sharedState->table[i].chairs[k] = 0;
                }
                sharedState->table[i].full = false;
                sem_post(&sharedState->total_table_sem);
            }

            printf("Customer %d left Table %d, Chair %d\n", dequeued_customer, i, j);
            sem_post(&sharedState->mutex_access);
            break;

        } else if (sharedState->table[i].full && i == NUM_TABLES - 1) {
            sem_wait(&sharedState->total_table_sem);
            i = -1;
        }

        if (table_found) {
            printf("FOUND\n");
            break;
        }
    }
    

    // Clean up
    if (munmap(sharedState, sizeof(SharedMemoryStruct)) == -1) {
        perror("munmap");
    }

    return 0;
}
