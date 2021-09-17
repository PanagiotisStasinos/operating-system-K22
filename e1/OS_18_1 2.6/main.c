#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "graph.h"
#include "menu.h"

#define MAX_NAME_LENGHT 10

int main (int argc, char **argv)
{

			///////////////////////
			/*     Arguments     */
			///////////////////////
	char arg1[3];
	char arg2[3];

	strcpy(arg1, "-i");
	strcpy(arg2, "-o");

	char* input = NULL;   // .txt given
	char* output = NULL;  // .txt e3odou

	if( argc == 1){
		printf("No arguments were given!\n");
		return(0);  //terminating program
	}
	else if( argc == 3 ){
		if( strcmp(argv[2], arg1) == 0 || strcmp(argv[2], arg2) == 0 ){
			printf("More arguments were given!\nProgram Terminated\n");
			return(0);  //terminating program
		}
			
		if( strcmp(argv[1], arg1) == 0 ){
			input = (char*)malloc(strlen(argv[2]));
			strcpy(input, argv[2]);	
			printf("INPUT FILE : %s\nno OUTPUT FILE was given\n", input);
		}
		else{
 			printf("Error in the arguments given!\n");
			return 0;
		}
	}
	else if( argc < 5 ){
		printf("Less arguments were given!\n");
		return(0);  //terminating program
	}
	else if( argc == 5 ){
		if( strcmp(argv[1], arg1) == strcmp(argv[3], arg2) ){    // prwto argument to inout file
			input = (char*)malloc(strlen(argv[2]));
			strcpy(input, argv[2]);
			output = (char*)malloc(strlen(argv[4]));
			strcpy(output, argv[4]);
		}
		else if( strcmp(argv[1], arg2) == strcmp(argv[3], arg1)  ){   // prwto argument to output file
			output = (char*)malloc(strlen(argv[2]));
			strcpy(output, argv[2]);
			input = (char*)malloc(strlen(argv[4]));
			strcpy(input, argv[4]);
		}
		else{
			printf("Error in the arguments given!\n");
			return(0);  //terminating program
		}

		printf("INPUT FILE : %s\nOUTPUT FILE : %s\n", input, output);
		
	}
	else{
		printf("More arguments were given!\nProgram Terminated\n");
		return(0);  //terminating program
	}
	
	/////////////////////////////////////////////////////
	//    READ INPUT FILE AND INITIALIZE HASH TABLE    //
	/////////////////////////////////////////////////////
	struct hash_table_head* head = NULL;

	init_hash_table(&head);


	FILE *fp;
	fp = fopen (input, "r");
	if ( fp == NULL ) // apotuxia anoigmatos arxeiou keimenou
	{
		printf("Could not open input file\n");
		return -1;
	}

	char* name1;  //
	char* name2;  //
	char* cost;   //
	int c;

	int count=0;  // number of edges

	char* line = NULL;
	size_t len = 0;
	ssize_t read;

	const char s1[2] = "\t";
	const char s2[2] = "\n";
	while( (read = getline(&line, &len, fp)) != -1 ){
		count++;

//		printf("%d %s",count, line);
		name1 = strtok( line , s1 );
		name2 = strtok( NULL , s1 );
		cost = strtok( NULL , s2);
		c = atoi(cost);

		insert_node(&head, &name1);
		insert_node(&head, &name2);
		insert_vert(&head, &name1, &name2, c);

	}

	fclose(fp);
	if(line)
		free(line);

	FILE *fp1 = NULL;
	if(output != NULL){
		fp1 = fopen (output, "w+");
		if ( fp1 == NULL ) // apotuxia anoigmatos arxeiou keimenou
		{
			printf("Could not open output file\n");
		}
	}
	
	////////////////
	//    MENU    //
	////////////////
    	char *buffer;
   	size_t bufsize = 16;
   	size_t characters;

   	buffer = (char *)malloc(bufsize * sizeof(char));

	char *entolh;
	const char s[2] = " ";


	char *node_name;
	char *node_name_2;
	char *weight; int w;
	char *nweight; int nw;
	char *k; int int_k;
	char *l; int int_l;

	char* more_arguments_flag;

	char *temp_buffer;  //krataei autousia thn entolh

	char* search_buffer[10];

	while(1){
	   	printf(">> Type something: ");
	   	characters = getline(&buffer,&bufsize,stdin);
/// gia na dexetai kai to sketo enter
if( characters == 1 ){
	buffer[characters] = '\0';
}
else{
	buffer[characters-1] = '\0';
}


		temp_buffer = (char *)malloc(strlen(buffer) * sizeof(char));
		strcpy(temp_buffer, buffer);

		entolh = strtok( buffer , s );
//  - 1   i(nsert) Ni
		if( strcmp( entolh , "i\0" ) == 0 ){
			node_name = strtok( NULL , s );
			if( node_name == NULL ){
				printf("Less argument given\n");
			}
			else{
				more_arguments_flag = strtok( NULL, s );
				if( more_arguments_flag == NULL ){
					if(fp1!=NULL){
						fprintf(fp1, "%s\n", temp_buffer);
					}	
					insert_Ni(&head, &node_name, &fp1);
				}
				else{
					printf("More arguments given\n");
				}
			}
		}
//  - 2   (i)n(sert) Ni Nj weight
		else if( strcmp( entolh , "n\0" ) == 0 ){
			node_name = strtok( NULL , s );
			if( node_name != NULL ){
				node_name_2 = strtok( NULL , s );
				if( node_name_2 != NULL ){
					weight = strtok( NULL , s );
					if( weight != NULL ){
						w = atoi(weight);
						more_arguments_flag = strtok( NULL, s );
						if( more_arguments_flag == NULL ){
							if(fp1!=NULL){
								fprintf(fp1, "%s\n", temp_buffer);
							}
							insert_Ni_Nj_weight(&head, &node_name, &node_name_2, w, &fp1);
						}
						else{
							printf("More arguments given\n");
						}
					}
					else{ printf("Less argument given\n"); }
				}
				else{ printf("Less argument given\n"); }
			}
			else{ printf("Less argument given\n"); }	
		}
//  - 3   d(elete) Ni
		else if( strcmp( entolh , "d\0" ) == 0 ){
			node_name = strtok( NULL , s );
			if( node_name == NULL ){
				printf("Less argument given\n");
			}
			else{
				more_arguments_flag = strtok( NULL, s );
				if( more_arguments_flag == NULL ){
					if(fp1!=NULL){
						fprintf(fp1, "%s\n", temp_buffer);
					}
					delete_Ni(&head,&node_name,&fp1);
				}
				else{
					printf("More arguments given\n");
				}
			}
		}
//  - 4   (de)l(ete) Ni Nj weight
		else if( strcmp( entolh , "l\0" ) == 0 ){
			node_name = strtok( NULL , s );
			if( node_name != NULL ){
				node_name_2 = strtok( NULL , s );
				if( node_name_2 != NULL ){
					weight = strtok( NULL , s );
					if( weight != NULL ){
						w = atoi(weight);
						more_arguments_flag = strtok( NULL, s );
						if( more_arguments_flag == NULL ){
							if(fp1!=NULL){
								fprintf(fp1, "%s\n", temp_buffer);
							}
							delete_Ni_Nj_weight(&head, &node_name, &node_name_2, w, &fp1);
						}
						else{
							printf("More arguments given\n");
						}
					}
					else{
						if(fp1!=NULL){
							fprintf(fp1, "%s\n", temp_buffer);
						}
						delete_Ni_Nj_weight(&head, &node_name, &node_name_2, -1, &fp1); 
					}
				}
				else{ printf("Less argument given\n"); }
			}
			else{ printf("Less argument given\n"); }
		}
//  - 5   m(odify) Ni Nj weight nweight
		else if( strcmp( entolh, "m\0" ) == 0 ){
			node_name = strtok( NULL , s );
			if( node_name != NULL ){
				node_name_2 = strtok( NULL , s );
				if( node_name_2 != NULL ){
					weight = strtok( NULL , s );
					if( weight != NULL ){
						w = atoi(weight);
						nweight = strtok( NULL, s );
						if( nweight != NULL ){
							nw = atoi(nweight);
							more_arguments_flag = strtok( NULL, s );
							if( more_arguments_flag == NULL ){
								if(fp1!=NULL){
									fprintf(fp1, "%s\n", temp_buffer);
								}
								modify_Ni_Nj_weight_nweight(&head, &node_name, &node_name_2, w, nw, &fp1);
							}
							else{
								printf("More arguments given\n");
							}
						}
						else{ printf("Less argument given\n"); }
					}
					else{ printf("Less argument given\n"); }
				}
				else{ printf("Less argument given\n"); }
			}
			else{ printf("Less argument given\n"); }
		}
//  - 6   r(eceiving) Ni
		else if( strcmp( entolh , "r\0" ) == 0 ){
			node_name = strtok( NULL , s );
			if( node_name == NULL ){
				printf("Less argument given\n");
			}
			else{
				more_arguments_flag = strtok( NULL, s );
				if( more_arguments_flag == NULL ){
					if(fp1!=NULL){
						fprintf(fp1, "%s\n", temp_buffer);
					}
					receiving_Ni(&head, &node_name, &fp1);
				}
				else{
					printf("More arguments given\n");
				}
			}
		}
//  - 7   c(irclefind) Ni
		else if( strcmp( entolh , "c\0" ) == 0 ){
			node_name = strtok( NULL , s );
			if( node_name == NULL ){
				printf("Less argument given\n");
			}
			else{
				more_arguments_flag = strtok( NULL, s );
				if( more_arguments_flag == NULL ){
					if(fp1!=NULL){
						fprintf(fp1, "%s\n", temp_buffer);
					}	
					circlefind_Ni(&head, &node_name, &fp1);
				}
				else{
					printf("More arguments given\n");
				}
			}
		}
//  - 8   f(indcircles) Ni k
		else if( strcmp( entolh , "f\0" ) == 0 ){
			node_name = strtok( NULL , s );
			if( node_name != NULL ){
				k = strtok( NULL , s );
				if( k != NULL ){
					int_k = atoi(k);
					more_arguments_flag = strtok( NULL, s );
					if( more_arguments_flag == NULL ){
						if(fp1!=NULL){
							fprintf(fp1, "%s\n", temp_buffer);
						}
						findcircles_i_k(&head, &node_name, int_k, &fp1);
					}
					else{
						printf("More arguments given\n");
					}
				}
				else{ printf("Less argument given\n"); }
			}
			else{ printf("Less argument given\n"); }
			
		}
//  - 9   t(raceflow) Ni Nj l
		else if( strcmp( entolh , "t\0" ) == 0 ){
			node_name = strtok( NULL , s );
			if( node_name != NULL ){
				node_name_2 = strtok( NULL , s );
				if( node_name_2 != NULL ){
					l = strtok( NULL , s );
					if( l != NULL ){
						int_l = atoi(l);
						more_arguments_flag = strtok( NULL, s );
						if( more_arguments_flag == NULL ){
							if(fp1!=NULL){
								fprintf(fp1, "%s\n", temp_buffer);
							}	
							traceflow_Ni_Nj_l(&head, &node_name, &node_name_2, int_l, &fp1);
						}
						else{
							printf("More arguments given\n");
						}
					}
					else{ printf("Less argument given\n"); }
				}
				else{ printf("Less argument given\n"); }
			}
			else{ printf("Less argument given\n"); }
			
		}
//  - 10  e(xit)
		else if( strcmp( entolh , "e\0" ) == 0 ){
			more_arguments_flag = strtok( NULL, s );
			if( more_arguments_flag == NULL ){
				if(fp1!=NULL){
					fprintf(fp1, "%s\n", temp_buffer);
					fprintf(fp1, "\texit program\n");
				}	
				printf("exit program\n");
				break;
			}
			else{
				printf("More arguments given\n");
			}
		}
//  -  prints hash table (just for debbuging)
		else if( strcmp( entolh , "p\0" ) == 0 ){
			more_arguments_flag = strtok( NULL, s );
			if( more_arguments_flag == NULL ){	
				printf("print hash table\n");
				print_hash_table(&head);
				//print_hash_table2(&head);
			}
			else{
				printf("More arguments given\n");
			}
		}
		else if( strcmp (temp_buffer,"\0") ){
			printf("No command given\n");
		}
		else{
			printf("There is no command '%s'\n",temp_buffer);
		}
	}

	free_hash_table(&head);
	if(output != NULL){
		free(output);
	}	
	free(input);

	if(fp1!=NULL){
		fclose(fp1);
	}
}
