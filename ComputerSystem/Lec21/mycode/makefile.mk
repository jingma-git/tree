CC=gcc
CFLAGS=-g -O2 -Wall
LDLIBS=-lpthread

all: echoclient echoserver

echoclient: echoclient.c csapp.h csapp.c
echoserver: echoserver.c csapp.h csapp.c
clean:
	rm -f *.o *~ *.exe hostinfo echoclient echoserver csapp.o