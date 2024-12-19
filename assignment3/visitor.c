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
    
    char log_message[256];

    // 
    enqueue(sharedState, getpid());
    int dequeued_customer;
    dequeue(sharedState, &dequeued_customer);

    
    printf( "Customer %d: Looking for a table...\n", dequeued_customer);
    // log_event(log_message);

    bool table_found = false;
    //if the monitor wants to use the shared memory it should wait until the visitor is done
    
    for (int i = 0; i < NUM_TABLES; i++) {

        if (!sharedState->table[i].full) {

            // sem_wait(&sharedState->mutex_access);
            // Try to find an empty chair at the table
            int j;
            for ( j = 0; j < NUM_CHAIRS; j++)
            {
                if (sharedState->table[i].chairs[j] == 0)
                {
                    // sem_wait(&sharedState->mutex_access);
                    Order order = generateRandomOrder();
                    sharedState->statistics.counter_water += order.water;
                    sharedState->statistics.counter_wine += order.wine;
                    sharedState->statistics.counter_cheese += order.cheese;
                    sharedState->statistics.counter_salad += order.salad;
                    sharedState->statistics.total_visitors++;

                    
                    // enqueueOrder(sharedState, dequeued_customer);
                    // sem_wait(&sharedState->receptionist_access);
                    
                    // printf("Customer %d: Order placed\n", dequeued_customer);

                    sharedState->table[i].chairs[j] = dequeued_customer;
                    
                    log_event(i, sharedState);
                    // sem_post(&sharedState->mutex_access);
                    break;
                }
            }

            // sem_wait(&sharedState->mutex_access);
            sharedState->table[i].full_chairs++;
            table_found = true;

            // Mark the table as full if all chairs are occupied
            if(sharedState->table[i].full_chairs == NUM_CHAIRS){
                sharedState->table[i].full = true;
            }
            
           
            // sem_post(&sharedState->mutex_access);

            // Seed the random number generator
            srand(time(NULL)^ getpid());
            // Simulate dining time for a random duration between [0.70 * resttime , resttime]
            int min_dining_time = (int)(0.7 * resttime);
            int random_dining_time = min_dining_time + rand() % (resttime - min_dining_time + 1);
            // printf("Customer %d is dining for %d seconds\n", dequeued_customer, random_dining_time);    
            sleep(random_dining_time);

            // sem_wait(&sharedState->mutex_access);
            // Customer leaves the table
            if (sharedState->table[i].full_chairs > 0)
            {
                sharedState->table[i].full_chairs--; 
            }
            sharedState->table[i].chairs[j] = 0;
            // If all chairs are empty, mark the table as not full
            if (sharedState->table[i].full_chairs == 0 ) {
                sharedState->table[i].full = false;
                sem_post(&sharedState->total_table_sem);
            }
            // Log customer leaving the table
            snprintf(log_message, sizeof(log_message), "Customer %d left Table %d, Chair %d", dequeued_customer, i, j);
            log_event(i, sharedState);
            // sem_post(&sharedState->mutex_access);
            break;

        }else if (sharedState->table[i].full && i==NUM_TABLES-1)
        {
            // printf("Customer %d: No tables available. Waiting...\n", dequeued_customer);
            sem_wait(&sharedState->total_table_sem);
            i = -1;
        }
        
        if (table_found) {
            
            break;
        }
    }
    // //monitor access to the shared memory
    // // sem_post(&sharedState->mutex_access);

   
    // Clean up
    if (munmap(sharedState, sizeof(SharedMemoryStruct)) == -1) {
        perror("munmap");
    }

    return 0;
}
