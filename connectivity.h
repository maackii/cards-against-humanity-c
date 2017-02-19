#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "players.h"

#define D_ERROR
#define D_INFO
#define D_DEBUG

#ifdef D_ERROR
  #define pERROR(format, args...) fprintf(stderr, "[ERROR]| "format"\n", args)
#endif
#ifdef D_INFO
  #define pINFO(format, args...)  fprintf(stdout, "[INFO] | "format"\n", args)
#endif
#ifdef D_DEBUG
  #define pDEBUG(format, args...) fprintf(stdout, "[DEBUG]| "format"\n", args)
#endif

#ifndef D_ERROR
  #define pERROR(format, args...)
#endif
#ifndef D_INFO
  #define pINFO(format, args...)
#endif
#ifndef D_DEBUG
  #define pDEBUG(format, args...)
#endif

#ifndef CONNECTIVITY_H_INCLUDED
#define CONNECTIVITY_H_INCLUDED

#define ERROR -1
#define SUCCESS 1

#define SHOW_ERROR_HANDLING 1

/*bitflags & ctrl messages*/

#define MSG_CTRL        0
#define MSG_DATA        1

#define C_TYPE_NEW_ROUND        0
#define C_TYPE_OK               1
#define C_TYPE_DISPLAY_CARDS    2
#define C_TYPE_DISPLAY_ANSWERS  4
#define C_TYPE_RESET            8

/*typeFlags*/

#define D_TYPE_HANDCARDS  0
#define D_TYPE_QUESTION   1
#define D_TYPE_NAME       2
#define D_TYPE_POINTS     4
#define D_TYPE_ROLE       8
#define D_TYPE_REPLIES   16

/*ERROR HANDLING*/
void gimme_good_lines(char *message, int line);

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
