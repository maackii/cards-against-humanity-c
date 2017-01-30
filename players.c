#include <stdio.h>
#include <sys/socket.h>
//#include <errno.h>
//#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>




/****************************************************************player list functions*/


void printNodes(player_t* head){                          /******remove after testing*/
    player_t* current = head;
    while (current != NULL){
        printf("ID = %d:\n", current->socketID);
        current = current->nextPlayer;
    }
    return;
}

void destroyPlayers(player_t** head) {
    player_t *current = NULL;
    current = *head;
    player_t *next = NULL;

    while (current != NULL) {
        if (current->cardText != NULL){
            free(current->cardText);
        }
        free(current->name);
        next = current->nextPlayer;
        free(current);
        current = next;
    }
}

player_t* createPlayer(player_t *head, int socket) {
    player_t* newPlayer = NULL;
    newPlayer = malloc(sizeof(player_t));
    if (newPlayer == NULL) {
        return NULL;
    }
    newPlayer->socketID = socket;
    newPlayer->handCards = 0;
    newPlayer->requestedCards = 0;
    newPlayer->points = 0;
    newPlayer->name = NULL;
    newPlayer->cardText = NULL;
    newPlayer->role = (head == NULL ? czar : player);
    newPlayer->nextPlayer = head;
    return newPlayer;
}

