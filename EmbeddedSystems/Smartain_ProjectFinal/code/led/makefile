CC=arm-linux-gcc
light: light.o luxcalc.o
	$(CC) light.o luxcalc.o -s -o light
light.o: light.c
	$(CC) -c light.c
luxcalc.o: luxcalc.c
	$(CC) -c luxcalc.c -o luxcalc.o
clean:
	rm -rf *.o light
