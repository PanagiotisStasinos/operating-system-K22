#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "graph.h"

int compare_node_name(struct graph_node* node, char** name){
	return strcmp(node->name, *name);
}

int print_node(struct graph_node* node){
	printf("NAME %s - ",node->name);

	struct list_of_edges* temp_edge;

	if(node->first == NULL){
		printf("No Outcoming edges\n");
	}
	else{
		temp_edge = node->first;
		printf("Outcoming edges %d -",node->edge_count);
		while(temp_edge != NULL){
			printf("(%s|%d) ",temp_edge->node->name,temp_edge->cost);

			temp_edge = temp_edge->next_edge;
		}
		printf("\n");
	}
}
