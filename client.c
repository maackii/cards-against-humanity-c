
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include "players.h"
#include "connectivity.h"
#include "cardpiles.h"

#define HANDCARDS_MAX 5

void create_playerArray(player_t *player, gameState_t game){
    player_t *curPlayer = NULL;
    player_t *head = NULL;
    int i;

    for(i = 0; i < (game.numbPlayers -1); i++){
        head = createPlayer(curPlayer, -1);
        curPlayer = head;
    }
    player->nextPlayer = head;
}

// wählt antworten aus handcards, löscht diese nach auswahl aus handcards und fügt sie zu replies hinzu
void choose_replies(player_t *player, int gaps){
    int i, scan, check_input = 1;

    printf("GAPS: %d\n", gaps);

    gimme_good_lines("", __LINE__);
    if(player->replies == NULL){
        player->replies = malloc(gaps * sizeof(char*));
        for (i = 0; i < gaps; i++){
            player->replies[i] = NULL;
            gimme_good_lines("", __LINE__);
        }
    }

    for (i = 0; i < gaps; i++){

        gimme_good_lines("", __LINE__);

        do {
            printf("Please choose answer for %dst gap. [type 1-%d]\n", i + 1, MAXHANDCARDS);
            scanf("%d", &scan);
            check_input = scan;

            if(check_input < 1 || check_input > MAXHANDCARDS){
                printf("Wrong input. Please choose number between 1 and %d.\n", HANDCARDS_MAX);
            }else if(player->cardText[check_input-1] == NULL){
                printf("Wrong input, answer already used.\n");
            }

        }while(check_input < 1 || check_input > MAXHANDCARDS || player->cardText[check_input -1] == NULL);

        gimme_good_lines("", __LINE__);
        player->replies[i] = malloc(strlen(player->cardText[scan -1]) * sizeof(char));
        strcpy(player->replies[i], player->cardText[scan -1]);
        player->cardText[scan -1] = NULL;
        player->handCards--;

    }
    gimme_good_lines("", __LINE__);
    for (i = 0; i < gaps; i++){
        printf("reply %d: %s\n", i, player->replies[i]);
    }
}

void display_cards(player_t player, gameState_t game){
    int i;

    printCard(game.question, 1);

    for(i = 0; player.role == PLAYER && i < HANDCARDS_MAX; i++){
        if(player.cardText[i] != NULL)
            printCard(player.cardText[i], 0);
    }

}

void display_replies(player_t *player, gameState_t game){
    player_t *curPlayer = NULL;
    int i, j;
    printf("\n*******ALL GIVEN ANSWERS*******\n");
    for(i = 0, curPlayer = player->nextPlayer; curPlayer->replies[0] != NULL; curPlayer = curPlayer->nextPlayer, i++){
        printf("[%d]", i+1);
        for(j = 0; j < gaps(game.question); j++) {
            printf(" %s\n", curPlayer->replies[j]);
        }
    }
}

void chooseSend_bestReply(player_t *player, gameState_t game){
    int scan, check_input, i;
    uint8_t winnerID;
    player_t *curPlayer = NULL;

    printf("Please choose best answer(s) [Type 1-%d]\n", game.numbPlayers);
    scanf("%d", &scan);
    check_input = scan;

    if(check_input < 1 || check_input > game.numbPlayers){
        printf("Wrong input. Please choose number between 1 and %d.\n", game.numbPlayers);
    }

    for(i = 0, curPlayer = player->nextPlayer; i < scan; i++, curPlayer = curPlayer->nextPlayer);
    winnerID = (uint8_t ) curPlayer->socketID;

    sendDataPackage(player->socketID, D_TYPE_WINNER, winnerID, 0, NULL);
}

uint8_t ok_cardNumber(player_t *player){

    if(player->handCards == MAXHANDCARDS){
        printf("number handCards is ok!\n");
        return 1;
    }
    else return 0;
}

void update_status(player_t *player, gameState_t *game){

    uint8_t typeFlag = -1, typeID = -1, getInt = 111;
    int numb_messg = 0, i = 0, j = 0;
    char** recMessages/*[] = {"bli", "bla", "blubb", "blopp", "bleh"}*/;
    player_t *curPlayer = NULL;

    gimme_good_lines("", __LINE__);

    printf("received data type message(s)!\n");
    typeFlag = getStatus(player->socketID);

    gimme_good_lines("", __LINE__);

    switch (typeFlag){

        case D_TYPE_HANDCARDS:

            numb_messg = getDataPackage(player->socketID, &recMessages, &typeID);
            for(i = 0, j = 0; i < numb_messg && j < 5; j++){

                //printf("i = %d, j = %d\n", i, j); //Only testing

                gimme_good_lines("", __LINE__);

                if(player->cardText[j] == NULL){

                    gimme_good_lines("", __LINE__);

                    player->cardText[j] = malloc(strlen(recMessages[i]) * sizeof(char));
                    strcpy(player->cardText[j], recMessages[i]);

                    (player->handCards)++;
                    i++;
                }
            }

            printf("number of handcards: %d\n", player->handCards);
            break;

        case D_TYPE_REPLIES:

            //player->nextPlayer->socketID = 5;
            numb_messg = getDataPackage(player->socketID, &recMessages, &typeID);

            for(curPlayer = player->nextPlayer; curPlayer->nextPlayer != NULL && curPlayer->socketID != typeID; curPlayer = curPlayer->nextPlayer){
                printf("typeID: %d, curPlayer_ID: %2d &nextPlayer: %p\n", typeID, curPlayer->socketID, curPlayer->nextPlayer);
            }
                printf("außen typeID: %d, curPlayer_ID: %2d &nextPlayer: %p\n", typeID, curPlayer->socketID, curPlayer->nextPlayer);


            if(curPlayer->socketID == typeID){
                for(i = 0; i < MAXREPLIES; i++){
                    free(curPlayer->replies[i]);
                    curPlayer->replies[i] = NULL;
                    gimme_good_lines("", __LINE__);
                }
                for(i = 0; i < numb_messg; i++){
                    curPlayer->replies[i] = malloc(strlen(recMessages[i]) * sizeof(char));
                    strcpy(curPlayer->replies[i], recMessages[i]);
                    gimme_good_lines("", __LINE__);
                }
                for(i = 0; i < numb_messg; i++){
                    printf("REPLIES OF PLAYER %d: %s\n",curPlayer->socketID, curPlayer->replies[i]);
                }
            }

            else{
                gimme_good_lines("", __LINE__);
                printf("found no player with ID %d, searching for blank player....", typeID);

                for(curPlayer = player->nextPlayer; curPlayer->nextPlayer != NULL && curPlayer->socketID != -1; curPlayer = curPlayer->nextPlayer){
                    printf("ID = %d\n", curPlayer->socketID);
                }
                    printf("außen ID = %d\n", curPlayer->socketID);

                if(curPlayer->socketID == -1){
                    gimme_good_lines("", __LINE__);
                    printf("found blank player filling in replies....\n");
                    curPlayer->socketID = typeID;

                    for(i = 0; i < numb_messg; i++){
                        curPlayer->replies[i] = malloc(strlen(recMessages[i]) * sizeof(char));
                        strcpy(curPlayer->replies[i], recMessages[i]);
                        gimme_good_lines("", __LINE__);
                    }

                    for(i = 0; i < numb_messg; i++){
                        printf("REPLIES OF PLAYER %d: %s\n",curPlayer->socketID, curPlayer->replies[i]);
                    }

                }else{
                    gimme_good_lines("", __LINE__);
                    perror("error while searching player in player array to save received replies.\n");
                }
            }

            break;

        case D_TYPE_QUESTION:

            numb_messg = getDataPackage(player->socketID, &recMessages, &typeID);
            gimme_good_lines("", __LINE__);
            //free(game->question);
            game->question = malloc(strlen(recMessages[0]) * sizeof(char));
            strcpy(game->question, recMessages[0]);

            printf("[%d gaps] question: %s\n", typeID, game->question);

            break;

        case D_TYPE_NAME:

            numb_messg = getDataPackage(player->socketID, &recMessages, &typeID);
            gimme_good_lines("", __LINE__);
            player->name = malloc(strlen(recMessages[0]) * sizeof(char));
            strcpy(player->name, recMessages[0]);
            printf("Ur [ID: %d] name is '%s'\n", typeID, player->name);

            break;

        case D_TYPE_POINTS:

            getIntPackage(player->socketID, &getInt);

            gimme_good_lines("", __LINE__);
            player->points = getInt;
            printf("U have %d points!\n", player->points);

            break;


        case D_TYPE_ROLE:

            getIntPackage(player->socketID, &getInt);


            gimme_good_lines("", __LINE__);
            player->role = getInt;

            if(player->role == CARDCZAR){
                printf("YOU ARE CARDCZAR!");
            }else{
                printf("YOU ARE REGULAR PLAYER!");
            }

            break;

        default:

            gimme_good_lines("", __LINE__);
            printf("no valid typeID\n");
            break;
    }

}


int main(int argc, char* argv[]) {


//-------------------connect client start---------------------------
    struct hostent * server;
    int mySocket, portNumbr, check, i;
    intmax_t cnt;
    char buffer [128];
    char* messages[2] = {"These are", "test messages"};
    uint8_t instruction = 0, typeFlag = -1, typeID = -1;
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
    //printf("I am connected with the ID %d\n", player.socketID);

//-------------------connect client end---------------------------


//----------------------------STATE MACHINE START--------------------------------

    //awesome variables
    int data_type = -1, break_loop = 0;
    uint8_t  ctrl = 111;
    gameState_t game;

    //initialize Player.cards[x] with NULL
    player.cardText = malloc( 5 * sizeof(char*));
    for(i = 0; i < 5; i++){
        player.cardText[i] = NULL;
    }
    //initialize game & player for prompt testing
    player.points = 3;
    game.leaderName = "Nemo";
    game.winner = 0;
    game.round = 2;
    game.scoreLeader = 3;
    game.numbPlayers = 5;
    game.numbExpectedAnswers = 2;


    create_playerArray(&player, game);

    //While loop
    while(break_loop < 10) {

        // Status Update
        //ctrl = 111;
        data_type = getStatus(player.socketID);

        //data_type = MSG_DATA;  //just for testing

        //NEEDED INFOS: topScore, name of topPlayer, own curPoints, curQuest, roundNumb, numbPlayers


        if(MSG_CTRL == data_type) {

            gimme_good_lines("is ctrl message", __LINE__);
            getIntPackage(player.socketID, &ctrl);

        }else if(MSG_DATA == data_type){

            gimme_good_lines("", __LINE__);
            update_status(&player, &game);

        }else{
            printf("no new ctrl- or data-messages\n");
        }



        break_loop++;
        //Switch statement
        switch(ctrl) {

            // Display new Round / New Game prompt / display winner & game status
            case C_TYPE_NEW_ROUND:
                //CTRL send ok if number of cards == 5
                if(1 == ok_cardNumber(&player)){
                    sendIntPackage(player.socketID, MSG_CTRL, C_TYPE_OK);
                    printf("Sent Cardnumber-OK to Server \n");
                }
                ctrl = 111;
                break;

                //Choose & send funny answers
            case C_TYPE_DISPLAY_CARDS:
                //display answers and / or question
                display_cards(player, game);
                //choose funny answer(s)
                gimme_good_lines("", __LINE__);
                if(player.role == PLAYER) {
                    choose_replies(&player, gaps(game.question));
                    gimme_good_lines("", __LINE__);
                    //send funny answer(s)
                    sendDataPackage(player.socketID, D_TYPE_HANDCARDS, (uint8_t) player.socketID, gaps(game.question), player.replies);
                    printf("sent replies to server..\n");
                }
                ctrl = 111;
                break;


                //Czar choose winner
            case C_TYPE_DISPLAY_ANSWERS:

                pINFO("Clientstate %s", "display_answers");
                //get all funny answers

                //display all funny answers to all (shuffled)
                display_replies(&player, game);
                //Czar chooses winner
                //Czar sends winner to server
                chooseSend_bestReply(&player, game);
                break;

            default:
                printf("...\n");
                break;

        }
    }

//----------------------------STATE MACHINE END--------------------------------

    close(player.socketID);

    return 0;
}
