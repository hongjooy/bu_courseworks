
output: main.o fifo.o
	gcc main.o fifo.o -o twentyfour

main.o: main.c
	gcc -c main.c

bits.o: fifo.c fifo.h
	gcc -c fifo.c

clean:
	rm *.o twentyfour

