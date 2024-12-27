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
    sharedMemory->served_visitors++;

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
        sem_init(&ob->order_ready[i], 1, 0); // Initialize each semaphore to 0
    }
}

int enqueueOrder(SharedMemoryStruct *sharedMemory, pid_t value) {
    OrderBuffer *ob = &sharedMemory->receprionist_buffer;

    // Wait for an empty slot to become available
    sem_wait(&sharedMemory->buffer_empty);

    // Lock the buffer for mutual exclusion
    sem_wait(&sharedMemory->mutex_buffer_order);

    // Write the visitor's PID to the buffer at the tail
    int slot = ob->tail;
    ob->order_buffer[slot] = value;

    // Advance the tail pointer
    ob->tail = (ob->tail + 1) % (NUM_CHAIRS * NUM_TABLES);

    // Update total visitors statistic
    sharedMemory->statistics.total_visitors++;

    // Unlock the buffer
    sem_post(&sharedMemory->mutex_buffer_order);

    // Signal that a filled slot is now available
    sem_post(&sharedMemory->buffer_full);

    printf("Enqueued visitor ID %d in slot %d.\n", value, slot);

    return slot;
}


int dequeueOrder(SharedMemoryStruct *sharedMemory , pid_t *value) {
    OrderBuffer *ob = &sharedMemory->receprionist_buffer;

    // Wait for a filled slot to become available
    sem_wait(&sharedMemory->buffer_full);

    // Lock the buffer for mutual exclusion
    sem_wait(&sharedMemory->mutex_buffer_order);

    // Retrieve the visitor's PID from the buffer at the head
    int slot = ob->head;
    *value = ob->order_buffer[slot];
    ob->order_buffer[slot] = 0;

    // Advance the head pointer
    ob->head = (ob->head + 1) % (NUM_CHAIRS * NUM_TABLES);

    // Unlock the buffer
    sem_post(&sharedMemory->mutex_buffer_order);

    // Signal that an empty slot is now available
    sem_post(&sharedMemory->buffer_empty);

    printf("Dequeued visitor ID %d from slot %d.\n", *value, slot);

    return slot;
}

void cleanupOrderBuffer(OrderBuffer *ob) {
    for (int i = 0; i < (NUM_CHAIRS*NUM_TABLES); i++)
    {
        sem_destroy(&ob->order_ready[i]);
    }
    
}