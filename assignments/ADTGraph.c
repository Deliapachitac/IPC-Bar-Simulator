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
    List outgoing; // List of outgoing edges from this node
    List ingoing;  // List of incoming edges to this node
};

struct vertex{
    char* mydate;  // Date of the edge
    int money;     // Weight of the edge
    GraphNode dest;   // Destination node of the edge
    GraphNode sourse; // Source node of the edge
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

void destroy_value(GraphNode node){
    // free(node->id);
}

///////////////////////////////////////////////////////////
// Graph Functions
///////////////////////////////////////////////////////////

// Creates and returns a new graph
Graph graph_create(){
    Graph mygraph = malloc(sizeof(*mygraph));
    mygraph->size = 0;
    mygraph->mylist = list_create(compare_objects, destroy_value);

    return mygraph;
}

// Creates and returns a new graph node with a given id
GraphNode graph_node_create(int id){
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
    if(list_find_node(graph->mylist , new_node)!= NULL){
        printf("The node with id %d already exists\n",id);
        return;
    }
    list_insert(graph->mylist, new_node);        // Insert it into the graph list
    graph->size++;                               // Increment the size of the graph
}

void graph_remove_node(Graph graph, int id){
    GraphNode node_to_remove = graph_node_create(id);  // Create a node to search for

    for (ListNode node = list_first(graph->mylist); node != NULL; node = list_next(graph->mylist, node)) {
        GraphNode graph_node = (GraphNode)list_node_value(graph->mylist, node);
        
        for (ListNode edge_node = list_first(graph_node->outgoing); edge_node != NULL; edge_node = list_next(graph_node->outgoing, edge_node)) {
            Vertex edge = (Vertex)list_node_value(graph_node->outgoing, edge_node);
            
            if(edge->sourse->id == id || edge->dest->id == id){    
                
                graph_remove_edge(graph,edge->dest,edge->sourse);
            }
        
        }
    }
    list_remove(graph->mylist, list_find_node(graph->mylist, node_to_remove));  // Remove the node
}


void graph_insert_edge(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse, int money, char* mydate){
    ListNode node_found1 = list_find_node(graph->mylist, vertex_sourse);
    ListNode node_found2 = list_find_node(graph->mylist, vertex_dest);
    GraphNode temp1, temp2;

    // If source node not found, add it
    if (node_found1 == NULL) {
        graph_add_node(graph, vertex_sourse->id);
        node_found1 = list_find_node(graph->mylist, vertex_sourse);
    }
    // If destination node not found, add it
    if (node_found2 == NULL) {
        graph_add_node(graph, vertex_dest->id);
        node_found2 = list_find_node(graph->mylist, vertex_dest);
    }

    temp1 = (GraphNode)list_node_value(graph->mylist, node_found1);
    temp2 = (GraphNode)list_node_value(graph->mylist, node_found2);

    // Create and insert edge
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

    list_insert(temp1->outgoing, myvertexdest);  // Insert into outgoing list
    list_insert(temp2->ingoing, myvertexsour);   // Insert into ingoing list
}

// Retrieves an edge between two nodes in the graph
Vertex graph_get_edge(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse){
    ListNode node_found1 = list_find_node(graph->mylist, vertex_dest);
    ListNode node_found2 = list_find_node(graph->mylist, vertex_sourse);

    GraphNode node1 = (GraphNode)list_node_value(graph->mylist, node_found1);
    GraphNode node2 = (GraphNode)list_node_value(graph->mylist, node_found2);

    // Iterate over all nodes in the main graph list to find the edge
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
    return NULL;  // Return NULL if no edge is found
}


void graph_remove_edge(Graph graph, Pointer vertex_dest, Pointer vertex_sourse){
    
    Vertex edge = graph_get_edge(graph, vertex_dest, vertex_sourse);
    // If edge doesn't exist, exit function
    if (edge == NULL) {
        return;  
    }
    ListNode node_found1 = list_find_node(graph->mylist, vertex_sourse);
    GraphNode node1 = (GraphNode)list_node_value(graph->mylist, node_found1);
    
    // list_remove(node1->outgoing, edge);  
}

int graph_get_money(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse) {
    Vertex edge = graph_get_edge(graph, vertex_dest, vertex_sourse);
    if (edge != NULL) {
        return edge->money;  // Return edge weight if found
    }
    return 0;
}

char* graph_get_date(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse){
    Vertex edge = graph_get_edge(graph, vertex_dest, vertex_sourse);
    if (edge != NULL) {
        return edge->mydate;  // Return edge date if found
    }
    return NULL;
}


void graph_print(Graph graph, int output_fd, int buffer_size){

    // Iterate through all nodes in the graph list
    ListNode node = list_first(graph->mylist);
    while (node != NULL) {  
        
        char buffer[buffer_size];
        GraphNode graph_node = (GraphNode)list_node_value(graph->mylist, node);

        
        ListNode outgoing_edge = list_first(graph_node->outgoing);
        if (outgoing_edge == NULL) {
            snprintf(buffer, sizeof(buffer), " Node sourse: %d -> NULL\n",graph_node->id);
            write(output_fd, buffer, strlen(buffer));
        } else {
            while (outgoing_edge != NULL) {
                Vertex dest_node = (Vertex)list_node_value(graph_node->outgoing, outgoing_edge);
                
                snprintf(buffer, sizeof(buffer), " Node sourse: %d  Node destination: %d, Money: %d, Date: %s\n",
                    graph_node->id, dest_node->dest->id, dest_node->money, dest_node->mydate);
                write(output_fd, buffer, strlen(buffer));

                outgoing_edge = list_next(graph_node->outgoing, outgoing_edge);
            }
        }


        node = list_next(graph->mylist, node);
    }
}

void graph_print_outgoing(Graph graph, int id){

    GraphNode node = graph_node_create(id);  
    ListNode node_found1 = list_find_node(graph->mylist, node);
    if(node_found1 == NULL){
        printf("The node with doesnt exists\n");
        return;
    }
    node =(GraphNode)list_node_value(graph->mylist, node_found1);
    ListNode outgoing_edge = list_first(node->outgoing);
    while (outgoing_edge != NULL) {
        Vertex dest_node = (Vertex)list_node_value(node->outgoing, outgoing_edge);
        
        printf(" Node sourse: %d  Node destination: %d, Money: %d, Date: %s\n",
            node->id, dest_node->dest->id, dest_node->money, dest_node->mydate);

        outgoing_edge = list_next(node->outgoing, outgoing_edge);
    }

}
void graph_print_ingoing(Graph graph, int id ){
    GraphNode node = graph_node_create(id);  
    ListNode node_found1 = list_find_node(graph->mylist, node);
    if(node_found1 == NULL){
        printf("The node with doesnt exists\n");
        return;
    }
    node =(GraphNode)list_node_value(graph->mylist, node_found1);

    ListNode ingoing_edge = list_first(node->ingoing);
    while (ingoing_edge != NULL) {
        Vertex sour_node = (Vertex)list_node_value(node->ingoing, ingoing_edge);
        
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
