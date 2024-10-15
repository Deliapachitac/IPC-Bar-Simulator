#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 10000000

int main(int argc, char *argv[]){

    if (argc != 3) {
        printf( "The program has more arguments than needed");
        exit(EXIT_FAILURE);
    }

    int sourfile, destfile;

    // opening the files and check for failures
    sourfile = open(argv[1], O_RDONLY);
    if (sourfile <0) {
        printf("The file is not opened ");
        exit(EXIT_FAILURE);
    }
    destfile = open(argv[2], O_WRONLY );
    if (destfile <0) {
        printf("The file is not opened ");
        exit(EXIT_FAILURE);
    }
 
    // copy the 
    int read_var, write_var;
    char buffer[BUFFER_SIZE];
    if ((read_var = read(sourfile, buffer, BUFFER_SIZE)) > 0) {
        write_var = write(destfile, buffer, read_var);
        if(write_var < 0){
            printf("The write had a problem");
            exit(EXIT_FAILURE);
        }

    }else {
        printf("The read had a problem");
        exit(EXIT_FAILURE);
    }

    close(sourfile);
    close(destfile);
    return 0;
}

