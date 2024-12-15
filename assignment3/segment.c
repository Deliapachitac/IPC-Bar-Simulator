#include "segment.h"


void initBuffer(WaitingBuffer *cb,SharedMemoryStruct *sharedState ) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    sem_init(&sharedState->waiting_buffer_access , 1, 1); // Shared semaphore (pshared = 1)

    // Initialize position semaphores
    for (int i = 0; i < MAX_VISITORS; i++) {
        sem_init(&cb->position_sem[i], 1, 1);
    }
}


bool enqueue(WaitingBuffer *cb, int value) {
    
    int next;
    next = cb->head + 1;  // next is where head will point to after this write.
    if (next >= MAX_VISITORS){
        next = 0;
    }
        
    if (next == cb->tail){  // if the head + 1 == tail, circular buffer is full
        printf("Buffer is full. Cannot enqueue %d\n", value);
        return false;
    }

    cb->waiting_buffer[cb->head] = value;
    cb->head = next;
    
    return true;
}

bool dequeue(WaitingBuffer *cb, int *value) {
    
    int next;

    if (cb->head == cb->tail){  // if the head == tail, we don't have any data
        printf("Buffer is empty. Cannot dequeue.\n");
        return false;
    }
    next = cb->tail + 1;  // next is where tail will point to after this read.
    if(next >= MAX_VISITORS){
        next = 0;
    }

    *value = cb->waiting_buffer[cb->tail];
    cb->tail = next;
    
    return true;
}

void cleanupBuffer(WaitingBuffer *cb,SharedMemoryStruct *sharedState ) {
    sem_destroy(&sharedState->waiting_buffer_access);

    for (int i = 0; i < MAX_VISITORS; i++) {
        sem_destroy(&cb->position_sem[i]);
    }
}