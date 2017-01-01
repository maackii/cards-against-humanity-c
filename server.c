#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

/*Bitflags*/
#define hasTail 1

/*Personalized error handling*/

void awesomeError(const char *msg)
{
    perror(msg);
}

int main(int argc, char* argv[]) {

/*socket communication variables*/
    struct sockaddr_in clieAddr, servAddr;
    int mySockFile, newSockFile, clieLen, portNumbr;
    intmax_t cnt;
    intmax_t n = 0;
    uint8_t memSize;
    uint8_t* buffer;
    uint8_t statusID;


    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }


    /*I socket my socket*/
    mySockFile = socket(AF_INET, SOCK_STREAM, 0);
    if (mySockFile < 0){
        awesomeError("Socket Error!");
    }
    /*I fill my server address with 0s because that's what peoples on the web do*/
    bzero ( (char *) &servAddr, sizeof(servAddr));

    portNumbr = atoi(argv[1]);
    /*these are all standard code*/
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(portNumbr);

    /*I binds my socket*/
    if (bind (mySockFile, (struct sockaddr *) &servAddr, sizeof(servAddr))<0){
        awesomeError("Binding Error!");
    }
    /*we listen*/
    listen(mySockFile, 5);

    clieLen = sizeof(clieAddr);
    newSockFile = accept(mySockFile, (struct sockaddr*) &clieAddr, (socklen_t *) &clieLen);
    if (newSockFile < 0 ) {
        awesomeError("Error upon accepting");
    }

    /*Receiveroo*/

    cnt = read(newSockFile, &statusID,1);
    printf("my StatusID is %d\n", statusID);

    if (statusID & hasTail){
      cnt = read(newSockFile, &memSize, 1);
      printf("My awesome memSize is %d\n", memSize);
      if (cnt < 0){
            awesomeError("error reading memSize from sock");
      }
      buffer = malloc (memSize);
      cnt = read(newSockFile, buffer, memSize);
      printf("This is a word %s\n", buffer);
      if (cnt < 0){
          awesomeError("error reading String from sock");
      }
      /*end senderoo*/
      cnt = write(newSockFile, "Message(s) received", 19);

      if (cnt < 0) {
        awesomeError("ERROR writing to socket");
      }
    } else {
      cnt = write(newSockFile, "0 Message(s) sent", 17);
    }
    if (cnt < 0) {
        awesomeError("ERROR writing to socket");
    }
    free(buffer);

    close(newSockFile);
    close(mySockFile);
    return 0;

  }
