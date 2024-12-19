#include "segment.h"



int main(int argc, char *argv[]) {
    
    //Reading and saving the variables from the command line  
    int ordertime ;
    char shmname[50];   
    if(argc!=5) {
        printf("Incorrect input command line\n");
        exit(0);
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            i++;
            ordertime = atoi(argv[i]);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            i++;
            strcpy(shmname, argv[i]);
        } else {
            printf("Unknown flag : %s\n", argv[i]);
            exit(0);
        }
    }

    // Open shared memory
    int shm_fd = shm_open(shmname, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(0);
    }

    // Map shared memory
    SharedMemoryStruct *sharedState = (SharedMemoryStruct *)mmap(NULL, sizeof(SharedMemoryStruct), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (sharedState == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    

    // while(true){

        // sem_wait(&sharedState->mutex_access);

        // printf("Receptionist: Waiting for visitors...\n");

        // Wait for a visitor to be available
        // sem_wait(&sharedState->visitor_available);

        // Lock the receptionist semaphore to ensure one-at-a-time processing
        // sem_wait(&sharedState->receptionist_access);

        // pid_t visitor_id;
        // dequeueOrder(sharedState, &visitor_id);
        // if (visitor_id == -1) {
        //     printf("Receptionist: No visitors to process.\n");
        //     // sem_post(&sharedState->receptionist_access);
        //     continue;
        // }

        // printf("Receptionist: Preparing order for visitor %d\n", visitor_id);

        // sem_post(&sharedState->mutex_access);
        // Simulate the receptionist preparing the order
        srand(time(NULL)^ getpid());
        // Simulate preparing time for a random duration between [0.5 * ordertime , ordertime]
        int min_preparing_time = (int)(0.5 * ordertime);
        int random_preparing_time = min_preparing_time + rand() % (ordertime - min_preparing_time + 1);
        sleep(random_preparing_time);

        // Unlock receptionist semaphore
        // sem_post(&sharedState->receptionist_access);

    // }

    // Clean up
    if (munmap(sharedState, sizeof(SharedMemoryStruct)) == -1) {
        perror("munmap");
    }


    return 0;
}