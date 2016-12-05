CC=gcc
CFLAGS=-lpulse -lpulse-simple

.DEFAULT_GOAL := vrpm

parec: parec.o
	$(CC) $(CFLAGS) -o build/parec parec.o 

vrpm: vrpm.o
	$(CC) $(CFLAGS) -o build/vrpm vrpm.o 
