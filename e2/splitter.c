#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include "record.h"

// argv[0]  datafile
// argv[1]  height
// argv[2]  pattern
// argv[3]  skew
// argv[4]  start
// argv[5]  end
// argv[6]  fifo
// argv[7]  fifo_
// argv[8]  root pid

int main (int argc, char** argv){
	clock_t start_t, end_t, total_t;
    start_t = clock();

  	sprintf(argv[1], "%d", atoi(argv[1])-1);  // Height-1

  	char fifo[name_pipe_size];
  	strcpy(fifo,argv[6]);

  	char fifo1[name_pipe_size],fifo2[name_pipe_size];
  	snprintf(fifo1, name_pipe_size, "%s%s",fifo,"_1");
  	snprintf(fifo2, name_pipe_size, "%s%s",fifo,"_2");

    char fifo1_[name_pipe_size],fifo2_[name_pipe_size];
  	snprintf(fifo1_, name_pipe_size, "%s%s",fifo1,"_C");
  	snprintf(fifo2_, name_pipe_size, "%s%s",fifo2,"_C");

	char fifo1_T[name_pipe_size], fifo2_T[name_pipe_size];
    snprintf(fifo1_T, name_pipe_size, "%s%s",fifo1,"_T");
    snprintf(fifo2_T, name_pipe_size, "%s%s",fifo2,"_T");

	char start1[Buf_size_num_of_recs];
	char start2[Buf_size_num_of_recs];
	char end1[Buf_size_num_of_recs];
	char end2[Buf_size_num_of_recs];

	strcpy(start1,argv[4]);
	if(strcmp(argv[3],"-s")==0){
		int num_of_leafs = 1;
		for(int i=0 ; i<atoi(argv[1])+1 ; i++){
			num_of_leafs = num_of_leafs*2;
		}
		int sum=0;
		for(int i=1 ; i<=num_of_leafs ; i++){
			sum = sum + i;
		}
		int half_sum=0;
		for(int i=1 ; i<=num_of_leafs/2 ; i++){
			half_sum = half_sum + i;
		}
		int cen = atoi(argv[5])-atoi(argv[4]);

		cen = (half_sum*cen)/sum;

		sprintf(end1, "%d", atoi(argv[4])+cen );
		sprintf(start2, "%d", atoi(argv[4])+cen+1 );		
	}
	else{
		sprintf(end1, "%d",( ( atoi(argv[4]) + atoi(argv[5]) )/2) );
		sprintf(start2, "%d",( ( atoi(argv[4]) + atoi(argv[5]) )/2)+1 );
	}
	strcpy(end2,argv[5]);

	int ret_val1, ret_val2;
	int status1, status2;
	pid_t pid1, pid2;


	if(atoi(argv[1])>0){
		pid1 = fork();
		if(pid1==0){
			ret_val1 = execl("./splitter",argv[0],argv[1],argv[2],argv[3],start1,end1,fifo1,fifo1_,argv[8],NULL);
			return 0;
		}
		pid2 = fork();
		if(pid2==0){
			ret_val2 = execl("./splitter",argv[0],argv[1],argv[2],argv[3],start2,end2,fifo2,fifo2_,argv[8],NULL);
			return 0;
		}
	}
	else{ //height =1
		pid1 = fork();
		if(pid1==0){
			ret_val1 = execl("./searcher",argv[0],argv[1],argv[2],argv[3],start1,end1,fifo1,fifo1_,argv[8],NULL);
			return 0;
		}
		pid2 = fork();
		if(pid2==0){
			ret_val2 = execl("./searcher",argv[0],argv[1],argv[2],argv[3],start2,end2,fifo2,fifo2_,argv[8],NULL);
			return 0;
		}
	}

	mkfifo(fifo1_,0666);  	mkfifo(fifo2_,0666);
	mkfifo(fifo1,0666); 	mkfifo(fifo2,0666);
	mkfifo(fifo1_T,0666);	mkfifo(fifo2_T,0666);

	int fd1_ = open(fifo1_,O_RDONLY);		int fd2_ = open(fifo2_,O_RDONLY);
	int fd1 = open(fifo1,O_RDONLY);			int fd2 = open(fifo2,O_RDONLY);
	int fd1_T = open(fifo1_T, O_RDONLY);	int fd2_T = open(fifo2_T, O_RDONLY);

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);

// count of records
	int count1, count2;
	read(fd1_, &count1, sizeof(count1));
	read(fd2_, &count2, sizeof(count2));


// records
	MyRecord *buffer;
	buffer = (MyRecord*)malloc((count1+count2)*sizeof(MyRecord));
	read(fd1, &buffer[0], (count1)*sizeof(MyRecord));
	read(fd2, &buffer[count1], (count2)*sizeof(MyRecord));

// times
	int size_of_time_buffer = 2;
	for(int i=0 ; i<=atoi(argv[1]) ; i++){
		size_of_time_buffer = size_of_time_buffer*2;
	}
	size_of_time_buffer--;
	clock_t *timers = (clock_t*)malloc(sizeof(clock_t)*size_of_time_buffer);

	read(fd1_T, &timers[0],sizeof(timers[0])*(size_of_time_buffer/2));
	read(fd2_T, &timers[size_of_time_buffer/2],sizeof(timers[0])*(size_of_time_buffer/2));

// printing
    // for(int i=0 ; i<(count1+count2) ; i++){
    //    printf("%d) %ld %s %s  %s %d %s %s %-9.2f\n", i+1,\
    // buffer[i].custid, buffer[i].LastName, buffer[i].FirstName, \
    // buffer[i].Street, buffer[i].HouseID, buffer[i].City, buffer[i].postcode, \
    // buffer[i].amount);
    // }
// cllose fifos
	close(fd1);
	close(fd2);

	close(fd1_);
	close(fd2_);

    close(fd1_T);
    close(fd2_T);    
// rm fifos
	char rm[30];
  	snprintf(rm, 30, "%s%s%s","rm ",fifo1,"\0");
  	system(rm);
  	snprintf(rm, 30, "%s%s%s","rm ",fifo1_,"\0");
  	system(rm);
  	snprintf(rm, 30, "%s%s%s","rm ",fifo2,"\0");
  	system(rm);
  	snprintf(rm, 30, "%s%s%s","rm ",fifo2_,"\0");
  	system(rm);
  	snprintf(rm, 30, "%s%s%s","rm ",fifo1_T,"\0");
  	system(rm);
  	snprintf(rm, 30, "%s%s%s","rm ",fifo2_T,"\0");
  	system(rm);

  	int count = count1+count2;
// plithos rec
  	mkfifo(argv[7],0666);
    int fd_C = open(argv[7], O_WRONLY);
    write(fd_C, &count, sizeof(count));
    close(fd_C);
// recs
    mkfifo(argv[6],0666);
    int fd_B = open(argv[6], O_WRONLY);
    write(fd_B, buffer, count*sizeof(MyRecord));
    close(fd_B);
// time
    char fifo_T[name_pipe_size];
  	snprintf(fifo_T, name_pipe_size, "%s%s",fifo,"_T");
  	mkfifo(fifo_T,0666);
    int fd_T = open(fifo_T, O_WRONLY);

    end_t = clock();
    total_t = end_t-start_t;
    timers[size_of_time_buffer-1]=total_t;

    write(fd_T, timers, size_of_time_buffer*sizeof(timers[0]));
    close(fd_T);

    printf(" -> |Splitter%s| %d recs - %ld clocks\n",&(argv[7][4]),count1+count2,timers[size_of_time_buffer-1]);
    free(timers);  

	return 0;
}