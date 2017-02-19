//#include "read_text.h"
//#include <string.h>
//#include <unistd.h>
#include <fcntl.h>
//#include <sys/socket.h>
//#include <errno.h>
//#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "players.h"
#include "cardpiles.h"
#include "connectivity.h"

enum stateMachine{
    newRound,
    waitOK,
    waitReplies,
    waitWinner
};
/*****************************************************************SERVER FUNCTIONS ADDED*/
int sendCtrl(int CTRL_, player_t* head);
int resetStatus(player_t* head, int ctrl_);
int sendReplies(player_t* head, gameState_t game);
int sendPlayerUpdates(player_t* head, gameState_t game);
/**************************************************************************************/

int main(int argc, char* argv[]){
    int mySockFile, newConnect, portNumbr;
    int count = 0;
    player_t* headPlayer = NULL, *updatePlayer = NULL;
    pile_t* whiteCards = NULL, *whiteDiscard = NULL, *blackCards = NULL, *blackDiscard = NULL;

    //------------------------------------------------------REMOVE AFTER TESTING
    pERROR("I am a test %s", "message");
    pINFO("I am a test %s", "message");
    pDEBUG("I am a test %s", "message");

    gameState_t game = {3, 2, 0, 0, 0, 0, NULL, 0, NULL };
    createPile("answers_bearb.txt", &whiteCards, &whiteDiscard);
    createPile("questions_bearb.txt", &blackCards, &blackDiscard);
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
    while (n < game.numbPlayers){
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
    /********************************************************start GAME ENGINE*/

    //set socket to nonblockin - maybe create own function
    int status = fcntl(mySockFile, F_SETFL, fcntl(mySockFile, F_GETFL, 0) | O_NONBLOCK);
    if (status == -1){
      perror("calling fcntl");
    }

    while (game.round <= game.numbrRounds) {
      uint8_t check;
      player_t *current = NULL;

      switch (game.currentState) {

        case newRound :
            pINFO("Server state %s", "newRound");
            game.round++;
            updateHandcards(&headPlayer, &whiteCards, &whiteDiscard);
            updateQuestion(&game, &blackCards, &blackDiscard);
            if (game.round > 1) {
                updatePoints(&headPlayer, game.winner);
                updateRole(&headPlayer);
                updateLeader(headPlayer, &game);
            }
            sendPlayerUpdates(headPlayer, game);
            game.currentState = waitOK;
            sendCtrl(C_TYPE_NEW_ROUND, headPlayer);
            resetStatus(headPlayer, C_TYPE_RESET);
        break;

        case waitOK :
          current = NULL;
          check = 0;
          pINFO("Server state %s", "waitOK");


          current = headPlayer;
          while (current != NULL) {
              if ( (MSG_CTRL == getStatus(current->socketID)) && ((current->status) != C_TYPE_OK)) {
                if (SUCCESS == getIntPackage(current->socketID, &check)) {
                  if ((check == C_TYPE_OK) ) {
                    count++;
                    current->status = C_TYPE_OK;
                  }
                }
              }
            current = current->nextPlayer;
            }
            if (count == game.numbPlayers){
              game.currentState = waitReplies;
              sendCtrl(C_TYPE_DISPLAY_CARDS, headPlayer);
              resetStatus(headPlayer, C_TYPE_RESET);
              count = 0;
            }
        break;

        case waitReplies:
            current = NULL;
            check = 0;
            pINFO("Server state %s", "receiveReplies");
            current = headPlayer;
            while (current != NULL) {
              if ((MSG_DATA == getStatus(current->socketID)) && ((current->status) != C_TYPE_OK)) {
                  pDEBUG("Server state %s", "receiveReplies 2");
                if (D_TYPE_HANDCARDS == getStatus(current->socketID)) {
                  check = getDataPackage(current->socketID, &(current->replies), &check);
                  pDEBUG("SERVER %s", "Received DataPackage");
                  current->status = C_TYPE_OK;
                  count++;
                }
              }
              current = current->nextPlayer;
            }
            if (count == (game.numbPlayers-1)){
              pDEBUG("SERVER %s", "Reached Condition to change to waitWinner");
              resetStatus(headPlayer, C_TYPE_RESET);
              game.currentState = waitWinner;
              //sendReplies(headPlayer, game);
              //sendCtrl(C_TYPE_DISPLAY_ANSWERS, headPlayer);
              count = 0;
            }
        break;

        case waitWinner:
            current = NULL;
            check = 0;
            pINFO("Server state %s", "waitWinner");
            exit(-1);


        break;


        default :
            printf("Invalid State\n");
        }
    }

    killServer(&whiteCards, &whiteDiscard, &blackCards, &blackDiscard, mySockFile, &headPlayer, &game);
    return 0;
}


int sendCtrl(int ctrl_, player_t* head){
      player_t* current = NULL;
      current = head;
      while (current != NULL){
          sendIntPackage(current->socketID, MSG_CTRL, ctrl_);
          current = current->nextPlayer;
      }
      return SUCCESS;
}

int resetStatus(player_t* head, int ctrl_){
        player_t* current = NULL;
        current = head;
        while (current != NULL) {
            current->status = ctrl_;
            current = current->nextPlayer;
        }
        return SUCCESS;
}



int sendPlayerUpdates(player_t* head, gameState_t game){
  player_t* current = NULL;
  current = head;
  while (current != NULL){
      if(current->handCards < MAXHANDCARDS){
          sendDataPackage(current->socketID, D_TYPE_HANDCARDS, 0, (MAXHANDCARDS - current->handCards), current->cardText );
          pINFO("Server: Sent %s", "handcards");
          current->handCards = MAXHANDCARDS;
      }
      sendDataPackage(current->socketID, D_TYPE_QUESTION, game.numbExpectedAnswers, 1, &(game.question));
      pINFO("Server: Sent %s", "question");
      sendDataPackage(current->socketID, D_TYPE_POINTS, current->points, 0, NULL);
      pINFO("Server: Sent %s", "points");
      sendDataPackage(current->socketID, D_TYPE_ROLE, current->role, 0, NULL);
      pINFO("Server: Sent %s", "role");
      current = current->nextPlayer;
  }
return SUCCESS;
}

/*-----------------------------------------------burn after testing
updatePlayer = headPlayer;
for (int j = 0; j < game.numbPlayers; j++){
  for (int i=0; i < game.numbPlayers; i++){
  printCard(updatePlayer->cardText[i], 1);
  printf("\n");
}
updatePlayer = updatePlayer->nextPlayer;
}
*/
