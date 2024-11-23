#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include "Graph.h"


struct graph{
    int size;      //Number of nodes in the graph
    List mylist;   // List to store all graph nodes
};

struct graph_node{
    int id;        // The id of the graph node (unique)
    List outgoing; // List of outgoing edges from this node that will contain pointers to the struct vertex
    List ingoing;  // List of incoming edges to this node that will contain pointers to the struct vertex 
};

struct vertex{
    char* mydate;  // Date of the transaction
    int money;     // The amount of money the transaction was made
    GraphNode dest;   // The destination of the amount 
    GraphNode sourse; // The source of the money
};


///////////////////////////
// Helpful Functions
//////////////////////////

// Compares two graph nodes based on their id and returns 0 if the 2 nodes are equal 
int compare_objects(Pointer a, Pointer b) {
    GraphNode node_a = (GraphNode)a;
    GraphNode node_b = (GraphNode)b;

    if (node_a->id > node_b->id) {
        return 1;
    } else if (node_a->id < node_b->id) {
        return -1;
    } else {
        return 0;
    }
}

//Compares two vertexes and returns 0 if the vertexes are equal
int compare_vertexes(Pointer a, Pointer b) {
    Vertex vertex_a = (Vertex)a;
    Vertex Vertex_b = (Vertex)b;

    if (vertex_a->dest->id == Vertex_b->dest->id && vertex_a->sourse->id == Vertex_b->sourse->id && vertex_a->money==Vertex_b->money ) {
        return 0;
    } else {
        return -1;
    }
}

// Destroys the node 
void destroy_node(Pointer value){
    GraphNode node=(GraphNode)value;
    // list_destroy(node->outgoing);
    // list_destroy(node->ingoing);

}

void destroy_vertex(Pointer value){
    Vertex vertex=(Vertex)value;
    // free(vertex->mydate);
    // free(vertex);
}

///////////////////////////
// Graph Functions
///////////////////////////

Graph graph_create(){
    //Αllocate memory and initialize the variables
    Graph mygraph = malloc(sizeof(*mygraph));
    mygraph->size = 0;
    mygraph->mylist = list_create(compare_objects, destroy_node);

    return mygraph;
}

GraphNode graph_node_create(int id){
    // Creates a new graph node with the given id
    GraphNode node = malloc(sizeof(*node));
    node->id = id;
    node->ingoing = list_create(compare_vertexes, destroy_vertex);
    node->outgoing = list_create(compare_vertexes, destroy_vertex);

    return node;
}

int graph_size(Graph graph){
    return graph->size; 
}

void graph_add_node(Graph graph,HashTable table, int id){
    GraphNode new_node = graph_node_create(id);  // Create the new node
    
    //If the node exists we dont add the node 
    if(list_find_node(graph->mylist , new_node)!= NULL){ 
        printf("The node with id %d already exists\n",id);
        return;
    }
    hash_add(table, id , new_node);             // Insert into the hash table
    list_insert(graph->mylist, new_node);       // Insert it into the graph list
    graph->size++;                              // Increment the size of the graph
}

void graph_remove_node(Graph graph,HashTable table, int id){
    
    GraphNode graph_node = hash_find(table, id);

    list_remove(graph->mylist, graph_node ); 
    
    delete_item(table,id);
    graph->size --;
}

GraphNode graph_get_node(Graph graph, HashTable table, int id){

    GraphNode node = hash_find(table,id);  //Find the node with O(1) complexity
    //Check if the node exists
    if(node == NULL){              
        return NULL;
    }
    return node;
}

void graph_insert_edge(Graph graph, HashTable table,int id_dest, int id_sourse, int money, char* mydate){
    
    //Get the node from the graph to insert the weights
    GraphNode temp1 = graph_get_node(graph,table,id_sourse);
    GraphNode temp2 = graph_get_node(graph,table,id_dest);
    
    // If source node doesnt exists add to the graph
    if (temp1 == NULL) {
        graph_add_node(graph, table,id_sourse);
        temp1 = graph_get_node(graph,table,id_sourse);
    }
    // If destination node doesnt exists add to the graph 
    if (temp2 == NULL) {
        graph_add_node(graph,table, id_dest);
        temp2 = graph_get_node(graph,table,id_dest);
    }

    // Create the destination and sourse edges
    Vertex myvertexdest = malloc(sizeof(*myvertexdest));
    myvertexdest->money = money;
    myvertexdest->mydate = malloc(strlen(mydate) + 1);
    strcpy(myvertexdest->mydate, mydate);
    myvertexdest->dest = temp2;
    myvertexdest->sourse = temp1;

    Vertex myvertexsour = malloc(sizeof(*myvertexsour));
    myvertexsour->money = money;
    myvertexsour->mydate = malloc(strlen(mydate) + 1);
    strcpy(myvertexsour->mydate, mydate);
    myvertexsour->dest = temp2;
    myvertexsour->sourse = temp1;

    list_insert(temp1->outgoing, myvertexdest);  // Insert destination edge into outgoing list of the sourse node
    list_insert(temp2->ingoing, myvertexsour);   // Insert into sourse edge into ingoing list of the destination node
}

Vertex graph_get_edge(Graph graph,HashTable table, int id_dest, int id_sourse ,int money, char* date){
    //Get the node from the graph 
    GraphNode node1 = graph_get_node(graph,table,id_dest);
    GraphNode node2= graph_get_node(graph,table,id_sourse);

    // Iterate over all nodes in the main graph list to find the edge
    if(node1 != NULL && node2 !=NULL){
        for (ListNode node = list_first(graph->mylist); node != NULL; node = list_next(graph->mylist, node)) {
            GraphNode graph_node = (GraphNode)list_node_value(graph->mylist, node);

            // Check the outgoing edges of the source node
            if (graph_node->id == node2->id) {
                for (ListNode edge_node = list_first(graph_node->outgoing); edge_node != NULL; edge_node = list_next(graph_node->outgoing, edge_node)) {
                    Vertex edge = (Vertex)list_node_value(graph_node->outgoing, edge_node);
                    if ( edge->dest == node1) {
                        if(edge->money == money && strcmp(edge->mydate, date) == 0 ){
                            return edge;  // Return the found edge
                        }
                        
                    }
                }
            }
        }
    }
    return NULL;  // Return NULL if no edge is found OR no node is found
}

void graph_remove_edge(Graph graph,HashTable table,  int id_dest, int id_sourse, int money, char* date){
 
    Vertex edge = graph_get_edge(graph,table, id_dest, id_sourse,money,date);
    // If edge doesn't exist, exit function
    if (edge == NULL) {
        return;  
    }
    GraphNode node1 = graph_get_node(graph,table,id_sourse);

    list_remove(node1->outgoing, edge);  
}

//This is a function that returns the first vertex that he finds between the 2 nodes without the money and date 
Vertex graph_get_vertex(Graph graph,HashTable table, int id_dest, int id_sourse){
    GraphNode node1 = graph_get_node(graph,table,id_dest);
    GraphNode node2= graph_get_node(graph,table,id_sourse);

    if(node1 != NULL && node2 !=NULL){
        for (ListNode node = list_first(graph->mylist); node != NULL; node = list_next(graph->mylist, node)) {
            GraphNode graph_node = (GraphNode)list_node_value(graph->mylist, node);

            // Check the outgoing edges of the source node
            if (graph_node->id == node2->id) {
                for (ListNode edge_node = list_first(graph_node->outgoing); edge_node != NULL; edge_node = list_next(graph_node->outgoing, edge_node)) {
                    Vertex edge = (Vertex)list_node_value(graph_node->outgoing, edge_node);
                    if ( edge->dest == node1) {
                
                        return edge;  // Return the found edge                
                    }
                }
            }
        }
    }
    return NULL; // If not found return 0

}

int graph_get_money(Graph graph,HashTable table, int id_dest, int id_sourse) {
    // Get the edge between the nodes
    Vertex edge = graph_get_vertex(graph,table,id_dest,id_sourse);
    if(edge!= NULL){
        return edge->money;
    }
    return 0; // If not found return 0
}

char* graph_get_date(Graph graph,HashTable table,int id_dest, int id_sourse){

    Vertex edge = graph_get_vertex(graph,table,id_dest,id_sourse);
    if(edge!= NULL){
        return edge->mydate;
    }
    return NULL;// If not found return NULL
}

void graph_print(Graph graph,HashTable table, int output_fd, int buffer_size){

    // Iterate through all nodes in the graph list
    ListNode node = list_first(graph->mylist);
    while (node != NULL) {  

        GraphNode graph_node = (GraphNode)list_node_value(graph->mylist, node);// Get the current graph node
        char buffer[buffer_size];  // Create a buffer to store the output
        
        // Iterate through all outgoing edges of the current graph node 
        ListNode outgoing_edge = list_first(graph_node->outgoing);
        if (outgoing_edge == NULL) { 

            snprintf(buffer, sizeof(buffer), "%d -> NULL\n", graph_node->id);
            write(output_fd, buffer, strlen(buffer)); // Write the buffer to the output
        } else {
            while (outgoing_edge != NULL) {
                // Get the destination vertex 
                Vertex dest_node = (Vertex)list_node_value(graph_node->outgoing, outgoing_edge);
                if(hash_find(table,dest_node->dest->id)!=NULL){
                    // Stores in a buffer the source node, destination node, money, and date
                    snprintf(buffer, sizeof(buffer), "%d %d %d %s\n",
                        graph_node->id, dest_node->dest->id, dest_node->money, dest_node->mydate);

                    // Write the data from the buffer to the output file
                    write(output_fd, buffer, strlen(buffer)); 
                }
                outgoing_edge = list_next(graph_node->outgoing, outgoing_edge);
            }
        }
        node = list_next(graph->mylist, node);
    }
}

void graph_print_outgoing(Graph graph,HashTable table, int id){
    // Get the node with O(1) complexity
    GraphNode node = graph_get_node(graph,table,id); 

    printf("The person with id %d  tranfered to these people:\n",node->id);
    // If the node exists iterate through all of the nodes in the outgoing list
    if(node !=NULL){ 
        ListNode outgoing_edge = list_first(node->outgoing);
        while (outgoing_edge != NULL) {
            Vertex dest_node = (Vertex)list_node_value(node->outgoing, outgoing_edge);
            
            //Print in the console all the details 
            printf("        id: %d the amount of: %d on the date: %s\n",dest_node->dest->id, dest_node->money, dest_node->mydate);

            outgoing_edge = list_next(node->outgoing, outgoing_edge);
        }
    }
}

void graph_print_ingoing(Graph graph,HashTable table, int id ){
    // Get the node
    GraphNode node = graph_get_node(graph,table,id);
    printf("The person with id %d  received from these people:\n",node->id);

    // If the node exists iterate through all of the nodes in the ingoing list
    ListNode ingoing_edge = list_first(node->ingoing);
    while (ingoing_edge != NULL) {
        Vertex sour_node = (Vertex)list_node_value(node->ingoing, ingoing_edge);
        
        //Print in the console all the details 
        printf("        id: %d the amount of: %d on the date: %s\n",sour_node->sourse->id, sour_node->money, sour_node->mydate);

        ingoing_edge = list_next(node->ingoing, ingoing_edge);
    }
}

void graph_destroy(Graph graph){
    // Iterate through all nodes in the graph and destroy each one
    ListNode node = list_first(graph->mylist);
    while (node != NULL) {
        GraphNode graph_node = (GraphNode)list_node_value(graph->mylist, node);
        ListNode next = list_next(graph->mylist, node);  // Store the next node befire freeing the node 
        
        // Remove and destroy the current node
        list_remove(graph->mylist, node);                
        destroy_node(graph_node); 

        //next node                       
        node = next;                                     
    }

    // Destroy the graph's main list
    list_destroy(graph->mylist);
    
    // Free the graph
    free(graph);
}