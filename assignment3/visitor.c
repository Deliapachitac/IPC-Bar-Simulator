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
    
    sem_wait(&sharedState->buffer_access); // Wait for access to the buffer
    if (!enqueue(&sharedState->customerQueue, getpid()%2078800)) {
        printf("Customer %d: Could not join the queue, leaving...\n", getpid()%2077800);
        sem_post(&sharedState->buffer_access); // Release semaphore
        exit(1);
    }
    sem_post(&sharedState->buffer_access); // Release the semaphore
    
    while (true) {
        int group[4];
        int group_size = 0;
        // printf("delia\n");
        for (int i = 0; i < 4; i++) {
            sem_wait(&sharedState->buffer_access); // Wait for access to the buffer
            if (!dequeue(&sharedState->customerQueue, &group[i])) {
                sem_post(&sharedState->buffer_access); // Release the semaphore
                break; // Not enough customers for a group
            }
            sem_post(&sharedState->buffer_access); // Release the semaphore
            group_size++;
        }

        if (group_size == 0) {
            usleep(1000); // Wait before checking again
            continue;
        }

        printf("Group of %d customers: Looking for a table...\n", group_size);

        bool found_table = false;
        while (!found_table) {
            for (int i = 0; i < NUM_TABLES; i++) {
                sem_wait(&sharedState->table[i].table_sem);
                if (!sharedState->table[i].full) {
                    sharedState->table[i].full = true;
                    for (int j = 0; j < group_size; j++) {
                        sharedState->table[i].chairs[j] = group[j];
                        sharedState->table[i].full_chairs++;
                    }
                    printf("Group sat at Table %d\n", i + 1);
                    found_table = true;

                    // Simulate dining time
                    usleep(resttime * 1000);

                    for (int j = 0; j < group_size; j++) {
                        sharedState->table[i].chairs[--sharedState->table[i].full_chairs] = 0;
                    }

                    if (sharedState->table[i].full_chairs == 0) {
                        sharedState->table[i].full = false;
                    }

                    printf("Group left Table %d\n", i + 1);
                    sem_post(&sharedState->table[i].table_sem);
                    break;
                }
                sem_post(&sharedState->table[i].table_sem);
            }

            if (!found_table) {
                usleep(1000); // Wait and check again
            }
        }

        break;
    }

    // Clean up
    if (munmap(sharedState, sizeof(SharedMemoryStruct)) == -1) {
        perror("munmap");
    }

    return 0;
}
