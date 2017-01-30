#ifndef PLAYERS_H_INCLUDED
#define PLAYERS_H_INCLUDED


#define MAXPLAYERS 3
//#define MAXROUNDS 100
#define MAXHANDCARDS 2

/*Bitflags*/
#define Update          1
#define Collecting      2
#define Judging         4
#define Selection       8
#define ctrlMessage    16

#define ERROR -1
#define SUCCESS 1


enum role{
    czar,
    player
};

ttypedef struct player{
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

