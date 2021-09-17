#ifndef RECORD_H
#define RECORD_H

#define SIZEofBUFF 20
#define SSizeofBUFF 6
#define Buf_size_num_of_recs 12
#define name_pipe_size 24

typedef struct{
	long  	custid;
	char 	FirstName[SIZEofBUFF];
	char 	LastName[SIZEofBUFF];
	char	Street[SIZEofBUFF];
	int 	HouseID;
	char	City[SIZEofBUFF];
	char	postcode[SSizeofBUFF];
	float  	amount;
} MyRecord;

struct ListRec{
	MyRecord rec;
	struct ListRec* next;
};

struct ListHead{
	int count;
	struct ListRec* first;
};

#endif //record.h