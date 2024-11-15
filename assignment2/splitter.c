#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h> // wait
#include <stdbool.h> //boolean
#include <fcntl.h> //for the O_WRONLY
#include <ctype.h> //for the alpha

#define BUFFER_SIZE 256


// Function that removes punctuation marks from a word( for example: , . [] " )
void remove_punctuation(char *word) ;

// Check if the word is an integer
bool is_integer(const char *word);
 
//read a file 
void readfile(const char *filename,const char *outputname, int numread, off_t offset);

int main (int argc, char *argv[]){


    if(argc!= 5){
        printf("The parameters of the input command are wrong");
        exit(1);
    }

    readfile(argv[1],argv[2],atoi(argv[3]),atoi(argv[4]));
}

void readfile(const char *filename,const char *outputname, int lineread, off_t offset) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    //Open the output file for writing (create if not exists)
    int output_fd = open(outputname, O_WRONLY | O_CREAT | O_TRUNC);
    if (output_fd == -1) {
        perror("Error opening output file");
        close(fd);
        exit(1);
    }

    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("Error seeking file");
        close(fd);
        exit(1);
    }

    char line[BUFFER_SIZE];
    int line_pos = 0;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int countline = 0;
    char carry_over[BUFFER_SIZE] = ""; // Buffer to store partial word from the last read



    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        int start_index = 0;

        // If there is a carry-over from the last read, prepend it to the current buffer
        if (strlen(carry_over) > 0) {
            for (int i = 0; carry_over[i] != '\0'; i++) {
                line[line_pos++] = carry_over[i];
            }
            carry_over[0] = '\0'; // Clear the carry-over buffer
        }
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n' || buffer[i] == ' ') {       
                
                line[line_pos] = '\0';         
                if ( line_pos > 0 ) {
           
                    remove_punctuation(line);
                    if (strlen(line) > 0 && !is_integer(line)) {  // Check if it's not an integer
    
                        printf( "Word: %s\n", line);  // Write the last word to the output file
                    }
                }

                // Reset the line buffer for the next word
                line_pos = 0;
                countline++;
                
                // Stop reading if the desired number of lines is reached
                if (countline >= lineread) {
                    break;
                }
            } else {
                // Save the character to the line buffer
                line[line_pos++] = buffer[i];

                // Check if we are at the end of the buffer without reaching a delimiter
                if (i == bytes_read - 1 && buffer[i] != '\n' && buffer[i] != ' ') {
                    // Store the partial word in carry_over
                    line[line_pos] = '\0'; // Null-terminate
                    strcpy(carry_over, line);
                    line_pos = 0; // Reset the line position
                }

            }
        }
    }
    // Handle any remaining word after the loop
    if (line_pos > 0 ) {
        line[line_pos] = '\0';

        remove_punctuation(line);
        if (strlen(line) > 0 && !is_integer(line)) {  // Check if it's not an integer
        
            dprintf(output_fd, "Word: %s\n", line);  // Write the last word to the output file
        }  
    }


    // free(buffer);
    close(fd);
    close(output_fd);
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