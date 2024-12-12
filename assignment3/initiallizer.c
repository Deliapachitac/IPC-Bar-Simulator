#include "segment.h"



int main(int argc, char *argv[]) {
    srand(time(0));

    // Create POSIX shared memory
    int shm_fd = shm_open(MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Set the size of shared memory
    if (ftruncate(shm_fd, sizeof(SharedMemoryStruct)) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    // Map shared memory
    SharedMemoryStruct *sharedState = (SharedMemoryStruct *)mmap(NULL, sizeof(SharedMemoryStruct), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (sharedState == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Initialize table availability and circular buffer
    for (int i = 0; i < NUM_TABLES; i++) {
        sharedState->available[i] = NUM_CHAIRS;
    }
    initBuffer(&sharedState->customerQueue);

    int num_customers = 20; // Number of customers to simulate

    for (int i = 0; i < num_customers; i++) {
        pid_t pid = fork();
        if (pid == 0) {

            if (!enqueue(&sharedState->customerQueue, i)) {
                printf("Customer %d: Could not join the queue, leaving...\n", i);
                exit(0);
            }

            // Child process: Execute visitor.c
            char customer_id[10];
            sprintf(customer_id, "%d", i);

            char *args[] = {"./visitor", customer_id, NULL};
            execvp(args[0], args);

            // If execvp fails
            perror("execvp");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        usleep(rand() % (2 * 1000)); // Simulate staggered customer arrivals
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_customers; i++) {
        wait(NULL);
    }

    // Clean up shared memory
    if (munmap(sharedState, sizeof(SharedMemoryStruct)) == -1) {
        perror("munmap");
    }
    if (shm_unlink(MEMORY_NAME) == -1) {
        perror("shm_unlink");
    }

    return 0;
}
