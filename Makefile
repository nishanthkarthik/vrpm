CC=gcc
CFLAGS=-lpulse -lpulse-simple
CFLDBG=-std=c99 -pedantic -Wall -Werror -g

.DEFAULT_GOAL := vrpm

# parec: parec.o
# 	$(CC) $(CFLAGS) -o build/parec parec.o 

all: signalmain

vrpm: vrpm.o
	$(CC) $(CFLAGS) -o build/vrpm vrpm.o 

bridge: bridge.o
	$(CC) -o build/bridge bridge.o

signalmain: signalmain.c signal.o signal.h
	$(CC) $(CFLDBG) -o build/signalmain signal.o signalmain.c