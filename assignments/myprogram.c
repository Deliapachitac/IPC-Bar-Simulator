#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "ADTGraph.h"
#include "ADTHash.h"

#define BUFFER_SIZE 256


int main(int argc, char *argv[]){

    
    // Open the input file for reading only  
    int input_fd = open(argv[2], O_RDONLY);
    if (input_fd == -1) {
        perror("Error opening input file");
        return 1;
    }
    
    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    int countline=0;
    int line_pos = 0;
    ssize_t bytes_read;
    int node_id_source, node_id_dest, money;
    char date[20]; // To store the date 

    // Create the graph that we will use to enter the data from tyhe input file 
    Graph mygraph =graph_create(countline);

    // Read the input file line by line
    while ((bytes_read = read(input_fd, buffer, sizeof(buffer))) > 0) {
        // Process each character in the buffer
        for (int i = 0; i < bytes_read; i++) {
            // If we hit a newline, process the accumulated line
            if (buffer[i] == '\n') {
                line[line_pos] = '\0';  // Null-terminate the string

                // Parse the line
                if (sscanf(line, "%d %d %d %s", &node_id_source, &node_id_dest, &money, date) == 4) {
                   
                    graph_insert_edge(mygraph,node_id_dest,node_id_source,money,date);
                    countline++;
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
        
    printf("1.Insert a new node or more in the graph (i)\n");
    printf("2.Insert a new edge between 2 nodes with money and a date (n)\n");
    printf("3.Delete a node from the graph (d)\n");
    printf("4.Delete an edge or more between 2 nodes (l)\n");
    printf("5.Modify the money and the date between 2 edges (m)\n");
    printf("6.Find all the outgoing transactions from the given node (f)\n");
    printf("7.Find all the receiving transactions from the given node (r) \n");
    printf("8.Exit the program and print in the output file (e) \n");
        
        
    bool flag = true;
    while(flag){

       
        char input[50]; // To store the whole input line
        char code[10]; // To store the command string
        int ids[50]; // Array to store the ids
        int numCount = 0; // Count of numbers
            
        printf("Enter the command with the right parameters ");
        // Read the whole line of input
        fgets(input, sizeof(input), stdin); 

        // Use sscanf to extract the command
        sscanf(input, "%s", code);

        // Remove the newline character if present
        input[strcspn(input, "\n")] = 0;

       
        if( strcmp(code, "i") == 0){

            char *tok = strtok(input + strlen(code) + 1, " "); // Get the ids after command
            while (tok != NULL) {
                ids[numCount++] = atoi(tok); // Convert string to int and store it
                tok = strtok(NULL, " "); // Move to the next token
            }

            for (int i = 0; i < numCount ; i++)
            {
                graph_add_node(mygraph, ids[i]);
            }
            
            
        }else if( strcmp(code, "n") == 0){
            
            char *tok = strtok(input + strlen(code) + 1, " "); 
            for (int i = 0; i < 3; i++) {
                if (tok != NULL) {
                    ids[i] = atoi(tok); // Convert string to int and store it
                    tok = strtok(NULL, " "); // Move to the next token
                } else {
                    printf("Not enough variables\n");
                }
            }

            // Capture the remaining string
            if (tok != NULL) {
                strcpy(date, tok); // Copy the date
            } else {
                date[0] = '\0'; // If no date is provided, set it to an empty string
            }
            
            //if the nodes dont exists then it creates the new nodes and adds them into the graph
            graph_insert_edge(mygraph, ids[1],ids[0],ids[2],date);
    
        }else if( strcmp(code, "d") == 0){

            char *tok = strtok(input + strlen(code) + 1, " "); // Get the ids after command
            while (tok != NULL) {
                ids[numCount++] = atoi(tok); // Convert string to int and store it
                tok = strtok(NULL, " "); // Move to the next token
            }

            for (int i = 0; i < numCount; i++)
            {
                graph_remove_node(mygraph,ids[i]);
            }
            

        }else if( strcmp(code, "l") == 0){

            char *tok = strtok(input + strlen(code) + 1, " "); 
            for (int i = 0; i < 2; i++) {
                if (tok != NULL) {
                    ids[i] = atoi(tok); // Convert string to int and store it
                    tok = strtok(NULL, " "); // Move to the next token
                } else {
                    printf("Not enough variables\n");
                }
            }

            graph_remove_edge(mygraph,graph_node_create(ids[1]),graph_node_create(ids[0]));
            
        }else if( strcmp(code, "m") == 0){
            char date1[20]; // To store the date that we will change
            char *tok = strtok(input + strlen(code) + 1, " "); 
            for (int i = 0; i < 3; i++) {
                if (tok != NULL) {
                    ids[i] = atoi(tok); // Convert string to int and store it
                    tok = strtok(NULL, " "); // Move to the next token
                } else {
                    printf("Not enough variables\n");
                }
            }
            if (tok != NULL) {
                strcpy(date, tok); // Copy the first string
                tok = strtok(NULL, " "); // Move to the next token
            } else {
                date[0] = '\0'; // If no string is provided, set it to an empty string
            }

            // Capture the second string
            if (tok != NULL) {
                strcpy(date1, tok); // Copy the second string
            } else {
                date1[0] = '\0'; // If no string is provided, set it to an empty string
            }


            graph_remove_edge(mygraph,graph_node_create(ids[1]),graph_node_create(ids[0]));

            graph_insert_edge(mygraph,graph_node_create(ids[1]),graph_node_create(ids[0]),ids[3],date1);

        }else if( strcmp(code, "f") == 0){
            char *tok = strtok(input + strlen(code) + 1, " "); 
            if (tok != NULL) {
                ids[0] = atoi(tok); // Convert string to int and store it
                tok = strtok(NULL, " "); // Move to the next token
            } else {
                printf("Not enough variables\n");
            }

            graph_print_outgoing(mygraph, ids[0]);
            
            
        }else if( strcmp(code, "r") == 0){

            char *tok = strtok(input + strlen(code) + 1, " "); 
            if (tok != NULL) {
                ids[0] = atoi(tok); // Convert string to int and store it
                tok = strtok(NULL, " "); // Move to the next token
            } else {
                printf("Not enough variables\n");
            }

            graph_print_ingoing(mygraph, ids[0]);


        }else if( strcmp(code, "e") == 0){

            //Open the output file for writing (create if not exists, truncate if it does)
            int output_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd == -1) {
                perror("Error opening output file");
                close(input_fd);
                return 0;
            }

            //Prints the graph in the outputfile
            graph_print(mygraph, output_fd,BUFFER_SIZE);

            //Close the output file 
            close(output_fd);
            
            flag= false;

        }else {

            printf("Wrong input. Try again");
        }
    
    }

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
