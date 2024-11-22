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
  
    //  pipe read ends
    int pipe_read_fd= atoi(argv[1]);
    int pipe_write_fd= atoi(argv[3]);
    
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
                    
                    // printf("Word: %s\n", word);
                }
                start = i + 1; // Move to the start of the next word
            }
        }
    }

    if (bytesRead == -1) {
        perror("read failed");
    }



    HashNode node = hash_first(mytable);
    while (node != NULL) {
        char *value = hash_find_value(mytable, node);
        int frequency_number=get_counter(mytable,value);
        if (value == NULL) {
            break; // Safety check, should not happen
        }

        int length = strlen(value);
        if (write(pipe_write_fd, &length, sizeof(length)) == -1) {
            perror("write length to pipe failed");
        }
        if (write(pipe_write_fd, value, strlen(value)) == -1) {
            perror("write word to pipe failed");
        }
        // Write the frequency number
        if (write(pipe_write_fd, &frequency_number, sizeof(frequency_number)) == -1) {
            perror("write frequency to pipe failed");
        }
        node = hash_next(mytable, node);
    }
    
    int termination_marker = 0;
    if (write(pipe_write_fd, &termination_marker, sizeof(termination_marker)) == -1) {
        perror("write termination marker failed");
    }

    
    // if(hash_find(mytable,"the")!=NULL){
    //     printf("the number  %d\n",get_counter(mytable,"the"));
    // }
        
    // printf("den yparxeiiiii %s \n",(char *)hash_find_value(mytable, hash_first(mytable)));
    
    if (kill(getppid(), SIGUSR2) == -1) {
        perror("Failed to send SIGUSR2 to parent");
    }
    exit(0);
}