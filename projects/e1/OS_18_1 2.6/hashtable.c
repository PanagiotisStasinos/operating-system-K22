#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "hashtable.h"
#include "graph.h"

#define HASH_KEY 7

int hash_function(struct hash_table_head** head, char** name){
	int value=0;
	char* temp;
	int count=0;
	temp = (char*)malloc(strlen(*name));
	strcpy(temp, *name);
	while( temp[count] != '\0'){
		value = value + temp[count];
		count++;
	}
	return value%( (*head)->hash_key ) ;
}

struct graph_node* node_exists(struct hash_table_head** head,  char** name, int hash_value){	//returns 1 if doesn t exist, returns 0 if it exists
	struct bucket* temp_bucket = (*head)->table->buckets[hash_value];
	struct list_of_nodes* temp_node ;

	if( temp_bucket->first_node != NULL ){  // uparxoun nodes se auto to bucket
		temp_node = temp_bucket->first_node;
		while( temp_node != NULL ){ 
			if( compare_node_name(temp_node->node, name) == 0 ){	//an ta onomata einai idia
				return temp_node->node;
			}
			temp_node = temp_node->next_node;
		}
	}
	return NULL;			
}

int init_hash_table(struct hash_table_head** head){
	int b=0;  //buckets count
	int n=0;  //bucket's nodes count

	*head = (struct hash_table_head*)malloc(sizeof(struct hash_table_head));

	(*head)->node_count = 0;
	(*head)->hash_key = HASH_KEY;
	(*head)->table = (struct hash_table*)malloc(sizeof(struct hash_table));

	(*head)->table->buckets = (struct bucket**)malloc(HASH_KEY * sizeof(struct bucket*));
	for(b=0 ; b<HASH_KEY ; b++){
		(*head)->table->buckets[b] = NULL ;
		(*head)->table->buckets[b] = (struct bucket*)malloc(sizeof(struct bucket));

		(*head)->table->buckets[b]->num_of_nodes = 0;
		(*head)->table->buckets[b]->first_node = NULL;
	}
}


		//////////////////////////////////
		//				//
		//	PRINTING FUNCTIONS (3)	//
		//				//
		//////////////////////////////////

int print_hash_table(struct hash_table_head** head){
	int b=0;  //buckets count

	if( (*head) == NULL ){
		printf("There is no hash to print\n");
	}
	else{
		printf("plithos nodes %d\n",(*head)->node_count);
		printf("hash key %d\n",(*head)->hash_key);

		for(b=0 ; b<(*head)->hash_key ; b++){
			if( (*head)->table->buckets[b]->first_node  == NULL ){  //elegxei an h prwth eggrafh kathe bucket einai kenh
				printf("Bucket %d is empty\n\n", b);
			}
			else{
				printf("Bucket %d nodes %d :\n", b, (*head)->table->buckets[b]->num_of_nodes);
				print_bucket((*head)->table->buckets[b]);
				printf("\n");
			}
		}
	}
}

int print_bucket(struct bucket* buc){
	struct list_of_nodes* temp = buc->first_node;

	while( temp != NULL ){
		print_node(temp->node);
		temp=temp->next_node;
	}
 
}

int print_hash_table2(struct hash_table_head** head){
	int b; //bucket_counter
	struct bucket* temp_bucket ;
	struct list_of_nodes* temp_node ;
	struct list_of_edges* temp_edge ;
	struct list_of_edges* prev_edge ;
printf("hash table node %d\n",(*head)->node_count);
	for(b=0 ; b<(*head)->hash_key ; b++){  //for all buckets
		temp_bucket = (*head)->table->buckets[b];
		printf("bucket %d - %d\n",b,temp_bucket->num_of_nodes);
		temp_node = temp_bucket->first_node;
		while(temp_node != NULL){  // for all nodes
			temp_edge = temp_node->node->first;
			prev_edge = temp_node->node->first;
			while(temp_edge != NULL){
				printf("%s - %d -> %s\n",temp_node->node->name,temp_edge->cost,temp_edge->node->name);
				
				prev_edge = temp_edge;
				temp_edge = temp_edge->next_edge;
			}
			temp_node = temp_node->next_node;			
		}
	}
}
		//////////////////////////////////////////
		//					//
		//	INSERT DATA FUNCTIONS (3)	//
		//					//
		//////////////////////////////////////////

int insert_node(struct hash_table_head** head, char** name){
	int hash_value = hash_function(head, name);
	struct graph_node* temp_node;
	if ( node_exists(head, name, hash_value) == NULL){
		temp_node = (struct graph_node*)malloc(sizeof(struct graph_node));

		temp_node->name = (char*)malloc(strlen(*name));

		strcpy(temp_node->name, *name);
		temp_node->edge_count = 0;
		temp_node->first= NULL;

		(*head)->node_count ++;

		add_node_to_bucket( (*head)->table->buckets[hash_value], temp_node );
		return 0;
	}
	return -1;
}

int add_node_to_bucket(struct bucket* buc, struct graph_node* node){
	struct list_of_nodes* temp;

	buc->num_of_nodes++;

	if(buc->num_of_nodes ==1){
		buc->first_node = (struct list_of_nodes*)malloc(sizeof(struct list_of_nodes));
		buc->first_node->node = node;
		buc->first_node->next_node = NULL;
	}
	else{
		temp = buc->first_node;
		while( temp->next_node != NULL ){
			temp = temp->next_node;
		}

		temp->next_node = (struct list_of_nodes*)malloc(sizeof(struct list_of_nodes));
		temp->next_node->node = node;
		temp->next_node->next_node = NULL;
	}
}

int insert_vert(struct hash_table_head** head, char** name1, char** name2, int cost){
	int hash_value1 = hash_function(head, name1);
	int hash_value2 = hash_function(head, name2);

	struct graph_node* temp1 ;
	struct graph_node* temp2 ;

	temp1 = node_exists(head, name1, hash_value1);
	if(temp1 == NULL){
		//printf("Node %s doesn t exist\n",*name1);
		return -1;
	}
	temp2 = node_exists(head, name2, hash_value2);
	if(temp2 == NULL){
		//printf("Node %s doesn t exist\n",*name2);
		return -1;
	}

	struct list_of_edges* temp;

	temp1->edge_count++;

	if( temp1->edge_count == 1){
		temp1->first = (struct list_of_edges*)malloc(sizeof(struct list_of_edges));

		temp1->first->cost = cost;
		temp1->first->node = temp2;
		temp1->first->next_edge = NULL;
	}
	else{
		temp = temp1->first;
		while( temp->next_edge != NULL ){
			temp = temp->next_edge;
		}
		temp->next_edge = (struct list_of_edges*)malloc(sizeof(struct list_of_edges));
		temp->next_edge->cost = cost;
		temp->next_edge->node = temp2;
		temp->next_edge->next_edge = NULL;		
	}
	return 0;
}

		//////////////////////////////////////////
		//					//
		//	DELETE DATA FUNCTIONS (3)	//
		//					//
		//////////////////////////////////////////


int delete_node(struct hash_table_head** head, char** name){
	int hash_value = hash_function(head, name);

	struct graph_node* temp;

	temp = node_exists(head, name, hash_value);
	if(temp == NULL){
		//printf("Node %s doesn t exist\n",*name);
		return -1;
	}
	(*head)->node_count--;

	struct bucket* temp_bucket = (*head)->table->buckets[hash_value];
	struct list_of_nodes* temp_node ;
	struct list_of_nodes* prev_node ;

	if( temp_bucket->first_node != NULL ){  // uparxoun nodes se auto to bucket
		temp_node = temp_bucket->first_node;
		if( compare_node_name(temp_node->node, name) == 0 ){
			delete_edges(head,name);

			temp_bucket->first_node = temp_node->next_node;
			
			free_edges_list(temp_node->node->first);
			free(temp_node->node->name);
			free(temp_node->node);
			free(temp_node);

			temp_bucket->num_of_nodes--;
			return 0;
		}
		else{
			prev_node = temp_node;
			temp_node = temp_node->next_node;
			while( temp_node != NULL ){
				if( compare_node_name(temp_node->node, name) == 0 ){	//an ta onomata einai idia
					delete_edges(head,name);
					prev_node->next_node = temp_node->next_node;

					free_edges_list(temp_node->node->first);
					free(temp_node->node->name);
					free(temp_node->node);
					free(temp_node);

					temp_bucket->num_of_nodes--;
					return 0;
				}
				prev_node = temp_node;
				temp_node = temp_node->next_node;
			}
		}
	}
	return -1;
}

int delete_edges(struct hash_table_head** head, char** name){
	int b; //bucket_counter

	struct bucket* temp_bucket ;
	struct list_of_nodes* temp_node ;
	struct list_of_edges* temp_edge ;
	struct list_of_edges* prev_edge ;

	for(b=0 ; b<(*head)->hash_key ; b++){  //for all buckets
		temp_bucket = (*head)->table->buckets[b];

		temp_node = temp_bucket->first_node;
		while(temp_node != NULL){  // for all nodes
			temp_edge = temp_node->node->first;
			prev_edge = temp_node->node->first;
			if(temp_edge != NULL){
				if( compare_node_name(temp_edge->node, name) == 0 ){
					//printf("%s - %d -> %s\n",temp_node->node->name,temp_edge->cost,*name);
					temp_node->node->first = temp_edge->next_edge;
temp_node->node->edge_count--;
					free(temp_edge);
				}
				prev_edge = temp_edge;
				temp_edge = temp_edge->next_edge;
				while(temp_edge != NULL){
					if( compare_node_name(temp_edge->node, name) == 0 ){
						//printf("%s - %d -> %s\n",temp_node->node->name,temp_edge->cost,*name);
						prev_edge->next_edge = temp_edge->next_edge;
temp_node->node->edge_count--;
						free(temp_edge);
						temp_edge = prev_edge->next_edge;
					}
					else{
						prev_edge = temp_edge;
						temp_edge = temp_edge->next_edge;
					}
				}
			}

			temp_node = temp_node->next_node;			
		}
	}
}

int delete_edge(struct hash_table_head** head, char** name_i, char** name_j, int w){
	int hash_value_i = hash_function(head, name_i);
	int hash_value_j = hash_function(head, name_j);

	struct graph_node* temp_i = node_exists(head, name_i, hash_value_i);
	if(temp_i == NULL){
		return -1;
	}
	struct graph_node* temp_j = node_exists(head, name_j, hash_value_j);
	if(temp_j == NULL){
		return -2;
	}

	struct bucket* temp_bucket = (*head)->table->buckets[hash_value_i];
	struct list_of_nodes* temp_node = temp_bucket->first_node;
	struct list_of_edges* temp_edge ;
	struct list_of_edges* prev_edge ;

	if( w ==-1 ){
		while(temp_node != NULL){  // for all nodes
			temp_edge = temp_node->node->first;
			prev_edge = temp_node->node->first;
			if(temp_edge != NULL){
				if( compare_node_name(temp_edge->node, name_j) == 0 ){
					temp_node->node->first = temp_edge->next_edge;
temp_node->node->edge_count--;
					free(temp_edge);
				}
				prev_edge = temp_edge;
				temp_edge = temp_edge->next_edge;
				while(temp_edge != NULL){
					if( compare_node_name(temp_edge->node, name_j) == 0 ){
						prev_edge->next_edge = temp_edge->next_edge;
temp_node->node->edge_count--;
						free(temp_edge);
						temp_edge = prev_edge->next_edge;
					}
					else{
						prev_edge = temp_edge;
						temp_edge = temp_edge->next_edge;
					}
				}
			}
			temp_node = temp_node->next_node;			
		}
		return 0;
	}

	while(temp_node != NULL){  // for all nodes
		temp_edge = temp_node->node->first;
		prev_edge = temp_node->node->first;
		if(temp_edge != NULL){
			if( compare_node_name(temp_edge->node, name_j) == 0 ){
				if( temp_edge->cost == w ){
					//printf("%s - %d -> %s\n",temp_node->node->name,temp_edge->cost, *name_j);
					temp_node->node->first = temp_edge->next_edge;
temp_node->node->edge_count--;
					free(temp_edge);
					return 0;
				}
			}
			prev_edge = temp_edge;
			temp_edge = temp_edge->next_edge;
			while(temp_edge != NULL){
				if( compare_node_name(temp_edge->node, name_j) == 0 ){
					if( temp_edge->cost == w ){
						//printf("%s - %d -> %s\n",temp_node->node->name,temp_edge->cost,*name_j);
						prev_edge->next_edge = temp_edge->next_edge;
temp_node->node->edge_count--;
						free(temp_edge);
						return 0;
					}
				}
				prev_edge = temp_edge;
				temp_edge = temp_edge->next_edge;
			}
		}
		temp_node = temp_node->next_node;			
	}
	return -3;
}

///////////////
//   MODIFY  //
///////////////

int modify_edge(struct hash_table_head** head, char** name_i, char** name_j, int w, int nw){
	int hash_value_i = hash_function(head, name_i);
	int hash_value_j = hash_function(head, name_j);

	struct graph_node* temp_i = node_exists(head, name_i, hash_value_i);
	if(temp_i == NULL){
		return -1;
	}
	struct graph_node* temp_j = node_exists(head, name_j, hash_value_j);
	if(temp_j == NULL){
		return -2;
	}

	struct list_of_edges* temp_edge = temp_i->first ;
	struct list_of_edges* prev_edge = temp_i->first ;

	if(temp_edge != NULL){
		if( compare_node_name(temp_edge->node, name_j) == 0 ){
			if( temp_edge->cost == w ){
				//printf("%s - %d -> %s  new: %d\n",temp_i->name,temp_edge->cost, *name_j,nw);
				temp_i->first->cost = nw;
				return 0;
			}
		}
		prev_edge = temp_edge;
		temp_edge = temp_edge->next_edge;
		while(temp_edge != NULL){
			if( compare_node_name(temp_edge->node, name_j) == 0 ){
				if( temp_edge->cost == w ){
					//printf("%s - %d -> %s\n",temp_i->name,temp_edge->cost,*name_j);
					temp_edge->cost = nw;
					return 0;
				}
			}
			prev_edge = temp_edge;
			temp_edge = temp_edge->next_edge;
		}
	}
	return -3;
}


//////////////////
//   RECEIVING  //
//////////////////
int receiving_edges(struct hash_table_head** head, char** name, FILE** fp){
	int hash_value = hash_function(head, name);

	struct graph_node* temp = node_exists(head, name, hash_value);
	if(temp == NULL){
		return -1;
	}

	int b; //bucket_counter
	int flag = 0;  // 0 kanena edge kai 1 toulaxiston 1 edge
	struct bucket* temp_bucket ;
	struct list_of_nodes* temp_node ;
	struct list_of_edges* temp_edge ;
	struct list_of_edges* prev_edge ;

	for(b=0 ; b<(*head)->hash_key ; b++){  //for all buckets
		temp_bucket = (*head)->table->buckets[b];

		temp_node = temp_bucket->first_node;
		while(temp_node != NULL){  // for all nodes
			temp_edge = temp_node->node->first;
			prev_edge = temp_node->node->first;
			while(temp_edge != NULL){
				if( compare_node_name(temp_edge->node, name) == 0 ){
					if(flag == 0){
						printf("Rec-edges\t|%s|->%d->|%s|\n",temp_node->node->name,temp_edge->cost,temp_edge->node->name);
						if(*fp!=NULL){
							fprintf(*fp, "\tRec-edges\t|%s|->%d->|%s|\n",temp_node->node->name,temp_edge->cost,temp_edge->node->name);
						}
						flag = 1;
					}
					else{
						printf("\t\t|%s|->%d->|%s|\n",temp_node->node->name,temp_edge->cost,temp_edge->node->name);
						if(*fp!=NULL){
							fprintf(*fp, "\t\t\t|%s|->%d->|%s|\n",temp_node->node->name,temp_edge->cost,temp_edge->node->name);
						}
					}
				}
				prev_edge = temp_edge;
				temp_edge = temp_edge->next_edge;
			}
			temp_node = temp_node->next_node;
		}
	}
	if( flag==0 ){
		return -2;
	}
}


		//////////////////////////////////
		//				//
		//	  FREE FUNCTIONS (4)	//
		//				//
		//////////////////////////////////


int free_hash_table(struct hash_table_head** head){
	for(int i=0; i<(*head)->hash_key; i++){
		free_bucket((*head)->table->buckets[i]);
	}
	free((*head)->table->buckets);
	(*head)->table->buckets = NULL;

	free( (*head)->table );
	(*head)->table = NULL;

	free( *head );
	*head = NULL;
}

int free_bucket(struct bucket* buc){
	buc->num_of_nodes = 0;
	if(buc->first_node != NULL){
		free_node_list(buc->first_node);
	}
	buc->first_node = NULL;
}

int free_node_list(struct list_of_nodes* tmp_list_node){
	if(tmp_list_node->next_node != NULL){
		free_node_list(tmp_list_node->next_node);
	}

	free_edges_list(tmp_list_node->node->first);
	free(tmp_list_node->node->name);
	free(tmp_list_node->node);
	free(tmp_list_node);
}

int free_edges_list( struct list_of_edges* list ){
	if( list != NULL){
		free_edges_list(list->next_edge);
		free(list);
	}
}














