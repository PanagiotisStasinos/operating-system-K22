all: 	\
	myport vessel port_master monitor

myport: myport.c
	gcc -g myport.c -o myport -lpthread

vessel: vessel.c
	gcc -g vessel.c -o vessel -utils.h -lpthread

port_master: port_master.c
	gcc -g port_master.c -o port_master -utils.h -lpthread

monitor: monitor.c
	gcc -g monitor.c -o monitor -utils.h -lpthread

clean:
	rm -f 	\
		myport vessel port_master monitor
