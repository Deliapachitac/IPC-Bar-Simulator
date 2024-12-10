#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum DRINK_OPTIONS {
    WATER,
    WINE
};

enum FOOD_OPTIONS {
    CHEESE,
    SALAD
};

int main(int argc, char *argv[]) {
    
    //Reading and saving the variables from the command line  
    int ordertime ,shmid;   
    if(argc!=5) {
        printf("Incorrect input command line\n");
        exit(0);
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            i++;
            ordertime = atoi(argv[i]);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            i++;
            shmid = atoi(argv[i]);
        } else {
            printf("Unknown flag : %s\n", argv[i]);
            exit(0);
        }
    }

    printf("Maximum order time: %d\n", ordertime);
    printf("Shared memory ID: %d\n", shmid);

    return 0;
}