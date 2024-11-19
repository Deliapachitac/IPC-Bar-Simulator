#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h> //boolean
#include <fcntl.h> //for the O_WRONLY
#include <ctype.h> //for the alpha

#define BUFFER_SIZE 256

#include "Hash.h"


// Function that removes punctuation marks from a word( for example: , . [] " )
void remove_punctuation(char *word) ;

// Check if the word is an integer
bool is_integer(const char *word);

int hash_for_builders(const char *str, int numbuilders);

int main (int argc, char *argv[]){


    // if(argc!= 6){
    //     printf("The parameters of the input command are wrong");
    //     exit(1);
    // }
    
    ssize_t bytes_read;
    char line[BUFFER_SIZE];
    int line_pos = 0;
    char buffer[BUFFER_SIZE];
    int countline_execlusion = 0;

    int fd_exclusionlist = open(argv[2], O_RDONLY);
    if (fd_exclusionlist == -1) {
        perror("Error opening file");
        exit(1);
    }
    
    // Count the lines of the file
    char ch;
    while ((bytes_read = read(fd_exclusionlist, &ch, 1)) > 0) {
        if (ch == '\n') {
            countline_execlusion++;
        }
    }
    
    lseek(fd_exclusionlist, 0, SEEK_SET);
    char temp_word[50];
    HashTable exclusion_table = create_hash_table(countline_execlusion^2);

    while ((bytes_read = read(fd_exclusionlist, buffer, sizeof(buffer))) > 0) {
        
        for (int i = 0; i < bytes_read; i++) {
            
            if (buffer[i] == '\n' ) {       
                
                line[line_pos] = '\0';         
                if ( line_pos > 0 ) {
           
                    strcpy(temp_word,line);
                    hash_add(exclusion_table,temp_word);  
                }

                line_pos = 0;
                
            } else {
                // Save the character to the line buffer
                line[line_pos++] = buffer[i];

            }
            
           
        }
        
    }
   
    close(fd_exclusionlist);

    //read input file
    int lineread = atoi(argv[3]);
    off_t offset=atoi(argv[4]);
    int numOfBuilders = atoi(argv[5]);
    int *pipe_write_fds = malloc(numOfBuilders * sizeof(int));
    if (!pipe_write_fds) {
        perror("malloc failed");
        return 1;
    }

    for (int i = 0; i < numOfBuilders; i++) {
        pipe_write_fds[i] = atoi(argv[6 + i]);
    }


    // Simulate data processing and writing to pipes
    // for (int i = 0; i < numOfBuilders; i++) {
    //     char message[100];
    //     snprintf(message, sizeof(message), "%s", word_to_send);

    //     // Write to the corresponding pipe
    //     if (write(pipe_write_fds[i], message, strlen(message)) == -1) {
    //         perror("write to pipe failed");
    //         free(pipe_write_fds);
    //         return 1;
    //     }
    // }

    

    line_pos = 0;
    int countline = 0;
    int flag = 0;

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("Error seeking file");
        close(fd);
        exit(1);
    }

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        
        for (int i = 0; i < bytes_read; i++) {
            // Stop reading if the desired number of lines is reached
            if (countline >= lineread) {
                flag = 1;
                break;
            }
            if (buffer[i] == '\n' || buffer[i] == ' ') {       
                
                line[line_pos] = '\0';         
                if ( line_pos > 0 ) {
           
                    remove_punctuation(line);
                    if (strlen(line) > 0 && !is_integer(line)) {  // Check if it's not an integer
                        
                        if (hash_find(exclusion_table, line) == NULL) {
                            
                            
                            int index_builder= hash_for_builders(line,numOfBuilders);

                            // Write to the corresponding pipe
                            if (write(pipe_write_fds[index_builder], line, strlen(line)) == -1) {
                                perror("write to pipe failed");
                                free(pipe_write_fds);
                                return 1;
                            }

                            // printf("The line is %d Word: %s\n", countline, line);  // Print word if not in hash table
                        }
                    
                    }
                }

                line_pos = 0;
                if(buffer[i] == '\n'){
                    countline++;
                }
                
            } else {
                // Save the character to the line buffer
                line[line_pos++] = buffer[i];

            }
           
        }
        if (flag) {
            break;  // break the outer loop if the flag is set
        }
    }

    close(fd);
    
    delete_hash_table(exclusion_table);
    exit(0);
}

int hash_for_builders(const char *str, int numbuilders){

    unsigned long hash = 5381;  // Starting value for djb2
    int c;

    // Iterate over each character of the string
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    return hash % numbuilders;
}


void remove_punctuation(char *word) {
    int i = 0, j = 0;
    while (word[i] != '\0') {
        //The function "isalpha" distinguish the alphabetic characters from the non-alphabetic
        if (isalpha(word[i])) { 
            word[j++] = word[i];
        }
        i++;
    }
    word[j] = '\0';  
}

bool is_integer(const char *word) {

    for (int i = 0; word[i] != '\0'; i++) {
        if (!isdigit(word[i])) {
            return false; 
        }
    }
    return true; 
}