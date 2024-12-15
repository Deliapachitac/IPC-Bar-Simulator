#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "segment.h"


int main(int argc, char *argv[]){
    
    //Reading and saving the variables from the command line 
    char shmname[50] ;
    if(argc!=3){
        printf("Incorrect input command line\n");
        exit(0);
    }  
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            i++;
            strcpy(shmname,argv[i]);
        } else {
            printf("Unknown flag : %s\n", argv[i]);
            exit(0);
        }
    }

    // Open shared memory
    int shm_fd = shm_open(shmname, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Map shared memory
    SharedMemoryStruct *sharedState = (SharedMemoryStruct *)mmap(NULL, sizeof(SharedMemoryStruct), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (sharedState == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    

    //stop the processes mutex_access
////////////

    // Open the output file for writing the statistics(create if not exists)
    int output_fd = open("outputFile", O_WRONLY | O_CREAT | O_TRUNC);
    if (output_fd == -1) {
        perror("Error opening output file");
        return 0;
    }
    dprintf(output_fd,"This are the statistics of the program\n");


    dprintf(output_fd,"The bar had %.6f total customers\n",sharedState->statistics.total_visitors);
    dprintf(output_fd,"The avarage staying time in the bar per customer was %.6f\n",sharedState->statistics.avarage_staying_time);
    dprintf(output_fd,"The avarage waiting time to enter thhe bar per customer was \n",sharedState->statistics.avarage_waiting_time);
    dprintf(output_fd,"The receptionist has prepared %d plates with cheeses\n",sharedState->statistics.counter_cheese);
    dprintf(output_fd,"The receptionist has prepared %d salads \n",sharedState->statistics.counter_salad);
    dprintf(output_fd,"The receptionist has prepared %d cups of water \n",sharedState->statistics.counter_water);
    dprintf(output_fd,"The receptionist has prepared %d glasses of wine\n",sharedState->statistics.counter_wine);
    dprintf(output_fd,"The total staying time in the bar is %.6f \n",sharedState->statistics.total_staying_time);
    dprintf(output_fd,"The total waiting time in the bar is %.6f\n",sharedState->statistics.total_waiting_time);

    close(output_fd);


    return 0 ;
}