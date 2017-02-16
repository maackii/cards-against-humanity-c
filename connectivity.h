#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "players.h"


#ifndef CONNECTIVITY_H_INCLUDED
#define CONNECTIVITY_H_INCLUDED

#define ERROR -1
#define SUCCESS 1

/*bitflags & ctrl messages*/

#define MSG_CTRL_MESSAGE     0
#define MSG_DATA_MESSAGE     1
#define MSG_NEW_ROUND        2
#define MSG_OK               4
#define MSG_DISPLAY_CARDS    8
#define MSG_DISPLAY_ANSWERS 16

/*typeFlags*/

#define D_TYPE_HANDCARDS 1
#define D_TYPE_QUESTION 2
#define D_TYPE_NAME 4
#define D_TYPE_POINTS 8
#define D_TYPE_ROLE 16

/*
enum dataType{
  handcards,
  question,
  name,
  points,
  role
};
 */

/*Connection Functions*/
int createMainSocket(int port);                       /*Create main Socket*/
int connectClient(int mainSocket);                    /*Receive Client Addresses*/
void awesomeError(const char *msg);                   /*Personalized error handling*/
void closeConnections(player_t *head, int mainSocket); /*..close all*/
int connectToServer(int port, struct hostent* pServer);

/*data transfer Functions*/
//int getMessages(player_t* current);
//char* readLine(int currentSocket, uint8_t memSize);
int sendDataPackage(int socket, uint8_t typeFlag, uint8_t typeID, int numberOfMessages, char** messages);
int getDataPackage(int socket, char** *messages, uint8_t *typeID);
char* readLine(int currentSocket, uint8_t memSize);
int sendIntPackage(int socket, uint8_t flag, uint8_t payload);
int getIntPackage (int socket, uint8_t* payload);
uint8_t getStatus(int socket);

//int getInt(int socket, uint8_t* payload, uint8_t statusID);






#endif
