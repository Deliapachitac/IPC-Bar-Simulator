#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>

#define NUM_TABLES 3
#define NUM_CHAIRS 4
#define SECOND 1000
#define BUFFER_SIZE 10 // Circular buffer size

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

// Circular buffer functions
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

bool dequeue(CircularBuffer *cb, int *value) {
    if (isEmpty(cb)) {
        printf("Buffer is empty. Cannot dequeue.\n");
        return false;
    }
    *value = cb->buffer[cb->head];
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    cb->count--;
    return true;
}

void simulateCustomer(TableState *sharedState) {
    int customer_id;
    while (true) {
        if (!dequeue(&sharedState->customerQueue, &customer_id)) {
            usleep(SECOND); // Wait before checking again
            continue;
        }

        srand(time(0) + customer_id);
        printf("Customer %d: Looking for a table...\n", customer_id);
        usleep(rand() % (3 * SECOND));

        for (int i = 0; i < NUM_TABLES; i++) {
            if (sharedState->available[i] > 0) {
                sharedState->available[i]--;
                printf("Customer %d: Sat at Table %d\n", customer_id, i + 1);

                // Simulate dining time
                usleep(rand() % (3 * SECOND));

                // Leave table
                sharedState->available[i]++;
                printf("Customer %d: Left Table %d\n", customer_id, i + 1);
                return;
            }
        }
        printf("Customer %d: Could not find a table, leaving...\n", customer_id);
    }
}

int main() {
    srand(time(0));

    // Create POSIX shared memory
    int shm_fd = shm_open("/shared_table_state", O_CREAT | O_RDWR, 0666);
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
        if (fork() == 0) {
            // Add customer to the circular buffer
            if (!enqueue(&sharedState->customerQueue, i)) {
                printf("Customer %d: Could not join the queue, leaving...\n", i);
                exit(0);
            }
            simulateCustomer(sharedState);
            exit(0);
        }
        usleep(rand() % (2 * SECOND));
    }

    for (int i = 0; i < num_customers; i++) {
        wait(NULL);
    }

    // Clean up shared memory
    if (munmap(sharedState, sizeof(TableState)) == -1) {
        perror("munmap");
    }
    if (shm_unlink("/shared_table_state") == -1) {
        perror("shm_unlink");
    }

    return 0;
}
