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

void destroy_value (GraphNode node){
    // free(node->id);
    
}

Graph graph_create(){

    Graph mygraph = malloc(sizeof(*mygraph));
    mygraph->size=0;
    mygraph->mylist = list_create(compare_objects,destroy_value);

    // GraphNode new  = malloc(sizeof(*new));
    // new->ingoing=list_create(compare_objects,destroy_value);
    // new->outgoing= list_create(compare_objects,destroy_value);
	// new->id= -1;
    // list_insert(mygraph->mylist,new);

    return mygraph;
}

GraphNode graph_node_create(int id){
    GraphNode node =  malloc(sizeof(*node));
    node->id =id;
    node->ingoing =list_create(compare_objects,destroy_value);
    node->outgoing =list_create(compare_objects,destroy_value);

    return node;
}

int graph_size(Graph graph){
    return graph->size;
}

void graph_add_node(Graph graph, int id){

    // Δημιουργία του νέου κόμβου
	GraphNode new = malloc(sizeof(*new));
	new->id = id;
    new->ingoing=list_create(compare_objects,destroy_value);
    new->outgoing=list_create(compare_objects,destroy_value);

    list_insert(graph->mylist, new);
    graph->size++;
    
}

void graph_remove_node(Graph graph, int id){

    GraphNode node =  malloc(sizeof(*node));
    node->id =id;
    node->ingoing =list_create(compare_objects,destroy_value);
    node->outgoing =list_create(compare_objects,destroy_value);
    
    list_remove(graph->mylist,list_find_node(graph->mylist,node));

}

void graph_insert_edge(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse, int money, char* mydate){

    
    ListNode node_found1 = list_find_node(graph->mylist, vertex_sourse);
    ListNode node_found2 = list_find_node(graph->mylist, vertex_dest);
    GraphNode temp1,temp2;
    if(node_found1 == NULL){
        
        graph_add_node(graph,vertex_sourse->id);
        node_found1 = list_find_node(graph->mylist, vertex_sourse);
    
    }
    if(vertex_dest == NULL){
        graph_add_node(graph,vertex_dest->id);
        node_found2= list_find_node(graph->mylist,vertex_dest);
        
        
    }
    

    temp1 = (GraphNode)list_node_value(graph->mylist, node_found1);
    temp2 = (GraphNode)list_node_value(graph->mylist, node_found2);

    
   

    Vertex myvertexdest = malloc(sizeof(*myvertexdest));
    myvertexdest->money=money;
    myvertexdest->mydate = malloc(strlen(mydate) + 1);
    strcpy(myvertexdest->mydate,mydate);
    myvertexdest->dest=temp2;
    myvertexdest->sourse=temp1;

    Vertex myvertexsour = malloc(sizeof(*myvertexsour));
    myvertexsour->money=money;
    myvertexsour->mydate = malloc(strlen(mydate) + 1);
    strcpy(myvertexsour->mydate,mydate);
    myvertexsour->dest=temp2;
    myvertexsour->sourse=temp1;

    
    list_insert(temp1->outgoing,myvertexdest);
    list_insert(temp2->ingoing,myvertexsour);
    

    
}

Vertex graph_get_edge(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse){
    
    ListNode node_found1 = list_find_node(graph->mylist, vertex_dest);
    ListNode node_found2 = list_find_node(graph->mylist, vertex_sourse);

    GraphNode node1 = (GraphNode)list_node_value(graph->mylist, node_found1);
    GraphNode node2 = (GraphNode)list_node_value(graph->mylist, node_found2);
    
    // Iterate over all nodes in the main graph list
    for (ListNode node = list_first(graph->mylist); node != NULL; node = list_next(graph->mylist, node)) {
        GraphNode graph_node = (GraphNode)list_node_value(graph->mylist, node);

        // Check if the current graph node is the source node we are looking for
        if (graph_node->id == node2->id) {
            
            // Iterate over all outgoing edges of the source node
            for (ListNode edge_node = list_first(graph_node->outgoing); edge_node != NULL; edge_node = list_next(graph_node->outgoing, edge_node)) {
                Vertex edge = (Vertex)list_node_value(graph_node->outgoing, edge_node);
                // Check if the destination of the edge matches the target destination node
                if (edge->dest== node1) {
                    
                    return edge;  
                }
            }
        }
    }
    return NULL;
}

void graph_remove_edge(Graph graph, Pointer vertex_dest, Pointer vertex_sourse){

    Vertex edge =graph_get_edge(graph,vertex_dest,vertex_sourse);
    if (edge == NULL) {
        return ;  
    }
    ListNode node_found1 = list_find_node(graph->mylist, vertex_sourse);
    GraphNode node1 = (GraphNode)list_node_value(graph->mylist, node_found1);
    
    // list_remove(node1->outgoing,edge);
    // list_remove(node2->ingoing,edge);

}


int graph_get_money(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse) {
    
    // Return the money if the edge exists
    Vertex edge =graph_get_edge(graph,vertex_dest,vertex_sourse);
    if (edge != NULL) {
        return edge->money;  
    }
    return 0;
}

char* graph_get_date(Graph graph, GraphNode vertex_dest, GraphNode vertex_sourse){
    
    // Return the date if the edge exists
    Vertex edge =graph_get_edge(graph,vertex_dest,vertex_sourse);
    if (edge != NULL) {
        return edge->mydate;  
    }
    return 0;
}


void graph_print(Graph graph) {
    // Print the number of nodes in the graph
    printf("Graph contains %d nodes:\n", graph->size);

    // Iterate through all nodes in the list of the graph
    ListNode node = list_first(graph->mylist);
    while (node != NULL) {
        
        GraphNode graph_node = (GraphNode)list_node_value(graph->mylist, node);
        printf("Node %d:", graph_node->id);

        // Iterate through all outgoing edges of this node
        ListNode outgoing_edge = list_first(graph_node->outgoing);
        if (outgoing_edge == NULL) {
            // If no outgoing edges, print NULL
            printf("  NULL\n");
        } else {
            while (outgoing_edge != NULL) {
                // Get the GraphNode destination for the outgoing edge
                Vertex dest_node =  (Vertex)list_node_value(graph_node->outgoing, outgoing_edge); ;
                //

                printf("    -> Node %d\n", dest_node->dest->id);

                // Move to the next outgoing edge
                outgoing_edge = list_next(graph_node->outgoing, outgoing_edge);
            }
        }

        // Move to the next node in the graph
        node = list_next(graph->mylist, node);
    }
}



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
