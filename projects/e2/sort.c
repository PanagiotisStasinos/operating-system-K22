#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "record.h"

int main (int argc, char** argv){

	int count = atoi(argv[1]);
    MyRecord rec;
	system("sort -k 1,1 Temp.bin > Sorted.bin");
	printf("Sorted Results (%d)\n",count);
	FILE *sorted = fopen("Sorted.bin","rb");
	FILE *final_sorted = fopen("SortedResults.bin","wb");
	for (int i=0; i<count ; i++) {
	    fread(&(rec.custid), sizeof(long), 1, sorted);
	    fseek(sorted , 1 , SEEK_CUR);

	    fread(&(rec.LastName), sizeof(rec.LastName), 1, sorted);
	    fseek (sorted , 1 , SEEK_CUR);

		fread(&(rec.FirstName), sizeof(rec.FirstName), 1, sorted);
	    fseek (sorted , 1 , SEEK_CUR);

		fread(&(rec.Street), sizeof(rec.Street), 1, sorted);
	    fseek (sorted , 1 , SEEK_CUR);

		fread(&(rec.HouseID), sizeof(rec.HouseID), 1, sorted);
	    fseek (sorted , 1 , SEEK_CUR);

		fread(&(rec.City), sizeof(rec.City), 1, sorted);
	    fseek (sorted , 1 , SEEK_CUR);

		fread(&(rec.postcode), sizeof(rec.postcode), 1, sorted);
	    fseek (sorted , 1 , SEEK_CUR);

		fread(&(rec.amount), sizeof(rec.amount), 1, sorted);

		fseek (sorted , 1 , SEEK_CUR);

	    printf("%d) |%ld|%s|%s|%s|%d|%s|%s|%.2f|\n", i+1,\
			rec.custid, rec.LastName, rec.FirstName, \
			rec.Street, rec.HouseID, rec.City, rec.postcode, \
			rec.amount);
	    
	    fwrite(&rec,1,sizeof(rec),final_sorted);
	}
	fclose(sorted);
	fclose(final_sorted);

	return 0;
}