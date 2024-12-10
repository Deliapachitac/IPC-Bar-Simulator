#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
    
    //Reading and saving the variables from the command line 
    int shmid ;
    if(argc!=3){
        printf("Incorrect input command line\n");
        exit(0);
    }  
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            i++;
            shmid = atoi(argv[i]);
        } else {
            printf("Unknown flag : %s\n", argv[i]);
            exit(0);
        }
    }

    printf("Shared memory ID: %d\n", shmid);
    return 0 ;
}