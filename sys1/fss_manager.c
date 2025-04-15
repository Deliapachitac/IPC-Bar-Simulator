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



#include <sys/times.h> 
#include <ctype.h> 

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

    //Open the logfile for reading 
    int input_fd = open(log_file, O_RDONLY);
    if (input_fd == -1) {
        perror("Error opening input file");
        return 1;
    }

    // Create named pipes
    if (mkfifo(PIPE_IN, 0666) == -1) {
        perror("mkfifo");
    } 
    if (mkfifo(PIPE_OUT, 0666) == -1) {
        perror("mkfifo");
    } 

    char buffer[SIZE];

    // === Named pipe χρήση ===
    pid_t pid = fork();
    if (pid == 0) {
        // Child process - γράφει στο FIFO
        int fifo_write = open(PIPE_IN, O_WRONLY);
        const char *msg2 = "Hello from named pipe!";
        write(fifo_write, msg2, strlen(msg2) + 1);
        close(fifo_write);
        exit(0);
    } else {
        // Parent process - διαβάζει από το FIFO
        int fifo_read = open(PIPE_IN, O_RDONLY);
        read(fifo_read, buffer, sizeof(buffer));
        printf("Read from named pipe: %s\n", buffer);
        close(fifo_read);
    }

    // Καθαρίζουμε το named pipe
    unlink(PIPE_IN);
    unlink(PIPE_OUT);
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/inotify.h>
// #include <unistd.h>

// #define EVENT_SIZE (sizeof(struct inotify_event))
// #define BUF_LEN (1024 * (EVENT_SIZE + 16))

// int main() {
//     int fd = inotify_init();
//     if (fd < 0) {
//         perror("inotify_init");
//         exit(1);
//     }

//     int wd = inotify_add_watch(fd, "./mydir", IN_CREATE | IN_MODIFY | IN_DELETE);

//     char buffer[BUF_LEN];

//     printf("Monitoring ./mydir...\n");
//     while (1) {
//         int length = read(fd, buffer, BUF_LEN);
//         if (length < 0) {
//             perror("read");
//         }

//         int i = 0;
//         while (i < length) {
//             struct inotify_event *event = (struct inotify_event *)&buffer[i];
//             if (event->len) {
//                 if (event->mask & IN_CREATE)
//                     printf("File created: %s\n", event->name);
//                 else if (event->mask & IN_DELETE)
//                     printf("File deleted: %s\n", event->name);
//                 else if (event->mask & IN_MODIFY)
//                     printf("File modified: %s\n", event->name);
//             }
//             i += EVENT_SIZE + event->len;
//         }
//     }

//     inotify_rm_watch(fd, wd);
//     close(fd);

//     return 0;
// }
