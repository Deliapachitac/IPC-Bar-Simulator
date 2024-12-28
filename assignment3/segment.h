#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/times.h>  
#include <unistd.h>      

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

//Struct for the waiting buffer of the bar which represents the queue of the bar
typedef struct
{
    pid_t waiting_buffer[MAX_VISITORS]; // Array to store the PIDs of the visitors
    int head;
    int tail;
    sem_t position_sem[MAX_VISITORS]; // Semaphore for each position in the buffer

}WaitingBuffer;

//Struct for the tables of the bar
typedef struct 
{
    bool full; //if the table is full or not
    pid_t chairs[NUM_CHAIRS]; //who sits where
    int full_chairs; //how many chairs are full in the table 

}Table;

//Struct for the order buffer of the bar which represents the queue of the receptionist
typedef struct 
{
    pid_t order_buffer[NUM_CHAIRS*NUM_TABLES];
    int head;
    int tail;
    sem_t order_ready[NUM_CHAIRS*NUM_TABLES];

}OrderBuffer;

//Struct for the shared memory that is used by the processes
typedef struct {
    WaitingBuffer waiting_buffer;  
    sem_t mutex_buffer_wait; // Semaphore for mutual exclusion of the  waiting buffer
    
    OrderBuffer receprionist_buffer;
    sem_t mutex_buffer_order; // Semaphore for mutual exclusion of the order buffer
    sem_t buffer_empty; // Semaphore to signal that the order buffer is empty
    sem_t buffer_full; // Semaphore to signal that the order buffer is full

    Stats statistics;
    sem_t mutex_access; // We need a semaphore so that the monitor can access the shared memory struct without the visitors interfering
    
    Table table[NUM_TABLES];
    sem_t table_mutex; // Semaphore for mutual exclusion of the table
    sem_t table_reset; // Semaphore to signal that a table has been reset so that the visitors can check again for an empty table
     
    sem_t receptionist_access;// Semaphore to signal that a visitor has found an empty seat and the receptionist should take the order

    sem_t logging; // Semaphore for mutual exclusion of the log file

    //This variable is used to terminate the receptionist process 
    // It keeps track of the number of visitors that have been served
    int served_visitors;

} SharedMemoryStruct;

//Struct for the order of the visitors that it isnt included in the shared memory struct
typedef struct {
    int water;
    int wine;
    int cheese;
    int salad;
} Order;


// Functions for the Buffers (iniitialization, insertion, removal, cleanup)
void initWaitingBuffer(WaitingBuffer *cb);
void enqueueWaiting(SharedMemoryStruct *sharedMemory, pid_t value);
void dequeueWaiting(SharedMemoryStruct *sharedMemory, pid_t *value);
void cleanupWaitingBuffer(WaitingBuffer *cb );

void initOrderBuffer(OrderBuffer *ob);
int enqueueOrder(SharedMemoryStruct *sharedMemory, pid_t value);
int dequeueOrder(SharedMemoryStruct *sharedMemory, pid_t *value);
void cleanupOrderBuffer(OrderBuffer *ob);