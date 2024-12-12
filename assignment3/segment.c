#include "segment.h"


void initBuffer(WaitingBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

bool isFull(WaitingBuffer *cb) {
    return cb->count == MAX_VISITORS;
}

bool isEmpty(WaitingBuffer *cb) {
    return cb->count == 0;
}

bool dequeue(WaitingBuffer *cb, int *value) {
    if (isEmpty(cb)) {
        printf("Buffer is empty. Cannot dequeue.\n");
        return false;
    }
    *value = cb->waiting_buffer[cb->head];
    cb->head = (cb->head + 1) % MAX_VISITORS;
    cb->count--;
    return true;
}

bool enqueue(WaitingBuffer *cb, int value) {
    if (isFull(cb)) {
        printf("Buffer is full. Cannot enqueue %d\n", value);
        return false;
    }
    cb->waiting_buffer[cb->tail] = value;
    cb->tail = (cb->tail + 1) % MAX_VISITORS;
    cb->count++;
    return true;
}