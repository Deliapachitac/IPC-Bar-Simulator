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
#include <semaphore.h>

#define MAX_VISITORS 100 
#define NUM_TABLES 3
#define NUM_CHAIRS 4

#define MEMORY_NAME "/path_to_nemea"

//Struct for the statistics of the bar 
struct stats
{
    float avarage_waiting_time;
    float total_waiting_time;

    float avarage_staying_time;
    float total_staying_time;

    int counter_wine;
    int counter_water;
    int counter_salad;
    int counter_cheese;

    int total_visitors;
};

typedef struct
{
    pid_t waiting_buffer[MAX_VISITORS];
    int head;
    int tail;
    int count;
    sem_t position_sem[MAX_VISITORS];

}WaitingBuffer;

struct table
{
    bool full;
    pid_t chairs[4];//who sits where
    int full_chairs;
};

struct order_buffer
{
    int order_buffer[12];
    int front;
    int back;
    sem_t chair_sem[12];
};

typedef struct {
    int available[NUM_TABLES]; // Array to track availability of tables
    WaitingBuffer customerQueue; // Embedded circular buffer
} SharedMemoryStruct;


// void initBuffer(WaitingBuffer *cb);
// bool isFull(WaitingBuffer *cb);
// bool isEmpty(WaitingBuffer *cb);
// bool dequeue(WaitingBuffer *cb, int *value);
// bool enqueue(WaitingBuffer *cb, int value);