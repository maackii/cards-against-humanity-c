#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "cards.h"


#ifndef CONNECTIVITY_H_INCLUDED
#define CONNECTIVITY_H_INCLUDED

#define ERROR -1
#define SUCCESS 1




/*Connection Functions*/
int createMainSocket(int port);                       /*Create main Socket*/
int connectClient(int mainSocket);                    /*Receive Client Addresses*/
void awesomeError(const char *msg);                   /*Personalized error handling*/
void closeConnections(player_t *head, int mainSocket); /*..close all*/

int connectToServer(int port, struct hostent* pServer);

/*Statemachine Functions*/
int getMessages(player_t* current);
char* readLine(int currentSocket, uint8_t memSize);         /*Receive Client Messages*/
int sendPackage(int socket,uint8_t flag, char** messages, int numberOfMessages);
int getPackage(int socket, char** *messages, uint8_t statusID);
uint8_t getStatus(int socket);

int sendInt(int socket,uint8_t flag, uint8_t payload);
int getInt(int socket, uint8_t* payload, uint8_t statusID);






#endif
