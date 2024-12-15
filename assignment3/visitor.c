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
    
    if (!enqueue(&sharedState->customerQueue,sharedState, getpid()%2078800)) {
        printf("Customer %d: Could not join the queue, leaving...\n", getpid()%2077800);
        sem_post(&sharedState->buffer_access); // Release semaphore
        exit(1);
    }

    while (true) {
        int dequeued_customer;
        // sem_wait(&sharedState->customerQueue.buffer_access); // Synchronize access to the buffer

        if (!dequeue(&sharedState->customerQueue,sharedState, &dequeued_customer)) {
            sem_post(&sharedState->buffer_access); // Release semaphore
            // usleep(1000); // Wait before checking again
            continue;
        }
        srand(time(0) + dequeued_customer);
        // sem_post(&sharedState->customerQueue.buffer_access); // Release semaphore

        
        printf("Customer %d: Looking for a table...\n", dequeued_customer);
        usleep(rand() % (3 * 1000));

        bool found_table = false;

        for (int i = 0; i < NUM_TABLES; i++) {
            // Check table availability
            if (!sharedState->table[i].full) {
                // Mark table as full and take a seat
                sharedState->table[i].full = true;
                sharedState->table[i].chairs[sharedState->table[i].full_chairs++] = dequeued_customer;

                printf("Customer %d: Sat at Table %d\n", dequeued_customer, i + 1);

                // Simulate dining time
                usleep(rand() % (3 * 1000));

                // Leave the table
                sharedState->table[i].full_chairs--;
                sharedState->table[i].chairs[sharedState->table[i].full_chairs] = 0;

                if (sharedState->table[i].full_chairs == 0) {
                    sharedState->table[i].full = false; // Mark table as available
                }

                printf("Customer %d: Left Table %d\n", dequeued_customer, i + 1);
                found_table = true;
                break;
            }
        }

        if (!found_table) {
            printf("Customer %d: Could not find a table, leaving...\n", dequeued_customer);
        }

        break;
    }

    // Clean up
    if (munmap(sharedState, sizeof(SharedMemoryStruct)) == -1) {
        perror("munmap");
    }

    return 0;
}
