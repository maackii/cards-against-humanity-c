/**********************************************************************
Todo:
  - State Machine Functions
  - input Number of clients
  - input number of rounds
  - Make Server conform (put the hton)
  - integrate read Functions
  - make Header Files
  - do visualisation
**********************************************************************/


#include <stdio.h>
#include <sys/socket.h>
//#include <errno.h>
//#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#define MAXPLAYERS 3
//#define MAXROUNDS 100
//#define MAXHANDCARDS 5

/*Boolean friends*/
//#define TRUE  1
//#define FALSE 0

/*Bitflags*/
#define hasTail   1
//#define hasReply  2
//#define hasWinner 4
/*player*/


enum result{
    success,
    fail
};

enum role{
    cardCzar,
    regular
};

typedef struct player{
    int role;
    int socketID;
    int amountCards;
    int numberAnswers;
    int points;
    char* name;
    char** answers;
    struct player* nextPlayer;
}player_t;



/*Connection Functions*/
int createMainSocket(int port);                       /*Create main Socket*/
int connectClient(int mainSocket);                    /*Receive Client Addresses*/
void awesomeError(const char *msg);                   /*Personalized error handling*/
void closeConnections(player_t *head, int mainSocket); /*..close all*/

/*Statemachine Functions*/
int getMessages(player_t* current);
char* readLine(int currentSocket, uint8_t memSize);         /*Receive Client Messages*/
//void dealCards();

//void sendMessages();
//void updatePlayers();
//void sendPoints();



/*enum states{
    startRound,               Deals required amount of cards (differnce to player.amountCards)
    receiveReplies,         Gets answers from regular players -> Status Flag reply
    sendReplies,            Sends answers to all with sendMessages function
    waitWinner,
    playerUpdate,
    newRound
  }stateMachineStatus;*

list functions*/
player_t* createPlayer(player_t* head);         /*add player to list*/
void printNodes(player_t* head);
void destroyPlayers(player_t* head);

/*heres the main course*/
int main(int argc, char* argv[]){

    int mySockFile, newConnect, portNumbr;
    player_t* headPlayer = NULL;
    intmax_t n = 0;
    int status = 0;
    int numbrRounds = 1;
    int numbrMessages = 0;
    //stateMachineStatus = startRound;

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

        status = getMessages(headPlayer);
        //printf("%s\n", headPlayer->answers[1]);
        //printNodes(headPlayer);
        closeConnections(headPlayer, mySockFile);
        //destroyPlayers(headPlayer);


    //Awesome SwitchMachine
    /*while (1){

        //connectSpectators1,
        switch (stateMachineStatus) {

            case (connectSpectators1):
                //todo
                break;

            case (startRound):
                //dealCards();
                stateMachineStatus = receiveReplies;
                break;

            case (receiveReplies):
                if (numbrMessages<(MAXPLAYERS-1)){
                    numbrMessages += getMessages(clientAddrs, answers);
                } else {
                    numbrMessages = 0;
                    stateMachineStatus = sendReplies;
                }
                break;

            case (sendReplies):
                //sendMessages();
                stateMachineStatus = waitWinner;
                break;

            case (waitWinner):
                getMessages(clientAddrs, answers);
                stateMachineStatus = playerUpdate;
                break;

            case (playerUpdate):
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
    }*/
    closeConnections(headPlayer, mySockFile);
    destroyPlayers(headPlayer);
    return 0;
}


/*Functions*/

int createMainSocket(int port) {
    int mainSockFile = 0 ;
    struct sockaddr_in servAddr;
    printf("nonblock?\n");
    /*I socket my socket*/
    mainSockFile = socket(AF_INET, SOCK_STREAM, 0);
    if (mainSockFile < 0){
        awesomeError("Socket Error!");
    }
    /*I fill my server address with 0s because that's what peoples on the web do*/
    bzero ( (char *) &servAddr, sizeof(servAddr));
    /*these are all standard code*/
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(port);

    /*I binds my socket*/
    if (bind (mainSockFile, (struct sockaddr *) &servAddr, sizeof(servAddr))<0){
        awesomeError("Binding Error!");
    }
    printf("My socket ID is %d\n", mainSockFile);
    return mainSockFile;
}

int connectClient(int mainSocket){
    int clieLen, incomingConn;
    struct sockaddr_in clieAddr;

    clieLen = sizeof(clieAddr);
    incomingConn = accept(mainSocket, (struct sockaddr*) &clieAddr, (socklen_t *) &clieLen);
    if (incomingConn < 0 ) {
        //awesomeError("Waiting for connection");
        return -1;
    }
    printf("Got incoming Connection with ID: %d\n", incomingConn);  /*test message*/
    return incomingConn;
}

int getMessages(player_t* current){
    int newSockFile, numbMessages;
    uint8_t requestSize;
    uint8_t statusID;
    char* tempBuffer = NULL;
    intmax_t cnt = 0;
    int n;

    newSockFile = current->socketID;
    printf("My ID: %d\n", current->socketID);

    cnt = read(newSockFile, &statusID,1);
    printf("my StatusID is %d\n", statusID);

    //if (statusID & hasTail){

    current->numberAnswers = 2;
    current->answers = malloc((current->numberAnswers) * sizeof(char*));

    for (n = 0; n < 2; n++){

      cnt = read(newSockFile, &requestSize, 1);
      printf("My awesome memSize is %d\n", requestSize);

      if (cnt < 0){
        awesomeError("error reading memSize from sock");
        return fail;
      }

      tempBuffer = readLine(newSockFile, requestSize);

      if (tempBuffer == NULL){
        return fail;
      } else {

        current->answers[n] = tempBuffer;
      }

    }
    return success;
}

char* readLine(int currentSocket, uint8_t memSize){
    char* buffer = NULL;
    intmax_t cnt = 0;
    printf("%d\n", memSize);
    buffer = malloc (memSize+1);
    cnt = read(currentSocket, buffer, memSize);
    buffer[memSize] = 0;
    printf("This is a word %s\n", buffer);
    if (cnt < 0){
        awesomeError("error reading String from sock");
        free(buffer);
        return NULL;
    }
    cnt = write(currentSocket, "Message(s) received", 19);
    if (cnt < 0) {
        awesomeError("ERROR writing to socket");
        cnt = write(currentSocket, "0 Message(s) sent", 17);
    }
    return buffer;
}

void closeConnections(player_t* head, int mainSocket){
    player_t* current = head;
    while (current != NULL) {
        close(current->socketID);
        current = current->nextPlayer;
    }
    close(mainSocket);
    return;
}

void awesomeError(const char *msg) {
    perror(msg);
}

player_t* createPlayer(player_t* head) {
    player_t* newPlayer = NULL;
    newPlayer = malloc(sizeof(player_t)); /*make head*/
    if (newPlayer == NULL) {
        return head;
    }
    newPlayer->amountCards = 0;
    newPlayer->numberAnswers = 0;
    newPlayer->points = 0;
    newPlayer->answers = NULL;
    newPlayer->role = (head == NULL ? cardCzar : regular);
    newPlayer->nextPlayer = head;

    return newPlayer;
}

void printNodes(player_t* head){
    player_t* current = head;
    while (current != NULL){
        printf("Message= %s:\n", current->answers[0]);
        current = current->nextPlayer;
    }
    return;
}

void destroyPlayers(player_t* head) {
    player_t* current = head;
    player_t* next;
    //while (current != NULL) {
        //next = current->nextPlayer;
        //if (current->answers != NULL){
          //free(current->answers);
        //}
        free(current);
        //current = next;
    //}
}



/*set to nonblockin - maybe create own function
      int status = fcntl(mySockFile, F_SETFL, fcntl(mySockFile, F_GETFL, 0) | O_NONBLOCK);
      if (status == -1){
      perror("calling fcntl");
      // handle the error.
}
*/
