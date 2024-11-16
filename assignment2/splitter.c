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
 
//read a file 
void readfile(const char *filename, int numread, off_t offset);

int main (int argc, char *argv[]){


    if(argc!= 5){
        printf("The parameters of the input command are wrong");
        exit(1);
    }
    
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
    HashTable exclusion_table = create_hash_table(countline_execlusion);

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
    
                        if (!hash_find(exclusion_table, line)) {
                            printf("The line is %d Word: %s\n", countline, line);  // Print word if not in hash table
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