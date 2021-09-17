all: 	\
	Verify myfind splitter searcher sort

myfind: root.c
	gcc -g root.c -o myfind

splitter: splitter.c
	gcc -g splitter.c -o splitter

searcher: searcher.c
	gcc -g searcher.c -o searcher

sort: sort.c
	gcc -g sort.c -o sort


Verify: Verify.c
	gcc -g Verify.c -o Verify

clean:
	rm -f 	\
		Verify myfind splitter searcher sort SortedResults.bin UnSortedResults.bin fifo*.pipe
