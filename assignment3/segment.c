#include "segment.h"


void initBuffer(WaitingBuffer *cb ) {
    cb->head = 0;
    cb->tail = 0;
    
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

    // sem_wait(&cb->position_sem[cb->head]);

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

    // sem_post(&cb->position_sem[cb->tail]);
    return true;
}

void cleanupBuffer(WaitingBuffer *cb ) {
   
    for (int i = 0; i < MAX_VISITORS; i++) {
        sem_destroy(&cb->position_sem[i]);
    }
}

void initOrderBuffer(OrderBuffer *ob) {
    ob->head = 0;
    ob->tail = 0;
    for (int i = 0; i < NUM_CHAIRS * NUM_TABLES; i++) {
        ob->order_buffer[i] = 0;
        sem_init(&ob->chair_sem[i], 1, 1);
    }
}

bool enqueueOrder(OrderBuffer *ob, int value) {
    int next;
    next = ob->head + 1;  // next is where head will point to after this write.
    if (next >= NUM_CHAIRS * NUM_TABLES) {
        next = 0;
    }

    if (next == ob->tail) {  // if the head + 1 == tail, circular buffer is full
        printf("Order buffer is full. Cannot enqueue %d\n", value);
        return false;
    }


    ob->order_buffer[ob->head] = value;
    ob->head = next;


    return true;
}

bool dequeueOrder(OrderBuffer *ob, int *value) {
    int next;

    if (ob->head == ob->tail) {  // if the head == tail, we don't have any data
        printf("Order buffer is empty. Cannot dequeue.\n");
        return false;
    }
    next = ob->tail + 1;  // next is where tail will point to after this read.
    if (next >= NUM_CHAIRS * NUM_TABLES) {
        next = 0;
    }

   *value = ob->order_buffer[ob->tail];
    ob->tail = next;

    return true;
}

void cleanupOrderBuffer(OrderBuffer *ob) {
    for (int i = 0; i < NUM_CHAIRS * NUM_TABLES; i++) {
        sem_destroy(&ob->chair_sem[i]);
    }
}