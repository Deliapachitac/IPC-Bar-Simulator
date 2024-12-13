#include "segment.h"

int main(int argc, char *argv[]) {
    
   int customer_id = atoi(argv[1]);

    // Open shared memory
    int shm_fd = shm_open(MEMORY_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Map shared memory
    SharedMemoryStruct *sharedState = (SharedMemoryStruct *)mmap(
        NULL, sizeof(SharedMemoryStruct), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (sharedState == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    srand(time(0) + customer_id);

    while (true) {
        int dequeued_customer;
        sem_wait(&sharedState->customerQueue.buffer_access); // Synchronize access to the buffer

        if (!dequeue(&sharedState->customerQueue, &dequeued_customer)) {
            sem_post(&sharedState->customerQueue.buffer_access); // Release semaphore
            usleep(1000); // Wait before checking again
            continue;
        }

        sem_post(&sharedState->customerQueue.buffer_access); // Release semaphore

        if (dequeued_customer != customer_id) {
            continue; // Not the correct customer; loop again
        }

        printf("Customer %d: Looking for a table...\n", customer_id);
        usleep(rand() % (3 * 1000));

        bool found_table = false;

        for (int i = 0; i < NUM_TABLES; i++) {
            // Check table availability
            if (!sharedState->table[i].full) {
                // Mark table as full and take a seat
                sharedState->table[i].full = true;
                sharedState->table[i].chairs[sharedState->table[i].full_chairs++] = customer_id;

                printf("Customer %d: Sat at Table %d\n", customer_id, i + 1);

                // Simulate dining time
                usleep(rand() % (3 * 1000));

                // Leave the table
                sharedState->table[i].full_chairs--;
                sharedState->table[i].chairs[sharedState->table[i].full_chairs] = 0;

                if (sharedState->table[i].full_chairs == 0) {
                    sharedState->table[i].full = false; // Mark table as available
                }

                printf("Customer %d: Left Table %d\n", customer_id, i + 1);
                found_table = true;
                break;
            }
        }

        if (!found_table) {
            printf("Customer %d: Could not find a table, leaving...\n", customer_id);
        }

        break;
    }

    // Clean up
    if (munmap(sharedState, sizeof(SharedMemoryStruct)) == -1) {
        perror("munmap");
    }

    return 0;
}
