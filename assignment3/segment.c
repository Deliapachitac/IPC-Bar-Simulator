#include "segment.h"


void initBuffer(WaitingBuffer *cb,SharedMemoryStruct *sharedState ) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    sem_init(&sharedState->buffer_access , 1, 1); // Shared semaphore (pshared = 1)

    // Initialize position semaphores
    for (int i = 0; i < MAX_VISITORS; i++) {
        sem_init(&cb->position_sem[i], 1, 1);
    }
}

bool isFull(WaitingBuffer *cb,SharedMemoryStruct *sharedState ) {
    return cb->count == MAX_VISITORS;
}

bool isEmpty(WaitingBuffer *cb,SharedMemoryStruct *sharedState) {
    return cb->count == 0;
}

bool enqueue(WaitingBuffer *cb,SharedMemoryStruct *sharedState , int value) {
    sem_wait(&sharedState->buffer_access); // Wait for access to the buffer

    if (isFull(cb,sharedState)) {
        printf("Buffer is full. Cannot enqueue %d\n", value);
        sem_post(&sharedState->buffer_access); // Release the semaphore
        return false;
    }

    cb->waiting_buffer[cb->tail] = value;
    cb->tail = (cb->tail + 1) % MAX_VISITORS;
    cb->count++;

    sem_post(&sharedState->buffer_access); // Release the semaphore
    return true;
}

bool dequeue(WaitingBuffer *cb,SharedMemoryStruct *sharedState , int *value) {
    sem_wait(&sharedState->buffer_access); // Wait for access to the buffer

    if (isEmpty(cb,sharedState)) {
        printf("Buffer is empty. Cannot dequeue.\n");
        sem_post(&sharedState->buffer_access); // Release the semaphore
        return false;
    }

    *value = cb->waiting_buffer[cb->head];
    cb->head = (cb->head + 1) % MAX_VISITORS;
    cb->count--;

    sem_post(&sharedState->buffer_access); // Release the semaphore
    return true;
}

void cleanupBuffer(WaitingBuffer *cb,SharedMemoryStruct *sharedState ) {
    sem_destroy(&sharedState->buffer_access);

    for (int i = 0; i < MAX_VISITORS; i++) {
        sem_destroy(&cb->position_sem[i]);
    }
}