#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "read_text.h"

#define MAX_STR_LEN 512

int main() {
    int i;
    char** arr;
    t_list list1;
    t_list list2;
    t_list draw_list;
    t_list discard_list;
    t_node* random;
    printf("Hello, World!\n");
    list1=create_list("all_quests", list1);
    list2=create_list(NULL,list2);
    /*printf("[main] 01\n"); /*TEST TEST TEST*/
    /*printf("[main] 02\n"); /*TEST TEST TEST*/
    /*for(i=0; i<3;i++)
        /*elem_pileChange(4,&list1, &list2);*/
        /*change_pile(1, &list1, &list2);
    /*printf("[main] 03\n"); /*TEST TEST TEST*/

    /*random=choose_random_card(&list1, &list2);*/
    for(i=0;i<92;i++){
        arr=gimme(5, &list1, &list2);
        if(list1.elem_cnt<3 || list1.elem_cnt>273)
            print_strArray(5,arr);
    }



    /*printf("[%d] (%d gaps) %s", 1,find_elem(1,&list1)->gaps, find_elem(1,&list1)->str); /*TEST TEST TEST*/
    delete_list(&list1);
    return 0;
}