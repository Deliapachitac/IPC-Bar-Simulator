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

#define MAX_VISITORS 20
#define NUM_TABLES 3
#define NUM_CHAIRS 4


//Struct for the statistics of the bar 
typedef struct
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
}Stats;

typedef struct
{
    pid_t waiting_buffer[MAX_VISITORS];
    int head;
    int tail;
    int count;
    sem_t position_sem[MAX_VISITORS];

}WaitingBuffer;

typedef struct 
{
    bool full;
    pid_t chairs[NUM_CHAIRS];//who sits where
    int full_chairs; //how many chairs are full in the table 
    sem_t table_sem;
}Table;

typedef struct 
{
    int order_buffer[NUM_CHAIRS*NUM_TABLES];
    int head;
    int tail;
    int count;
    sem_t chair_sem[NUM_CHAIRS*NUM_TABLES];
}OrderBuffer;

typedef struct {
    WaitingBuffer customerQueue;  
    sem_t waiting_buffer_access; 
    Stats statistics;
    Table table[NUM_TABLES];

    sem_t mutex_access;
    sem_t receptionist_access;
    

} SharedMemoryStruct;


void initBuffer(WaitingBuffer *cb,SharedMemoryStruct *sharedState);
// bool isFull(WaitingBuffer *cb,SharedMemoryStruct *sharedState);
// bool isEmpty(WaitingBuffer *cb,SharedMemoryStruct *sharedState);
bool dequeue(WaitingBuffer *cb,int *value);
bool enqueue(WaitingBuffer *cb, int value);