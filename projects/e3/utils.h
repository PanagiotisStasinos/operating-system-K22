#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <sys/shm.h>
#include <semaphore.h>

#define KEY 1515
#define DIFF_TYPES 3
#define TOTAL_NUM_OF_SHIPS 25

struct parking_space{
	int free;	//free 0, in use 1
	char parking_type;
	char parked_ship_type;	//init with '-'
	int vessel_id;	//init with -1
	time_t arrival;
	time_t departure;
};

struct config_info{
	char type;	//L, M, S
	int capacity;
	int charge;	//xrewsh ana 30'
	sem_t full;	//if full==capacity no free parking spaces
	struct parking_space* moor_area;	//table with size of capacity
};

struct shmseg{
	float amount;
	int count;
	struct config_info info[DIFF_TYPES];
	sem_t mutex;	//elegxei thn kinhsh
	int end_flag;	//arxikopoihsh me 0
};

void print_park_info(int count, struct config_info* info){
	int valp;
	struct parking_space* tmp_park;
	for (int i=0; i<count; ++i){
		sem_getvalue(&(info[i].full),&valp);
		printf("Type %c\tCapacity %d\tCharge %d Full %d\n", info[i].type, info[i].capacity, info[i].charge, valp);
		for(int j=0; j<info[i].capacity; j++){
			tmp_park = info[i].moor_area + j;
			printf("free %d, parking_type %c, parked_ship_type %c, vessel_id %d\n",
				tmp_park->free, 
				tmp_park->parking_type, 
				tmp_park->parked_ship_type, 
				tmp_park->vessel_id);
		}
	}
}

void print_park_info1(int count, struct config_info* info, float amount){
	printf("-------MONITOR------\n");
	int valp;
	struct parking_space* tmp_park;
	for (int i=0; i<count; ++i){
		sem_getvalue(&(info[i].full),&valp);
		printf("Type %c\t free %d/%d\n", info[i].type, valp, info[i].capacity);
	}
	printf("amount : %.2f\n--------------------\n",amount);
}

void print_park_info2(int count, struct config_info* info, float amount, FILE *fp){
	// fprintf(fp,"%s","-------PORT_MASTER------\n");
	int valp;
	struct parking_space* tmp_park;
	time_t t;
	t = time(NULL);
	char* time_str;
	time_str = ctime(&t);
	(void)fprintf(fp,"%s%s","time : ", time_str);
	for (int i=0; i<count; ++i){
		sem_getvalue(&(info[i].full),&valp);
		fprintf(fp,"%s%c%s%d%s%d%s","Type ",info[i].type,"\t free ",valp,"/",info[i].capacity,"\n");
	}
	fprintf(fp, "%s%.2f%s","amount : ",amount,"\n\n");
}
#endif /*utils.h*/