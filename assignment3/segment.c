#include "segment.h"


void initBuffer(WaitingBuffer *cb ) {
    cb->head = 0;
    cb->tail = 0;
    
    // Initialize position semaphores
    for (int i = 0; i < MAX_VISITORS; i++) {
        sem_init(&cb->position_sem[i], 1, 0);
    }
}


void enqueue(SharedMemoryStruct *sharedMemory, pid_t value) {
    WaitingBuffer *cb = &sharedMemory->waiting_buffer;

    // Lock the buffer
    sem_wait(&sharedMemory->mutex_buffer_wait);

    // Wait for an empty slot
    int next_tail = (cb->tail + 1) % MAX_VISITORS;

    // Write value to the current tail
    int slot = cb->tail;
    cb->waiting_buffer[slot] = value;
    cb->tail = next_tail; // Advance tail pointer

    // Unlock the buffer
    sem_post(&sharedMemory->mutex_buffer_wait);

    // Signal the slot semaphore
    sem_post(&cb->position_sem[slot]);

    // printf("Enqueued %d in slot %d\n", value, slot);
   
}

void dequeue(SharedMemoryStruct *sharedMemory, pid_t *value) {
    WaitingBuffer *cb = &sharedMemory->waiting_buffer;

    // Lock the buffer to check state and access the buffer
    sem_wait(&sharedMemory->mutex_buffer_wait);

    // Check if the buffer is empty
    if (cb->head == cb->tail) {
        // Buffer is empty; no value to dequeue
        *value = -1;
        sem_post(&sharedMemory->mutex_buffer_wait); // Unlock the buffer
        return;
    }

    // Determine the slot at head
    int slot = cb->head;

    // Unlock the buffer while waiting for the slot semaphore
    sem_post(&sharedMemory->mutex_buffer_wait);
    sem_wait(&cb->position_sem[slot]);
    

    // Lock the buffer
    sem_wait(&sharedMemory->mutex_buffer_wait);

    // Retrieve the value from the current head
    *value = cb->waiting_buffer[slot];
    cb->head = (cb->head + 1) % MAX_VISITORS; // Advance head pointer

    // Unlock the buffer
    sem_post(&sharedMemory->mutex_buffer_wait);

    // printf("Dequeued %d from slot %d\n", *value, slot);

}

// // Display the contents of the buffer
// void displayBuffer(WaitingBuffer *cb) {
//     printf("Buffer contents: ");
//     for (int i = 0; i < MAX_VISITORS ; i++) {
//         int index = (cb->head + i) % MAX_VISITORS;
//         printf("%d ", cb->waiting_buffer[index]);
//     }
//     printf("\n");
// }
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
        sem_init(&ob->chair_sem[i], 1, 0);
    }
}

void enqueueOrder(SharedMemoryStruct *sharedMemory, pid_t value) {
    OrderBuffer *cb = &sharedMemory->order_buffer;
    
    // Wait for an empty slot (using semaphore on the slot)
    int slot = cb->tail;
    sem_wait(&cb->chair_sem[slot]);  // Wait for the slot to be available

    // Lock the buffer to ensure mutual exclusion when accessing shared memory
    sem_wait(&sharedMemory->mutex_buffer_order);

    // Enqueue the visitor by writing its PID to the buffer at the tail slot
    cb->order_buffer[slot] = value;

    // Advance the tail pointer
    cb->tail = (cb->tail + 1) % MAX_VISITORS;  // Wrap around if needed

    // Unlock the buffer after modifying the shared memory
    sem_post(&sharedMemory->mutex_buffer_order);

    // Signal that the slot has been filled by the current visitor
    sem_post(&cb->chair_sem[slot]);  // Signal that the slot is filled (if needed)

    // Lock the buffer
    // sem_wait(&sharedMemory->mutex_buffer_order);
    // // Wait for an empty slot
    // int next_tail = (cb->tail + 1) % MAX_VISITORS;
    // // Write value to the current tail
    // int slot = cb->tail;
    // cb->order_buffer[slot] = value;
    // cb->tail = next_tail; // Advance tail pointer
    // // Unlock the buffer
    // sem_post(&sharedMemory->mutex_buffer_order);
    // // Signal the slot semaphore
    // sem_post(&cb->chair_sem[slot]);

    // printf("Enqueued %d in slot %d\n", value, slot);
   
}

void dequeueOrder(SharedMemoryStruct *sharedMemory , pid_t *value) {
    OrderBuffer *cb = &sharedMemory->order_buffer;

    // Lock the buffer to check state and access the buffer
    sem_wait(&sharedMemory->mutex_buffer_order);

    // Check if the buffer is empty
    if (cb->head == cb->tail) {
        // Buffer is empty; no value to dequeue
        *value = -1;
        sem_post(&sharedMemory->mutex_buffer_order); // Unlock the buffer
        return;
    }

    // Determine the slot at head
    int slot = cb->head;

    // Unlock the buffer while waiting for the slot semaphore
    sem_post(&sharedMemory->mutex_buffer_order);
    sem_wait(&cb->chair_sem[slot]);
    

    // Lock the buffer
    sem_wait(&sharedMemory->mutex_buffer_order);

    // Retrieve the value from the current head
    *value = cb->order_buffer[slot];
    cb->head = (cb->head + 1) % MAX_VISITORS; // Advance head pointer

    // Unlock the buffer
    sem_post(&sharedMemory->mutex_buffer_order);

    // printf("Dequeued %d from slot %d\n", *value, slot);
}

void cleanupOrderBuffer(OrderBuffer *ob) {
    for (int i = 0; i < NUM_CHAIRS * NUM_TABLES; i++) {
        sem_destroy(&ob->chair_sem[i]);
    }
}