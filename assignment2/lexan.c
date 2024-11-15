#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h> // wait
#include <stdbool.h> //boolean
#include <fcntl.h> //for the O_WRONLY
#include <ctype.h> //for the alpha

#define BUFFER_SIZE 256
#define INITIAL_WORD_CAPACITY 200

struct transferdata
{
    int received_lines;
    int line_position;
};

typedef struct tranferdata* TranferData;

// Function that removes punctuation marks from a word( for example: , . [] " )
void remove_punctuation(char *word) ;

// Check if the word is an integer
bool is_integer(const char *word);
 
//read a file 
void readfile(const char *filename,const char *outputname, int numread, off_t offset);

int main(int argc, char *argv[]) {

    //Variables to save the input parameters
    char inputFile[20];
    char exclusionList[20];
    char outputFile[20];
    int numOfSplitter,numOfBuilders,topPopular;
    
    // Check if the number of parameters are correct
    if( argc!= 13){
        printf("The parameters of the input command are wrong");
        exit(1);
    }

    // Save the parameters in variables
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 ) {
            strcpy(inputFile, argv[++i]); 
        }
        else if (strcmp(argv[i], "-l") == 0) {
            numOfSplitter = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-m") == 0 ) {
            numOfBuilders = atoi(argv[++i]); 
        }
        else if (strcmp(argv[i], "-t") == 0) {
            topPopular = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-e") == 0 ) {
            strcpy(exclusionList, argv[++i]); 
        }
        else if (strcmp(argv[i], "-o") == 0 ) {
            strcpy(outputFile, argv[++i]); 
        }
        else {
            printf("Unknown parameter: %s\n", argv[i]);
            exit(1); 
        }
    }

    //Open the input file for reading 
    int input_fd = open(inputFile, O_RDONLY);
    if (input_fd == -1) {
        perror("Error opening input file");
        return 1;
    }

    // Variables to store data while reading
    ssize_t bytes_read;
    int countline = 0;

    // Count the lines of the file
    char ch;
    while ((bytes_read = read(input_fd, &ch, 1)) > 0) {
        if (ch == '\n') {
            countline++;
        }
    }

    //Close the input file
    close(input_fd);

    //open again the input file to map the first byte of every line 
    input_fd = open(inputFile, O_RDONLY);
    if (input_fd == -1) {
        perror("Error opening input file");
        return 1;
    }


    off_t offsets[countline];
    off_t byte_offset = 0;
    int line_number = 0;

    if (line_number < countline) {
        offsets[line_number] = byte_offset;
    }

    while (read(input_fd, &ch, 1) == 1) {
        byte_offset++;
        if (ch == '\n') {
            line_number++;
            if (line_number < countline) {
                offsets[line_number] = byte_offset;
            }
        }
    }

    //Close the input file
    close(input_fd);
    

    // Read the file and print each word
    // char line[BUFFER_SIZE];
    // int line_pos = 0;
    // char buffer[BUFFER_SIZE];
    // while ((bytes_read = read(input_fd, buffer, sizeof(buffer))) > 0) {
    //     for (int i = 0; i < bytes_read; i++) {
    //         if (buffer[i] == '\n' || buffer[i] == ' ') {       
    //             // Null-terminate the word and print it
    //             line[line_pos] = '\0';         
    //             if ( line_pos > 0 ) {
    //                 // Resize the array if necessary
    //                 if (word_count >= word_capacity) {
    //                     word_capacity *= 2;  // Double the capacity
    //                     words = realloc(words, word_capacity * sizeof(char *));
    //                     if (words == NULL) {
    //                         perror("Error reallocating memory for words");
    //                         close(input_fd);
    //                         return 1;
    //                     }
    //                 }              
    //                 remove_punctuation(line);
    //                 if (strlen(line) > 0 && !is_integer(line)) {  // Check if it's not an integer
    //                     words[word_count] = strdup(line);
    //                     if (words[word_count] == NULL) {
    //                         perror("Error duplicating word");
    //                         close(input_fd);
    //                         return 1;
    //                     }
    //                     word_count++;
    //                     // dprintf(output_fd, "Word: %s\n", line);  // Write the last word to the output file
    //                 }
    //             }
    //             // Reset the line buffer for the next word
    //             line_pos = 0;
    //         } else {
    //             // Save the character to the line buffer
    //             line[line_pos++] = buffer[i];
    //         }
    //     }
    // }
    // Handle any remaining word after the loop
    // if (line_pos > 0 ) {
    //     line[line_pos] = '\0';
    //     if (word_count >= word_capacity) {
    //         word_capacity *= 2;
    //         words = realloc(words, word_capacity * sizeof(char *));
    //         if (words == NULL) {
    //             perror("Error reallocating memory for words");
    //             close(input_fd);
    //             return 1;
    //         }
    //     }
    //     remove_punctuation(line);
    //     if (strlen(line) > 0 && !is_integer(line)) {  // Check if it's not an integer
    //         words[word_count] = strdup(line);
    //         if (words[word_count] == NULL) {
    //             perror("Error duplicating word");
    //             close(input_fd);
    //             return 1;
    //         }
    //         word_count++;
    //         // dprintf(output_fd, "Word: %s\n", line);  // Write the last word to the output file
    //     }  
    // }
    




    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    printf("The file has lines:%d\n",countline);

    pid_t pid;
    int pipefds1[2];
    if (pipe(pipefds1) == -1) { 
        perror("pipe failed");
        exit(1);
    }
    int pipefds2[2];
    if (pipe(pipefds2) == -1) { 
        perror("pipe failed");
        exit(1);
    }


    for (int i = 0; i < numOfSplitter; i++) {

        //Create a process
        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
        
        // Child process
        if (pid == 0) {
            
            close(pipefds1[1]); // Close the write end of the pipe in the child process
            close(pipefds2[1]);

            int received_lines_to_read ;
            read(pipefds1[0], &received_lines_to_read, sizeof(received_lines_to_read));
            off_t offset_to_read;
            read(pipefds2[0], &offset_to_read, sizeof(offset_to_read));
            
            
            readfile(inputFile,outputFile,received_lines_to_read, offset_to_read );
            
            printf("Child %d PID: %d received: %ld\n", i + 1, getpid(), offset_to_read); // Print received message
            
            
            close(pipefds1[0]); // Close the read end after use
            close(pipefds2[0]); // Close the read end after use
            exit(0); // Terminate child process
        }
    }
    if (pid > 0) {
        // Parent process
        close(pipefds1[0]); // Close the read end of the pipe in the parent process
        close(pipefds2[0]); // Close the read end of the pipe in the parent process
        
        //Calculate how many lines will each splitter have 
        int calculating_lines = countline / numOfSplitter;
        // if (countline % numOfSplitter != 0) {
        //     calculating_lines++; // If lines don't divide evenly, some splitters will process one more line
        // }
        
        int line;
        for (int i = 0; i < numOfSplitter; i++)
        {   
            line=calculating_lines*i;
            // printf("The line number :%d\n", line);
            write(pipefds1[1], &calculating_lines, sizeof(calculating_lines)); 
            write(pipefds2[1], &offsets[line], sizeof(offsets[line])); 
        }

        close(pipefds1[1]); // Close the write end after writing
        close(pipefds2[1]); // Close the write end after writing

        // Parent waits for each child process to terminate
        for (int i = 0; i < numOfSplitter; i++) {
            wait(NULL); // Wait for child processes to terminate
        }
    }
   

    //Open the output file for writing (create if not exists)
    int output_fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC);
    if (output_fd == -1) {
        perror("Error opening output file");
        close(input_fd);
        return 0;
    }

    // Now write all words to the output file
    // for (int i = 0; i < word_count; i++) {
    //     dprintf(output_fd, "Word: %s\n", words[i]);
    //     free(words[i]);  // Free the memory allocated for each word
    // }
    // // Free the memory for the array of words
    // free(words);

    close(output_fd);


    return 0;
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