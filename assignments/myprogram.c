#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "ADTGraph.h"
#include "ADTHash.h"

#define BUFFER_SIZE 256


int main(int argc, char *argv[]){

    // int opt;
    // char *inputFile = NULL;
    // char *outputFile = NULL;
    // printf("argv %s",argv[2]);

    // "i:o:" means -i requires an argument, -o requires an argument
    // while ((opt = getopt(argc, argv, "i:o:")) != -1) {
    //     switch (opt) {
    //         case 'i':
    //             inputFile = optarg;  // optarg holds the argument for -i
    //             break;
    //         case 'o':
    //             outputFile = optarg; // optarg holds the argument for -o
    //             break;
    //         case '?': // Handle unknown options
    //             if (optopt == 'i' || optopt == 'o') {
    //                 fprintf(stderr, "Option -%c requires an argument.\n", optopt);
    //             } else {
    //                 fprintf(stderr, "Unknown option: -%c\n", optopt);
    //             }
    //             return 1;
    //     }
    // }
    
    // Open the input file for reading only  
    int input_fd = open(argv[2], O_RDONLY);
    if (input_fd == -1) {
        perror("Error opening input file");
        return 1;
    }
    
    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    int line_pos = 0;
    ssize_t bytes_read;
    int node_id_source, node_id_dest, money;
    char date[20];

    // Create the graph that we will use to enter the data from tyhe input file 
    Graph mygraph =graph_create();

    // Read the input file chunk by chunk
    while ((bytes_read = read(input_fd, buffer, sizeof(buffer))) > 0) {
        // Process each character in the buffer
        for (int i = 0; i < bytes_read; i++) {
            // If we hit a newline, process the accumulated line
            if (buffer[i] == '\n') {
                line[line_pos] = '\0';  // Null-terminate the string

                // Parse the line
                if (sscanf(line, "%d %d %d %s", &node_id_source, &node_id_dest, &money, date) == 4) {
                   
                    GraphNode node_sourse =  graph_node_create(node_id_source);
                    GraphNode node_dest =  graph_node_create(node_id_dest);
                    graph_insert_edge(mygraph,node_dest,node_sourse,money,date);
                   
                } else {
                    // If the line is not correctly formatted, skip it or print an error
                    fprintf(stderr, "Error parsing line: %s\n", line);
                }

                // Reset the line buffer
                line_pos = 0;
            }else {
                // Accumulate characters into the line buffer
                line[line_pos++] = buffer[i];

                // Prevent buffer overflow
                if (line_pos >= BUFFER_SIZE - 1) {
                    fprintf(stderr, "Line too long, truncating.\n");
                    line_pos = BUFFER_SIZE - 2;
                }
            }
        }
    }

    // Close the input file
    close(input_fd);




    //////////////////////////////////////////////////////////////////
    //Open the output file for writing (create if not exists, truncate if it does)
    int output_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("Error opening output file");
        close(input_fd);
        return 1;
    }

    //Prints the graph in the outputfile
    graph_print(mygraph, output_fd,BUFFER_SIZE);

    //Close the output file 
    close(output_fd);
   
    return 0;
} 

// Graph mygraph = graph_create();
    // printf("The size is : %d\n", graph_size(mygraph));

   
    // for (int i = 0; i < 10; i=i+2)
    // {
    //     graph_add_node(mygraph, i);
        
    // }
    // printf("The size is : %d\n", graph_size(mygraph));
    // GraphNode node =  graph_node_create(4);
    // GraphNode node2 =  graph_node_create(7);


    // graph_remove_node(mygraph,2);

    // graph_print(mygraph);
    // printf("The size is : %d\n", graph_size(mygraph));


    // graph_insert_edge(mygraph, node, node2, 23, "12-3-4");

    // graph_print(mygraph);

    // printf(" the money is : %d",graph_get_money(mygraph,node,node2));
    // printf(" the date is : %s",graph_get_date(mygraph,node,node2));
