#include "segment.h"

int main(int argc, char *argv[]) {
    srand(time(0));

    // Create a POSIX shared memory
    int shm_fd = shm_open(MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(0);
    }

    // Set the size of shared memory based on the struct in the segment.h
    if (ftruncate(shm_fd, sizeof(SharedMemoryStruct)) == -1) {
        perror("ftruncate");
        exit(0);
    }

    // Map the shared memory
    SharedMemoryStruct *sharedState = (SharedMemoryStruct *)mmap(NULL, sizeof(SharedMemoryStruct), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (sharedState == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    // Initialize shared memory structures
    for (int i = 0; i < NUM_TABLES; i++) {
        sharedState->table[i].full = false;
        sharedState->table[i].full_chairs = 0;
        for (int j = 0; j < NUM_CHAIRS; j++) {
            sharedState->table[i].chairs[j] = 0; // Empty chairs
        }
    }
    initBuffer(&sharedState->customerQueue);


    //
    int num_customers = 20; 
    for (int i = 0; i < num_customers; i++) {
        pid_t pid = fork();
        if (pid == 0) {


            // Child process
            sem_wait(&sharedState->customerQueue.buffer_access); // Wait for buffer access

            if (!enqueue(&sharedState->customerQueue, i)) {
                printf("Customer %d: Could not join the queue, leaving...\n", i);
                sem_post(&sharedState->customerQueue.buffer_access); // Release semaphore
                exit(1);
            }

            sem_post(&sharedState->customerQueue.buffer_access); // Release semaphore


            // Child process: Execute visitor.c
            char customer_id[10];
            sprintf(customer_id, "%d", i);

            char *args[] = {"./visitor", customer_id, NULL};
            execvp(args[0], args);

            // If execvp fails
            perror("execvp");
            exit(0);
        } else if (pid < 0) {
            perror("fork");
            exit(0);
        }

        usleep(rand() % (1000 * 300)); // Simulate staggered customer arrival
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_customers; i++) {
        wait(NULL);
    }

    // Clean up shared memory
    if (munmap(sharedState, sizeof(SharedMemoryStruct)) == -1) {
        perror("munmap");
        exit(0);
    }
    if (shm_unlink(MEMORY_NAME) == -1) {
        perror("shm_unlink");
        exit(0);
    }

    return 0;
}
