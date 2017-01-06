#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "read_text.h"

#define MAX_STR_LEN 512

uint8_t gap_cnt(char*str){  /*only if any question contains "_" sign*/
    int i;
    uint8_t cnt;
    for (i=0, cnt=0; str[i]; i++)
        cnt += (str[i] == '_');

    return cnt;
}

t_node* add_new_back(t_list *list, char *answer){ /*gibt adresse des letzten nodes zurück*/
    t_node *new_node = malloc(sizeof(t_node));
    t_node *cur;

    /*printf("[add_new_back]: t1\n"); TEST TEST TEST*/

    new_node->used=0;
    new_node->strlen= (uint8_t) strlen(answer);
    /*printf("[add_new_back]: t2\n"); TEST TEST TEST*/
    new_node->str=malloc(strlen(answer)*sizeof(char)+1);
    strcpy(new_node->str,answer);    /*new_node befüllen, initialisieren*/
    /*printf("[add_new_back]: t3\n"); TEST TEST TEST*/
    new_node->gaps=gap_cnt(answer);  /*only works if any question contains "_" sign & none of the answers*/
    new_node->next=NULL;            /*Hier ringstruktur einfügen*/
    list->elem_cnt+=1;

    if(list->first_element==NULL) {
        list->first_element = new_node;
        /*printf("[add_new_back]: t4\n"); TEST TEST TEST*/
        return new_node;
    }
    /*printf("[add_new_back]: t4\n"); /*TEST TEST TEST*/
    for(cur=list->first_element;cur->next!=NULL;cur=cur->next){} /*liste durchlaufen bis ende*/

    /*printf("[add_new_back]: t5\n"); /*TEST TEST TEST*/
    cur->next=new_node;/*einhängen am ende*/
    /*printf("[add_new_back]: t5\n"); TEST TEST TEST*/
    return new_node;
}

t_list create_blank_list (t_list list){
    list.first_element=NULL;
    list.elem_cnt=0;
    return list;
}

t_list create_list(char *fn, t_list list) { /*initialisiere fn mit NULL für blank list*/
    FILE *fp;
    int test = 0;
    char *fget, *buffer = malloc(MAX_STR_LEN * sizeof(char));    /*zwischenspeicher*/

    if (fn != NULL) {
        list.elem_cnt = 0;
        list.first_element = NULL;
        /*printf("[create_list]: fn= '%s' \n", fn); /*TEST TEST TEST*/
        fp = fopen(fn, "r");
        if (fp == NULL) {
            perror("Error opening file");
            return list;
        }
        while ((fget = fgets(buffer, MAX_STR_LEN, fp)) != NULL) {        /*FILE durchlaufen, nodes erstellen*/

            /*printf("element_cnt: %d\n", list.elem_cnt); TEST TEST TEST*/
            /*printf("[create_list_while_if] test-no.: %d\n",test); TEST TEST TEST*/
            add_new_back(&list, buffer);
            /*printf("element cnt = %d\n", list.elem_cnt); TEST TEST TEST*/
            /*printf("[create_list_while_if_add] test-no.: %d\nbuffer: %p\nbuffer: %s\n",test++, buffer, buffer); TEST TEST TEST*/
        }
        /*printf("[create_list] test-no.: %d\n",test); TEST TEST TEST*/

        fclose(fp);
    } else {
        list=create_blank_list(list);
    }
    return list;
}

t_node * find_elem(int n, t_list *list){ /*gibt adresse der n-ten node in list zurück*/
    t_node* cur=NULL;
    t_node* buf=NULL;
    int cnt=0;

    for(cur=list->first_element;cnt<n;cur=cur->next, cnt++){
        buf=cur;
        if(cur->next==NULL){
            break;
        }
    }

    return buf;
}

void add(t_list *list, t_node *node){
    t_node*p=list->first_element;

    list->elem_cnt++;
    list->first_element=node;
    node->next=p;
}

t_node* remove_node(t_list *list, t_node* node){ /*hängt "node" aus "list" aus, gibt Adresse der ausgehängten node zurück*/
    t_node* buf;
    t_node* pre;

    if(list->first_element==node){ /*sonderfall: das erste Element soll ausgehängt werden */
        list->first_element=node->next;
        list->elem_cnt--;
        return node;
    }else{                   /*normalfall: zweites oder höheres element wird ausgehängt*/
        for(pre=list->first_element; pre->next!=node && pre->next!=NULL;pre=pre->next){}

        if(pre->next==NULL){
            printf("Reached End of list without findigen n-th element");
            return NULL;
        }else{
            pre->next=node->next;
            list->elem_cnt--;
            return node;
        }

    }
}

void elem_pileChange(int n, t_list* list1, t_list *list2){ /*n-te element aus list 1 aus- und in list 2 einhängen*/
    t_node* buf;
    t_node* pre;

    /*printf("[pileC] 00\n"); /*TEST TEST TEST*/

    if(list1->elem_cnt==0){
        printf("no elements left to switch... switching piles...");
        return;
    }
    /*printf("[pileC] 01\n"); /*TEST TEST TEST*/
    if(list1->elem_cnt==1){
        /*printf("[pileC] 02\n"); /*TEST TEST TEST*/
        buf=list1->first_element;
        /*printf("[pileC] 03\n"); /*TEST TEST TEST*/
        add_new_back(list2, buf->str);
        /*printf("[pileC] 04\n"); /*TEST TEST TEST*/
        list1->first_element=NULL;
        /*printf("[pileC] 05\n"); /*TEST TEST TEST*/
        list1->elem_cnt--;
        return;
    }

    buf=find_elem(n,list1);
    /*printf("\n1[elem_pileC] %s", buf->str); /*TEST TEST TEST*/
    if(buf!=list1->first_element) { /*Wenn buff nicht das erste Element in list 1 ist*/
        pre = find_elem(n - 1, list1);
        /*printf("2[elem_pileC] %s", buf->str); /*TEST TEST TEST*/
        pre->next = buf->next;
        /*printf("3[elem_pileC] %s", buf->str); /*TEST TEST TEST*/
        /*printf("4[elem_pileC] list 2 - firstEl=%p, elemCnt=%d\n", list2->first_element, list2->elem_cnt); /*TEST TEST TEST*/
    }else{ /*wenn buff das erste element in list 1 ist*/
        list1->first_element=list1->first_element->next;
    }
    add_new_back(list2, buf->str); /*einhängen in list2*/
        /*printf("5[elem_pileC]%s\n", list2->first_element->str); /*TEST TEST TEST*/
        list1->elem_cnt--;

    return;
} /*nicht in benutzung*/

void change_pile(int n, t_list*list1, t_list *list2){ /*hänge aus list1 aus und in list2 ein*/
    t_list buf;
    add(list2,remove_node(list1, find_elem(n, list1)));

    if(list1->elem_cnt<3 || list1->elem_cnt>273)
        printf("changepile: elementcnt l1= %d, l2= %d\n", list1->elem_cnt, list2->elem_cnt);

    if(list1->first_element==NULL){
        /*printf("if im changepile \n");*/
        buf = *list1;
        *list1 = *list2;
        *list2 = buf;
    }
    return;
}

void print_list(t_list list){
    t_node *cur;
    int cnt;
    /*printf("[print_list]: t1\n"); TEST TEST TEST*/
    for(cnt=1,cur=list.first_element;cur!=NULL;cur=cur->next,cnt++)
        printf("[%d] (%d gaps) %s", cnt,cur->gaps, cur->str);
    printf("element cnt = %d\n", list.elem_cnt); /*TEST TEST TEST*/

}

t_list *delete_front(t_list *list){
    t_node * p;

    if(list==NULL)
        return list;
    p=list->first_element->next; /*ringstruktur: if(list->next==list)*/
    free(list->first_element);
    list->first_element=p;
    return list;
}

void delete_list(t_list* list){ /*ohne Rückgabewert?*/

    while(list->first_element!=NULL){
        list=delete_front(list);
    }
    return;
}

t_node * choose_random_card(t_list* draw_pile, t_list* discard_pile){
    t_node *node;
    int n;

    srand(clock());
    n = rand();
    /*printf("random = %d\n", n); /*TEST TEST TEST*/
    n = (n%(draw_pile->elem_cnt))+1;
    /*printf("random modulo = %d\n", n); /*TEST TEST TEST*/
    node = find_elem(n,draw_pile);
    change_pile(n, draw_pile, discard_pile);

    return node;
}

char** gimme(int needed_cards, t_list* draw, t_list* discard){
    int i;
    t_node* node;

    char** stringlist= malloc(needed_cards*sizeof(char*));
    if(stringlist==NULL){
        perror("malloc failure in 'gimme'");
        return NULL;
    }

    for(i=0;i<needed_cards;i++){
        node=choose_random_card(draw,discard);
        stringlist[i]=malloc((strlen(node->str)+1)*sizeof(char)); /*strlen plus platz für null terminator*/
        strcpy(stringlist[i], node->str);
    }
    return stringlist;
}

void print_strArray(int needed_cards, char** arr){
    int i;

    for(i=0;i<needed_cards;i++){
        printf("[%d] %s", i, arr[i]);
    }
}
