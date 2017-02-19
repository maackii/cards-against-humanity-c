CC = gcc
CFLAGS = -Wall
DEPS = connectivity.h players.h
OBJS = connectivity.o players.o


all: connectserver connectclient


connectserver: server.o cardpiles.o $(OBJS)
	$(CC) -o connectserver server.o cardpiles.o $(OBJS)
connectclient: client.o connectivity.o cardpiles.o players.o
	$(CC) -o connectclient client.o connectivity.o cardpiles.o players.o

server.o : server.c $(DEPS) cardpiles.h
	$(CC) -c server.c
client.o : client.c $(DEPS)
	$(CC) -c client.c
connectivity.o : connectivity.c $(DEPS)
	$(CC) -c connectivity.c
players.o: players.c $(DEPS)
	$(CC) -c players.c
cardpiles.o: cardpiles.c $(DEPS)
	$(CC) -c cardpiles.c

clean :
	rm $(OBJS) client.o server.o cardpiles.o connectclient connectserver
