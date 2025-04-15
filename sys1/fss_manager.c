#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <stdbool.h> 
#include <fcntl.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h> 

#include "List.h"

#define PIPE_IN "fss_in"
#define PIPE_OUT "fss_out"
#define SIZE 256


struct dir_info {
    char source[SIZE];
    char target[SIZE];
    // int active_workers;
    // int error_count;

};

typedef struct dir_info *DirInfo;


int main(int argc, char *argv[]){

    char log_file[SIZE];
    char config_file[SIZE];
    int worker_limit =5;


    // Check if the number of parameters are correct
    if( argc!= 7){
        printf("The parameters of the input command are wrong");
        exit(1);
    }

    // Save the parameters in variables
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            strcpy(log_file, argv[++i]); 
        }
        else if (strcmp(argv[i], "-c") == 0 ) {
            strcpy(config_file, argv[++i]);
        }
        else if (strcmp(argv[i], "-n") == 0) {
            worker_limit = atoi(argv[++i]);
        }
        else {
            printf("Unknown parameter: %s\n", argv[i]);
            exit(1); 
        }
    }  
    
    // Create named pipes
    if (mkfifo(PIPE_IN, 0666) == -1) {
        perror("mkfifo");
    } 
    if (mkfifo(PIPE_OUT, 0666) == -1) {
        perror("mkfifo");
    } 

    // Create a list to store the directory pairs
    List dir_list = list_create(NULL, free);

    //Open the logfile for reading 
    int config_fd = open(config_file, O_RDONLY);
    if (config_fd == -1) {
        perror("Error opening input file");
        return 1;
    }

    // Variables to store data while reading
    char line[SIZE];
    int index = 0;
    char ch;
    ssize_t bytes;

    // Read the log file line by line
    while ((bytes = read(config_fd, &ch, 1)) == 1) {
        if (ch == '\n' || index >= SIZE - 1) {
            line[index] = '\0';

            if (index > 0) {
              
                DirInfo info = malloc(sizeof(struct dir_info));
                if (!info) {
                    perror("malloc");
                    exit(1);
                }

                if (sscanf(line, "%s %s", info->source, info->target) == 2) {
                    list_insert(dir_list, info);
                } else {
                    free(info); 
                }
            }

            index = 0; 
        } else {
            line[index++] = ch;
        }
    }
    if (index > 0) { 
        line[index] = '\0';
        DirInfo info = malloc(sizeof(struct dir_info));
        if (sscanf(line, "%s %s", info->source, info->target) == 2) {
            list_insert(dir_list, info);
        } else {
            free(info);
        }
    }

    // Close the config file
    close(config_fd);
  

    // Create the workers
    int active_workers = 0;
    for (int i = 0; i < worker_limit; i++) {
        if (active_workers < worker_limit) {
            
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                exit(1);
            }

            //child process
            if (pid == 0) {
                
                // Dynamically allocate memory for the variables and pipes we will put in the arguments of the exec         
                char **exec_args = malloc(3 * sizeof(char *));
                exec_args[0] = "./worker";
                exec_args[1] = PIPE_IN;
                exec_args[2] = PIPE_OUT;
                
                //the exec replaces this line in the process with a new program
                execvp(exec_args[0], exec_args);perror("exevp failed");// if the program will continue after the exec then the exec failed
                exit(0); 

            // parent process
            } else {
        
                // log_event(....);
            }
            
            
            active_workers++;
        } else {
            // Βαλε σε ουρα (queue) για εκτελεση πιο μετα
            break;
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < active_workers; i++) {
        wait(NULL);
    }
    
    //Unlink the named pipes
    unlink(PIPE_IN);
    unlink(PIPE_OUT);
}



    // // === Named pipe χρήση ===
    // pid_t pid = fork();
    // if (pid == 0) {
    //     // Child process - γράφει στο FIFO
    //     int fifo_write = open(PIPE_IN, O_WRONLY);
    //     const char *msg2 = "Hello from named pipe!";
    //     write(fifo_write, msg2, strlen(msg2) + 1);
    //     close(fifo_write);
    //     exit(0);
    // } else {
    //     // Parent process - διαβάζει από το FIFO
    //     int fifo_read = open(PIPE_IN, O_RDONLY);
    //     read(fifo_read, buffer, sizeof(buffer));
    //     printf("Read from named pipe: %s\n", buffer);
    //     close(fifo_read);
    // }