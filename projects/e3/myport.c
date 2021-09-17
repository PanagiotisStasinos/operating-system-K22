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
	char flag[2]="-l";
	char* cong_f = NULL;   // configuration file given

	if( argc==3 && strcmp(argv[1],flag)==0 ){
		cong_f = (char*)malloc(strlen(argv[2])*sizeof(char));
		strcpy( cong_f , argv[2]);
	}
	else{
		printf("Corret syntax is: ./myport -l configfile\n");
		return(0);  //terminating program
	}

   		///////////////////////
		/*    Config File    */
		///////////////////////
	FILE *fp=NULL;
	fp = fopen(cong_f, "r");
	if(fp==NULL){
		printf("Cannot open configuration file!\n");
		return -1;
	}

	int count=DIFF_TYPES;	//plithos diaforetikwn ploiwn
	printf("%d types of ships\n", count);

	size_t bufsize = 20;
	char buffer[bufsize];
	char *b = buffer;
	size_t characters;

	struct shmseg tmp;
	tmp.count=DIFF_TYPES;
	char* temp_type;
	int total_capacity=0;
	for(int i=0; i<count; ++i){
		characters=getline(&b,&bufsize,fp);
		temp_type = strtok(b, ",");
		tmp.info[i].type = temp_type[0];
		tmp.info[i].capacity = atoi(strtok(NULL, ","));
		total_capacity += tmp.info[i].capacity;
		tmp.info[i].charge = atoi(strtok(NULL, ","));
	}
	fclose(fp);

	//////////////////////////////////
	//		CREATE SHARED MEMORY 	//
	//////////////////////////////////	/*START 4*/
	int shmid; //shared memory ID
	struct shmseg *shmp;
	int mem_size = sizeof(struct shmseg)+total_capacity*sizeof(struct parking_space);
	shmid = shmget(KEY, mem_size, IPC_CREAT | 0666);
	if(shmid==-1){
		printf("Cannot create shared segment\n");
		return -1;
	}
	shmp = shmat(shmid, NULL, 0);
	if(shmp == (void*)-1){
		printf("Failed to attach memory\n");
		return -1;
	}									/*END 4*/

	//////////////////////////////////////////////
	//		INITIALIZATION OF SHARED MEMORY 	//
	//////////////////////////////////////////////	/*START 5*/
	shmp->count = tmp.count;
	shmp->amount = 0;
	shmp->end_flag = 0;
	struct parking_space* tmp_park;
	for(int i=0; i<count; ++i){
		shmp->info[i].type = tmp.info[i].type;
		shmp->info[i].capacity = tmp.info[i].capacity;
		shmp->info[i].charge = tmp.info[i].charge;

		sem_init(&(shmp->info[i].full),1,shmp->info[i].capacity);

		if(i==0){
			shmp->info[i].moor_area = (struct parking_space*)((void*)shmp+sizeof(struct shmseg));
		}
		else{
			shmp->info[i].moor_area = (struct parking_space*)(shmp->info[i-1].moor_area) + shmp->info[i-1].capacity;
		}

		for(int j=0; j<shmp->info[i].capacity; j++){
			tmp_park = shmp->info[i].moor_area + j;
			tmp_park->free = 0;
			tmp_park->parking_type = shmp->info[i].type;
			tmp_park->parked_ship_type = '-';
			tmp_park->vessel_id = -1;
			tmp_park->arrival=0;
			tmp_park->departure=0;
		}
	}
	print_park_info(count,shmp->info);			/*END 5*/
	printf("\n");
	sem_init(&(shmp->mutex),1,1);
	//////////////////////////////////////////
	//		EXEC PORT_MASTER AND MONITOR 	//
	//////////////////////////////////////////	/*START 6*/
	char str_id[10];
   	sprintf(str_id, "%d", shmid);

   	char str_mem_size[20];
   	sprintf(str_mem_size, "%d", mem_size);

	pid_t pid_1 = fork();
	int ret_val_1;
	int status_1;
	if(pid_1==0){
		ret_val_1 = execl("./port_master","./port_master","-c","charges.txt","-s",str_id,"-sm",str_mem_size,NULL);
		printf("Failed to execute port_master\n");
		return 0;
	}

	pid_t pid_3 = fork();
	int ret_val_3;
	int status_3;
	if(pid_3==0){
		ret_val_3 = execl("./monitor","./monitor","-d","2","-t","0","-s",str_id,"-sm",str_mem_size,NULL);
		printf("Failed to execute monitor\n");
		return 0;
	}											/*END 6*/

	//////////////////////////////////////////
	//		EXEC PORT_MASTER AND MONITOR 	//
	//////////////////////////////////////////	/*START 7*/
	pid_t pid_v[TOTAL_NUM_OF_SHIPS];
	int ret_val_v[TOTAL_NUM_OF_SHIPS];
	int status_v[TOTAL_NUM_OF_SHIPS];

	char str_park_period[10];
	int park_period;
	char id[5];
	char type[2];
	int r;
	for(int i=0; i<TOTAL_NUM_OF_SHIPS; i++){
		sleep(1);

		srand(time(NULL));
		park_period = (rand()%10)+10;
		sprintf(str_park_period,"%d",park_period);

		sprintf(id,"%d",i);

		r = rand()%3;
		if(r==0){
			type[0]='S';
		}
		else if(r==1){
			type[0]='M';
		}
		else{
			type[0]='L';
		}
		type[1]='\0';

		pid_v[i] = fork();
		if(pid_v[i]==0){
			ret_val_v[i] = execl("./vessel","./vessel","-u",type,"-t",type,"-p",str_park_period,"-s",str_id,"-sm",str_mem_size,"-i",id,NULL);
			printf("Failed to execute vessel_%d\n",i);
			return 0;
		}
	}

	for(int i=0; i<TOTAL_NUM_OF_SHIPS; i++){
		waitpid(pid_v[i], &status_v[i], 0);
	}

	sem_wait(&(shmp->mutex));
		shmp->end_flag = 1;
	sem_post(&(shmp->mutex));

	sem_destroy(&(shmp->mutex));


	waitpid(pid_1, &status_1, 0);
	waitpid(pid_3, &status_3, 0);

if(shmdt(shmp)==-1){
	printf("Failed to detach\n");
	return -1;
}

int err;
err = shmctl(shmid, IPC_RMID, 0);
if(err==-1){
	perror("Removal");
}

	free(cong_f);
	printf("END\n");
	return 0;
}