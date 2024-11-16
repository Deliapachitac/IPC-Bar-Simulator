#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h> // wait
#include <stdbool.h> //boolean
#include <fcntl.h> //for the O_WRONLY
#include <ctype.h> //for the alpha

#define BUFFER_SIZE 256

#include "Hash.h"

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

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    pid_t pid;
    int pipe_splitters1[2];
    if (pipe(pipe_splitters1) == -1) { 
        perror("pipe failed");
        exit(1);
    }
    int pipe_splitters2[2];
    if (pipe(pipe_splitters2) == -1) { 
        perror("pipe failed");
        exit(1);
    }
    int pipe_builders1[2];
    if (pipe(pipe_builders1) == -1) { 
        perror("pipe failed");
        exit(1);
    }
    
    int pipe_builder_splitter[numOfBuilders][2];  // Pipes between each builder and corresponding splitter
    for (int i = 0; i < numOfBuilders; i++) {
        if (pipe(pipe_builder_splitter[i]) == -1) {
            perror("Pipe for builder-splitter communication failed");
            exit(1);
        }
    }
    // for (int i = 0; i < numOfBuilders; i++) {

    //     // pid = fork();
    //     if (pid < 0) {
    //         perror("fork failed");
    //         exit(1);
    //     }
        
    //     if (pid == 0) {
    //         close(pipe_builders1[0]);  // Close the read end of the builder pipe
    //         printf("Builder %d with PID %d created.\n", i , getpid());
            

    //         exit(0);  
    //     }
    // }
    for (int i = 0; i < numOfSplitter; i++) {

        //Create a process
        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
        
        // Child process
        if (pid == 0) {
            
            close(pipe_splitters1[1]); // Close the write end of the pipe in the child process
            close(pipe_splitters2[1]);

            int received_lines_to_read ;
            read(pipe_splitters1[0], &received_lines_to_read, sizeof(received_lines_to_read));
            off_t offset_to_read;
            read(pipe_splitters2[0], &offset_to_read, sizeof(offset_to_read));
            
            close(pipe_splitters1[0]); // Close the read end after use
            close(pipe_splitters2[0]); // Close the read end after use
            close(pipe_builders1[1]);  // Close the read end of the builder pipe
            

            // Convert int and off_t values to strings
            char lines_to_read_str[10];
            char offset_str[20];

            // Use sprintf to convert the int and off_t values to strings
            sprintf(lines_to_read_str, "%d", received_lines_to_read);
            sprintf(offset_str, "%ld", offset_to_read);  // off_t is typically long int, so use %ld

            printf("splitter %d with PID %d created.\n", i , getpid());
            char *exec_args[] = {
                "./splitter",  // Assuming the compiled output of splitter.c is named "splitter"
                inputFile,     
                exclusionList,    
                lines_to_read_str,
                offset_str,    
                NULL           
            };
            execvp(exec_args[0], exec_args);
            perror("execl failed");
            exit(0); 
        }
    }
    
    if (pid > 0) {
        
        // Parent process
        close(pipe_splitters1[0]); // Close the read end of the pipe in the parent process
        close(pipe_splitters2[0]); // Close the read end of the pipe in the parent process
        close(pipe_builders1[1]);  // Close the write end of the builder pipe

        //Calculate how many lines will each splitter have 
        int add=0;
        
        int array[numOfSplitter];
        for (int i = 0; i < numOfSplitter; i++)
        {
            int calculating_lines = countline / numOfSplitter;
            if (countline % numOfSplitter != 0 ) {
                add++;
                if( add <= countline % numOfSplitter){
                    array[i]=calculating_lines+1;
                }else {
                    array[i]=calculating_lines;
                }
           
            }else {
                array[i]=calculating_lines;
            }
        }
        
        int line;
        for (int i = 0; i < numOfSplitter; i++)
        {   
            line=array[i]*i;
            
            write(pipe_splitters1[1], &array[i], sizeof(array[i])); 
            write(pipe_splitters2[1], &offsets[line], sizeof(offsets[line])); 
        }

        close(pipe_splitters1[1]); // Close the write end after writing
        close(pipe_splitters2[1]); // Close the write end after writing
        close(pipe_builders1[0]);  // Close the read end of the builder pipe

        // Parent waits for each child process to terminate
        for (int i = 0; i < numOfSplitter; i++) {
            wait(NULL); // Wait for child processes to terminate
        }
    }
   



    //Open the output file for writing (create if not exists)
    // int output_fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC);
    // if (output_fd == -1) {
    //     perror("Error opening output file");
    //     close(input_fd);
    //     return 0;
    // }
    // close(output_fd);


    return 0;
}


