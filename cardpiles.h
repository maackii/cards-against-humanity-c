#ifndef INCLUDE_CARDPILES_H
#define INCLUDE_CARDPILES_H

#define MAX_STR_LEN 512
#define MAX_CARD_WID 23
#define MAX_LINE 10
#define MAX_TEXT_Len 18
#define BLACKBACK "\033[30;107m"        /*questions gaps >  0*/
#define WHITEBACK "\033[97;40m"         /*answers   gaps == 0*/
#define RESET "\033[39;49m"
#define ERROR -1
#define SUCCESS 1
#define TRUE 1
#define FALSE -1

typedef struct card {
    int used;                   /*  if already used =1, if not used yet =0*/
    char* text;                 /*  answer or question text*/
    uint8_t numbChar;
    uint8_t numbGaps;           /*  gaps==0 => white card, >0 => black card*/
    struct card* next;
}t_card;

typedef struct pile{
    int cnt;
    t_card* card;
}t_pile;

/***************************************************PILE MANAGEMENT*/
int createPile(char* fileName, t_pile **pile);
int createEmptyPile(t_pile ***pile);
void freePile(t_pile** pile);

/**************************************************CARD MANAGEMENT*/
void printCards(t_pile* pile);                      /*Remove after testing*/
t_card* drawRandomCard(t_pile** draw, t_pile** discard);
t_card* addCard(t_card *head, char* text);    /*Missing error handling*/
uint8_t gaps(char* str);

/**************************************************PRINT CARD FUNCTIONS*/
void printCard(char* str, int cardColor); /*card color 0 == Weiß, >0 == Schwarz*/


#endif
