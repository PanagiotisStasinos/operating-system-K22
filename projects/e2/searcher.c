#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include "record.h"

// argv[0]  datafile
// argv[1]  height=1
// argv[2]  pattern
// argv[3]  skew_flag
// argv[4]  start
// argv[5]	end
// argv[6]  fifo
// argv[7]  fifo_
// argv[8]  root pid

void insert_rec(struct ListHead* head, MyRecord record){
   head->count++;
   struct ListRec* temp = head->first;
   if(head->first == NULL){
      head->first = (struct ListRec*)malloc(sizeof(struct ListRec));
      memcpy(&(head->first->rec) ,&record, sizeof(record));
      head->first->next = NULL;
   }
   else{
      while(temp->next != NULL){
         temp = temp->next;
      }
      temp->next = (struct ListRec*)malloc(sizeof(struct ListRec));
      memcpy(&(temp->next->rec) ,&record, sizeof(record));
      temp->next->next = NULL;
   }
}

void free_list(struct ListHead* head){
   struct ListRec* temp = head->first;
   struct ListRec* next;
   for(int i=0 ; i<(head->count) ; i++){
      next = temp->next;
      free(temp);
      temp = next;
   }
}

int main (int argc, char** argv)
{
      clock_t start_t, end_t, total_t;
      start_t = clock();

   	FILE *fpb;
   	MyRecord rec;

   	fpb = fopen (argv[0],"rb");
   	if (fpb==NULL) {
      	printf("Cannot open binary file\n");
      	return 1;
   	}

      struct ListHead head;
      head.count = 0;
      head.first = NULL;
      
      fseek(fpb,atoi(argv[4])*sizeof(rec),SEEK_SET);
   	char temp_str[130];
      int temp_str_len, loops, arg_len;
      char pattern[50];
      for (int i=atoi(argv[4]); i<=atoi(argv[5]) ; i++) {
      	fread(&rec, sizeof(rec), 1, fpb);

         sprintf(temp_str, "%ld%s%s%s%d%s%s%.2f",rec.custid, rec.FirstName, rec.LastName, rec.Street, rec.HouseID, rec.City, rec.postcode, rec.amount);
         temp_str_len = strlen(temp_str);
         // printf("|%s|%d\n",temp_str,temp_str_len);

         arg_len = strlen(argv[2]);
         loops = temp_str_len - arg_len +1;

         for(int j=0 ; j<=loops ; j++){
            strncpy(pattern, &(temp_str[j]), arg_len+1);
            pattern[arg_len]='\0';
            // for(int s=0 ; s<j ; s++){printf(" ");}
            // printf("%s\n",pattern);

            if(strcmp(pattern,argv[2])==0){
               insert_rec(&head,rec);
               break;
            }
         }
   	}

      struct ListRec* temp = head.first;
      MyRecord *buffer;
      buffer = (MyRecord*)malloc((head.count)*sizeof(MyRecord));
      int index=0;
      while(temp!=NULL){
         memcpy(&buffer[index],&(temp->rec), sizeof(rec));
         index++;
         temp = temp->next;
      }
// printing
/*      for(int i=0 ; i<head.count ; i++){
         printf("-%d) %ld %s %s  %s %d %s %s %-9.2f\n", head.count,\
      buffer[i].custid, buffer[i].LastName, buffer[i].FirstName, \
      buffer[i].Street, buffer[i].HouseID, buffer[i].City, buffer[i].postcode, \
      buffer[i].amount);
      }*/

      char *ptr=NULL;
      long root_id = strtol(argv[8], &ptr, 10);
      if(ptr==NULL){
         printf("free(ptr);\n");
         free(ptr);
      }

// count
      mkfifo(argv[7],0666);
      int fd_ = open(argv[7], O_WRONLY);
      write(fd_, &(head.count), sizeof(head.count));
      close(fd_);
// recs
      mkfifo(argv[6],0666);
      int fd = open(argv[6], O_WRONLY);
      write(fd, buffer, (head.count)*sizeof(MyRecord));
      close(fd);

   	fclose(fpb);
      free(buffer);
      free_list(&head);
// time
      char fifo_T[name_pipe_size];
      snprintf(fifo_T, name_pipe_size, "%s%s",argv[6],"_T");
      mkfifo(fifo_T,0666);
      int fd_T = open(fifo_T, O_WRONLY);

      end_t = clock();
      // total_t = (double)(end_t-start_t)/CLOCKS_PER_SEC;
      total_t = end_t-start_t;
      printf(" -> |Searcher%s| %d recs - %ld clocks - (%s,%s)\n",&(argv[7][4]),head.count,total_t,argv[4],argv[5]);
      write(fd_T, &total_t, sizeof(total_t));
      close(fd_T);    

      kill(root_id,SIGUSR2);
   	return 0;
}