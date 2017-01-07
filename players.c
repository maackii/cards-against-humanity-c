#include <stdio.h>
#include <sys/socket.h>
//#include <errno.h>
//#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "cards.h"




player_t* createPlayer(player_t* head) {
    player_t* newPlayer = NULL;
    newPlayer = malloc(sizeof(player_t)); /*make head*/
    if (newPlayer == NULL) {
        return head;
    }
    newPlayer->numbCards = 0;
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
