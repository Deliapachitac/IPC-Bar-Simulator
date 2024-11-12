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


// Function that removes punctuation marks from a word( for example: , . [] " )
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

// Check if the word is an integer
bool is_integer(const char *word) {

    for (int i = 0; word[i] != '\0'; i++) {
        if (!isdigit(word[i])) {
            return false; 
        }
    }
    return true; 
}

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
    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    int line_pos = 0;
    ssize_t bytes_read;
    int countline = 0;

    // Dynamic array to store words (start with an initial capacity)
    char **words = malloc(INITIAL_WORD_CAPACITY * sizeof(char *));
    if (words == NULL) {
        perror("Error allocating memory for words");
        close(input_fd);
        return 1;
    }

    int word_count = 0;
    int word_capacity = INITIAL_WORD_CAPACITY;


    // Count the lines of the file
    char ch;
    while ((bytes_read = read(input_fd, &ch, 1)) > 0) {
        if (ch == '\n') {
            countline++;
        }
    }

    // Move to the beginning of the file to read again
    lseek(input_fd, 0, SEEK_SET);

    // Read the file and print each word
    while ((bytes_read = read(input_fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n' || buffer[i] == ' ') {
            
                // Null-terminate the word and print it
                line[line_pos] = '\0';
                
                if ( line_pos > 0 ) {

                    // Resize the array if necessary
                    if (word_count >= word_capacity) {
                        word_capacity *= 2;  // Double the capacity
                        words = realloc(words, word_capacity * sizeof(char *));
                        if (words == NULL) {
                            perror("Error reallocating memory for words");
                            close(input_fd);
                            return 1;
                        }
                    }

                    
                    remove_punctuation(line);
                    if (strlen(line) > 0 && !is_integer(line)) {  // Check if it's not an integer
                        words[word_count] = strdup(line);
                        if (words[word_count] == NULL) {
                            perror("Error duplicating word");
                            close(input_fd);
                            return 1;
                        }

                        word_count++;
                        // dprintf(output_fd, "Word: %s\n", line);  // Write the last word to the output file
                    }
                }

                // Reset the line buffer for the next word
                line_pos = 0;
            } else {
                // Save the character to the line buffer
                line[line_pos++] = buffer[i];
            }
        }
    }

    // Handle any remaining word after the loop
    if (line_pos > 0 ) {
        line[line_pos] = '\0';
        if (word_count >= word_capacity) {
            word_capacity *= 2;
            words = realloc(words, word_capacity * sizeof(char *));
            if (words == NULL) {
                perror("Error reallocating memory for words");
                close(input_fd);
                return 1;
            }
        }
        remove_punctuation(line);
        if (strlen(line) > 0 && !is_integer(line)) {  // Check if it's not an integer
            words[word_count] = strdup(line);
            if (words[word_count] == NULL) {
                perror("Error duplicating word");
                close(input_fd);
                return 1;
            }

            word_count++;
            // dprintf(output_fd, "Word: %s\n", line);  // Write the last word to the output file
        }  
    }
    close(input_fd);












    int lines_per_splitter = countline / numOfSplitter;
    if (countline % numOfSplitter != 0) {
        lines_per_splitter++; // If lines don't divide evenly, some splitters will process one more line
    }

    pid_t pid;

    // // Loop to create numofsplitters child processes
    // for (i = 0; i < numOfSplitter; i++) {
    //     pid = fork(); // Create a new process (fork)

    //     if (pid == 0) {
    //         // Child process
    //         printf("Child %d PID: %d\n", i+1, getpid()); // Print child process ID
    //         sleep(1); // Simulate some work
    //         exit(0); // Terminate child process
    //     } else if (pid < 0) {

    //         perror("fork failed");
    //         exit(1);
    //     }
    // }
    
    // // Parent process
    // for (i = 0; i < numOfSplitter; i++) {
    //     // Wait for each child process to terminate
    //     wait(NULL); // Wait for child processes to terminate
    // }
    // // Print parent process ID
    // printf("Parent PID: %d\n", getpid()); // Print parent process ID

    

    for (int i = 0; i < numOfSplitter; i++) {
        
        // Create a pipe
        int pipefds[2];
        if (pipe(pipefds) == -1) { 
            perror("pipe failed");
            exit(1);
        }

        //Create a process
        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
        
        // Child process
        if (pid == 0) {
            
            close(pipefds[1]); // Close the write end of the pipe in the child process

            char buffer[100];
            read(pipefds[0], buffer, sizeof(buffer)); // Read from the pipe
            printf("Child %d PID: %d received: %s\n", i + 1, getpid(), buffer); // Print received message

            close(pipefds[0]); // Close the read end after use
            exit(0); // Terminate child process
        }else {
            // Parent process
            close(pipefds[0]); // Close the read end of the pipe in the parent process

            char message[] = "Hello from parent"; // Message to send to the child
            write(pipefds[1], message, sizeof(message)); // Write to the pipe

            close(pipefds[1]); // Close the write end after writing
        }
    }

    // Parent waits for each child process to terminate
    for (int i = 0; i < numOfSplitter; i++) {
        wait(NULL); // Wait for child processes to terminate
    }

    // Print parent process ID
    printf("Parent PID: %d\n", getpid()); // Print parent process ID














    //Open the output file for writing (create if not exists)
    int output_fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC);
    if (output_fd == -1) {
        perror("Error opening output file");
        close(input_fd);
        return 0;
    }

    // Now write all words to the output file
    for (int i = 0; i < word_count; i++) {
        dprintf(output_fd, "Word: %s\n", words[i]);
        free(words[i]);  // Free the memory allocated for each word
    }

    // Free the memory for the array of words
    free(words);

    close(output_fd);


    return 0;
}
