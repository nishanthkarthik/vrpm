CC=gcc
CFLAGS=-lpulse -lpulse-simple

.DEFAULT_GOAL := vrpm

parec: parec.o
	$(CC) $(CFLAGS) -o parec parec.o 

vrpm: vrpm.o
	$(CC) $(CFLAGS) -o vrpm vrpm.o 
