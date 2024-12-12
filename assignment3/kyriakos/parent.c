#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

/* Semaphores and shared memory */
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define CHILDNUM 10
#define SHM_SIZE 1024 // Large enough to store any arbitrary message

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
    /* Create semaphore array (1 semaphore) */
    int semid = semget(IPC_PRIVATE, CHILDNUM, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    /* Create semaphore for parent */
    int semid_parent = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    if (semid_parent == -1) {
        perror("semget");
        exit(1);
    }

    /* Initialize the semaphore*/
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    arg.val = 0;
    if (semctl(semid_parent, 0, SETVAL, arg) == -1) {
        perror("semctl");
        exit(1);
    }

    /* Initialize the semaphores */
    arg.val = 0;
    for (int i = 0; i < CHILDNUM; i++) {
        if (semctl(semid, i, SETVAL, arg) == -1) {
            perror("semctl");
            exit(1);
        }
    }

    /* Create shared memory */
    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    /* Attach the shared memory */
    char *shmaddr = shmat(shmid, 0, 0);
    if (shmaddr == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    /* Fork the child processes */
    for (int i = 0; i < CHILDNUM; i++) {

        /* Fork */
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        } 
        else if (pid == 0) {

            printf("-----------------------------------RUNNING CHILD PROCESS %d\n", i);
            /* Child process */
            char childnum[10];
            char semidstr[10];
            char semidstr_parent[10];
            char shmidstr[10];

            /* Convert the child number, semaphore id, and shared memory id to strings */
            sprintf(childnum, "%d", i);
            sprintf(semidstr, "%d", semid);
            sprintf(shmidstr, "%d", shmid);
            sprintf(semidstr_parent, "%d", semid_parent);


            /* Execute the child process */
            execlp("./child", "./child", childnum, semidstr, semidstr_parent, shmidstr, NULL);
            perror("execlp");
            exit(1);
        }

        printf("-----------------------------------RUNNING PARENT PROCESS\n");

        /* Parent process */
        waitt(semid, i);
        
        /* Message to send to the child process */
        sprintf(shmaddr, "Hello from parent, child %d", i);

        printf("Message to child %d: %s\n", i, shmaddr);

        /* Unlock the semaphore */
        post(semid_parent, 0);

    }

    /* Wait for all the child processes to finish */
    for (int i = 0; i < CHILDNUM; i++) {
        wait(NULL);
    }
    
    /* Clean up shared memory */
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
    }

    /* Clean up semaphore */
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl");
    }

    if (semctl(semid_parent, 0, IPC_RMID) == -1) {
        perror("semctl");
    }

    return 0;
}