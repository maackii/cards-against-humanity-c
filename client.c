#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "cards.h"
#include "connectivity.h"



int main(int argc, char* argv[]) {

    struct hostent * server;
    int mySocket, portNumbr, check;
    intmax_t cnt;
    char buffer [128];
    char* messages[2] = {"These are", "test messages"};

    player_t player = {regular, -1, 0, 0, 0, 0, NULL, NULL, NULL};

    if (argc < 3) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(0);
    }
    portNumbr = atoi(argv[1]);
    server = gethostbyname(argv[2]);
    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host");
        exit(0);
    }

    /*I socket my socket*/
    player.socketID = connectToServer(portNumbr, server);
    if(player.socketID < 0){
      awesomeError("Could not create socket!");
      exit(-1);
    }
    /*Senderoo*/
    player.answers = malloc(sizeof(char*) * 5);
    for(int i = 0; i < 5; i++) player.answers[i] = NULL;

    //get cards!
    char** tmpCharArray = NULL;
    int status = getPackage(player.socketID, &(tmpCharArray), 0);
    for(int i = 0; i < status; i++){
      for(int j = 0; j < 5 && tmpCharArray[i] != NULL; j++){
        if(player.answers[j] == NULL){
          player.answers[j] = tmpCharArray[i];
          tmpCharArray[i] = NULL;
          player.numberAnswers++;
          printf("String %d: %s\n", i +1, player.answers[i]);
        }
      }
    }
    free(tmpCharArray);

    //get question
    status = getPackage(player.socketID, &(tmpCharArray), 0);
    if(status > 0)printf("%s\n (press 0-5)\n", tmpCharArray[0]);
    free(tmpCharArray);

    int tmpInt = -1;
    scanf("%d", &tmpInt);

    //send answer
    status = sendPackage(player.socketID, payloadIsString, &(player.answers[tmpInt]), 1);
    if(status < 0)awesomeError("Dangit!\n");
    free(player.answers[tmpInt]);
    player.answers[tmpInt] = NULL;
    player.numberAnswers--;


    close(player.socketID);

    return 0;
}
