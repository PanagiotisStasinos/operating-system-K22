#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
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
	char flag1[3]="-t\0";
	char flag2[3]="-u\0";
	char flag3[3]="-p\0";
	char flag4[3]="-m\0";
	char flag5[3]="-s\0";
	char flag6[4]="-sm\0";
	char flag7[3]="-i\0";

	char* type = NULL;
	char* postype = NULL;
	char* parkperiod = NULL;
	char* mantime = NULL;
	char* shmid_str = NULL;
	char* mem_size = NULL;
	char* id = NULL;

	if(argc==13){
		for(int i=1 ; i<argc ; i++){
			if( strcmp(argv[i],flag1)==0 ){
				// printf("%d - %s\n",i+1,argv[i+1]);
				type = (char*)malloc(strlen(argv[i+1])*sizeof(char));
				strcpy( type, argv[i+1]);
				i++;
			}
			else if( strcmp(argv[i],flag2)==0 ){
				// printf("%d - %s\n",i+1,argv[i+1]);
				postype = (char*)malloc(strlen(argv[i+1])*sizeof(char));
				strcpy( postype, argv[i+1] );
				i++;
			}
			else if( strcmp(argv[i],flag3)==0 ){
				// printf("%d - %s\n",i+1,argv[i+1]);
				parkperiod = (char*)malloc(strlen(argv[i+1])*sizeof(char));
				strcpy( parkperiod, argv[i+1] );
				i++;
			}
			else if( strcmp(argv[i],flag4)==0 ){
				// printf("%d - %s\n",i+1,argv[i+1]);
				mantime = (char*)malloc(strlen(argv[i+1])*sizeof(char));
				strcpy( mantime, argv[i+1] );
				i++;
			}
			else if( strcmp(argv[i],flag5)==0 ){
				// printf("%d - %s\n",i+1,argv[i+1]);
				shmid_str = (char*)malloc(strlen(argv[i+1])*sizeof(char));
				strcpy( shmid_str, argv[i+1] );
				i++;
			}
			else if( strcmp(argv[i],flag6)==0 ){
				// printf("%d - %s\n",i+1,argv[i+1]);
				mem_size = (char*)malloc(strlen(argv[i+1])*sizeof(char));
				strcpy( mem_size, argv[i+1] );
				i++;
			}
			else if( strcmp(argv[i],flag7)==0 ){
				// printf("%d - %s\n",i+1,argv[i+1]);
				id = (char*)malloc(strlen(argv[i+1])*sizeof(char));
				strcpy( id, argv[i+1] );
				i++;
			}
			else{
				printf("Corret syntax is: ./vessel -t type -u postype -p parkperiod -m mantime -s shmid -sm shared_mem_size -i id\n");
				return 0;
			}
		}
	}
	else {
		printf("Corret syntax is: ./vessel -t type -u postype -p parkperiod -m mantime -s shmid -sm shared_mem_size -i id\n");
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
	// printf("VESSEL %d\n",atoi(parkperiod));


	int index_of_type;
	int full_value;
	if(*type==*postype){
		if(*type=='S'){
			printf("small\n");
			index_of_type=0;
			sem_getvalue(&(shmp->info[0].full),&full_value);
			if(full_value==0){
				printf("Vessel_%c_%s will be blocked because %s praking spaces are full\n", type[0], id, type);
			}
			sem_wait(&(shmp->info[0].full));	//	vessel waits because parking spaces of its type are full
		}
		else if(*type=='M'){
			index_of_type=1;
			printf("MEDIUM\n");
			sem_getvalue(&(shmp->info[1].full),&full_value);
			if(full_value==0){
				printf("Vessel_%c_%s will be blocked because %s praking spaces are full\n", type[0], id, type);
			}
			sem_wait(&(shmp->info[1].full));	//	vessel waits because parking spaces of its type are full
		}
		else{
			index_of_type=2;
			printf("LARGE\n");
			sem_getvalue(&(shmp->info[2].full),&full_value);
			if(full_value==0){
				printf("Vessel_%c_%s will be blocked because %s praking spaces are full\n", type[0], id, type);
			}
			sem_wait(&(shmp->info[2].full));	//	vessel waits because parking spaces of its type are full
		}

	}


	struct parking_space* tmp_park;
	if(index_of_type==0){
		tmp_park = (struct parking_space*)((void*)shmp+sizeof(struct shmseg));
	}
	else if(index_of_type==1){
		tmp_park = (struct parking_space*)((void*)shmp+sizeof(struct shmseg));
		tmp_park += shmp->info[0].capacity;
	}
	else{
		tmp_park = (struct parking_space*)((void*)shmp+sizeof(struct shmseg));
		tmp_park += shmp->info[0].capacity;
		tmp_park += shmp->info[1].capacity;
	}

	sem_wait(&(shmp->mutex));
		printf("IN vessel_%c_%s index_type %d \n", type[0],id,index_of_type);
		for(int i=0; i<shmp->info[index_of_type].capacity; i++){
			tmp_park += i;
			if(tmp_park->free==0){
				tmp_park->free=1;
				tmp_park->parked_ship_type = *type;
				tmp_park->vessel_id = atoi(id);
				tmp_park->arrival = time(NULL);

				break;
			}
		}
	sem_post(&(shmp->mutex));

	srand(time(NULL));
	int r = rand()%(atoi(parkperiod)-2);
	r += 2;
	sleep(r);

	printf("\tvessel_%c_%s %d %.2f\n", type[0], id, r, (float)shmp->info[index_of_type].charge*((float)r/(float)30));

	sleep(atoi(parkperiod)-r);


	sem_wait(&(shmp->mutex));
		tmp_park->departure = time(NULL);
		printf("OUT vessel_%c_%s %ld\n", type[0], id, tmp_park->departure-tmp_park->arrival);
		shmp->amount += (float)shmp->info[index_of_type].charge*((float)(tmp_park->departure-tmp_park->arrival)/(float)30);

		tmp_park->free=0;
		tmp_park->parked_ship_type = '-';
		tmp_park->vessel_id = -1;
		tmp_park->arrival = 0;
		tmp_park->departure = 0;
	sem_post(&(shmp->mutex));

	sem_post(&(shmp->info[index_of_type].full));


	if(shmdt(shmp)==-1){
		printf("Failed to detach\n");
		return -1;
	}

	// printf("end vessel %d\n", atoi(parkperiod));
}