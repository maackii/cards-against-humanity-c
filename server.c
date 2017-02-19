//#include "read_text.h"
//#include <string.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <sys/socket.h>
//#include <errno.h>
//#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "players.h"
#include "cardpiles.h"
#include "connectivity.h"

#define MAX_STR_LEN 512


enum stateMachine{
    newRound,
    receiveReplies,
    waitWinner
};

int checkStatus(int players, int ctrl_, player_t** head);
int sendCtrl(int CTRL_, player_t* head);
int resetStatus(player_t** head, int ctrl_);

int main(int argc, char* argv[]){
    int mySockFile, newConnect, portNumbr;
    player_t* headPlayer = NULL, *updatePlayer = NULL;
    pile_t* whiteCards = NULL, *whiteDiscard = NULL, *blackCards = NULL, *blackDiscard = NULL;


    //------------------------------------------------------REMOVE AFTER TESTING
    pERROR("I am a test %s", "message");
    pINFO("I am a test %s", "message");
    pDEBUG("I am a test %s", "message");

    gameState_t game = {0, 2, 0, 0, 0, 0, NULL, 0, NULL };
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
    printf("HOHOHO\n");
    printf("Game winner ID = %d\n", game.winner);

    switch(game.currentState){

        case newRound :
            pINFO("Server state %s", "newRound");
            game.round++;
            updateHandcards(&headPlayer, &whiteCards, &whiteDiscard);
            updateQuestion(&game, &blackCards, &blackDiscard);
            if (game.round > 1){
                updatePoints(&headPlayer, game.winner);
                updateRole(&headPlayer);
                updateLeader(headPlayer, &game);
            }
            updatePlayer = headPlayer;
            while (updatePlayer!= NULL){
                if(updatePlayer->handCards < MAXHANDCARDS){
                    sendDataPackage(updatePlayer->socketID, D_TYPE_HANDCARDS, 0, (MAXHANDCARDS - updatePlayer->handCards), updatePlayer->cardText );
                    updatePlayer->handCards = MAXHANDCARDS;
                }
                sendDataPackage(updatePlayer->socketID, D_TYPE_QUESTION, game.numbExpectedAnswers, 1, &(game.question));
                sendDataPackage(updatePlayer->socketID, D_TYPE_POINTS, updatePlayer->points, 0, NULL);
                sendDataPackage(updatePlayer->socketID, D_TYPE_ROLE, updatePlayer->role, 0, NULL);
                updatePlayer = updatePlayer->nextPlayer;
            }

            if (SUCCESS == checkStatus(game.numbPlayers, C_TYPE_OK, &headPlayer)){
                sendCtrl(C_TYPE_NEW_ROUND, headPlayer);
                resetStatus(&headPlayer, C_TYPE_RESET);
                game.currentState = receiveReplies;
            }
            break;

        case receiveReplies:

            break;

        default :
            printf("Invalid State\n" );
    }

    //(&whiteCards, &whiteDiscard, &blackCards, &blackDiscard, mySockFile, &headPlayer, &game);
    return 0;
}

int checkStatus(int players, int ctrl_, player_t** head){
    int count = 0;
    uint8_t check = 0;
    player_t* current = NULL;
    while (count < players){
        current = *head;
        while (current!=NULL){
            check = getStatus(current->socketID);
            if (check == MSG_CTRL){
                if(SUCCESS == getIntPackage(current->socketID, &check)){
                    if ( (check == ctrl_) && ((current->status) != ctrl_)){
                        count++;
                        current->status = C_TYPE_OK;
                    }
                }
            }
            current = current->nextPlayer;
        }
    }
    return SUCCESS;
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

int resetStatus(player_t** head, int ctrl_){
    player_t *current = NULL;
    current = *head;
    while (current != NULL) {
        current->status = ctrl_;
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
