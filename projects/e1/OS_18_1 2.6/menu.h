#ifndef MENU_H_
#define MENU_H_

//  - 1   i(nsert) Ni
int insert_Ni(struct hash_table_head** head, char** node_name, FILE** fp);
//  - 2   (i)n(sert) Ni Nj weight
int insert_Ni_Nj_weight(struct hash_table_head** head, char** node_i_name, char** node_j_name, int w, FILE** fp);
//  - 3   d(elete) Ni
int delete_Ni(struct hash_table_head** head, char** node_name, FILE** fp);
//  - 4   (de)l(ete) Ni Nj weight
int delete_Ni_Nj_weight(struct hash_table_head** head, char** node_i_name, char** node_j_name, int w, FILE** fp);
//  - 5   m(odify) Ni Nj weight nweight
int modify_Ni_Nj_weight_nweight(struct hash_table_head** head, char** node_i_name, char** node_j_name, int w, int nw, FILE** fp);
//  - 6   r(eceiving) Ni
int receiving_Ni(struct hash_table_head** head, char** node_name, FILE** fp);
//  - 7   c(irclefind) Ni
int circlefind_Ni(struct hash_table_head** head, char** node_name, FILE** fp);
//  - 8   f(indcircles) Ni k
int findcircles_i_k(struct hash_table_head** head, char** node_name, int k, FILE** fp);
//  - 9   t(raceflow) Ni Nj l
int traceflow_Ni_Nj_l(struct hash_table_head** head, char** node_i_name, char** node_j_name, int l, FILE** fp);


#endif
