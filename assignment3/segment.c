#include "segment.h"


void initBuffer(WaitingBuffer *cb ) {
    cb->head = 0;
    cb->tail = 0;
    
    // Initialize position semaphores
    for (int i = 0; i < MAX_VISITORS; i++) {
        sem_init(&cb->position_sem[i], 1, 1);
    }
}


void enqueue(SharedMemoryStruct *sharedMemory, pid_t value) {
    WaitingBuffer *cb = &sharedMemory->waiting_buffer;

    // Calculate the next tail
    int next_tail = (cb->tail + 1) % MAX_VISITORS;

    // Reserve the position in the buffer
    sem_wait(&cb->position_sem[cb->tail]);
    
    // Lock the buffer for mutual exclusion so that only one process can access the buffer at a time
    sem_wait(&sharedMemory->mutex_buffer_wait);

    // Write value to the current tail
    int slot = cb->tail;
    cb->waiting_buffer[slot] = value;

    // Advance tail pointer
    cb->tail = next_tail; 

    // Unlock the buffer for other processes
    sem_post(&sharedMemory->mutex_buffer_wait);
   
}

void dequeue(SharedMemoryStruct *sharedMemory, pid_t *value) {
    WaitingBuffer *cb = &sharedMemory->waiting_buffer;

    // Lock the buffer
    sem_wait(&sharedMemory->mutex_buffer_wait);

    // Determine the slot at head
    int slot = cb->head;

    // Retrieve the value from the current head
    *value = cb->waiting_buffer[slot];
    cb->waiting_buffer[slot] = 0;
    cb->head = (cb->head + 1) % MAX_VISITORS; // Advance head pointer

    // Unlock the buffer
    sem_post(&sharedMemory->mutex_buffer_wait);

    // Post to the slot semaphore
    sem_post(&cb->position_sem[slot]);

}

// Display the contents of the buffer
void displayBuffer(WaitingBuffer *cb) {
    printf("Buffer contents: ");
    for (int i = 0; i < MAX_VISITORS ; i++) {
        // int index = (cb->head + i) % MAX_VISITORS;
        printf("%d ", cb->waiting_buffer[i]);
    }
    printf("\n");
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

void enqueueOrder(SharedMemoryStruct *sharedMemory, pid_t value) {
    OrderBuffer *cb = &sharedMemory->receprionist_buffer;
    
    // Calculate the next tail
    int next_tail = (cb->tail + 1) % (NUM_CHAIRS*NUM_TABLES);

    // Reserve the position in the buffer
    sem_wait(&cb->chair_sem[cb->tail]);
    
    // Lock the buffer for mutual exclusion so that only one process can access the buffer at a time
    sem_wait(&sharedMemory->mutex_buffer_order);

    // Write value to the current tail
    int slot = cb->tail;
    cb->order_buffer[slot] = value;

    // Advance tail pointer
    cb->tail = next_tail; 

    // Unlock the buffer for other processes
    sem_post(&sharedMemory->mutex_buffer_order);

    printf("Enqueued %d in slot %d\n", value, slot);
   
}

void dequeueOrder(SharedMemoryStruct *sharedMemory , pid_t *value) {
    OrderBuffer *cb = &sharedMemory->receprionist_buffer;
    
    // Lock the buffer
    sem_wait(&sharedMemory->mutex_buffer_order);

    // Determine the slot at head
    int slot = cb->head;

    // Retrieve the value from the current head
    *value = cb->order_buffer[slot];
    cb->order_buffer[slot] = 0;
    cb->head = (cb->head + 1) % (NUM_CHAIRS*NUM_TABLES); 

    // Unlock the buffer
    sem_post(&sharedMemory->mutex_buffer_order);

    // Post to the slot semaphore
    sem_post(&cb->chair_sem[slot]);
}

void cleanupOrderBuffer(OrderBuffer *ob) {
    for (int i = 0; i < NUM_CHAIRS * NUM_TABLES; i++) {
        sem_destroy(&ob->chair_sem[i]);
    }
}