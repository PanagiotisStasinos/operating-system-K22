mygraph : main.o graph.o hashtable.o menu.o
	cc -o mygraph main.o graph.o hashtable.o menu.o

main.o : main.c graph.h
	cc -c main.c
graph.o : graph.c graph.h
	cc -c graph.c
hashtable.o : hashtable.c hashtable.h
	cc -c hashtable.c
menu.o : menu.c menu.h
	cc -c menu.c
clean :
	rm -f *.o
