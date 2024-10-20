///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Graph με χρηση λιστας
//
///////////////////////////////////////////////////////////

#include "ADTGraph.h"
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

// void destroy_value (GraphNode node){
//     free(node->id);
    
// }

Graph graph_create(){

    Graph mygraph = malloc(sizeof(*mygraph));
    mygraph->size=0;
    mygraph->mylist = list_create(compare_objects);

    GraphNode new  = malloc(sizeof(*new));
    new->ingoing=list_create(compare_objects);
    new->outgoing= list_create(compare_objects);
	new->id= -1;
    list_insert(mygraph->mylist,new);

    return mygraph;
}

int graph_size(Graph graph){
    return graph->size;
}

void graph_add_node(Graph graph, int id){

    // Δημιουργία του νέου κόμβου
	GraphNode new = malloc(sizeof(*new));
	new->id = id;
    new->ingoing=list_create(compare_objects);
    new->outgoing=list_create(compare_objects);

    list_insert(graph->mylist, new);
    graph->size++;
    
}



void graph_remove_node(Graph graph, int id){

    list_remove(graph->mylist, list_find_node(graph->mylist,id));
}


void graph_insert_edge(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse, int money, char* mydate){

    if(vertex_dest == NULL){
        vertex_dest = malloc(sizeof(*vertex_dest));
	    vertex_dest->id = -1;
        vertex_dest->ingoing=list_create(compare_objects);
        vertex_dest->outgoing=list_create(compare_objects);
    }
    if(vertex_sourse == NULL){
        vertex_sourse = malloc(sizeof(*vertex_sourse));
	    vertex_sourse->id = -1;
        vertex_sourse->ingoing=list_create(compare_objects);
        vertex_sourse->outgoing=list_create(compare_objects);
    }
    
    list_insert(vertex_dest->ingoing,vertex_sourse);
    list_insert(vertex_sourse->outgoing,vertex_dest);


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
      
        if ((GraphNode)list_node_value(graph->mylist, node) == vertex_sourse) {

            for (ListNode edge_node = list_first(vertex_sourse->outgoing); edge_node != NULL; edge_node = list_next(vertex_sourse->outgoing, edge_node)) {
                Vertex edge = list_node_value(vertex_sourse->outgoing, edge_node);

                if ((GraphNode)node == vertex_dest) {
                    return edge->money;  
                }
            }
        }
    }

    
}


char* graph_get_date(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse){
    for (ListNode node = list_first(graph->mylist); node != NULL; node = list_next(graph->mylist, node)) {
      
        if ((GraphNode)node == vertex_sourse) {
            
            for (ListNode edge_node = list_first(vertex_sourse->outgoing); edge_node != NULL; edge_node = list_next(vertex_sourse->outgoing, edge_node)) {
                Vertex edge = list_node_value(vertex_sourse->outgoing, edge_node);

                if ((GraphNode)edge_node == vertex_dest) {
                    return edge->mydate;  
                }
            }
        }
    }
}

void graph_print(Graph graph){
    ListNode node =list_first(graph->mylist);
    for(int i =0; i < graph->size ; i++){
        GraphNode node_out= (GraphNode)list_first(node_out->outgoing);
        for (int j = 0; j < list_size(node_out->outgoing); j++)
        {
            printf("%d -> %d\n",(int)list_node_value(graph->mylist,node),(int)list_node_value(node_out->outgoing,(ListNode)node_out));
            node_out=list_next(node_out->outgoing,(ListNode)node_out);
        }
        
        node=list_next(graph->mylist, node);
    }
}


int main(void){

    
    Graph mygraph = graph_create();
    printf("The size is : %d\n", graph_size(mygraph));

    for (int i = 0; i < 10; i=i+2)
    {
        graph_add_node(mygraph, i);
        printf("%p      ", list_node_value(mygraph->mylist, list_find_node(mygraph->mylist,i)  ));

    }
     
    return 0;
}
