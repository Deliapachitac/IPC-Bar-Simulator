#include <stdio.h>
#include "ADTGraph.h"


int main(void){

    
    Graph mygraph = graph_create();
    printf("The size is : %d\n", graph_size(mygraph));

   
    for (int i = 0; i < 10; i=i+2)
    {
        graph_add_node(mygraph, i);
        
    }
    printf("The size is : %d\n", graph_size(mygraph));
    GraphNode node =  graph_node_create(4);
    GraphNode node2 =  graph_node_create(7);


    graph_remove_node(mygraph,2);

    graph_print(mygraph);
    printf("The size is : %d\n", graph_size(mygraph));


    graph_insert_edge(mygraph, node, node2, 23, "12-3-4");

    graph_print(mygraph);

    printf(" the money is : %d",graph_get_money(mygraph,node,node2));
    printf(" the date is : %s",graph_get_date(mygraph,node,node2));


    return 0;
}