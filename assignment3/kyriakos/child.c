#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

/* Semaphores and shared memory */
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

/* Lock semaphore */
void waitt(int semid, int semnum) {
    struct sembuf sb = {semnum, -1, 0}; // Wait for the semaphore to be unlocked
    semop(semid, &sb, 1);
}

/* Unlock semaphore */
void post(int semid, int semnum) {
    struct sembuf sb = {semnum, 1, 0}; // Unlock the semaphore
    semop(semid, &sb, 1);
}

int main (int argc, char *argv[]) 
{
    printf("----------------------------CHILD STARTED\n");
    /* Print the parameters */
    printf("Child process: %d\n", getpid());
    printf("Child number: %s\n", argv[1]);
    printf("Semaphore id: %s\n", argv[2]);
    printf("Parent Semaphore id: %s\n", argv[3]);
    printf("Shared memory id: %s\n", argv[4]);

    /* Get the semaphore id and shared memory id */
    int semid = atoi(argv[2]);
    int shmid = atoi(argv[4]);

    /* Used to find the correct semaphore */
    int semid_parent = atoi(argv[3]);
    int childnum = atoi(argv[1]);

    /* Says to the parent that it is ready */
    post(semid, childnum);

    /* Lock the semaphore */
    waitt(semid_parent, 0);

    /* Attach the shared memory */
    char *shmaddr = shmat(shmid, 0, 0);
    if (shmaddr == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    /* Print the message */
    printf("Message from parent: %s\n", shmaddr);

    /* Signal the semaphore */
    post(semid, childnum);

    printf("----------------------CHILD ENDED\n\n\n");

    return 0;

}