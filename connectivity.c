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


int connectToServer(int port, struct hostent* pServer) {

    int mySockFile;
    struct sockaddr_in servAddr;

    mySockFile = socket(AF_INET, SOCK_STREAM, 0);
    if (mySockFile < 0){
        awesomeError("Socket Error!");
        return mySockFile;
    }

    bzero ( (char *) &servAddr, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    bcopy((char *)pServer->h_addr,
          (char *)&servAddr.sin_addr.s_addr,
          pServer->h_length);
    servAddr.sin_port = htons(port);
    if (connect(mySockFile,(struct sockaddr *) &servAddr,sizeof(servAddr)) < 0){
      awesomeError("ERROR connecting");

      //--------------------------------------exit program here!!
      exit(-1);
    }
    return mySockFile;
}


int createMainSocket(int port) {
    int mainSockFile = 0 ;
    struct sockaddr_in servAddr;
    printf("nonblock?\n");
    /*I socket my socket*/
    mainSockFile = socket(AF_INET, SOCK_STREAM, 0);
    if (mainSockFile < 0){
        awesomeError("Socket Error!");
    }
    /*I fill my server address with 0s because that's what peoples on the web do*/
    bzero ( (char *) &servAddr, sizeof(servAddr));
    /*these are all standard code*/
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(port);

    /*I binds my socket*/
    if (bind (mainSockFile, (struct sockaddr *) &servAddr, sizeof(servAddr))<0){
        awesomeError("Binding Error!");
    }
    printf("My socket ID is %d\n", mainSockFile);
    return mainSockFile;
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
    printf("Got incoming Connection with ID: %d\n", incomingConn);  /*test message*/
    return incomingConn;
}

/*int dealCards(player_t* head, pilePointer, discardPointer, flag){
    player_t* current = head;
    int amount;
    int cnt = 0;

    while(current!=NULL){
      switch (flag) {
        case (hasWhiteC):
          if(current->numbCards < MAXHANDCARDS){
          amount = MAXHANDCARDS - current->numbCards;}
          else {break};
          if(success == sendPackage)
          current->numbCards = MAXHANDCARDS;
          cnt++;





          return success;
    }
    return 0;
  }
}
*/


//removed neSockFile
int getMessages(player_t* current){
    int  numbMessages;
    uint8_t requestSize;
    uint8_t statusID;
    char* tempBuffer = NULL;
    intmax_t cnt = 0;
    int n;

    printf("My ID: %d\n", current->socketID);

    cnt = read(current->socketID, &statusID,1);
    printf("my StatusID is %d\n", statusID);

    //if (statusID & hasTail){

    current->numberAnswers = 2;
    current->answers = malloc((current->numberAnswers) * sizeof(char*));

    for (n = 0; n < 2; n++){

        cnt = read(current->socketID, &requestSize, 1);
        printf("My awesome memSize is %d\n", requestSize);

        if (cnt < 0){
            awesomeError("error reading memSize from sock");
            return fail;
        }

        tempBuffer = readLine(current->socketID, requestSize);

        if (tempBuffer == NULL){
            return fail;
        } else {

            current->answers[n] = tempBuffer;
        }

    }
    return success;
}
//added 2-dimensional array for messages
int sendPackage(int socket,uint8_t flag, char** messages, int numberOfMessages){
    uint8_t* package = NULL;
    intmax_t cnt;
    int packageSize, track = 1;
    int sumOfPayloadLength = 0;

    if(socket < 0) return ERROR;

    for (int n = 0; n < numberOfMessages; n++) {
        sumOfPayloadLength += strlen(messages[n]);
    }
    packageSize = sumOfPayloadLength + numberOfMessages + 1 + 1;
    package = malloc((packageSize) * sizeof(uint8_t));
    if(package == NULL){
        return fail;
    }
    package[0] = flag | payloadIsString;
    package[1] = numberOfMessages;
    for (int n = 0, track = 2; n < numberOfMessages; n++){
        package[track] = (uint8_t) strlen(messages[n]);
        memcpy( &(package[track+1]), messages[n], strlen(messages[n])); //added +1
        track += package[track] + 1;
    }
    cnt = write(socket, package, packageSize);
    free(package);
    return (cnt < 0 ? fail : success);
    //awesomeError("ERROR writing to socket");
}


int getPackage(int socket, char** *messages, uint8_t statusID){
  uint8_t  numbMessages;
  uint8_t stringSize;
  char* tempBuffer = NULL;
  intmax_t cnt = 0;

  if(socket < 0) return ERROR;

  printf("My ID: %d\n", socket);

  if(statusID == 0){
    statusID = getStatus(socket);
    if(statusID==0) return 0;
    printf("my StatusID is %d\n", statusID);
  }

  if (statusID & payloadIsString){
    cnt = read(socket, &numbMessages,1);

    if(cnt < 0 || numbMessages < 0){
      awesomeError("Number of strings in Packet unclear!");
      return ERROR;
    }

    printf("Receiving %d messages.\n", numbMessages);

    *messages = malloc(numbMessages*sizeof(char*));

    if(*messages == NULL) {
      awesomeError("Error allocating memory!");
      return ERROR;
    }

    for (int n = 0; n < numbMessages; n++){

        cnt = read(socket, &stringSize, 1);
        if (cnt < 0){
            awesomeError("error reading memSize from sock");
            return ERROR;
        }
        printf("size of payload string %d is %d bytes\n",n+1, stringSize);

        (*messages)[n] = readLine(socket, stringSize);
    }


  }

  return numbMessages;
}


int sendInt(int socket,uint8_t flag, uint8_t payload){
  uint8_t* package = NULL;
  intmax_t cnt;
  uint8_t packageSize = 0;

  if(socket < 0) return ERROR;

  packageSize = 2;
  package = malloc((packageSize) * sizeof(uint8_t));
  if(package == NULL){
      return fail;
  }
  package[0] = flag | payloadIsInt;
  package[1] = payload;
  cnt = write(socket, package, packageSize);
  free(package);
  return (cnt < 0 ? fail : success);
}


int getInt(int socket, uint8_t* payload, uint8_t statusID){
  intmax_t cnt = 0;

  if(socket < 0 || payload == NULL) return ERROR;

  printf("My ID: %d\n", socket);

  if(statusID == 0){
    statusID = getStatus(socket);
    if(statusID==0) return 0;
    printf("my StatusID is %d\n", statusID);
  }

  if (statusID & payloadIsInt){
    cnt = read(socket, payload,1);

    if(cnt < 0 || *payload < 0){
      awesomeError("Number of strings in Packet unclear!");
      return ERROR;
    }

    printf("Receivied integer %d.\n", *payload);
  }

  return SUCCESS;
}




char* readLine(int currentSocket, uint8_t memSize){
    char* buffer = NULL;
    intmax_t cnt = 0;
    printf("%d\n", memSize);
    buffer = malloc (memSize+1);
    cnt = read(currentSocket, buffer, memSize);
    buffer[memSize] = 0;
    if (cnt < 0){
        awesomeError("error reading String from sock");
        free(buffer);
        return NULL;
    }
    //cnt = write(currentSocket, "Message(s) received", 19);
    if (cnt < 0) {
        awesomeError("ERROR writing to socket");
        //cnt = write(currentSocket, "0 Message(s) sent", 17);
    }
    return buffer;
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

void awesomeError(const char *msg) {
    perror(msg);
}

uint8_t getStatus(int socket){
  uint8_t statusID = 0;
  if(socket < 0) return ERROR;
  int cnt = read(socket, &statusID,1);
  return statusID;
}
