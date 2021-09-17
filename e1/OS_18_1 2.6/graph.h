#ifndef GRAPH_H_
#define GRAPH_H_

struct graph_node
{
	char* name;
	int edge_count;
	struct list_of_edges* first;
};

struct list_of_nodes
{
	struct graph_node* node;
	struct list_of_nodes* next_node;
};

struct list_of_edges
{
	int cost;
	struct graph_node* node;
	struct list_of_edges* next_edge;		
};

int compare_node_name(struct graph_node* node, char** name);

int print_node(struct graph_node* node);

#endif
