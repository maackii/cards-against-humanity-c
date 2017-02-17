//#include "read_text.h"
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "players.h"
//#include "piles.h"
#include "connectivity.h"


#define MAX_STR_LEN 512
#define NUMBER_OF_PLAYERS 1
#define MAX_HAND_CARDS 5

enum states{
    newRound,
    receiveReplies,
    waitWinner
}stateMachineStatus;



int main(int argc, char* argv[]){

    int mySockFile, newConnect, portNumbr;


    char* cards[] = {"hey", "We", "Are", "Some", "Cards"};
    player_t* headPlayer = NULL, *curPlayer = NULL, *momPlayer=NULL, *czar = NULL;
    char** question = NULL, **answ = NULL;
    char* buf = malloc(MAX_STR_LEN * sizeof(char));

    int n = 0;
/*
    int status = 0;
    int numbrRounds = 1;
    int numbrMessages = 0;
  */

    //stateMachineStatus = startRound;

    /****************************************************PILES MANAGEMENT*/
    //t_list answ_draw = NULL, answ_dis = NULL, quest_draw = NULL, quest_dis = NULL;


    /****************************************************Start Game and Read from Console*/
    if (argc < 2){
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    portNumbr = atoi(argv[1]);
    mySockFile = createMainSocket(portNumbr);

    /***************************************************Connect & initiate Players array*/
    while (n < NUMBER_OF_PLAYERS){
        listen(mySockFile,5);
        newConnect = connectClient(mySockFile);

        if (newConnect>0){
            headPlayer = createPlayer(headPlayer, newConnect); /*create player bekommt die socket ID*/
            headPlayer->socketID = newConnect;
            n++;
            printf("Client [%d] has val: %d\n", n, headPlayer->socketID);
        }
    }

    printf("all clients connected\n");
    czar = headPlayer;

    if(SUCCESS == sendDataPackage(headPlayer->socketID, 1, 0, 5, cards)){
      printf("Success!\n");
    }

    /*if (SUCCESS == sendIntPackage(headPlayer->socketID, CTRL_MESSAGE, OK)){
      printf("Hurray\n");
    }
    char** recvMessages = NULL;
    //stateMachineStatus stateMachine = newRound;

    /****************************************************StateMachine*/
 /*while (1 == 1){
   switch (stateMachine) {
     case newRound:
     updateAll();
     sendDataPackage();
     ctrl Message: display All smth-
     receiveOK();

      if (cnt == NUMBER_OF_PLAYERS){
      stateMachine = receiveReplies;
      break;
      case receiveReplies:

    }


   }

 }
 */

    closeConnections(headPlayer, mySockFile);
    destroyPlayers(&headPlayer);
    return 0;
}


/*Functions*/




/*set to nonblockin - maybe create own function
      int status = fcntl(mySockFile, F_SETFL, fcntl(mySockFile, F_GETFL, 0) | O_NONBLOCK);
      if (status == -1){
      perror("calling fcntl");
      // handle the error.
}
*/
