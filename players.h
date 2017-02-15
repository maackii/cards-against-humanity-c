#ifndef PLAYERS_H_INCLUDED
#define PLAYERS_H_INCLUDED


#define MAXPLAYERS 3
//#define MAXROUNDS 100
#define MAXHANDCARDS 2



#define ERROR -1
#define SUCCESS 1


enum role{
    czar,
    player
};

typedef struct player{
    int socketID;
    int role;
    int handCards;
    int requestedCards;
    int points;
    char* name;
    char** cardText;                /** [0] = question **/
    struct player* nextPlayer;
}player_t;



/****************************************************************player list functions*/
player_t* createPlayer(player_t* head, int socket);
void printNodes(player_t* head);    /******remove after testing*/
void destroyPlayers(player_t** head);


#endif
