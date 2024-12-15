#include "segment.h"

int main(int argc, char *argv[]) {

    int resttime ,ordertime;
    char shmname[50];
    if(argc!=7){
        printf("Incorrect input command line\n");
        exit(0);
    }  
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 && i + 1 < argc) {
            i++;
            resttime = atoi(argv[i]);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            i++;
            strcpy(shmname, argv[i]);
        }else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            i++;
            ordertime = atoi(argv[i]);
        } else {
            printf("Unknown flag : %s\n", argv[i]);
            exit(0);
        }
    }

    // Create a POSIX shared memory
    int shm_fd = shm_open(shmname, O_CREAT | O_RDWR, 0666);
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
    initBuffer(&sharedState->customerQueue,sharedState);


    // Initialize the semaphore
    if (sem_init(&sharedState->buffer_access, 1, 1) == -1) { // Shared semaphore
        perror("sem_init");
        exit(1);
    }

    int num_customers = 15;
    for (int i = 0; i < num_customers; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            // sem_wait(&sharedState->customerQueue.buffer_access); // Wait for buffer access

            printf("Child process: the i is %d\n", i);

            // if (!enqueue(&sharedState->customerQueue,sharedState, i)) {
            //     printf("Customer %d: Could not join the queue, leaving...\n", i);
            //     sem_post(&sharedState->buffer_access); // Release semaphore
            //     exit(1);
            // }

            // sem_post(&sharedState->customerQueue.buffer_access); // Release semaphore

            // Execute visitor.c
            char visitor_resttime[10];
            sprintf(visitor_resttime, "%d", resttime);

            char *args[] = {"./visitor", "-d",visitor_resttime,"-s",shmname, NULL};
            execvp(args[0], args);

            // If execvp fails
            perror("execvp");
            exit(1);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }
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
    if (shm_unlink(shmname) == -1) {
        perror("shm_unlink");
        exit(0);
    }

    return 0;
}
