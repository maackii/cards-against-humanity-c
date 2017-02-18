#ifndef PLAYERS_H_INCLUDED
#define PLAYERS_H_INCLUDED

#include "cardpiles.h"
#define MAXPLAYERS 3
//#define MAXROUNDS 100
#define MAXHANDCARDS 2
#define MAXREPLIES 3
#define MAXNAME 16

#define ERROR     -1
#define SUCCESS   1

#define CARDCZAR  0
#define PLAYER    1


typedef struct gameState{
  int numbrRounds;
  int numbPlayers;
  int round;
  int winner;             /*winner socketID*/
  int currentState;
  int scoreLeader;
  char* leaderName;
  int numbExpectedAnswers;
  char* question;
}gameState_t;

typedef struct player{
    int socketID;
    int status;
    int role;
    int handCards;
    int points;
    char* name;
    char** cardText;
    char** replies;             /** [0] = question **/
    struct player* nextPlayer;
}player_t;



/****************************************************************player list functions*/
player_t* createPlayer(player_t* head, int socket);
void printNodes(player_t* head);    /******remove after testing*/
void destroyGame(player_t** head, gameState_t* game);
int updateHandcards(player_t** head, pile_t** draw, pile_t** discard);
int updatePoints(player_t** head, int winnerID);
int updateRole(player_t** head);)
int updateLeader(player_t* head, gameState_t* game);


#endif
