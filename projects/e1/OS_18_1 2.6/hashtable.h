#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "graph.h"

struct hash_table_head
{
	int node_count;
	int hash_key;
	struct hash_table* table;
};

struct hash_table
{
	struct bucket** buckets;
};

struct bucket{
	int num_of_nodes;
	struct list_of_nodes* first_node;
};



int hash_function(struct hash_table_head**, char** name); 
int init_hash_table(struct hash_table_head** head);
struct graph_node* node_exists(struct hash_table_head** head, char** name, int hash_value);

// PRINTING FUNCTIONS
int print_hash_table(struct hash_table_head** head);
int print_bucket(struct bucket* bucket);
int print_hash_table2(struct hash_table_head** head);

// INSERT DATA FUNCTONS
int insert_node(struct hash_table_head** head, char** name);
int insert_vert(struct hash_table_head** head, char** name1, char** name2, int cost);
int add_node_to_bucket(struct bucket* buc, struct graph_node* node);

// DELETE DATA FUNCTIONS
int delete_node(struct hash_table_head** head, char** name);
int delete_edges(struct hash_table_head** head, char** name);
int delete_edge(struct hash_table_head** head, char** name_i, char** name_j, int w);

// MODIFY
int modify_edge(struct hash_table_head** head, char** name_i, char** name_j, int w, int nw);

// RECEIVING
int receiving_edges(struct hash_table_head** head, char** name, FILE** fp);

// FREE FUNCTIONS
int free_hash_table(struct hash_table_head** head);
int free_bucket(struct bucket* buc);
int free_node_list(struct list_of_nodes* node);
int free_edges_list( struct list_of_edges* list );

#endif
