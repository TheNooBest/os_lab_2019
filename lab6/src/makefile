CC=g++
CFLAGS=-I.

all : client server

libr.o : libr.h
	$(CC) -o libr.o -c libr.c $(CFLAGS)

client : libr.o
	$(CC) -o cl libr.o client.c $(CFLAGS)

server : libr.o
	$(CC) -o serv libr.o server.c -lpthread $(CFLAGS)

clean :
	rm libr.o