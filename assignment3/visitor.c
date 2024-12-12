#include "segment.h"

int main(int argc, char *argv[]) {
    
    // Open shared memory
    int shm_fd = shm_open(MEMORY_NAME, O_RDWR, 0666);
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


    int customer_id;
    while (true) {
        if (!dequeue(&sharedState->customerQueue, &customer_id)) {
            usleep(1000); // Wait before checking again
            continue;
        }

        srand(time(0) + customer_id);
        printf("Customer %d: Looking for a table...\n", customer_id);
        usleep(rand() % (3 * 1000));

        for (int i = 0; i < NUM_TABLES; i++) {
            if (sharedState->available[i] > 0) {
                sharedState->available[i]--;
                printf("Customer %d: Sat at Table %d\n", customer_id, i + 1);

                // Simulate dining time
                usleep(rand() % (3 * 1000));

                // Leave table
                sharedState->available[i]++;
                printf("Customer %d: Left Table %d\n", customer_id, i + 1);
                return 0;
            }
        }
        printf("Customer %d: Could not find a table, leaving...\n", customer_id);
    }


    // Clean up
    if (munmap(sharedState, sizeof(SharedMemoryStruct)) == -1) {
        perror("munmap");
    }

    return 0;
}
