#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
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

bool isFull(CircularBuffer *cb) {
    return cb->count == BUFFER_SIZE;
}

bool isEmpty(CircularBuffer *cb) {
    return cb->count == 0;
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <customer_id>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // int customer_id = atoi(argv[1]);

    // Open shared memory
    int shm_fd = shm_open(MEMORY_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Map shared memory
    TableState *sharedState = (TableState *)mmap(NULL, sizeof(TableState), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (sharedState == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }


    int customer_id;
    while (true) {
        if (!dequeue(&sharedState->customerQueue, &customer_id)) {
            usleep(1000); // Wait before checking again
            continue;
        }

        srand(time(0) + customer_id);
        printf("Customer %d: Looking for a table...\n", customer_id);
        usleep(rand() % (3 * 1000));

        for (int i = 0; i < NUM_TABLES; i++) {
            if (sharedState->available[i] > 0) {
                sharedState->available[i]--;
                printf("Customer %d: Sat at Table %d\n", customer_id, i + 1);

                // Simulate dining time
                usleep(rand() % (3 * 1000));

                // Leave table
                sharedState->available[i]++;
                printf("Customer %d: Left Table %d\n", customer_id, i + 1);
                return 0;
            }
        }
        printf("Customer %d: Could not find a table, leaving...\n", customer_id);
    }


    // Clean up
    if (munmap(sharedState, sizeof(TableState)) == -1) {
        perror("munmap");
    }

    return 0;
}
