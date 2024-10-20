
////////////////////////////////////////////////////////////////////////
//
// ADT Graph
//
////////////////////////////////////////////////////////////////////////

#pragma once 
#include "ADTList.h"


typedef struct graph* Graph;
typedef struct graph_node* GraphNode;
typedef struct vertex* Vertex;


// Δημιουργεί και επιστρέφει ένα γράφο, στο οποίο τα στοιχεία συγκρίνονται με βάση
// τη συνάρτηση compare.

Graph graph_create();

// Επιστρέφει τον αριθμό στοιχείων (κορυφών) που περιέχει ο γράφος graph.

int graph_size(Graph graph);

//Προσθετει ενα κομβο στην λιστα του graph
void graph_add_node(Graph graph, int id);

void graph_remove_node(Graph graph, int id);


// Προσθέτει μια ακμή με βάρος weight στο γράφο
void graph_insert_edge(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse, int money, char* mydate);

// Αφαιρεί μια ακμή από το γράφο
void graph_remove_edge(Graph graph, Pointer vertex_dest, Pointer vertex_sourse);

// Επιστρέφει το βάρος της ακμής ανάμεσα στις δύο κορυφές
int graph_get_money(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse);
char* graph_get_date(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse);

void graph_print(Graph graph);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει το γράφος.
// Οποιαδήποτε λειτουργία πάνω στο γράφο μετά το destroy είναι μη ορισμένη.
void graph_destroy(Graph graph);
