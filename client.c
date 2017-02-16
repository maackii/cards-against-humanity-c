#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "players.h"
#include "connectivity.h"

/*linked list for answers
struct answerCollect{
  int playerID;
  char** answers;
};
*/

void update_status(player_t player){

    uint8_t typeFlag = -1, typeID = -1;
    int numb_messg = 0, i = 0, j = 0;
    char* recMessages[] = {"bli", "bla", "blubb", "blopp", "bleh"};

    printf("received data type message(s):\n");
    typeFlag = getStatus(player.socketID);
    //numb_messg = getDataPackage(player.socketID, &recMessages, &typeID);
    /*for (int i = 0; i < cnt; i++){
        printf("%s\n", recMessages[i]);
    }*/
    typeID = D_TYPE_HANDCARDS;


    switch (typeID){
        case D_TYPE_HANDCARDS:

            for(i = 0; i < numb_messg; i++){
                for(j = 0; j < 5; j++){

                    if(player.cardText[j] == NULL){

                        player.cardText[j] = malloc(strlen(recMessages[i]) * sizeof(char));
                        strcpy(player.cardText[j], recMessages[i]);

                    }

                }
            }

            for (int i = 0; i < 5; i++){
                printf("%s\n", player.cardText[i]);
            }

            break;

        case D_TYPE_NAME:

            break;

        case D_TYPE_POINTS:

            break;

        case D_TYPE_QUESTION:

            break;

        case D_TYPE_ROLE:

            break;

        default:
            printf("no valid typeID");
            break;
    }

}


int main(int argc, char* argv[]) {

    struct hostent * server;
    int mySocket, portNumbr, check;
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



    if (MSG_DATA_MESSAGE == getStatus(player.socketID)){
      typeFlag = getStatus(player.socketID);
      printf("I am type Flag: %d\n", typeFlag);
      cnt = getDataPackage(player.socketID, &recMessages, &typeID);
        printf("Success\n");
        for (int i = 0; i < cnt; i++){
          printf("%s\n", recMessages[i]);
        }

      }


    /*type = getStatus(player.socketID);
    if (type == MSG_CTRL_MESSAGE){
      if (SUCCESS == getIntPackage(player.socketID, &instruction)){
        printf("Ctrl Message number %d\n", instruction);
      }
    }*/


    //----------------------------STATE MACHINE START--------------------------------

    //awesome variables
    int data_type = -1, break_loop = 0;
    uint8_t  ctrl = 111;

    //While loop
    while(break_loop < 20) {

        // Status Update
        data_type = getStatus(player.socketID);

        data_type = MSG_DATA_MESSAGE;  //just for testing


        if(MSG_CTRL_MESSAGE == data_type) {
            getIntPackage(player.socketID, &ctrl);

        }else if(MSG_DATA_MESSAGE == data_type){
            update_status(player);

        }else{
            printf("no new ctrl- or data-messages\n");
        }

        //Switch statement
        switch(ctrl) {


            // Display new Round / New Game prompt / display winner & game status
            case 0:
                            //Check if first round

                            //get hand cards & question from server

                            //Check number of cards

                            //CTRL send ok if number of cards == 5
                break;


            //Choose & send funny answers
            case 2:
                            //display answers and / or question

                            //choose funny answer(s)

                            //send funny answer(s)
                break;


            //Czar choose winner
            case 4:

                            //get all funny answers

                            //display all funny answers to all (shuffled)

                            //Czar chooses winner

                            //Czar sends winner to server
                break;


            default:
                printf("non valid status! ...restart switching...\n");
                break_loop++;
                break;


        }
    }

    //----------------------------STATE MACHINE END--------------------------------






    close(player.socketID);

    return 0;
}
