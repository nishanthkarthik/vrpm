TARGETBRIDGE = ../build/bridge
TARGETPULSE = ../build/record

LIBS = -lpulse -lpulse-simple -fopenmp -lfftw3
CC = gcc
CFLAGS = -Wall -O3

.PHONY: default all clean

default: $(TARGETBRIDGE)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGETBRIDGE): bridge.o
	$(CC) $(CFLAGS) bridge.o -Wall $(LIBS) -o $@

$(TARGETPULSE): vrpm.o
	$(CC) $(CFLAGS) vrpm.o -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)

