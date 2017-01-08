/**********************************************************************
Todo:
  - State Machine Functions
  - input Number of clients
  - input number of rounds
  - Make Network conform (put the hton)
  - integrate card Functions
  - do visualisation


  - delete2DStringArray(char** array, int arrayLength)
  - connectSpectator is NOT a state machine state
  - make funktion for sending and receiving an integer as a payload -> for the judge to send who won
  - make clients specify a name as a command line argument, attach it to the player struct and send it to the server right after connecting!
**********************************************************************/

//#include "read_text.h"
#include <stdio.h>
#include <sys/socket.h>
//#include <errno.h>
//#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "players.h"
#include "connectivity.h"



/*Boolean friends*/
//#define TRUE  1
//#define FALSE 0

//void dealCards();

//void sendMessages();
//void updatePlayers();
//void sendPoints();


enum states{
    startRound,             //Deals required amount of cards (differnce to player.amountCards):
    receiveReplies,         //Gets answers from regular players -> Status Flag reply
    sendReplies,            //Sends answers to all with sendMessages function
    waitWinner,
    playerUpdate,
    newRound
}stateMachineStatus;



/*heres the main course*/
int main(int argc, char* argv[]){

    int mySockFile, newConnect, portNumbr;
    char* cards[] = {"hey", "We", "Are", "Some", "Cards"};
    player_t* headPlayer = NULL;
    intmax_t n = 0;
    int status = 0;
    int numbrRounds = 1;
    int numbrMessages = 0;
    //stateMachineStatus = startRound;
    char* note[1] = {"Hello Pizzzz!"};
    /*read from console*/
    if (argc < 2){
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    portNumbr = atoi(argv[1]);
    mySockFile = createMainSocket(portNumbr);

    while (n < 1){
        listen(mySockFile,5);
        //printf("Starting Connection Process.\n");
        newConnect = connectClient(mySockFile);

        if (newConnect>0){
            headPlayer = createPlayer(headPlayer); /*create player bekommt sie socket ID*/
            headPlayer->socketID = newConnect;
            n++;
            //printf("Client [%jd] has val: %d\n", n, headPlayer->socketID);
        }
    }

    //status = getMessages(headPlayer);

    char** recvMessages = NULL;

    //status = sendPackage(headPlayer->socketID,payloadIsString, note, 1);

    //status = getPackage(headPlayer->socketID, &recvMessages, 0);

    //for(int i = 0; i < status; i++){
    //  printf("String %d: %s\n", i +1, recvMessages[i]);
    //}


    //closeConnections(headPlayer, mySockFile);

    //-------------------------------------------------------------------leave out after testing
    //exit(0);

    //destroyPlayers(headPlayer);


    //Awesome SwitchMachine
    while (1==1){                                  //ToDo: make while use stateMachineStatus to know if it should continue

        //ToDo: connectSpectators1,
        switch (stateMachineStatus) {

            case startRound:
                //int dealCards();
                //ToDo: check player struct for how many cards he has and give him some

                status = sendPackage(headPlayer->socketID,payloadIsString, cards, 5);
                if(status == ERROR) exit(-1);
                headPlayer->numbCards = 5;

                //send Questions
                char* testQuestion = "What does _ know about me?";
                status = sendPackage(headPlayer->socketID,payloadIsString, &testQuestion, 1);
                if(status == ERROR) exit(-1);

                stateMachineStatus = receiveReplies;
                break;

            case receiveReplies:              //ToDo: wait for all replies, attach strings to cards, do not forget to set the number of attached cards and how many cards player still has in hand
                if (numbrMessages<(1)){
                    //numbrMessages += getMessages(clientAddrs, answers);
                    //HANDLE MEMORY!
                    for(int i = 0; i < headPlayer->numberAnswers; i++ )free(headPlayer->answers[i]);
                    if(headPlayer->numberAnswers > 0) free(headPlayer->answers);
                    headPlayer->numberAnswers = 0;

                    status = getPackage(headPlayer->socketID, &(headPlayer->answers), 0);
                    for(int i = 0; i < status; i++){
                      headPlayer->numberAnswers++;
                      printf("String %d: %s\n", i +1, headPlayer->answers[i]);
                    }

                    numbrMessages++;

                } else {
                    numbrMessages = 0;
                    stateMachineStatus = sendReplies;
                }
                break;

            case (sendReplies):                  //ToDo: send the answers to the judge (along with playernumbers so he can send those back)
                //sendMessages();
                exit(0);
                stateMachineStatus = waitWinner;
                break;

            case (waitWinner):                   //ToDo: receive answer from judge -> int
                stateMachineStatus = playerUpdate;
                break;

            case (playerUpdate):                //ToDo: send point as integers to players -> player knows if he won (if he got points, he won)
                //updatePlayers();
                //sendPoints();
                stateMachineStatus = newRound;
                break;

            case (newRound):
                //numberOk = 3;
                break;

            default:
                return 0;
                //exit somehow;
        }
    }

    closeConnections(headPlayer, mySockFile);
    //destroyPlayers(headPlayer, note, hasQuestion);
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
