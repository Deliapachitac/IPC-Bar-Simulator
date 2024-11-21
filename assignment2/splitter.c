#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h> //boolean
#include <fcntl.h> //for the O_WRONLY
#include <ctype.h> //for the alpha
#include <signal.h>  // For kill(

#define BUFFER_SIZE 256

#include "Hash.h"


// Function that removes punctuation marks from a word( for example: , . [] " )
void remove_punctuation(char *word) ;

//
void transform_capitals(char *str);

// Check if the word is an integer
bool is_integer(const char *word);

//
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
    HashTable exclusion_table = create_hash_table(countline_execlusion*countline_execlusion);

    while ((bytes_read = read(fd_exclusionlist, buffer, sizeof(buffer))) > 0) {
        
        for (int i = 0; i < bytes_read; i++) {
            
            if (buffer[i] == '\n' ) {       
                
                line[line_pos] = '\0';         
                if ( line_pos > 0 ) {
           
                    hash_add(exclusion_table,strdup(line));  
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
                        transform_capitals(line);
                        if (hash_find(exclusion_table, line) == NULL) {
                            
                            
                            int index_builder= hash_for_builders(line,numOfBuilders);

                            // Append a space to the word before writing
                            strcat(line, " ");

                            // Write to the corresponding pipe
                            if (write(pipe_write_fds[index_builder], line, strlen(line)) == -1) {
                                perror("write to pipe failed");
                                free(pipe_write_fds);
                                return 1;
                            }

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
    if ( kill(getppid(), SIGUSR1)) {
        perror("Failed to send SIGUSR1 to parent");
    }
    
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

void transform_capitals(char *str) {
    int i = 0;
    while (str[i] != '\0') { // Traverse until the end of the string
        str[i] = tolower(str[i]); // Convert character to lowercase
        i++;
    }
}