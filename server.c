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
#include "cardpiles.h"
#include "connectivity.h"


#define MAX_STR_LEN 512
#define NUMBER_OF_PLAYERS 2

enum stateMachine{
    newRound,
    receiveReplies,
    waitWinner
};

//---------------------------------------moved struct to players.h
/*typedef struct gameState{
  int numbrRounds;
  int numbPlayers;
  int round;
  int winner;
  int currentState;
  int scoreLeader;
  char* leaderName;
  int numbExpectedAnswers;
  char* question;
}gameState_t;*/


int main(int argc, char* argv[]){

    int mySockFile, newConnect, portNumbr;
    player_t* headPlayer = NULL, *curPlayer = NULL;
    pile_t* whiteCards = NULL, *whiteDiscard = NULL, *blackCards = NULL, *blackDiscard = NULL;

    //------------------------------------------------------REMOVE AFTER TESTING
    int check;
    gameState_t game = {0, 0, 0, 0, 0, 0, NULL, 0, NULL };
    createPile("answ.txt", &whiteCards, &whiteDiscard);
    createPile("quest.txt", &blackCards, &blackDiscard);
    card_t* test = NULL;
    char* cards[] = {"hey", "We","Are", "Some", "Cards"};
    printf("Number of Cards: %d\n", whiteCards->cnt);

    /****************************************Start Game and Read from Console*/
    if (argc < 2){
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    portNumbr = atoi(argv[1]);
    mySockFile = createMainSocket(portNumbr);

    /***************************************************Connect & initiate Players array*/
    int n = 0;
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

    //------------------------------------------------------REMOVE AFTER TESTING
    printf("all clients connected\n");
    game.currentState = newRound;
    game.winner = headPlayer->socketID;
    game.round = 2;
    printf("HOHOHO\n");
    printf("Game winner ID = %d\n", game.winner);


    switch(game.currentState){

        case newRound :
          game.round++;
          updateHandcards(&headPlayer, &whiteCards, &whiteDiscard);
          updateQuestion(&game, &blackCards, &blackDiscard);
          if (game.round > 1){
              updatePoints(&headPlayer, game.winner);
              updateRole(&headPlayer);
              updateLeader(headPlayer, &game);
          }
          curPlayer = headPlayer;
          while (curPlayer!= NULL){
              /*if(curPlayer->handCards < MAXHANDCARDS){
                sendDataPackage(curPlayer->socketID, D_TYPE_HANDCARDS, 0, (MAXHANDCARDS - curPlayer->handCards), curPlayer->cardText );
                curPlayer->handCards = MAXHANDCARDS;
              }*/
              sendDataPackage(curPlayer->socketID, D_TYPE_QUESTION, game.numbExpectedAnswers, 1, &(game.question));

              if (game.round > 1){
                sendDataPackage(curPlayer->socketID, D_TYPE_POINTS, curPlayer->points, 0, NULL);
                sendDataPackage(curPlayer->socketID, D_TYPE_ROLE, curPlayer->role, 0, NULL);
              }
        curPlayer = curPlayer->nextPlayer;
        }
        break;
        default :
         printf("Invalid State\n" );
    }

    freePile(&whiteCards);
    freePile(&whiteDiscard);
    closeConnections(headPlayer, mySockFile);
    destroyGame(&headPlayer, &game);
    return 0;
}


/*if (SUCCESS == sendIntPackage(headPlayer->socketID, CTRL_MESSAGE, OK)){
  printf("Hurray\n");
}*/

/*if(SUCCESS == sendDataPackage(headPlayer->socketID, 13, 0, 5, cards)){
  printf("Success!\n");
}*/

/*set to nonblockin - maybe create own function
      int status = fcntl(mySockFile, F_SETFL, fcntl(mySockFile, F_GETFL, 0) | O_NONBLOCK);
      if (status == -1){
      perror("calling fcntl");
      // handle the error.
}*/

/*-----------------------------------------------burn after testing
curPlayer = headPlayer;
for (int j = 0; j < NUMBER_OF_PLAYERS; j++){
  for (int i=0; i < NUMBER_OF_PLAYERS; i++){
  printCard(curPlayer->cardText[i], 1);
  printf("\n");
}
curPlayer = curPlayer->nextPlayer;
}
*/
