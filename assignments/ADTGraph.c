///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Graph με χρηση λιστας
//
///////////////////////////////////////////////////////////

#include <ADTGraph.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

struct graph{
    int size;
    List mylist;
 
};

struct graph_node{
    
    int id;
    List outgoing;
    List ingoing;
};

struct vertex{
    char* mydate;
    int money;
    GraphNode dest;
    GraphNode sourse;

};


// void destroy_value (GraphNode node){
//     free(node->id);
    
// }

Graph graph_create(){

    Graph mygraph = malloc(sizeof(*mygraph));
    mygraph->size=0;
    mygraph->mylist = list_create();

    GraphNode new  = malloc(sizeof(*new));
    new->ingoing=list_create();
    new->outgoing= list_create();
	new->id= -1;
    list_insert_next(mygraph->mylist,new, NULL );

    return mygraph;
}

int graph_size(Graph graph){
    return graph->size;
}

void graph_add_node(Graph graph, int id){

    // Δημιουργία του νέου κόμβου
	GraphNode new = malloc(sizeof(*new));
	new->id = id;
    new->ingoing=list_create();
    new->outgoing=listcreate();

    list_insert(graph->mylist, new);
    graph->size++;
    
}



void graph_remove_node(Graph graph, int id){

   // list_remove(graph->mylist, list_find_node(graph->mylist,id,));
}


void graph_insert_edge(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse, int money, char* mydate){

    if(vertex_dest == NULL){
        vertex_dest = malloc(sizeof(*vertex_dest));
	    vertex_dest->id = -1;
        vertex_dest->ingoing=list_create();
        vertex_dest->outgoing=listcreate();
    }
    if(vertex_sourse == NULL){
        vertex_sourse = malloc(sizeof(*vertex_sourse));
	    vertex_sourse->id = -1;
        vertex_sourse->ingoing=list_create();
        vertex_sourse->outgoing=listcreate();
    }
    
    list_insert(vertex_dest->ingoing,vertex_sourse);
    list_insert(vertex_sourse->outgoing,vertex_dest);
<<<<<<< HEAD


    Vertex myvertex = malloc(sizeof(*myvertex));
    myvertex->money=money;
    strcpy(myvertex->mydate,mydate);
    myvertex->dest=vertex_dest;
    myvertex->sourse=vertex_sourse;
}

void graph_remove_edge(Graph graph, Pointer vertex_dest, Pointer vertex_sourse){


}

int graph_get_money(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse){

    for (ListNode node = list_first(graph->mylist); node != NULL; node = list_next(graph->mylist, node)) {
      
        if (node == vertex_sourse) {

            for (ListNode edge_node = list_first(vertex_sourse->outgoing); edge_node != NULL; edge_node = list_next(vertex_sourse->outgoing, edge_node)) {
                Vertex edge = list_node_value(vertex_sourse->outgoing, edge_node);

                if (edge_node == vertex_dest) {
                    return edge->money;  
                }
            }
        }
    }

    
}


char* graph_get_date(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse){
    for (ListNode node = list_first(graph->mylist); node != NULL; node = list_next(graph->mylist, node)) {
      
        if (node == vertex_sourse) {
            
            for (ListNode edge_node = list_first(vertex_sourse->outgoing); edge_node != NULL; edge_node = list_next(vertex_sourse->outgoing, edge_node)) {
                Vertex edge = list_node_value(vertex_sourse->outgoing, edge_node);

                if (edge_node == vertex_dest) {
                    return edge->mydate;  
                }
            }
        }
    }
}

int main(void){
    





    return 0;
}
=======


    Vertex myvertex = malloc(sizeof(*myvertex));
    myvertex->money=money;
    strcpy(myvertex->mydate,mydate);
    myvertex->dest=vertex_dest;
    myvertex->sourse=vertex_sourse;
}

void graph_remove_edge(Graph graph, Pointer vertex_dest, Pointer vertex_sourse){


}

int graph_get_money(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse){

    for (ListNode node = list_first(graph->mylist); node != NULL; node = list_next(graph->mylist, node)) {
      
        if (node == vertex_sourse) {

            for (ListNode edge_node = list_first(vertex_sourse->outgoing); edge_node != NULL; edge_node = list_next(vertex_sourse->outgoing, edge_node)) {
                Vertex edge = list_node_value(vertex_sourse->outgoing, edge_node);

                if (edge_node == vertex_dest) {
                    return edge->money;  
                }
            }
        }
    }

    
}


char* graph_get_date(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse){
    for (ListNode node = list_first(graph->mylist); node != NULL; node = list_next(graph->mylist, node)) {
      
        if (node == vertex_sourse) {
            
            for (ListNode edge_node = list_first(vertex_sourse->outgoing); edge_node != NULL; edge_node = list_next(vertex_sourse->outgoing, edge_node)) {
                Vertex edge = list_node_value(vertex_sourse->outgoing, edge_node);

                if (edge_node == vertex_dest) {
                    return edge->mydate;  
                }
            }
        }
    }
}

int main(void){
    





    return 0;
}
>>>>>>> b841755ff0534e262897a5898f04d30451fa6a36
