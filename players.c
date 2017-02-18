#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "players.h"
#include "cardpiles.h"

/****************************************************************player list functions*/

void printNodes(player_t* head){                          /******remove after testing*/
    player_t* current = head;

    while (current != NULL){
        printf("ID = %d | NAME = %s |POINTS = %d | ROLE = %d\n", current->socketID, current->name, current->points, current->role);
        current = current->nextPlayer;
    }
    return;
}

void destroyGame(player_t** head, gameState_t* game) {
    player_t *current = NULL, *next = NULL;
    current = *head;

    while (current != NULL) {
        if (current->cardText != NULL){
            for(int i = 0; i < MAXHANDCARDS; i++){
                free(current->cardText[i]);
            }
        }
        if (current->replies != NULL) {
            for (int i = 0; i < MAXREPLIES; i++) {
                free(current->replies[i]);
            }
        }
        free(current->cardText);
        free(current->replies);
        free(current->name);
        next = current->nextPlayer;
        free(current);
        current = next;
    }
    free(game->leaderName);
    free(game->question);
}

player_t* createPlayer(player_t* head, int socket) {
    player_t* newPlayer = NULL;
    newPlayer = malloc(sizeof(player_t));
    if (newPlayer == NULL) {
        perror("Couldn't create Player");
        return NULL;
    }
    newPlayer->socketID = socket;
    newPlayer->handCards = 0;
    newPlayer->points = 0;
    newPlayer->name = NULL;
    newPlayer->role = (head == NULL ? CARDCZAR : PLAYER);
    newPlayer->nextPlayer = head;
    newPlayer->cardText = NULL;
    newPlayer->cardText = malloc(MAXHANDCARDS * sizeof(char*));
    for (int i = 0; i < (MAXHANDCARDS); i++){
        newPlayer->cardText[i] = NULL;
    }
    newPlayer->replies = NULL;
    newPlayer->replies = malloc(MAXREPLIES * sizeof(char*));
    for (int i = 0; i < (MAXREPLIES); i++){
        newPlayer->replies[i] = NULL;
    }

    return newPlayer;
}

int updateHandcards(player_t** head, pile_t** draw, pile_t** discard){
    player_t *current = NULL;
    card_t* temp = NULL;
    current = *head;
    while (current != NULL) {
        for(int i = 0; i < (MAXHANDCARDS - current->handCards); i++){
            if(current->cardText[i] != NULL){
                free(current->cardText[i]);
                current->cardText[i] = NULL;
            }
            temp = drawRandomCard(draw, discard);
            if (temp==NULL){
                perror("Couldn't draw card.");
                return ERROR;
            }
            current->cardText[i] = malloc((strlen(temp->text)+1)* sizeof(char));
            if (current->cardText[i]==NULL){
                perror("Couldn't update hand cards.");
                return ERROR;
            }
            strcpy(current->cardText[i], temp->text);
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
        current = current->nextPlayer;
    }
    return (cnt == 1? SUCCESS:ERROR);
}

int updateRole(player_t** head){
    player_t *current = NULL;
    current = *head;
    while (current->role != CARDCZAR){
        current = current->nextPlayer;
    }
    current->role = PLAYER;
    if(current->nextPlayer == NULL){
        (*head)->role = CARDCZAR;
    } else {
        current->nextPlayer->role = CARDCZAR;
    }
    return SUCCESS;
}

int updateLeader(player_t* head, gameState_t* game){
    player_t *current = NULL;
    char buffer[MAXNAME+1];
    int mostPoints = 0;
    current = head;
    while (current != NULL){
        if( mostPoints < current->points){
            strcpy(buffer, current->name);
            mostPoints = current->points;
        }
        current = current->nextPlayer;
    }
    if (game->leaderName != NULL){
        free (game->leaderName);
    }
    game->leaderName = malloc((strlen(buffer)+1)*sizeof(char));
    if (game->leaderName == NULL){
        perror("Couldn't create leaderName");
        return ERROR;
    }
    strcpy(game->leaderName, buffer);
    game->scoreLeader = mostPoints;
    return SUCCESS;
}

int updateQuestion(gameState_t* game, pile_t** draw, pile_t** discard){
    card_t* temp = NULL;
    temp = drawRandomCard(draw, discard);
    if (temp==NULL){
        return ERROR;
    }
    if (game->question != NULL ){
        free(game->question);
        game->question = NULL;
    }
    game->question = malloc((strlen(temp->text)+1)* sizeof(char));
    strcpy(game->question, temp->text);
    game->numbExpectedAnswers = temp->numbGaps;
    return SUCCESS;

}
