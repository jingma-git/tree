CC=gcc
CFLAGS=-g -O2 -Wall
LDLIBS=-lpthread

all: echoclient

echoclient: echoclient.c csapp.h csapp.c
clean:
	rm -f *.o *~ *.exe hostinfo echoclient echoserveri csapp.o