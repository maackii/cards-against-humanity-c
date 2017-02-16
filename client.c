#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "players.h"
#include "connectivity.h"



int main(int argc, char* argv[]) {

    struct hostent * server;
    int mySocket, portNumbr, check;
    intmax_t cnt;
    char buffer [128];
    char* messages[2] = {"These are", "test messages"};
    uint8_t instruction = 0, defaultV = 0;
    int type = 0;
    player_t player = {0, -1, 0, 0, 0, 0, NULL, NULL, NULL};
    char** recMessages = NULL;

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
      //awesomeError("Could not create socket!");
      exit(-1);
    }
    printf("I am connected\n");
    if (DATA_MESSAGE == getStatus(player.socketID)){
      defaultV = getStatus(player.socketID);
      cnt = getDataPackage(player.socketID, &recMessages, &instruction);
        printf("Success\n");
        for (int i = 0; i < cnt; i++){
          printf("%s\n", recMessages[i]);
        }

      }


    /*type = getStatus(player.socketID);
    if (type == CTRL_MESSAGE){
      if (SUCCESS == getIntPackage(player.socketID, &instruction)){
        printf("Ctrl Message number %d\n", instruction);
      }
    }*/


    close(player.socketID);

    return 0;
}
