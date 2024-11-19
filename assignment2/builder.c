#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> //for the O_WRONLY

#include "Hash.h"
#define BUFFER_SIZE 256

int main(int argc, char *argv[]){

   
    
    int numOfBuilders = atoi(argv[1]);
  
    // Parse the pipe read ends
    int pipe_read_fd= atoi(argv[1]);
    
    // Buffer for reading data
    char buffer[BUFFER_SIZE];

    // Read from each pipe
    printf("Builder  (PID %d) reading from pipe %d...\n", getpid(), pipe_read_fd);

    ssize_t bytesRead;
    while ((bytesRead = read(pipe_read_fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the buffer
        printf("Builder received from pipe %d: %s\n", pipe_read_fd, buffer);
    }

    if (bytesRead == 0) {
        printf("EOF reached on pipe %d.\n", pipe_read_fd);
    } else if (bytesRead == -1) {
        perror("read failed");
    }
    


    
    
    exit(0);
}