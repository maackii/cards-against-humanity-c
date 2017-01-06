#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/*Bitflags*/
#define hasTail 1
#define MAXHANDCARDS 2

enum result{
    success,
    fail
};

int openSocket(int i, struct hostent *pHostent);
int sendPackage(char* buffer, int socketfd);
void readPackage(char* buffer, int socketfd);
void awesomeError(const char *msg);
void printHandcards(char** cards);
int sendReplies(char** cards, int amount, int socketfd);

int main(int argc, char* argv[]) {

    struct hostent * server;
    int mySocket, portNumbr, check, points;
    char buffer [128];


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
    mySocket = openSocket(portNumbr, server);
    /*Create Handcardarray*/
    char** handCards = malloc((1)* (sizeof(char*)));
    handCards[0] = "Hello";
    handCards[1] = "Pizzza mit World";
    printHandcards(handCards);

    check = sendReplies(handCards, 2, mySocket);
    //check = sendPackage(buffer, mySocket);
    readPackage(buffer, mySocket);


    //free(handCards[0]);
    //free(handCards[1]);

    close(mySocket);

    return 0;
}

void readPackage(char* buffer, int socketfd){
    bzero(buffer,128);
    intmax_t cnt;
    cnt = read(socketfd, buffer,127);
    if (cnt < 0){
        awesomeError("ERROR reading from socket");
    }
    printf("%s\n",buffer);
    return;
}

int sendReplies(char** cards, int amount, int socketfd){
    uint8_t* package = NULL;
    intmax_t cnt;
    int n, packageSize, track = 1;
    int totalstringLen = 0;
    for (n = 0; n < amount; n++) {
        totalstringLen += strlen(cards[n]);
    }
    packageSize = totalstringLen + amount + hasTail;
    package = malloc((packageSize) * sizeof(uint8_t));
    if(package == NULL){
        return fail;
    }
    package[0] = hasTail;
    for (n = 0; n < amount; n++){
        package[track] = (uint8_t) strlen(cards[n]);
        track++;
        memcpy( &(package[track]), cards[n], strlen(cards[n]));
        track += strlen(cards[n]);
    }
    cnt = write(socketfd, package, packageSize);
    free(package);
    return (cnt < 0 ? fail : success);
    //awesomeError("ERROR writing to socket");
}

int sendPackage(char* buffer, int socketfd){
    uint8_t* package = NULL;
    intmax_t cnt = 0;
    //uint8_t buffLen = 0;
    printf("Please enter a Player Name.\n");
    bzero(buffer,128);
    fgets(buffer,127,stdin);
    //buffer[strcspn(buffer, "\n")] = 0;
    buffer[strlen(buffer)-1]= 0;

    if (strlen(buffer) > 0){
        package =  malloc((strlen(buffer))+2 * sizeof(uint8_t));
        package[0] = hasTail;
        package[1] = strlen(buffer);
        memcpy( &(package[2]), buffer, strlen(buffer));

    } else {
        package = malloc(1);
        package[0]=0;
    }

    cnt = write(socketfd, package, (strlen(buffer)+2));
    free(package);
    if (cnt < 0){
        awesomeError("ERROR writing to socket");
        return fail;
    } else {return success;
    }
}
void awesomeError(const char *msg){
    perror(msg);
}
int openSocket(int port, struct hostent* pServer) {

    int mySockFile;
    struct sockaddr_in servAddr;

    mySockFile = socket(AF_INET, SOCK_STREAM, 0);
    if (mySockFile < 0){
        awesomeError("Socket Error!");
    }

    bzero ( (char *) &servAddr, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    bcopy(pServer->h_addr,
          (char *)&servAddr.sin_addr.s_addr,
          pServer->h_length);
    servAddr.sin_port = htons(port);
    if (connect(mySockFile,(struct sockaddr *) &servAddr,sizeof(servAddr)) < 0)
        awesomeError("ERROR connecting");
    return mySockFile;
}
void printHandcards(char** cards){
    int n, len;
    for (n=0; n<MAXHANDCARDS; n++){
        len = strlen(cards[n]);
        printf("Card %d: %s [%d]\n",n+1, cards[n], len);


    }
    return;
}
