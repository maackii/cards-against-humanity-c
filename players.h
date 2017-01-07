


#ifndef PLAYERS_H_INCLUDED
#define PLAYERS_H_INCLUDED


#define MAXPLAYERS 3
//#define MAXROUNDS 100
#define MAXHANDCARDS 2

/*Bitflags*/
#define hasTail         1
#define payloadIsString 2
#define payloadIsInt    4
#define ctrlMessage     8

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
    int numbCards;
    int numberAnswers;
    int question;
    int points;
    char* name;
    char** answers;
    struct player* nextPlayer;
}player_t;


/*player list functions*/
player_t* createPlayer(player_t* head);         /*add player to list*/
void printNodes(player_t* head);
void destroyPlayers(player_t* head);




#endif
