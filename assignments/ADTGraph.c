///////////////////////////////////////////////////////////
//
// Implementation of Graph using a list
//
///////////////////////////////////////////////////////////

#include "ADTGraph.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>


struct graph{
    int size;      // Number of nodes in the graph
    List mylist;   // List to store all graph nodes
};

struct graph_node{
    int id;        // The id of the graph node
    List outgoing; // List of outgoing edges from this node that will contain pointers to the struct vertex
    List ingoing;  // List of incoming edges to this node that will contain pointers to the struct vertex 
};

struct vertex{
    char* mydate;  // Date of the transaction
    int money;     // The amount of money the transaction was made
    GraphNode dest;   // The destination of the amount 
    GraphNode sourse; // The source of the money
};


///////////////////////////////////////////////////////////
// Helpful Functions
///////////////////////////////////////////////////////////

// Compares two graph nodes based on their id
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

// Destroys the node 
void destroy_value(GraphNode node){
    // free(node->id);
}

///////////////////////////////////////////////////////////
// Graph Functions
///////////////////////////////////////////////////////////

Graph graph_create(){
    //Αllocate memory and initialize the variables
    Graph mygraph = malloc(sizeof(*mygraph));
    mygraph->size = 0;
    mygraph->mylist = list_create(compare_objects, destroy_value);
    return mygraph;
}

GraphNode graph_node_create(int id){
    // Creates a new graph node with the given id
    GraphNode node = malloc(sizeof(*node));
    node->id = id;
    node->ingoing = list_create(compare_objects, destroy_value);
    node->outgoing = list_create(compare_objects, destroy_value);

    return node;
}

int graph_size(Graph graph){
    return graph->size; // Returns the size
}

void graph_add_node(Graph graph, int id){
    GraphNode new_node = graph_node_create(id);  // Create the new node
    if(list_find_node(graph->mylist , new_node)!= NULL){ //If the node exists we dont add the node 
        printf("The node with id %d already exists\n",id);
        return;
    }
    list_insert(graph->mylist, new_node);        // Insert it into the graph list
    graph->size++;                               // Increment the size of the graph
}

void graph_remove_node(Graph graph, int id){
    GraphNode node_to_remove = graph_node_create(id); 

    for (ListNode node = list_first(graph->mylist); node != NULL; node = list_next(graph->mylist, node)) {
        GraphNode graph_node = (GraphNode)list_node_value(graph->mylist, node);
        
        for (ListNode edge_node = list_first(graph_node->outgoing); edge_node != NULL; edge_node = list_next(graph_node->outgoing, edge_node)) {
            Vertex edge = (Vertex)list_node_value(graph_node->outgoing, edge_node);
            
            if(edge->sourse->id == id || edge->dest->id == id){    
                
                graph_remove_edge(graph,edge->dest->id,edge->sourse->id);
            }
        
        }
    }
    list_remove(graph->mylist, list_find_node(graph->mylist, node_to_remove));  // Remove the node
    graph->size --;
}

GraphNode graph_get_node(Graph graph, int id){

    GraphNode node = graph_node_create(id);  // Crete the node that we want to find
    ListNode node_found1 = list_find_node(graph->mylist, node);  
    if(node_found1 == NULL){                //Check if the node already exists
        printf("The node with doesnt exists\n");
        return NULL;
    }
    node =(GraphNode)list_node_value(graph->mylist, node_found1);
    
    return node;
}


void graph_insert_edge(Graph graph, int id_dest, int id_sourse, int money, char* mydate){
    
    //Get the node from the graph to insert the weights
    GraphNode temp1 = graph_get_node(graph,id_sourse);
    GraphNode temp2 = graph_get_node(graph,id_dest);
    
    // If source node doesnt exists add to the graph
    if (temp1 == NULL) {
        graph_add_node(graph, id_sourse);
        temp1 = graph_get_node(graph,id_sourse);
    }
    // If destination node doesnt exists add to the graph 
    if (temp2 == NULL) {
        graph_add_node(graph, id_dest);
        temp2 = graph_get_node(graph,id_dest);
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

Vertex graph_get_edge(Graph graph, int id_dest, int id_sourse){

    GraphNode node1 = graph_get_node(graph,id_dest);
    GraphNode node2= graph_get_node(graph,id_sourse);


    // Iterate over all nodes in the main graph list to find the edge
    if(node1 != NULL && node2 !=NULL){
        for (ListNode node = list_first(graph->mylist); node != NULL; node = list_next(graph->mylist, node)) {
            GraphNode graph_node = (GraphNode)list_node_value(graph->mylist, node);

            // Check the outgoing edges of the source node
            if (graph_node->id == node2->id) {
                for (ListNode edge_node = list_first(graph_node->outgoing); edge_node != NULL; edge_node = list_next(graph_node->outgoing, edge_node)) {
                    Vertex edge = (Vertex)list_node_value(graph_node->outgoing, edge_node);
                    if (edge->dest == node1) {
                        return edge;  // Return the found edge
                    }
                }
            }
        }
    }
    return NULL;  // Return NULL if no edge is found OR no node is found
}



void graph_remove_edge(Graph graph,  int id_dest, int id_sourse){
 
    Vertex edge = graph_get_edge(graph, id_dest, id_sourse);
    // If edge doesn't exist, exit function
    if (edge == NULL) {
        return;  
    }
    GraphNode node1 = graph_get_node(graph,id_sourse);
    
    // list_remove(node1->outgoing, edge);  
}

int graph_get_money(Graph graph, int id_dest, int id_sourse) {
    Vertex edge = graph_get_edge(graph, id_dest, id_sourse); // Get the edge between the nodes
    if (edge != NULL) {
        return edge->money;  // Return money if found
    }
    return 0; // If not found return 0
}

char* graph_get_date(Graph graph,int id_dest, int id_sourse){
    Vertex edge = graph_get_edge(graph, id_dest, id_sourse); // Get the edge between the nodes
    if (edge != NULL) {
        return edge->mydate;  // Return date if found
    }
    return NULL;// If not found return NULL
}


void graph_print(Graph graph, int output_fd, int buffer_size){

    // Iterate through all nodes in the graph list
    ListNode node = list_first(graph->mylist);
    while (node != NULL) {  

        GraphNode graph_node = (GraphNode)list_node_value(graph->mylist, node);// Get the current graph node
        char buffer[buffer_size];  // Create a buffer to store the output
        
        // Iterate through all outgoing edges of the current graph node 
        ListNode outgoing_edge = list_first(graph_node->outgoing);
        if (outgoing_edge == NULL) { 

            snprintf(buffer, sizeof(buffer), " Node source: %d -> NULL\n", graph_node->id);
            write(output_fd, buffer, strlen(buffer)); // Write the buffer to the output
        } else {
            while (outgoing_edge != NULL) {
                // Get the destination vertex 
                Vertex dest_node = (Vertex)list_node_value(graph_node->outgoing, outgoing_edge);
                
                // Stores in a buffer the source node, destination node, money, and date
                snprintf(buffer, sizeof(buffer), " Node source: %d  Node destination: %d, Money: %d, Date: %s\n",
                    graph_node->id, dest_node->dest->id, dest_node->money, dest_node->mydate);

                // Write the formatted data to the output
                write(output_fd, buffer, strlen(buffer)); 

                outgoing_edge = list_next(graph_node->outgoing, outgoing_edge);
            }
        }
        node = list_next(graph->mylist, node);
    }
}

void graph_print_outgoing(Graph graph, int id){
    // Get the node
    GraphNode node = graph_get_node(graph,id); 

    // If the node exists iterate through all of the nodes in the outgoing list
    if(node !=NULL){ 
        ListNode outgoing_edge = list_first(node->outgoing);
        while (outgoing_edge != NULL) {
            Vertex dest_node = (Vertex)list_node_value(node->outgoing, outgoing_edge);
            
            //Print in the console all the details 
            printf(" Node sourse: %d  Node destination: %d, Money: %d, Date: %s\n",
                node->id, dest_node->dest->id, dest_node->money, dest_node->mydate);

            outgoing_edge = list_next(node->outgoing, outgoing_edge);
        }
    }
}
void graph_print_ingoing(Graph graph, int id ){
    // Get the node
    GraphNode node = graph_get_node(graph,id);

    // If the node exists iterate through all of the nodes in the ingoing list
    ListNode ingoing_edge = list_first(node->ingoing);
    while (ingoing_edge != NULL) {
        Vertex sour_node = (Vertex)list_node_value(node->ingoing, ingoing_edge);
        
        //Print in the console all the details 
        printf(" Node sourse: %d  Node ingoing: %d, Money: %d, Date: %s\n",
            node->id, sour_node->sourse->id, sour_node->money, sour_node->mydate);

        ingoing_edge = list_next(node->ingoing, ingoing_edge);
    }
}

void graph_destroy(Graph graph){
    
}

///////////////////////////////////////////////////////////

// int main(void){
//     Graph mygraph = graph_create();
//     printf("The size is : %d\n", graph_size(mygraph));
//     for (int i = 0; i < 10; i=i+2)
//     {
//         graph_add_node(mygraph, i);  
//     }
//     printf("The size is : %d\n", graph_size(mygraph));
//     GraphNode node =  graph_node_create(4);
//     GraphNode node2 =  graph_node_create(7);
//     if(list_find_node(mygraph->mylist, node )== NULL){
//         printf("delia");
//     }
//      graph_remove_node(mygraph,2);
//      if(list_find_node(mygraph->mylist, node )== NULL){
//         printf("delia");
//     }
//     graph_print(mygraph);
//     printf("The size is : %d\n", graph_size(mygraph));
//     graph_insert_edge(mygraph, node, node2, 23, "12-3-4");
//     graph_print(mygraph);
//     printf(" the money is : %d",graph_get_money(mygraph,node,node2));
//     printf(" the date is : %s",graph_get_date(mygraph,node,node2));
//     // graph_remove_edge(mygraph,node,node2);
//     // printf(" the money is : %d",graph_get_money(mygraph,node,node2));
//     return 0;
// }
