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

    GraphNode node2 =  graph_node_create(6);

    graph_remove_node(mygraph,2);

    graph_print(mygraph);
    printf("The size is : %d\n", graph_size(mygraph));
	

    graph_insert_existing_edge(mygraph, node, node2, 23, "12-3-4");

    graph_print(mygraph);

    // printf(" the money is : %d",graph_get_money(mygraph,vertex_dest,vertex_sourse));
    // printf(" the date is : %s",);

    return 0;
}