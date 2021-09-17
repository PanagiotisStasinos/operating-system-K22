#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "hashtable.h"

//  - 1   i(nsert) Ni
int insert_Ni(struct hash_table_head** head, char** node_name, FILE** fp){
	if( insert_node(head, node_name) == 0){
		printf("Inserted |%s|\n",*node_name);
		if(*fp!=NULL){
			fprintf(*fp, "\tInserted |%s|\n",*node_name);
		}
	}
	else{
		printf("Node |%s| exists\n",*node_name);
		if(*fp!=NULL){
			fprintf(*fp, "\tNode |%s| exists\n",*node_name);
		}
	}
}

//  - 2   (i)n(sert) Ni Nj weight
int insert_Ni_Nj_weight(struct hash_table_head** head, char** node_i_name, char** node_j_name, int w, FILE** fp){
	if( insert_vert(head, node_i_name, node_j_name, w) == -1 ){
		printf("none\n");
		if(*fp!=NULL){
			fprintf(*fp, "\tnone\n");
		}
	}
	else{
		printf("Inserted |%s|->%d->|%s|\n",*node_i_name, w, *node_j_name);
		if(*fp!=NULL){
			fprintf(*fp, "\tInserted |%s|->%d->|%s|\n", *node_i_name, w, *node_j_name);
		}
	}

}

//  - 3   d(elete) Ni
int delete_Ni(struct hash_table_head** head, char** node_name, FILE** fp){
	if( delete_node(head, node_name) == 0 ){
		printf("Deleted |%s|\n",*node_name);
		if(*fp!=NULL){
			fprintf(*fp, "\tDeleted |%s|\n",*node_name);
		}
	}
	else{
		printf("Node |%s| does not exist - abort-d\n",*node_name);
		if(*fp!=NULL){
			fprintf(*fp, "\tNode |%s| does not exist - abort-d\n",*node_name);
		}
	}
}

//  - 4   (de)l(ete) Ni Nj weight
int delete_Ni_Nj_weight(struct hash_table_head** head, char** node_i_name, char** node_j_name, int w, FILE** fp){
	int returned_value = delete_edge(head, node_i_name, node_j_name, w);
	if( returned_value == -3){
		printf("|%s|->%d->|%s| does not exist - abort-l\n",*node_i_name,w,*node_j_name);
		if(*fp!=NULL){
			fprintf(*fp, "\t|%s|->%d->|%s| does not exist - abort-l\n",*node_i_name,w,*node_j_name);
		}
	}
	else if( returned_value == -2){
		printf("|%s| does not exist - abort-l\n",*node_j_name);
		if(*fp!=NULL){
			fprintf(*fp, "\t|%s| does not exist - abort-l\n",*node_j_name);
		}
	}
	else if( returned_value == -1){
		printf("|%s| does not exist - abort-l\n",*node_i_name);
		if(*fp!=NULL){
			fprintf(*fp, "\t|%s| does not exist - abort-l\n",*node_i_name);
		}
	}
	else if( returned_value == 0){
		printf("Del-vertex |%s|->%d->|%s|\n",*node_i_name,w,*node_j_name);
		if(*fp!=NULL){
			fprintf(*fp, "\tDel-vertex |%s|->%d->|%s|\n",*node_i_name,w,*node_j_name);
		}
	}
}

//  - 5   m(odify) Ni Nj weight nweight
int modify_Ni_Nj_weight_nweight(struct hash_table_head** head, char** node_i_name, char** node_j_name, int w, int nw, FILE** fp){
	int returned_value = modify_edge(head, node_i_name, node_j_name, w, nw);
	if( returned_value == -3){
		printf("|%s|->%d->|%s| does not exist - abort-l\n",*node_i_name,w,*node_j_name);
		if(*fp!=NULL){
			fprintf(*fp, "\t|%s|->%d->|%s| does not exist - abort-l\n",*node_i_name,w,*node_j_name);
		}
	}
	else if( returned_value == -2){
		printf("|%s| does not exist - abort-l\n",*node_j_name);
		if(*fp!=NULL){
			fprintf(*fp, "\t|%s| does not exist - abort-l\n",*node_j_name);
		}
	}
	else if( returned_value == -1){
		printf("|%s| does not exist - abort-l\n",*node_i_name);
		if(*fp!=NULL){
			fprintf(*fp, "\t|%s| does not exist - abort-l\n",*node_i_name);
		}
	}
	else if( returned_value == 0){
		printf("Mod-vertex |%s|->%d->|%s|\n",*node_i_name,w,*node_j_name);
		if(*fp!=NULL){
			fprintf(*fp, "\tMod-vertex |%s|->%d->|%s|\n",*node_i_name,w,*node_j_name);
		}
	}
}

//  - 6   r(eceiving) Ni
int receiving_Ni(struct hash_table_head** head, char** node_name, FILE** fp){
	int returned_value = receiving_edges(head, node_name, fp);
	if( returned_value == -1){
		printf("|%s| does not exist - abort-r\n",*node_name);
		if(*fp!=NULL){
			fprintf(*fp, "\t|%s| does not exist - abort-r\n",*node_name);
		}
	}
	else if( returned_value == -2){
		printf("No-rec-edges %s\n",*node_name);
		if(*fp!=NULL){
			fprintf(*fp, "\tNo-rec-edges %s\n",*node_name);
		}
	}	
}

//  - 7   c(irclefind) Ni
int circlefind_Ni(struct hash_table_head** head, char** node_name, FILE** fp){
	int hash_value = hash_function(head, node_name);

	struct graph_node* temp = node_exists(head, node_name, hash_value);
	if(temp == NULL){
		printf("|%s| does not exist - abort-c\n",*node_name);
		if(*fp!=NULL){
			fprintf(*fp, "\t|%s| does not exist - abort-c\n",*node_name);
		}
		return -1;
	}

	printf("Not implemented\n");
	if(*fp!=NULL){
		fprintf(*fp, "\tNot implemented\n");
	}
	return -2;
}

//  - 8   f(indcircles) Ni k
int findcircles_i_k(struct hash_table_head** head, char** node_name, int k, FILE** fp){
	int hash_value = hash_function(head, node_name);

	struct graph_node* temp = node_exists(head, node_name, hash_value);
	if(temp == NULL){
		printf("|%s| does not exist - abort-f\n",*node_name);
		if(*fp!=NULL){
			fprintf(*fp, "\t|%s| does not exist - abort-f\n",*node_name);
		}
		return -1;
	}

	printf("Not implemented\n");
	if(*fp!=NULL){
		fprintf(*fp, "\tNot implemented\n");
	}
	return -2;
}

//  - 9   t(raceflow) Ni Nj l
int traceflow_Ni_Nj_l(struct hash_table_head** head, char** node_i_name, char** node_j_name, int l, FILE** fp){
	int hash_value_i = hash_function(head, node_i_name);
	int hash_value_j = hash_function(head, node_j_name);

	struct graph_node* temp_i = node_exists(head, node_i_name, hash_value_i);
	if(temp_i == NULL){
		printf("|%s| does not exist - abort-t\n",*node_i_name);
		if(*fp!=NULL){
			fprintf(*fp, "\t|%s| does not exist - abort-t\n",*node_i_name);
		}
		return -1;
	}
	struct graph_node* temp_j = node_exists(head, node_j_name, hash_value_j);
	if(temp_j == NULL){
		printf("|%s| does not exist - abort-t\n",*node_j_name);
		if(*fp!=NULL){
			fprintf(*fp, "\t|%s| does not exist - abort-t\n",*node_j_name);
		}
		return -2;
	}

	printf("Not implemented\n");
	if(*fp!=NULL){
		fprintf(*fp, "\tNot implemented\n");
	}
	return -3;
}


