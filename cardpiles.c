#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "cardpiles.h"

/*int main(int argc, char* argv[]){

    pile_t* whiteCards = NULL;
    pile_t* whiteDiscard = NULL;
    whiteDiscard = malloc(sizeof(pile_t));


    card_t* test = NULL;
    char* fileName = "answ.txt";

    createPile(fileName, &whiteCards);
    printf("Number of Cards: %d\n", whiteCards->cnt);

    printCards(whiteCards);

    while(whiteCards->card != NULL) {

        test = drawRandomCard(&whiteCards, &whiteDiscard);

        printCard(test->text, test->numbGaps);

        //printf("I am left: %s\n", test->text);
        //printf("%s", whiteCards->card->text);
        //printf("WhiteCardStack:\n");
        //printCards(whiteCards);
        //printf("WhiteDiscardStack:\n");
        //printCards(whiteDiscard);
    }
    freePile(&whiteCards);
    freePile(&whiteDiscard);
    return 0;
}*/

/***************************************************PILE MANAGEMENT*/
int createPile(char* fileName, pile_t **draw, pile_t ** discard){
    FILE* sourceFile = NULL;
    int cnt;
    char buffer [MAX_STR_LEN];

    if (createEmptyPile(&draw) == ERROR){
        return ERROR;
    }
    if (createEmptyPile(&discard) == ERROR){
        return ERROR;
    }
    sourceFile = fopen(fileName, "r");
    if (sourceFile == NULL){
        perror ("Error opening file");
        return ERROR;
    } else {
        while (fgets(buffer, MAX_STR_LEN, sourceFile) != NULL) {
            cnt = strlen(buffer)-2;                                     /*This is weird*/
            buffer[cnt] = '\0';
            (*draw)->card = addCard((*draw)->card, buffer);
            (*draw)->cnt++;
        }
        fclose(sourceFile);
    }
    return SUCCESS;
}
int createEmptyPile(pile_t ***pile){
    **pile = malloc(sizeof(pile_t));
    if (*pile == NULL){
        return ERROR;
    }
    (**pile)->cnt = 0;
    (**pile)->card = NULL;

    return SUCCESS;
}
void freePile(pile_t** pile){
    card_t* temp = NULL;
    card_t* head = NULL;
    if (pile == NULL){
        return;
    }
    head = (*pile)->card;
    while (head != NULL){
        temp = head->next;
        free(head->text);
        free(head->next);
        head = temp;
    }
    free(*pile);
    return;
}
/**************************************************CARD MANAGEMENT*/
void printCards(pile_t* pile){
    card_t* curr = pile->card;

    while (curr != NULL) {
        printf("%s\n", curr->text);
        curr = curr->next;
    }
    return;
}
card_t* drawRandomCard(pile_t** draw, pile_t** discard) {
    card_t* head = NULL;
    card_t* temp = NULL;
    int n;
    srand(clock());
    n = rand();
    n = (n % ((*draw)->cnt));
    printf("Random? %d\n", n);                                  /*****************test*/

    head = (*draw)->card;

    /*Pop from CardPile*/
    if (n == 0 || head->next == NULL) {
        (*draw)->card = head->next;
    } else {
        for (int i = 0; i < n && head->next != NULL; i++) {
            temp = head;
            head = head->next;
        }
        temp->next = head->next;
    }

    /*Add to DiscardPile*/
    head->next = (*discard)->card;
    (*discard)->card= head;

    //printf("%s\n", rdmCard->text);                            /*****************test*/

    (*discard)->cnt++;
    (*draw)->cnt--;

  return head;
}
card_t *addCard(card_t *head, char *text) {
    card_t* newCard = NULL;
    newCard = malloc (sizeof(card_t));

    if (newCard == NULL ){
        perror("Error creating new Card");
        return NULL;
    }
    newCard->text = NULL;
    newCard->next = NULL;
    newCard->used = 0;
    newCard->numbChar = (uint8_t) strlen(text);
    newCard->text = malloc((strlen(text)) * sizeof(char));
    if (newCard->text == NULL){
        perror("Error adding text");
        free(newCard);
        return NULL;
    }
    strcpy(newCard->text, text);
    newCard->numbGaps = gaps(text);
    if (head == NULL){
        head = newCard;
        return head;
    }

    newCard->next = head;
    return newCard;
}
uint8_t gaps(char* str){
    uint8_t cnt = 0;
    for (int i = 0; i < (MAX_STR_LEN+1); i++)
    {cnt += (str[i] == '_');
    }
    return cnt;
}
/**************************************************PRINT CARD FUNCTIONS*/
void printCard(char* originalString, int card_type) {          /* 0 = Answers = WHITEBACK*/
    char* str = malloc(strlen(originalString)*sizeof(char));
    char* word;
    char* scheme;
    int cnt = 0, currLen, oldLen;
    scheme = (card_type == 0 ? WHITEBACK : BLACKBACK);
    strcpy(str, originalString);

    printf("%s", scheme);
    for (int j = 0; j < (MAX_CARD_WID); j++) {
        printf(j%2 == 0 ? "*" : " " );
    }
    printf("%s\n", RESET);
    word = strtok(str, " ");
    int start = TRUE;
    currLen = 0;

    while (cnt< MAX_LINE) {
        if (start == TRUE) {
            printf("%s* ", scheme);
            cnt++;
        }
        if (word != NULL) {
            oldLen = currLen;
            currLen += (strlen(word) + 1);
            if (currLen <= MAX_TEXT_Len) {
                printf("%s ", word);
                word = strtok (NULL, " ");
                start = FALSE ;
            } else {
                for (int i = oldLen; i < MAX_CARD_WID - 3; i++) {
                    printf(" ");
                }
                printf("*%s\n", RESET);
                currLen = 0;
                start = TRUE;
            }
        }
        if (word == NULL){
            for (int i = currLen; i < MAX_CARD_WID - 3; i++) {
                printf(" ");
            }
            printf("*%s\n", RESET);
            currLen = 0;
            start = TRUE;

        }
    }
    printf("%s", scheme);
    for (int j = 0; j < (MAX_CARD_WID); j++) {
        printf(j%2 == 0 ? "*" : " " );
    }
    printf("%s\n", RESET);
    free(str);
}
