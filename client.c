#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void awesomeError(const char *msg)
{
    perror(msg);
}

int main(int argc, char* argv[]) {
    struct sockaddr_in servAddr;
    struct hostent * server;

    int mySockFile, portNumbr;
    intmax_t cnt;
    char buffer[100];

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
    mySockFile = socket(AF_INET, SOCK_STREAM, 0);
    if (mySockFile < 0){
        awesomeError("Socket Error!");
    }

    bzero ( (char *) &servAddr, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&servAddr.sin_addr.s_addr,
          server->h_length);
    servAddr.sin_port = htons(portNumbr);
    if (connect(mySockFile,(struct sockaddr *) &servAddr,sizeof(servAddr)) < 0)
        awesomeError("ERROR connecting");

    printf("Please enter the message: ");

    bzero(buffer,100);
    fgets(buffer,99,stdin);
    cnt = write(mySockFile,buffer,strlen(buffer));
    if (cnt < 0)
        awesomeError("ERROR writing to socket");
    bzero(buffer,100);
    cnt = read(mySockFile,buffer,99);
    if (cnt < 0)
        awesomeError("ERROR reading from socket");
    printf("%s\n",buffer);
    close(mySockFile);
    return 0;
}
