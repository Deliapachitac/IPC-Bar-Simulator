#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>


// Constants for the number of tables and chairs
#define NUM_TABLES 3
#define NUM_CHAIRS 4
#define SECOND 1000

// Semaphore names
#define SEM_TABLE_1 "/table1_semaphore"
#define SEM_TABLE_2 "/table2_semaphore"
#define SEM_TABLE_3 "/table3_semaphore"


typedef struct {
    unsigned int table1_customers; // Number of customers at table 1
    unsigned int table2_customers; // Number of customers at table 2
    unsigned int table3_customers; // Number of customers at table 3
    unsigned int table1_cleared;   // Whether table 1 is fully cleared (0 or 1)
    unsigned int table2_cleared;   // Whether table 2 is fully cleared (0 or 1)
    unsigned int table3_cleared;   // Whether table 3 is fully cleared (0 or 1)
} SharedTableState;

// Global variables for shared memory and semaphores
sem_t *sem_table_1, *sem_table_2, *sem_table_3;
SharedTableState *sharedState;

// Function to clean semaphores and shared memory on signal
void cleanup(int signum) {
    sem_close(sem_table_1);
    sem_close(sem_table_2);
    sem_close(sem_table_3);

    shmdt((void *)sharedState);
    exit(signum);
}

// Simulate a customer trying to find a table
void simulateCustomer(int customer_id) {
    srand(time(0) + customer_id);

    // Simulate thinking and eating behavior
    printf("Customer %d: Looking for a table...\n", customer_id);
    usleep(rand() % (3 * SECOND));

    // Attempt to join a table (search for availability)
    if (sem_trywait(sem_table_1) == 0 && sharedState->table1_customers < NUM_CHAIRS) {
        // Join table 1
        sharedState->table1_customers++;
        printf("Customer %d: Sat at Table 1\n", customer_id);
        usleep(rand() % (3 * SECOND));
        sharedState->table1_customers--;
        printf("Customer %d: Left Table 1\n", customer_id);
        sem_post(sem_table_1);
    } else if (sem_trywait(sem_table_2) == 0 && sharedState->table2_customers < NUM_CHAIRS) {
        // Join table 2
        sharedState->table2_customers++;
        printf("Customer %d: Sat at Table 2\n", customer_id);
        usleep(rand() % (3 * SECOND));
        sharedState->table2_customers--;
        printf("Customer %d: Left Table 2\n", customer_id);
        sem_post(sem_table_2);
    } else if (sem_trywait(sem_table_3) == 0 && sharedState->table3_customers < NUM_CHAIRS) {
        // Join table 3
        sharedState->table3_customers++;
        printf("Customer %d: Sat at Table 3\n", customer_id);
        usleep(rand() % (3 * SECOND));
        sharedState->table3_customers--;
        printf("Customer %d: Left Table 3\n", customer_id);
        sem_post(sem_table_3);
    } else {
        printf("Customer %d: Could not find a table, leaving...\n", customer_id);
    }
}

int main(int argc, char *argv[]) {
    
    //Reading and saving the variables from the command line 
    // int resttime , shmid;
    // if(argc!=5){
    //     printf("Incorrect input command line\n");
    //     exit(0);
    // }  
    // for (int i = 1; i < argc; i++) {
    //     if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
    //         i++;
    //         resttime = atoi(argv[i]);
    //     } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
    //         i++;
    //         shmid = atoi(argv[i]);
    //     } else {
    //         printf("Unknown flag : %s\n", argv[i]);
    //         exit(0);
    //     }
    // }

    // printf("Maximum rest time: %d\n", resttime);
    // printf("Shared memory ID: %d\n", shmid);

////////////////////////////////////////////////////////////////////

    srand(time(0));
    signal(SIGUSR1, cleanup);

    // Create semaphores for the 3 tables
    sem_table_1 = sem_open(SEM_TABLE_1, O_CREAT | O_EXCL, 0600, NUM_CHAIRS);
    sem_table_2 = sem_open(SEM_TABLE_2, O_CREAT | O_EXCL, 0600, NUM_CHAIRS);
    sem_table_3 = sem_open(SEM_TABLE_3, O_CREAT | O_EXCL, 0600, NUM_CHAIRS);

    if (sem_table_1 == SEM_FAILED || sem_table_2 == SEM_FAILED || sem_table_3 == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }

    // Create shared memory for tables' states
    key_t key = ftok(argv[0], 0);
    int shm_id = shmget(key, sizeof(SharedTableState), IPC_CREAT | 0600);
    sharedState = shmat(shm_id, NULL, 0);

    if (sharedState == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    sharedState->table1_customers = 0;
    sharedState->table2_customers = 0;
    sharedState->table3_customers = 0;

    int num_customers = 20; // Number of customers to simulate
    for (int i = 0; i < num_customers; i++) {
        if (fork() == 0) {
            simulateCustomer(i + 1);
            exit(0);
        }
        usleep(rand() % (2 * SECOND));
    }

    for (int i = 0; i < num_customers; i++) {
        wait(NULL);
    }

    cleanup(SIGUSR1);

    sem_close(sem_table_1);
    sem_close(sem_table_2);
    sem_close(sem_table_3);

    sem_unlink(SEM_TABLE_1);
    sem_unlink(SEM_TABLE_2);
    sem_unlink(SEM_TABLE_3);

    shmdt((void *)sharedState);
    shmctl(shm_id, IPC_RMID, NULL);



    return 0;

}