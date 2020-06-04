CC=g++
CFLAGS=-I.

all : tcp udp

tcp : tcpc tcps
udp : udpc udps

tcpc :
	$(CC) -o tcpclient tcpclient.c $(CFLAGS)

tcps :
	$(CC) -o tcpserver tcpserver.c $(CFLAGS)

udpc :
	$(CC) -o udpclient udpclient.c $(CFLAGS)

udps :
	$(CC) -o udpserver udpserver.c $(CFLAGS)

erase :
	rm tcpclient
	rm tcpserver
	rm udpclient
	rm udpserver
