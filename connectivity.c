/*
 *      DataPackage structure
 *      | DATAflag | TYPEflag | TYPE ID | NUMB MESSAGES | STR LEN | DATA |
 */


#include <stdio.h>
#include <sys/socket.h>
//#include <errno.h>
//#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "connectivity.h"
#include "players.h"




/*************************************************************************connection functions*/
int createMainSocket(int port) {
    int mainSockFile = 0 ;
    struct sockaddr_in servAddr;
    printf("nonblock?\n");
    mainSockFile = socket(AF_INET, SOCK_STREAM, 0);
    if (mainSockFile < 0){
        //awesomeError("Socket Error!");
    }
    memset ((char *) &servAddr,0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(port);

    if (bind (mainSockFile, (struct sockaddr *) &servAddr, sizeof(servAddr))<0){
        //awesomeError("Binding Error!");
    }

    //---------------------------remove after testing!
    printf("My socket ID is %d\n", mainSockFile);

    return mainSockFile;
}

int connectToServer(int port, struct hostent* pServer) {

    int mySocket;
    struct sockaddr_in servAddr;

    mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket < 0){
        //awesomeError("Socket Error!");
        return mySocket;
    }
    memset ((char *) &servAddr,0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    memcpy((char *)&servAddr.sin_addr.s_addr,
           (char *)pServer->h_addr_list[0],
           pServer->h_length);
    servAddr.sin_port = htons(port);
    if (connect(mySocket,(struct sockaddr *) &servAddr,sizeof(servAddr)) < 0){
        //awesomeError("ERROR connecting");

        //--------------------------------------exit program here!!
        exit(-1);
    }
    return mySocket;
}

void closeConnections(player_t* head, int mainSocket){
    player_t* current = head;
    while (current != NULL) {
        close(current->socketID);
        current = current->nextPlayer;
    }
    close(mainSocket);
    return;
}

int connectClient(int mainSocket){
    int clieLen, incomingConn;
    struct sockaddr_in clieAddr;

    clieLen = sizeof(clieAddr);
    incomingConn = accept(mainSocket, (struct sockaddr*) &clieAddr, (socklen_t *) &clieLen);
    if (incomingConn < 0 ) {
        //awesomeError("Waiting for connection");
        return -1;
    }
    //---------------------------remove after testing!
    printf("Got incoming Connection with ID: %d\n", incomingConn);

    return incomingConn;
}

/*************************************************************************data transfer functions*/

uint8_t getStatus(int socket){
    uint8_t statusID = 0;
    if(socket < 0) return ERROR;
    int cnt = read(socket, &statusID,1);
    return statusID;
}

int sendDataPackage(int socket, uint8_t typeFlag, uint8_t typeID, char** messages, int numberOfMessages){
    uint8_t* package = NULL;
    intmax_t cnt;
    int packageSize;
    int sumOfPayloadLength = 0;

    if(socket < 0) return ERROR;

    for (int n = 0; n < numberOfMessages; n++) {
        sumOfPayloadLength += strlen(messages[n]);
    }
    packageSize = sumOfPayloadLength + numberOfMessages + 1 + 1 + 1 + 1;
    package = malloc((packageSize) * sizeof(uint8_t));
    if(package == NULL){
        return ERROR;
    }
    package[0] = DATA_MESSAGE;
    package[1] = typeFlag;
    package[2] = typeID;
    package[3] = numberOfMessages;

    for (int n = 0, track = 4; n < numberOfMessages; n++){
        package[track] = (uint8_t) strlen(messages[n]);
        memcpy( &(package[track+1]), messages[n], strlen(messages[n])); //added +1
    }
    cnt = write(socket, package, packageSize);

     free(package);
    return (cnt < 0 ? ERROR : SUCCESS);
    //awesomeError("ERROR writing to socket");
}

int sendIntPackage(int socket, uint8_t flag, uint8_t payload){
    uint8_t* package = NULL;
    intmax_t cnt;
    uint8_t packageSize = 0;

    if(socket < 0) return ERROR;

    packageSize = 2;
    package = malloc((packageSize) * sizeof(uint8_t));
    if(package == NULL){
        return ERROR;
    }
    package[0] = flag;
    package[1] = payload;
    cnt = write(socket, package, packageSize);
    free(package);
    return (cnt < 0 ? ERROR : SUCCESS);
}

int getIntPackage (int socket, uint8_t* payload){
  intmax_t cnt = 0;
  if(socket < 0 ) return ERROR;
  cnt = read(socket, payload,1);
    if(cnt < 0 || *payload < 0){
      return ERROR;
    }
    printf("Received integer %d.\n", *payload);
    return SUCCESS;
}

/*int main() {
    char **cards = malloc(1 * sizeof(char *));
    cards[0] = malloc(4 * sizeof(char));
    cards[0] = "Hell";
    if (SUCCESS == sendDataPackage(3, 1, 2, cards, 1)) {
        printf("Hello, World!\n");
    }
    return 0;
}
*/
