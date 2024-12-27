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
            sem_init(&sharedState->table[i].chair_sem[j], 1, 1); 
        }
    }
    sem_init(&sharedState->table_mutex, 1, 1);
    sem_init(&sharedState->table_reset, 1, 0);

    //Initialize the waiting buffer
    initBuffer(&sharedState->waiting_buffer);
    initOrderBuffer(&sharedState->receprionist_buffer);

    // Initialize all the semaphores
    sem_init(&sharedState->mutex_buffer_wait , 1,1 );
    sem_init(&sharedState->mutex_buffer_order , 1,1 );

    //
    sem_init(&sharedState->mutex_access, 1, 1);
    sem_init(&sharedState->receptionist_access, 1, 0);
    sem_init(&sharedState->visitor_processed, 1, 0);
    sem_init(&sharedState->logging, 1, 1);

    sem_init(&sharedState->buffer_empty, 1, NUM_CHAIRS * NUM_TABLES); // All slots are initially empty
    sem_init(&sharedState->buffer_full, 1, 0); // No slots are filled initially


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
    sharedState->served_visitors = 0;
    
    // //Create the receptionist process
    pid_t receptionist_pid = fork();
    if (receptionist_pid == 0) {
        char receptionist_ordertime[10];
        sprintf(receptionist_ordertime, "%d", ordertime);

        char *args[] = {"./receptionist", "-d",receptionist_ordertime,"-s",shmname, NULL};
        execvp(args[0], args);

        // If execvp fails
        perror("execvp");
        exit(1);
    } else if (receptionist_pid < 0) {
        perror("fork");
        exit(1);
    }

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




    /// Wait for the receptionist process to finish
    waitpid(receptionist_pid, NULL, 0);

    // Wait for all visitor processes to finish
    for (int i = 0; i < num_customers; i++) {
        pid_t visitor_pid = waitpid(-1, NULL, 0); // Wait for any child process (visitor)
        if (visitor_pid < 0) {
            perror("waitpid");
            exit(1);
        }
    }

    // Destroy the semaphores
    sem_destroy(&sharedState->mutex_buffer_wait);
    sem_destroy(&sharedState->mutex_buffer_order);
    sem_destroy(&sharedState->mutex_access);
    sem_destroy(&sharedState->receptionist_access);
    sem_destroy(&sharedState->visitor_processed);
    sem_destroy(&sharedState->logging);
    sem_destroy(&sharedState->buffer_empty);
    sem_destroy(&sharedState->buffer_full);
    sem_destroy(&sharedState->table_mutex);
    sem_destroy(&sharedState->table_reset);

    for (int i = 0; i < NUM_TABLES; i++)
    {
        for (int j = 0; j < NUM_CHAIRS; j++)
        {
            sem_destroy(&sharedState->table[i].chair_sem[j]);
        }
        sem_destroy(&sharedState->table_mutex);
    }
    

    // Clean up the waiting buffer
    cleanupBuffer(&sharedState->waiting_buffer);
    cleanupOrderBuffer(&sharedState->receprionist_buffer);

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
