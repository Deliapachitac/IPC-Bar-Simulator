
#include "List.h"
#include "Hash.h"  

// Pointers to the structs
typedef struct graph* Graph;
typedef struct graph_node* GraphNode;
typedef struct vertex* Vertex;

// Creating functions for a new graph and a new graph node 
Graph graph_create();
GraphNode graph_node_create(int id); 

// Returns the number of nodes that the graph has
int graph_size(Graph graph);

// Functions that adds , removes or gets a node in the list of the graph
void graph_add_node(Graph graph,HashTable table, int id);
void graph_remove_node(Graph graph,HashTable table, int id);
GraphNode graph_get_node(Graph graph,HashTable table, int id);

// Returns, inserts or remove an edge between two nodes in the graph
Vertex graph_get_edge(Graph graph,HashTable table, int id_dest, int id_sourse,int money, char* date);
void graph_insert_edge(Graph graph, HashTable table, int id_dest, int id_sourse, int money, char* mydate);
void graph_remove_edge(Graph graph,HashTable table, int id_dest, int id_sourse,int money, char* date);

// Returns the money or the date between the two nodes
int graph_get_money(Graph graph, HashTable table,int id_dest, int id_sourse);
char* graph_get_date(Graph graph, HashTable table, int id_dest, int id_sourse);

// Printing functions of  the graph 
void graph_print(Graph graph,HashTable table, int output_fd, int buffer_size); // Prints in the output file 
void graph_print_outgoing(Graph graph,HashTable table, int id);
void graph_print_ingoing(Graph graph,HashTable table, int id);

// Destroys a graph
void graph_destroy(Graph graph);
