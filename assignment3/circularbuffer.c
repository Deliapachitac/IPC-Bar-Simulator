#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SIZE 5  // Define the size of the circular buffer

typedef struct {
    int buffer[BUFFER_SIZE];  // Array to store buffer elements
    int head;                // Index of the head element
    int tail;                // Index of the tail element
    int count;               // Number of elements in the buffer
} CircularBuffer;

// Initialize the circular buffer
void initBuffer(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

// Check if the buffer is full
bool isFull(CircularBuffer *cb) {
    return cb->count == BUFFER_SIZE;
}

// Check if the buffer is empty
bool isEmpty(CircularBuffer *cb) {
    return cb->count == 0;
}

// Add an element to the buffer
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

// Remove an element from the buffer
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

// Display the contents of the buffer
void displayBuffer(CircularBuffer *cb) {
    printf("Buffer contents: ");
    for (int i = 0; i < cb->count; i++) {
        int index = (cb->head + i) % BUFFER_SIZE;
        printf("%d ", cb->buffer[index]);
    }
    printf("\n");
}

int main() {
    CircularBuffer cb;
    initBuffer(&cb);

    // Test the circular buffer
    enqueue(&cb, 1);
    enqueue(&cb, 2);
    enqueue(&cb, 3);
    enqueue(&cb, 4);
    enqueue(&cb, 5);
    displayBuffer(&cb);

    int value;
    dequeue(&cb, &value);
    printf("Dequeued: %d\n", value);
    displayBuffer(&cb);

    enqueue(&cb, 6);
    displayBuffer(&cb);

    return 0;
}
