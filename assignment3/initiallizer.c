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

    // Initialize the tables structs
    for (int i = 0; i < NUM_TABLES; i++) {
        sharedState->table[i].full = false; // Empty table
        sharedState->table[i].full_chairs = 0; // Empty chairs
        for (int j = 0; j < NUM_CHAIRS; j++) {
            sharedState->table[i].chairs[j] = 0; // No one is sitting
        }
        sem_init(&sharedState->table[i].table_sem, 1, 1);
    }

    //Initialize the waiting buffer
    initBuffer(&sharedState->customerQueue,sharedState);

    // Initialize all the semaphores
    sem_init(&sharedState->empty_buffer , 1, MAX_VISITORS-1);
    sem_init(&sharedState->full_buffer , 1, 0);
    sem_init(&sharedState->mutex_buffer , 1, 1);

    //
    sem_init(&sharedState->mutex_access, 1, 1);
    sem_init(&sharedState->receptionist_access, 1, 1);

    // Initialize all the varibles in  statistics struct
    sharedState->statistics.avarage_staying_time = 0;
    sharedState->statistics.avarage_waiting_time = 0;
    sharedState->statistics.counter_cheese = 0;
    sharedState->statistics.counter_salad = 0;
    sharedState->statistics.counter_water = 0;
    sharedState->statistics.counter_wine = 0;
    sharedState->statistics.total_visitors = 0;
    sharedState->statistics.total_staying_time = 0;
    sharedState->statistics.total_waiting_time = 0;
    

    // Create child processes
    int num_customers = 20;
    for (int i = 0; i < num_customers; i++) {
        pid_t pid = fork();
        if (pid == 0) {

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

    // Destroy the semaphores
    sem_destroy(&sharedState->empty_buffer);
    sem_destroy(&sharedState->full_buffer);
    sem_destroy(&sharedState->mutex_buffer);
    sem_destroy(&sharedState->mutex_access);
    sem_destroy(&sharedState->receptionist_access);
    for (int i = 0; i < NUM_TABLES; i++)
    {
        sem_destroy(&sharedState->table[i].table_sem);  
    }
    

    // Clean up the waiting buffer
    cleanupBuffer(&sharedState->customerQueue);


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
