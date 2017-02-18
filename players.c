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
#include "cardpiles.h"




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
            for(int i = 0; i < MAXHANDCARDS; i++){
            free(current->cardText[i]);
          }
          free(current->cardText);
        }
        if (current->replies != NULL){
            for(int i = 0; i < MAXREPLIES; i++){
            free(current->replies[i]);
          }
          free(current->replies);
        }
        free(current->name);
        next = current->nextPlayer;
        free(current);
        current = next;
    }
}

player_t* createPlayer(player_t* head, int socket) {
    player_t* newPlayer = NULL;
    newPlayer = malloc(sizeof(player_t));
    if (newPlayer == NULL) {
        return NULL;
    }
    newPlayer->socketID = socket;
    newPlayer->handCards = 0;
    newPlayer->points = 0;
    newPlayer->name = NULL;
    newPlayer->role = (head == NULL ? czar : player);
    newPlayer->nextPlayer = head;
    newPlayer->cardText = NULL;
    newPlayer->cardText = malloc(MAXHANDCARDS * sizeof(char*));
      for (int i = 0; i < (MAXHANDCARDS); i++){
        newPlayer->cardText[i] = NULL;
    }
    newPlayer->replies = NULL;
    newPlayer->replies = malloc(MAXREPLIES * sizeof(char*));
      for (int i = 0; i < (MAXREPLIES); i++){
        newPlayer->cardText[i] = NULL;
    }

    return newPlayer;
}

int updateHandcards(player_t** head, t_pile** draw, t_pile** discard){
  player_t *current = NULL;
  t_card* temp = NULL;
  current = *head;
  while (current != NULL) {
      for(int i = 0; i < (MAXHANDCARDS - current->handCards); i++){
        if(current->cardText[i] == NULL){
          temp = drawRandomCard(draw, discard);
          if (temp==NULL){
            return ERROR;
          }
          current->cardText[i] = malloc((strlen(temp->text)+1)* sizeof(char));
          strcpy(current->cardText[i], temp->text);
        }
      }

    current = current->nextPlayer;
  }

  return SUCCESS;
}

int updatePoints(player_t** head, int winnerID){
  player_t *current = NULL;
  int cnt = 0;
  current = *head;
  while (current != NULL) {
      if (current->socketID == winnerID){
        current->points++;
        cnt++;
      }
      current= current->nextPlayer;
  }
  return (cnt == 1? SUCCESS:ERROR);
}
