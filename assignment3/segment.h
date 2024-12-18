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

#define MAX_VISITORS 50
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
    sem_t position_sem[MAX_VISITORS];

}WaitingBuffer;

typedef struct 
{
    bool full;
    pid_t chairs[NUM_CHAIRS];//who sits where
    int full_chairs; //how many chairs are full in the table 
    // sem_t table_sem; 
}Table;

typedef struct 
{
    pid_t order_buffer[NUM_CHAIRS*NUM_TABLES];
    int head;
    int tail;
    sem_t chair_sem[NUM_CHAIRS*NUM_TABLES];
}OrderBuffer;

typedef struct {
    int water;
    int wine;
    int cheese;
    int salad;
} Order;

typedef struct {
    WaitingBuffer customerQueue;  
    sem_t mutex_buffer; // Semaphore for mutual exclusion of the buffer
    sem_t full_buffer; // Semaphore for full buffer
    sem_t empty_buffer; // Semaphore for empty buffer

    Stats statistics;
    sem_t mutex_access; // We need a semaphore so that the monitor can access the shared memory struct without the visitors interfering
    
    Table table[NUM_TABLES];
    sem_t total_table_sem;
     
    OrderBuffer order_Buffer;
    sem_t receptionist_access;

} SharedMemoryStruct;


void initBuffer(WaitingBuffer *cb);
void enqueue(SharedMemoryStruct *sharedMemory, pid_t value);
void dequeue(SharedMemoryStruct *sharedMemory, pid_t *value);
void cleanupBuffer(WaitingBuffer *cb );
void displayBuffer(WaitingBuffer *cb);

void initOrderBuffer(OrderBuffer *ob);
bool enqueueOrder(OrderBuffer *ob, int value);
bool dequeueOrder(OrderBuffer *ob, int *value);
void cleanupOrderBuffer(OrderBuffer *ob);