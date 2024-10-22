//////////////////////////
//
// ADT Graph 
//
/////////////////////////

#pragma once
#include "ADTList.h"  

// Pointers to the structs
typedef struct graph* Graph;
typedef struct graph_node* GraphNode;
typedef struct vertex* Vertex;

// Creates and returns a new graph 
Graph graph_create();

// Creates and returns a new graph node with a given id
GraphNode graph_node_create(int id); 

// Returns the number of elements contained in the graph
int graph_size(Graph graph);

// Adds a node to the list of nodes in the graph.
void graph_add_node(Graph graph, int id);

// Removes a node from the graph based on the given id
void graph_remove_node(Graph graph, int id);

// Returns an edge between two nodes in the graph
Vertex graph_get_edge(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse);

// Inserts an edge with a given weight (money and date mydate) between two nodes
void graph_insert_edge(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse, int money, char* mydate);

// Removes an edge between two nodes 
void graph_remove_edge(Graph graph, Pointer vertex_dest, Pointer vertex_sourse);

// Returns the money the two given nodes
int graph_get_money(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse);

// Returns the date two nodes
char* graph_get_date(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse);

// Printing functions of  the graph 
void graph_print(Graph graph, int output_fd, int buffer_size);
void graph_print_outgoing(Graph graph, int id);
void graph_print_ingoing(Graph graph, int id);


// Destroys a graph
void graph_destroy(Graph graph);
