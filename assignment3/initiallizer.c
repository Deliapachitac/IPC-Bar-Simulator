#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>

#define NUM_TABLES 3
#define NUM_CHAIRS 4
#define BUFFER_SIZE 10
#define MEMORY_NAME "/path_to_nemea"

// Circular buffer structure
typedef struct {
    int buffer[BUFFER_SIZE];
    int head;
    int tail;
    int count;
} CircularBuffer;

// Table structure to manage availability
typedef struct {
    int available[NUM_TABLES]; // Array to track availability of tables
    CircularBuffer customerQueue; // Embedded circular buffer
} TableState;

// Initialize circular buffer
void initBuffer(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

bool isFull(CircularBuffer *cb) {
    return cb->count == BUFFER_SIZE;
}

bool isEmpty(CircularBuffer *cb) {
    return cb->count == 0;
}

bool enqueue(CircularBuffer *cb, int value) {
    if (isFull(cb)) {
        printf("Buffer is full. Cannot enqueue %d\n", value);
        return false;
    }
    cb->buffer[cb->tail] = value;
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    cb->count++;
    return true;
}

int main(int argc, char *argv[]) {
    srand(time(0));

    // Create POSIX shared memory
    int shm_fd = shm_open(MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Set the size of shared memory
    if (ftruncate(shm_fd, sizeof(TableState)) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    // Map shared memory
    TableState *sharedState = (TableState *)mmap(NULL, sizeof(TableState), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
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
    if (munmap(sharedState, sizeof(TableState)) == -1) {
        perror("munmap");
    }
    if (shm_unlink(MEMORY_NAME) == -1) {
        perror("shm_unlink");
    }

    return 0;
}
