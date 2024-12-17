#include "segment.h"

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
    
    // Simulate customer behavior
    sem_wait(&sharedState->empty_buffer); // Wait for access to the buffer
    sem_wait(&sharedState->mutex_buffer); // Synchronize access to the buffer
    enqueue(&sharedState->customerQueue, getpid());
    sem_post(&sharedState->mutex_buffer); // Release access to the buffer
    sem_post(&sharedState->full_buffer); // Signal that the buffer is full
    
    
    int dequeued_customer;
    sem_wait(&sharedState->full_buffer); // Synchronize access to the buffer
    sem_wait(&sharedState->mutex_buffer); // Wait for access to the buffer
    dequeue(&sharedState->customerQueue, &dequeued_customer);
    sharedState->statistics.total_visitors++;
    sem_post(&sharedState->mutex_buffer); // Release access to the buffer
    sem_post(&sharedState->empty_buffer); // Release access to the buffer

    printf("Customer %d: Looking for a table...\n", dequeued_customer);
    bool table_found = false;

    for (int i = 0; i < NUM_TABLES; i++) {


        if (!sharedState->table[i].full) {

            int j;
            for ( j = 0; j < NUM_CHAIRS; j++)
            {
                if (sharedState->table[i].chairs[j] == 0)
                {
                    sharedState->table[i].chairs[j] = dequeued_customer;
                    printf("Customer %d sat at Table %d at chair %d\n", dequeued_customer, i,j);
                    break;
                }
            }

            sharedState->table[i].full_chairs++;
            table_found = true;

            if(sharedState->table[i].full_chairs == NUM_CHAIRS){
                sharedState->table[i].full = true;
            }
           

            // Seed the random number generator
            srand(time(NULL));
            // Simulate dining time for a random duration between 0.70 * resttime and resttime
            int min_dining_time = (int)(0.70 * resttime);
            int random_dining_time = min_dining_time + rand() % (resttime - min_dining_time + 1);
            sleep(random_dining_time);


            // Customer leaves the table
            if (sharedState->table[i].full_chairs > 0)
            {
                sharedState->table[i].full_chairs--; 
            }
            sharedState->table[i].full = true;
            sharedState->table[i].chairs[j] = 0;
            
            printf("Customer %d left Table %d from chair %d\n", dequeued_customer, i ,j);


        }
        if (table_found) {
            break;
        }
    }
    

   
    // Clean up
    if (munmap(sharedState, sizeof(SharedMemoryStruct)) == -1) {
        perror("munmap");
    }

    return 0;
}
