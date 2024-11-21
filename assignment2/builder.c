#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> //for the O_WRONLY
#include <signal.h>

#include "Hash.h"
#define BUFFER_SIZE 256




int main(int argc, char *argv[]){

   
    int numOfBuilders = atoi(argv[1]);
    int countline = atoi(argv[2]);
  
    // Parse the pipe read ends
    int pipe_read_fd= atoi(argv[1]);
    
    // Buffer for reading data
    char buffer[BUFFER_SIZE];

    HashTable mytable=create_hash_table(countline);

    ssize_t bytesRead;
    while ((bytesRead = read(pipe_read_fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the buffer
        
        int start = 0; // Start index of the current word
        for (int i = 0; i <= bytesRead; i++) {
            // Check for a space or end of buffer
            if (buffer[i] == ' ' || buffer[i] == '\0') {
                if (i > start) { // Ensure there is a word to print
                    char word[i - start + 1]; // Temporary storage for the word
                    strncpy(word, &buffer[start], i - start);
                    word[i - start] = '\0'; // Null-terminate the word

                    hash_add(mytable,strdup(word));
                    printf("Word: %s\n", word);
                }
                start = i + 1; // Move to the start of the next word
            }
        }
    }

    if (bytesRead == -1) {
        perror("read failed");
    }
    
    if(hash_find(mytable,"the")!=NULL){
        printf("the number  %d\n",get_counter(mytable,"the"));
    }
        
    // printf("den yparxeiiiii %s \n",(char *)hash_find_value(mytable, hash_first(mytable)));
    
    if (kill(getppid(), SIGUSR2) == -1) {
        perror("Failed to send SIGUSR2 to parent");
    }
    exit(0);
}