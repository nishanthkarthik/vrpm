CC=gcc
CFLAGS=-lpulse -lpulse-simple

.DEFAULT_GOAL := vrpm

# parec: parec.o
# 	$(CC) $(CFLAGS) -o build/parec parec.o 

all: vrpm bridge

vrpm: vrpm.o
	$(CC) $(CFLAGS) -o build/vrpm vrpm.o 

bridge: bridge.o
	$(CC) -o build/bridge bridge.o