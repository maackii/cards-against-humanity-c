#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "players.h"
#include "connectivity.h"
#include "cardpiles.h"
#include "cardpiles.c"              // ??????????ß      warum .c statt .h    ????????????

#define HANDCARDS_MAX 5

/*linked list for answers
struct answerCollect{
  int playerID;
  char** answers;
};
*/

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
            printf("Please choose answer for %dst gap. [type 1-5]\n", i + 1);
            scanf("%d", &scan);
            check_input = scan;

            if(check_input < 1 || check_input > 5){
                printf("Wrong input. Please choose number between 1 and %d.\n", HANDCARDS_MAX);
            }else if(player->cardText[check_input-1] == NULL){
                printf("Wrong input, answer already used.\n");
            }

        }while(check_input < 1 || check_input > 5 || player->cardText[check_input -1] == NULL);

        gimme_good_lines("", __LINE__);
        player->replies[i] = malloc(strlen(player->cardText[scan -1]) * sizeof(char));
        strcpy(player->replies[i], player->cardText[scan -1]);
        player->cardText[scan -1] = NULL;

    }
    gimme_good_lines("", __LINE__);
    for (i = 0; i < gaps; i++){
        printf("reply %d: %s\n", i, player->replies[i]);
    }
}

void display_cards(player_t player, gameState_t game){
    int i;

    printCard(game.question, 1);

    for(i = 0; player.role != CARDCZAR && i < HANDCARDS_MAX; i++){
        if(player.cardText[i] != NULL)
            printCard(player.cardText[i], 0);
    }

}

uint8_t ok_cardnumber(player_t *player){

    if(player->handCards == HANDCARDS_MAX){
        return C_TYPE_OK;
    }
    else return 0;

}

void update_status(player_t *player, gameState_t *game){

    uint8_t typeFlag = -1, typeID = -1, getInt = 111;
    int numb_messg = 0, i = 0, j = 0;
    char** recMessages/*[] = {"bli", "bla", "blubb", "blopp", "bleh"}*/;


    gimme_good_lines("", __LINE__);

    printf("received data type message(s)!\n");
    typeFlag = getStatus(player->socketID);

    //printf("typeFlag is: %d\n", typeFlag);        //testing

    /*for (int i = 0; i < cnt; i++){
        printf("%s\n", recMessages[i]);
    }*/
    //typeID = D_TYPE_HANDCARDS;

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

            /*for(i = 0; i < 5; i++){
                if(player->cardText[i] != NULL)
                    printCard(player->cardText[i], 0);
            }*/
            printf("number of handcards: %d\n", player->handCards);


            break;

        case D_TYPE_QUESTION:

            numb_messg = getDataPackage(player->socketID, &recMessages, &typeID);
            gimme_good_lines("", __LINE__);
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
            printf("U are role %d\n", player->role);

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
    printf("I am connected\n");

//-------------------connect client end---------------------------

    //---test---test---test---test---test---test---test---test---test---test----
    /*if (MSG_DATA == getStatus(player.socketID)){
      typeFlag = getStatus(player.socketID);
      printf("I am type Flag: %d\n", typeFlag);
      cnt = getDataPackage(player.socketID, &recMessages, &typeID);
        printf("Success\n");
        for (int i = 0; i < cnt; i++){
          printf("%s\n", recMessages[i]);
        }

      }*/


    /*type = getStatus(player.socketID);
    if (type == MSG_CTRL_MESSAGE){
      if (SUCCESS == getIntPackage(player.socketID, &instruction)){
        printf("Ctrl Message number %d\n", instruction);
      }
    }*/

    //---test---test---test---test---test---test---test---test---test---test----


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


    //While loop
    while(break_loop < 5) {

        // Status Update
        ctrl = 111;
        data_type = getStatus(player.socketID);

        //data_type = MSG_DATA;  //just for testing


        if(MSG_CTRL == data_type) {

            gimme_good_lines("is ctrl message\n", __LINE__);
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
                            //Check if first round

                            //Check number of cards

                            //CTRL send ok if number of cards == 5
                break;


            //Choose & send funny answers
            case C_TYPE_DISPLAY_CARDS:
                            //display answers and / or question
                display_cards(player, game);
                            //choose funny answer(s)
                gimme_good_lines("", __LINE__);
                choose_replies(&player, gaps(game.question));
                gimme_good_lines("", __LINE__);
                            //send funny answer(s)
                sendDataPackage(player.socketID, D_TYPE_HANDCARDS, (uint8_t ) player.socketID, gaps(game.question), player.replies);

                break;


            //Czar choose winner
            case C_TYPE_DISPLAY_ANSWERS:
                            //get all funny answers

                            //display all funny answers to all (shuffled)

                            //Czar chooses winner

                            //Czar sends winner to server
                break;

            case C_TYPE_OK:     //sends 1 if number of cards ok, sends 0 if not
                sendIntPackage(player.socketID, MSG_CTRL, ok_cardnumber(&player));
                break;

            default:
                printf("no valid / new status! ...restart switching...\n");
                break;

        }
    }

//----------------------------STATE MACHINE END--------------------------------






    close(player.socketID);

    return 0;
}
