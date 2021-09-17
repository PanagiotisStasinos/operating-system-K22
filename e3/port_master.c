#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

#include "utils.h"

int main (int argc, char** argv){
   			///////////////////////
			/*     Arguments     */
			///////////////////////
	char flag1[3]="-c\0";
	char flag2[3]="-s\0";
	char flag3[4]="-sm\0";

	char* charges = NULL;
	char* shmid_str = NULL;
	char* mem_size = NULL;

	if(argc==7){
		for(int i=1 ; i<argc ; i++){
			if( strcmp(argv[i],flag1)==0 ){
				// printf("%d - %s\n",i+1,argv[i+1]);
				charges = (char*)malloc(strlen(argv[i+1])*sizeof(char));
				strcpy( charges, argv[i+1]);
				i++;
			}
			else if( strcmp(argv[i],flag2)==0 ){
				// printf("%d - %s\n",i+1,argv[i+1]);
				shmid_str = (char*)malloc(strlen(argv[i+1])*sizeof(char));
				strcpy( shmid_str, argv[i+1] );
				i++;
			}
			else if( strcmp(argv[i],flag3)==0 ){
				// printf("%d - %s\n",i+1,argv[i+1]);
				mem_size = (char*)malloc(strlen(argv[i+1])*sizeof(char));
				strcpy( mem_size, argv[i+1] );
				i++;
			}
			else{
				printf("Corret syntax is: ./port_master -c charges -s shmid -sm shared_mem_size\n");
				return 0;
			}
		}
	}
	else {
		printf("Corret syntax is: ./port_master -c charges -s shmid -sm shared_mem_size\n");
		return(0);  //terminating program
	}


	int shmid; //shared memory ID
	struct shmseg *shmp;
	shmid = shmget(KEY, atoi(mem_size), IPC_CREAT | 0666);
	if(shmid==-1){
		printf("Cannot create shared segment\n");
		return -1;
	}
	shmp = shmat(shmid, NULL, 0);
	if(shmp == (void*)-1){
		printf("Failed to attach memory\n");
		return -1;
	}
	printf("PORT MASTER\n");

	FILE *fp = NULL;
	fp = fopen("public_ledger.txt", "w+");

	// for(int i=0; i<12; i++){
	for(;;){
		sleep(2);
		sem_wait(&(shmp->mutex));
		print_park_info2(DIFF_TYPES, shmp->info, shmp->amount, fp);
		if(shmp->end_flag==1){
			sem_post(&(shmp->mutex));
			break;
		}
		sem_post(&(shmp->mutex));
	}

	fclose(fp);

	if(shmdt(shmp)==-1){
		printf("Failed to detach\n");
		return -1;
	}

	printf("end port_master\n");
}