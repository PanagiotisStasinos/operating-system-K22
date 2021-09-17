#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "record.h"

int count_signals = 0;

void print_clocks(char* height, clock_t *timers);
void min_max(char* height, clock_t *timers);

void handler()
{
   signal(SIGUSR2,handler);
   printf("ROOT: I have received a SIGUSR2\n");
   count_signals++;
}

int main (int argc, char** argv){
    clock_t start_t, end_t, total_t;
    start_t = clock();
   			///////////////////////
			/*     Arguments     */
			///////////////////////
	char flag1[3]="-h";
	char flag2[3]="-d";
	char flag3[3]="-p";
	char flag4[3]="-s\0";


	char* datafile = NULL;   // binary input-file given
	char* height = NULL;
	char* pattern = NULL;
	char skew_flag[3] = "s-\0";

	if(argc==7 || argc==8){
		for(int i=1 ; i<argc ; i++){
			if( strcmp(argv[i],flag1)==0 ){  //Height
				printf("%d - %s\n",i+1,argv[i+1]);
				height = (char*)malloc(strlen(argv[i+1])*sizeof(char));
				strcpy( height, argv[i+1]);
				i++;
			}
			else if( strcmp(argv[i],flag2)==0 ){  //Datafile
				printf("%d - %s\n",i+1,argv[i+1]);
				datafile = (char*)malloc(strlen(argv[i+1])*sizeof(char));
				strcpy( datafile, argv[i+1] );
				i++;
			}
			else if( strcmp(argv[i],flag3)==0 ){  //Pattern
				printf("%d - %s\n",i+1,argv[i+1]);
				pattern = (char*)malloc(strlen(argv[i+1])*sizeof(char));
				strcpy( pattern, argv[i+1] );
				i++;
			}
			else if( strcmp(argv[i],flag4)==0 ){  //Skew
				//skew_flag = (char*)malloc(strlen(argv[i])*sizeof(char));
				strcpy( skew_flag, argv[i] );
                skew_flag[2]='\0';
				printf("Skew On\n\n");
			}
			else{
				printf("Corret syntax is: ./myfind -h Height -d Datafile -p Pattern -s\n");
				return 0;
			}
		}
	}
	else {
		printf("Corret syntax is: ./myfind -h Height -d Datafile -p Pattern -s\n");
		return(0);  //terminating program
	}

	if(atoi(height)>5){
		free(datafile);
		free(height);
		free(pattern);
		printf("height must be less or equal to 5\n");
		return 0;
	}

	pid_t my_pid = getpid();
	char m_pid[20];
	sprintf(m_pid,"%ld",(long)my_pid);

	FILE *fpb;
    long lSize;
    MyRecord rec;
    int numOfrecords;

    fpb = fopen (datafile,"rb");
	if (fpb==NULL) {
      	printf("Cannot open binary file\n");
      	return 1;
   	}

   	   // check number of records
   	fseek (fpb , 0 , SEEK_END);
   	lSize = ftell (fpb);
  	numOfrecords = (int) lSize/sizeof(rec);
   	fclose (fpb);

   	char start[2]="0";
   	char end[Buf_size_num_of_recs];
   	sprintf(end, "%d", numOfrecords-1);

	char fifo[name_pipe_size] = "fifo_1";
	mkfifo(fifo,0666);
	char fifo_[name_pipe_size] = "fifo_1_C";
	mkfifo(fifo_,0666);
	char fifo_T[name_pipe_size] = "fifo_1_T";
	mkfifo(fifo_T,0666);

	int reps=1;
	for(int i=0 ; i<atoi(height) ; i++){
 		reps = reps*2;
	}

	int ret_val;
	int status;
	pid_t pid = fork();
	if(pid==0){
		ret_val = execl("./splitter",datafile,height,pattern,skew_flag,start,end,fifo,fifo_,m_pid,NULL);
		return 0;
	}


	for(int i=0 ; i<reps ; i++){
 		signal(SIGUSR2,handler);
	}

  	mkfifo(fifo_,0666);
	mkfifo(fifo,0666);
	mkfifo(fifo_T,0666);

	int fd_ = open(fifo_,O_RDONLY);
	int fd = open(fifo,O_RDONLY);
	int fd_T = open(fifo_T,O_RDONLY);

	waitpid(pid, &status, 0);
	
	int count;
	read(fd_, &count, sizeof(count));
	MyRecord *buffer;
	buffer = (MyRecord*)malloc((count)*sizeof(MyRecord));
	read(fd, buffer, count*sizeof(MyRecord));

	int size_of_time_buffer = 2;
	for(int i=0 ; i<atoi(height) ; i++){
		size_of_time_buffer = size_of_time_buffer*2;
	}
	clock_t *timers = (clock_t*)malloc(sizeof(clock_t)*size_of_time_buffer);

	read(fd_T, timers, (size_of_time_buffer-1)*sizeof(timers[0]));

	FILE *temp;
	temp = fopen("Temp.bin","wb");
	FILE *unsorted;
	unsorted = fopen("UnSortedResults.bin","wb");
	char lf = '\n';
	char space = ' ';
	printf("\nUsorted Results (%d)\n",count);
	for(int i=0 ; i<count ; i++){
       printf("%d) |%ld|%s|%s|%s|%d|%s|%s|%.2f|\n", i+1,\
        buffer[i].custid, buffer[i].FirstName, buffer[i].LastName, \
        buffer[i].Street, buffer[i].HouseID, buffer[i].City, buffer[i].postcode, \
        buffer[i].amount);
       fwrite(&( buffer[i].custid ), 1, sizeof(rec.custid),temp);
       fwrite(&space,1,sizeof(space),temp);

       fwrite(&( buffer[i].FirstName ), 1, sizeof(rec.FirstName),temp);
       fwrite(&space,1,sizeof(space),temp);

       fwrite(&( buffer[i].LastName ), 1, sizeof(rec.LastName),temp);
       fwrite(&space,1,sizeof(space),temp);

       fwrite(&( buffer[i].Street ), 1, sizeof(rec.Street),temp);
       fwrite(&space,1,sizeof(space),temp);

       fwrite(&( buffer[i].HouseID ), 1, sizeof(rec.HouseID),temp);
       fwrite(&space,1,sizeof(space),temp);

       fwrite(&( buffer[i].City ), 1, sizeof(rec.City),temp);
       fwrite(&space,1,sizeof(space),temp);

       fwrite(&( buffer[i].postcode ), 1, sizeof(rec.postcode),temp);
       fwrite(&space,1,sizeof(space),temp);

       fwrite(&( buffer[i].amount ), 1, sizeof(rec.amount),temp);
       fwrite(&lf,1,sizeof(lf),temp);

       fwrite(&buffer[i],1,sizeof(buffer[i]),unsorted);
    }

	fclose(unsorted);
	fclose(temp);

	int statusf;
	pid_t pidf = fork();
	char c[12];
	sprintf(c,"%d",count);
	if(pidf==0){
		execl("./sort","./sort",c,NULL);
		return 0;
	}
	waitpid(pidf, &statusf, 0);

	system("rm Temp.bin");
	system("rm Sorted.bin");


    char rm[30];
  	snprintf(rm, 30, "%s%s%s","rm ",fifo,"\0");
  	system(rm);
  	snprintf(rm, 30, "%s%s%s","rm ",fifo_,"\0");
  	system(rm);
  	snprintf(rm, 30, "%s%s%s","rm ",fifo_T,"\0");
  	system(rm);

  	free(buffer);
	free(datafile);

	free(pattern);

	end_t = clock();
    total_t = end_t-start_t;
    timers[size_of_time_buffer-1]=total_t;

	printf(" -> |Root| %ld\n",timers[size_of_time_buffer-1]);

	//print_clocks(height,timers);

    min_max(height,timers);

    free(height);
	free(timers);
}


void min_max(char* height, clock_t *timers){
    if(atoi(height)==1){
      if(timers[0]<timers[1]){
        printf("[SEARCHERS] min:%ld clocks | max:%ld clocks | average:%ld clocks\n",timers[0],timers[1],(timers[0]+timers[1])/2);
      }
      else{
        printf("[SEARCHERS] min:%ld clocks | max:%ld clocks | average:%ld clocks\n",timers[1],timers[0],(timers[0]+timers[1])/2);
      }
      printf("[SPLITTER] (one splitter) %ld clocks\n",timers[2]);
      printf("[ROOT] %ld clocks\n",timers[3]);
    } 
    else if(atoi(height)==2){
      int num_of_leafs=1;
      for(int i=0 ; i<atoi(height) ; i++){
        num_of_leafs = num_of_leafs*2;
      }

      clock_t *s_tim = (clock_t*)malloc(num_of_leafs*sizeof(clock_t));
      s_tim[0] = timers[0];
      s_tim[1] = timers[1];
      s_tim[2] = timers[3];
      s_tim[3] = timers[4];

      clock_t *m_tim = (clock_t*)malloc((num_of_leafs-1)*sizeof(clock_t));
      m_tim[0] = timers[2];
      m_tim[1] = timers[5];
      m_tim[2] = timers[6];

      int max_index=0;
      int min_index=0;
      for(int i=1 ; i<num_of_leafs ; i++){
        if( s_tim[i] < s_tim[min_index] ){
          min_index = i;
        }
        else if( s_tim[i] > s_tim[max_index] ){
          max_index = i;
        }
      }
      printf("[SEARCHERS] min:%ld clocks | max:%ld clocks | average:%ld clocks\n",s_tim[min_index],s_tim[max_index],(s_tim[min_index]+s_tim[max_index])/2);

      max_index=0;
      min_index=0;      
      for(int i=1 ; i<num_of_leafs-1 ; i++){
        if( m_tim[i] < m_tim[min_index] ){
          min_index = i;
        }
        else if( m_tim[i] > m_tim[max_index] ){
          max_index = i;
        }
      }
      printf("[SPLITTER] min:%ld clocks | max:%ld clocks | average:%ld clocks\n",m_tim[min_index],m_tim[max_index],(m_tim[min_index]+m_tim[max_index])/2);

      printf("[ROOT] %ld clocks\n",timers[7]);

    }
    else if(atoi(height)==3){
      int num_of_leafs=1;
      for(int i=0 ; i<atoi(height) ; i++){
        num_of_leafs = num_of_leafs*2;
      }

      clock_t *s_tim = (clock_t*)malloc(num_of_leafs*sizeof(clock_t));
      s_tim[0] = timers[0];
      s_tim[1] = timers[1];
      s_tim[2] = timers[3];
      s_tim[3] = timers[4];
      s_tim[4] = timers[7];
      s_tim[5] = timers[8];
      s_tim[6] = timers[10];
      s_tim[7] = timers[11];

      clock_t *m_tim = (clock_t*)malloc((num_of_leafs-1)*sizeof(clock_t));
      m_tim[0] = timers[2];
      m_tim[1] = timers[5];
      m_tim[2] = timers[6];
      m_tim[3] = timers[9];
      m_tim[4] = timers[12];
      m_tim[5] = timers[13];
      m_tim[6] = timers[14];

      int max_index=0;
      int min_index=0;
      for(int i=1 ; i<num_of_leafs ; i++){
        if( s_tim[i] < s_tim[min_index] ){
          min_index = i;
        }
        else if( s_tim[i] > s_tim[max_index] ){
          max_index = i;
        }
      }
      printf("[SEARCHERS] min:%ld clocks | max:%ld clocks | average:%ld clocks\n",s_tim[min_index],s_tim[max_index],(s_tim[min_index]+s_tim[max_index])/2);

      max_index=0;
      min_index=0;      
      for(int i=1 ; i<num_of_leafs-1 ; i++){
        if( m_tim[i] < m_tim[min_index] ){
          min_index = i;
        }
        else if( m_tim[i] > m_tim[max_index] ){
          max_index = i;
        }
      }
      printf("[SPLITTER] min:%ld clocks | max:%ld clocks | average:%ld clocks\n",m_tim[min_index],m_tim[max_index],(m_tim[min_index]+m_tim[max_index])/2);

      printf("[ROOT] %ld clocks\n",timers[15]);

    }
    else if(atoi(height)==4){
      int num_of_leafs=1;
      for(int i=0 ; i<atoi(height) ; i++){
        num_of_leafs = num_of_leafs*2;
      }

      clock_t *s_tim = (clock_t*)malloc(num_of_leafs*sizeof(clock_t));
      s_tim[0] = timers[0];
      s_tim[1] = timers[1];
      s_tim[2] = timers[3];
      s_tim[3] = timers[4];
      s_tim[4] = timers[7];
      s_tim[5] = timers[8];
      s_tim[6] = timers[10];
      s_tim[7] = timers[11];
      s_tim[8] = timers[15];
      s_tim[9] = timers[16];
      s_tim[10] = timers[18];
      s_tim[11] = timers[19];
      s_tim[12] = timers[22];
      s_tim[13] = timers[23];
      s_tim[14] = timers[25];
      s_tim[15] = timers[26];

      clock_t *m_tim = (clock_t*)malloc((num_of_leafs-1)*sizeof(clock_t));
      m_tim[0] = timers[2];
      m_tim[1] = timers[5];
      m_tim[2] = timers[6];
      m_tim[3] = timers[9];
      m_tim[4] = timers[12];
      m_tim[5] = timers[13];
      m_tim[6] = timers[14];
      m_tim[7] = timers[17];
      m_tim[8] = timers[20];
      m_tim[9] = timers[21];
      m_tim[10] = timers[24];
      m_tim[11] = timers[27];
      m_tim[12] = timers[28];
      m_tim[13] = timers[29];
      m_tim[14] = timers[30];


      int max_index=0;
      int min_index=0;
      for(int i=1 ; i<num_of_leafs ; i++){
        if( s_tim[i] < s_tim[min_index] ){
          min_index = i;
        }
        else if( s_tim[i] > s_tim[max_index] ){
          max_index = i;
        }
      }
      printf("[SEARCHERS] min:%ld clocks | max:%ld clocks | average:%ld clocks\n",s_tim[min_index],s_tim[max_index],(s_tim[min_index]+s_tim[max_index])/2);

      max_index=0;
      min_index=0;      
      for(int i=1 ; i<num_of_leafs-1 ; i++){
        if( m_tim[i] < m_tim[min_index] ){
          min_index = i;
        }
        else if( m_tim[i] > m_tim[max_index] ){
          max_index = i;
        }
      }
      printf("[SPLITTER] min:%ld clocks | max:%ld clocks | average:%ld clocks\n",m_tim[min_index],m_tim[max_index],(m_tim[min_index]+m_tim[max_index])/2);

      printf("[ROOT] %ld clocks\n",timers[31]);

    }
    else if(atoi(height)==5){
      int num_of_leafs=1;
      for(int i=0 ; i<atoi(height) ; i++){
        num_of_leafs = num_of_leafs*2;
      }

      clock_t *s_tim = (clock_t*)malloc(num_of_leafs*sizeof(clock_t));
      s_tim[0] = timers[0];
      s_tim[1] = timers[1];
      s_tim[2] = timers[3];
      s_tim[3] = timers[4];
      s_tim[4] = timers[7];
      s_tim[5] = timers[8];
      s_tim[6] = timers[10];
      s_tim[7] = timers[11];
      s_tim[8] = timers[15];
      s_tim[9] = timers[16];
      s_tim[10] = timers[18];
      s_tim[11] = timers[19];
      s_tim[12] = timers[22];
      s_tim[13] = timers[23];
      s_tim[14] = timers[25];
      s_tim[15] = timers[26];
      s_tim[16] = timers[31];
      s_tim[17] = timers[32];
      s_tim[18] = timers[34];
      s_tim[19] = timers[35];
      s_tim[20] = timers[38];
      s_tim[21] = timers[39];
      s_tim[22] = timers[41];
      s_tim[23] = timers[42];
      s_tim[24] = timers[46];
      s_tim[25] = timers[47];
      s_tim[26] = timers[49];
      s_tim[27] = timers[50];
      s_tim[28] = timers[53];
      s_tim[29] = timers[54];
      s_tim[30] = timers[56];
      s_tim[31] = timers[57];

      clock_t *m_tim = (clock_t*)malloc((num_of_leafs-1)*sizeof(clock_t));
      m_tim[0] = timers[2];
      m_tim[1] = timers[5];
      m_tim[2] = timers[6];
      m_tim[3] = timers[9];
      m_tim[4] = timers[12];
      m_tim[5] = timers[13];
      m_tim[6] = timers[14];
      m_tim[7] = timers[17];
      m_tim[8] = timers[20];
      m_tim[9] = timers[21];
      m_tim[10] = timers[24];
      m_tim[11] = timers[27];
      m_tim[12] = timers[28];
      m_tim[13] = timers[29];
      m_tim[14] = timers[30];
            m_tim[15] = timers[33];
      m_tim[16] = timers[36];
      m_tim[17] = timers[37];
      m_tim[18] = timers[40];
      m_tim[19] = timers[43];
      m_tim[20] = timers[44];
      m_tim[21] = timers[45];
      m_tim[22] = timers[48];
      m_tim[23] = timers[51];
      m_tim[24] = timers[52];
      m_tim[25] = timers[55];
      m_tim[26] = timers[58];
      m_tim[27] = timers[59];
      m_tim[28] = timers[60];
      m_tim[29] = timers[61];
      m_tim[30] = timers[62];

      int max_index=0;
      int min_index=0;
      for(int i=1 ; i<num_of_leafs ; i++){
        if( s_tim[i] < s_tim[min_index] ){
          min_index = i;
        }
        else if( s_tim[i] > s_tim[max_index] ){
          max_index = i;
        }
      }
      printf("[SEARCHERS] min:%ld clocks | max:%ld clocks | average:%ld clocks\n",s_tim[min_index],s_tim[max_index],(s_tim[min_index]+s_tim[max_index])/2);

      max_index=0;
      min_index=0;      
      for(int i=1 ; i<num_of_leafs-1 ; i++){
        if( m_tim[i] < m_tim[min_index] ){
          min_index = i;
        }
        else if( m_tim[i] > m_tim[max_index] ){
          max_index = i;
        }
      }
      printf("[SPLITTER] min:%ld clocks | max:%ld clocks | average:%ld clocks\n",m_tim[min_index],m_tim[max_index],(m_tim[min_index]+m_tim[max_index])/2);

      printf("[ROOT] %ld clocks\n",timers[63]);


    }
}


void print_clocks(char* height, clock_t *timers){
    if(atoi(height)==1){
    	printf("Searcher_1_1 %ld clocks\n",timers[0]);
    	printf("Searcher_1_2 %ld clocks\n",timers[1]);
    	printf("Splitter_1 %ld clocks\n",timers[2]);
    	printf("Root %ld clocks\n",timers[3]);

    }
    else if(atoi(height)==2){
    	printf("Searcher_1_1_1 %ld clocks\n",timers[0]);
    	printf("Searcher_1_1_2 %ld clocks\n",timers[1]);

    	printf("Splitter_1_1 %ld clocks\n",timers[2]);

    	printf("Searcher_1_2_1 %ld clocks\n",timers[3]);
    	printf("Searcher_1_2_2 %ld clocks\n",timers[4]);

    	printf("Splitter_1_2 %ld clocks\n",timers[5]);
    	printf("Splitter_1 %ld clocks\n",timers[6]);
    	printf("Root %ld clocks\n",timers[7]);


    }
    else if(atoi(height)==3){
      printf("Searcher_1_1_1_1 %ld clocks\n",timers[0]);
      printf("Searcher_1_1_1_2 %ld clocks\n",timers[1]);
            printf("Splitter_1_1_1 %ld clocks\n",timers[2]);
      printf("Searcher_1_1_2_1 %ld clocks\n",timers[3]);
      printf("Searcher_1_1_2_2 %ld clocks\n",timers[4]);
            printf("Splitter_1_1_2 %ld clocks\n",timers[5]);
                printf("Splitter_1_1 %ld clocks\n",timers[6]);
      printf("Searcher_1_2_1_1 %ld clocks\n",timers[7]);
      printf("Searcher_1_2_1_2 %ld clocks\n",timers[8]);
            printf("Splitter_1_2_1 %ld clocks\n",timers[9]);
      printf("Searcher_1_2_2_1 %ld clocks\n",timers[10]);
      printf("Searcher_1_2_2_2 %ld clocks\n",timers[11]);
            printf("Splitter_1_2_2 %ld clocks\n",timers[12]);
                printf("Splitter_1_2 %ld clocks\n",timers[13]);
                    printf("Splitter_1 %ld clocks\n",timers[14]);
      printf("Root %ld clocks\n",timers[15]);   
    }
    else if(atoi(height)==4){
      printf("Searcher_1_1_1_1_1 %ld clocks\n",timers[0]);
      printf("Searcher_1_1_1_1_2 %ld clocks\n",timers[1]);
            printf("Splitter_1_1_1_1 %ld clocks\n",timers[2]);
      printf("Searcher_1_1_1_2_1 %ld clocks\n",timers[3]);
      printf("Searcher_1_1_1_2_2 %ld clocks\n",timers[4]);
            printf("Splitter_1_1_1_2 %ld clocks\n",timers[5]);
                printf("Splitter_1_1_1 %ld clocks\n",timers[6]);
      printf("Searcher_1_1_2_1_1 %ld clocks\n",timers[7]);
      printf("Searcher_1_1_2_1_2 %ld clocks\n",timers[8]);
            printf("Splitter_1_1_2_1 %ld clocks\n",timers[9]);
      printf("Searcher_1_1_2_2_1 %ld clocks\n",timers[10]);
      printf("Searcher_1_1_2_2_2 %ld clocks\n",timers[11]);
            printf("Splitter_1_1_2_2 %ld clocks\n",timers[12]);
                printf("Splitter_1_1_2 %ld clocks\n",timers[13]);
                    printf("Splitter_1_1 %ld clocks\n",timers[14]);
      printf("Searcher_1_2_1_1_1 %ld clocks\n",timers[15]);
      printf("Searcher_1_2_1_1_2 %ld clocks\n",timers[16]);
            printf("Splitter_1_2_1_1 %ld clocks\n",timers[17]);
      printf("Searcher_1_2_1_2_1 %ld clocks\n",timers[18]);
      printf("Searcher_1_2_1_2_2 %ld clocks\n",timers[19]);
            printf("Splitter_1_2_1_2 %ld clocks\n",timers[20]);
                printf("Splitter_1_2_1 %ld clocks\n",timers[21]);
      printf("Searcher_1_2_2_1_1 %ld clocks\n",timers[22]);
      printf("Searcher_1_2_2_1_2 %ld clocks\n",timers[23]);
            printf("Splitter_1_2_2_1 %ld clocks\n",timers[24]);
      printf("Searcher_1_2_2_2_1 %ld clocks\n",timers[25]);
      printf("Searcher_1_2_2_2_2 %ld clocks\n",timers[26]);
            printf("Splitter_1_2_2_2 %ld clocks\n",timers[27]);
                printf("Splitter_1_2_2 %ld clocks\n",timers[28]);
                    printf("Splitter_1_2 %ld clocks\n",timers[29]);
                        printf("Splitter_1 %ld clocks\n",timers[30]);
      printf("Root %ld clocks\n",timers[31]);   

    }
    else if(atoi(height)==5){
      printf("Searcher_1_1_1_1_1_1 %ld clocks\n",timers[0]);
      printf("Searcher_1_1_1_1_1_2 %ld clocks\n",timers[1]);
            printf("Splitter_1_1_1_1_1 %ld clocks\n",timers[2]);
      printf("Searcher_1_1_1_1_2_1 %ld clocks\n",timers[3]);
      printf("Searcher_1_1_1_1_2_2 %ld clocks\n",timers[4]);
            printf("Splitter_1_1_1_1_2 %ld clocks\n",timers[5]);
                printf("Splitter_1_1_1_1 %ld clocks\n",timers[6]);
      printf("Searcher_1_1_1_2_1_1 %ld clocks\n",timers[7]);
      printf("Searcher_1_1_1_2_1_2 %ld clocks\n",timers[8]);
            printf("Splitter_1_1_1_2_1 %ld clocks\n",timers[9]);
      printf("Searcher_1_1_1_2_2_1 %ld clocks\n",timers[10]);
      printf("Searcher_1_1_1_2_2_2 %ld clocks\n",timers[11]);
            printf("Splitter_1_1_1_2_2 %ld clocks\n",timers[12]);
                printf("Splitter_1_1_1_2 %ld clocks\n",timers[13]);
                    printf("Splitter_1_1_1 %ld clocks\n",timers[14]);
      printf("Searcher_1_1_2_1_1_1 %ld clocks\n",timers[15]);
      printf("Searcher_1_1_2_1_1_2 %ld clocks\n",timers[16]);
            printf("Splitter_1_1_2_1_1 %ld clocks\n",timers[17]);
      printf("Searcher_1_1_2_1_2_1 %ld clocks\n",timers[18]);
      printf("Searcher_1_1_2_1_2_2 %ld clocks\n",timers[19]);
            printf("Splitter_1_1_2_1_2 %ld clocks\n",timers[20]);
                printf("Splitter_1_1_2_1 %ld clocks\n",timers[21]);
      printf("Searcher_1_1_2_2_1_1 %ld clocks\n",timers[22]);
      printf("Searcher_1_1_2_2_1_2 %ld clocks\n",timers[23]);
            printf("Splitter_1_1_2_2_1 %ld clocks\n",timers[24]);
      printf("Searcher_1_1_2_2_2_1 %ld clocks\n",timers[25]);
      printf("Searcher_1_1_2_2_2_2 %ld clocks\n",timers[26]);
            printf("Splitter_1_1_2_2_2 %ld clocks\n",timers[27]);
                printf("Splitter_1_1_2_2 %ld clocks\n",timers[28]);
                    printf("Splitter_1_1_2 %ld clocks\n",timers[29]);
                        printf("Splitter_1_1 %ld clocks\n",timers[30]);
      printf("Searcher_1_2_1_1_1_1 %ld clocks\n",timers[31]);
      printf("Searcher_1_2_1_1_1_2 %ld clocks\n",timers[32]);
            printf("Splitter_1_2_1_1_1 %ld clocks\n",timers[33]);
      printf("Searcher_1_2_1_1_2_1 %ld clocks\n",timers[34]);
      printf("Searcher_1_2_1_1_2_2 %ld clocks\n",timers[35]);
            printf("Splitter_1_2_1_1_2 %ld clocks\n",timers[36]);
                printf("Splitter_1_2_1_1 %ld clocks\n",timers[37]);
      printf("Searcher_1_2_1_2_1_1 %ld clocks\n",timers[38]);
      printf("Searcher_1_2_1_2_1_2 %ld clocks\n",timers[39]);
                printf("Splitter_1_2_1_2_1 %ld clocks\n",timers[40]);
      printf("Searcher_1_2_1_2_2_1 %ld clocks\n",timers[41]);
      printf("Searcher_1_2_1_2_2_2 %ld clocks\n",timers[42]);
            printf("Splitter_1_2_1_2_2 %ld clocks\n",timers[43]);
                printf("Splitter_1_2_1_2 %ld clocks\n",timers[44]);
                    printf("Splitter_1_2_1 %ld clocks\n",timers[45]);
      printf("Searcher_1_2_2_1_1_1 %ld clocks\n",timers[46]);
      printf("Searcher_1_2_2_1_1_2 %ld clocks\n",timers[47]);
            printf("Splitter_1_2_2_1_1 %ld clocks\n",timers[48]);
      printf("Searcher_1_2_2_1_2_1 %ld clocks\n",timers[49]);
      printf("Searcher_1_2_2_1_2_2 %ld clocks\n",timers[50]);
            printf("Splitter_1_2_2_1_2 %ld clocks\n",timers[51]);
                printf("Splitter_1_2_2_1 %ld clocks\n",timers[52]);
      printf("Searcher_1_2_2_2_1_1 %ld clocks\n",timers[53]);
      printf("Searcher_1_2_2_2_1_2 %ld clocks\n",timers[54]);
            printf("Splitter_1_2_2_2_1 %ld clocks\n",timers[55]);
      printf("Searcher_1_2_2_2_2_1 %ld clocks\n",timers[56]);
      printf("Searcher_1_2_2_2_2_2 %ld clocks\n",timers[57]);
            printf("Splitter_1_2_2_2_2 %ld clocks\n",timers[58]);
                printf("Splitter_1_2_2_2 %ld clocks\n",timers[59]);
                    printf("Splitter_1_2_2 %ld clocks\n",timers[60]);
                        printf("Splitter_1_2 %ld clocks\n",timers[61]);
                            printf("Splitter_1 %ld clocks\n",timers[62]);
      printf("Root %ld clocks\n",timers[63]);     
    } 
}