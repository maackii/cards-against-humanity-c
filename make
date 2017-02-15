all: connectserver connectclient
CC = gcc
CFLAGS = -Wall 
DEPS = connectivity.h players.h
OBJS = connectivity.o players.o

connectserver: server.o $(OBJS)
	$(CC) -o connectserver server.o $(OBJS)
connectclient: client.o connectivity.o players.o
	$(CC) -o connectclient client.o $(OBJS)
server.o : server.c $(DEPS)
	$(CC) -c server.c  
client.o : client.c $(DEPS)
	$(CC) -c client.c
connectivity.o : connectivity.c $(DEPS)
	$(CC) -c connectivity.c
players.o: players.c $(DEPS)
	$(CC) -c players.c
clean :
	rm $(OBJS) client.o server.o connectclient connectserver