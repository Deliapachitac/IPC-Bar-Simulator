#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "Graph.h"
#include "Hash.h"

#define BUFFER_SIZE 256


int main(int argc, char *argv[]){

    //Check if the command linbe is correct
    if(argc!= 5){
        printf("Wrong input.Try again\n");
        exit(1);
    }

    //Open the input file for reading only and chech for errors
    int input_fd = open(argv[2], O_RDONLY);
    if (input_fd == -1) {
        perror("Error opening input file");
        return 1;
    }
    
    //Variables useful for the reading
    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    int line_pos = 0;
    ssize_t bytes_read;

    //Variables to store the data we read from the file 
    int node_id_source, node_id_dest, money;
    char date[20]; 

    // Count the lines of the file to create the hash table 
    char ch;
    int countline=0;
    while ((bytes_read = read(input_fd, &ch, 1)) > 0) {
        if (ch == '\n') {
            countline++;
        }
    }

    // Move to the beginning of the file so we can read again the data and store it to the graph and hash table 
    lseek(input_fd, 0, SEEK_SET);  

    // Create the graph and hash table that we will use to enter the data from  input file 
    Graph mygraph =graph_create();
    HashTable mytable = create_hash_table(countline);
    
    // Read the input file line by line
    while ((bytes_read = read(input_fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {

                //Transfer the data from the line buffer to the variables
                if (sscanf(line, "%d %d %d %s", &node_id_source, &node_id_dest, &money, date) == 4) {
                    
                    graph_insert_edge(mygraph,mytable,node_id_dest,node_id_source,money,date); // Insert in the graph and the hash table

                } else {
                    // If the line is not correct 
                    printf( "Wrong format in the line: %s\n", line);
                    exit(1);
                }

                // Reset the line buffer
                line_pos = 0;
            }
            else {
                //Save the data from the buffer that we read in the array "line[]"
                line[line_pos++] = buffer[i];
            }
        }
    }

    //We close the input file because we finished with the reading
    close(input_fd);
        
    //These are the functions that can be used 
    printf("1.Insert a new node or more in the graph (i)\n");
    printf("2.Insert a new edge between 2 nodes with money and a date (n)\n");
    printf("3.Delete a node from the graph (d)\n");
    printf("4.Delete an edge or more between 2 nodes (l)\n");
    printf("5.Modify the money and the date between 2 edges (m)\n");
    printf("6.Find all the outgoing transactions from the given node (f)\n");
    printf("7.Find all the receiving transactions from the given node (r) \n");
    printf("8.Exit the program and print in the output file (e) \n");
        
        
    bool flag = true; // Variable to exit the program 
    while(flag){
       
        //Varibles to read the input command 
        char input[50];
        char code[10];
        int ids[50]; 
        int numCount = 0; 
            
        printf("Enter the command with the right parameters: \n");

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
                tok = strtok(NULL, " "); // Move to the next parameter
            }

            // Insert the every node we read 
            for (int i = 0; i < numCount ; i++)
            {
                graph_add_node(mygraph,mytable, ids[i]); 
            }
            
        }else if( strcmp(code, "n") == 0){
            
            char *tok = strtok(input + strlen(code) + 1, " "); // Get the ids after command
            //There are only 3 numbers (Ni Nj sum)
            for (int i = 0; i < 3; i++) {
                if (tok != NULL) {
                    ids[i] = atoi(tok); // Convert string to int and store it
                    tok = strtok(NULL, " ");// Move to the next parameter
                } else {
                    printf("Not enough variables\n");
                    exit(1);
                }
            }

            //Read the last string
            if (tok != NULL) {
                strcpy(date, tok); // Strore the date 
            } else {
                printf("Not enough variables\n");
                exit(1);
            }
            
            //if the nodes dont exists then it creates the new nodes and adds them into the graph
            graph_insert_edge(mygraph,mytable, ids[1],ids[0],ids[2],date);
    
        }else if( strcmp(code, "d") == 0){

            char *tok = strtok(input + strlen(code) + 1, " "); // Get the ids after command
            while (tok != NULL) {
                ids[numCount++] = atoi(tok); // Convert string to int and store it
                tok = strtok(NULL, " "); // Move to the next token
            }

            for (int i = 0; i < numCount; i++)
            {
                graph_remove_node(mygraph,mytable,ids[i]);
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
            int amount=graph_get_money(mygraph,mytable,ids[1],ids[0]);
            strcpy(date,graph_get_date(mygraph,mytable,ids[1],ids[0]));

            graph_remove_edge(mygraph,mytable,ids[1],ids[0],amount,date);
            
        }else if( strcmp(code, "m") == 0){
            char date1[20]; // To store the date that we will change
            char *tok = strtok(input + strlen(code) + 1, " "); 
            for (int i = 0; i < 4; i++) {
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

            graph_remove_edge(mygraph,mytable,ids[1],ids[0],ids[2],date);

            graph_insert_edge(mygraph,mytable,ids[1],ids[0],ids[3],date1);

        }else if( strcmp(code, "f") == 0){
            char *tok = strtok(input + strlen(code) + 1, " "); // Get the id after command
            
            //We will read only one id (Ni)
            if (tok != NULL) {
                ids[0] = atoi(tok); // Convert string to int and store it
                tok = strtok(NULL, " "); 
            } else {
                printf("Not enough variables\n");
                exit(1);
            }

            //Function that prints all the transactions from the person with the given id 
            graph_print_outgoing(mygraph,mytable, ids[0]);
            
            
        }else if( strcmp(code, "r") == 0){

            char *tok = strtok(input + strlen(code) + 1, " "); // Get the id after the command
            if (tok != NULL) {
                ids[0] = atoi(tok); // Convert string to int and store it
                tok = strtok(NULL, " ");
            } else {
                printf("Not enough variables\n");
                exit(1);
            }

            //Function that prints all the incoming transactions from the person with the given id 
            graph_print_ingoing(mygraph,mytable, ids[0]);


        }else if( strcmp(code, "e") == 0){

            //Open the output file for writing (create if not exists, truncate if it does) and check for errors
            int output_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC);
            if (output_fd == -1) {
                perror("Error opening output file");
                close(input_fd);
                return 0;
            }

            //Prints the graph in the outputfile
            graph_print(mygraph,mytable, output_fd,BUFFER_SIZE);

            //Close the output file 
            close(output_fd);
            
            //Stop the program 
            flag= false;

            //Destroy all the structs


        }else {

            printf("Wrong input. Try again\n\n");
        }
    
    }

    return 0;
} 