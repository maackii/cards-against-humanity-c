#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "players.h"


#ifndef CONNECTIVITY_H_INCLUDED
#define CONNECTIVITY_H_INCLUDED

#define ERROR -1
#define SUCCESS 1

/*bitflags & ctrl messages*/

#define CTRL_MESSAGE     0
#define DATA_MESSAGE     1
#define NEW_ROUND        2
#define OK               4
#define DISPLAY_CARDS    8
#define DISPLAY_ANSWERS 16


/*Connection Functions*/
int createMainSocket(int port);                       /*Create main Socket*/
int connectClient(int mainSocket);                    /*Receive Client Addresses*/
void awesomeError(const char *msg);                   /*Personalized error handling*/
void closeConnections(player_t *head, int mainSocket); /*..close all*/
int connectToServer(int port, struct hostent* pServer);

/*data transfer Functions*/
//int getMessages(player_t* current);
//char* readLine(int currentSocket, uint8_t memSize);
int sendDataPackage(int socket, uint8_t typeFlag, uint8_t typeID, char** messages, int numberOfMessages);
int sendIntPackage(int socket, uint8_t flag, uint8_t payload);
int getIntPackage (int socket, uint8_t* payload);
uint8_t getStatus(int socket);

//int getInt(int socket, uint8_t* payload, uint8_t statusID);






#endif
