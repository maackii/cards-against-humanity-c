#ifndef INCLUDE_FOO_H
#define INCLUDE_FOO_H

typedef struct node {
    int used;           /*if already used =1, if not used yet =0*/
    char *str;         /*answer or question text*/
    uint8_t strlen;
    uint8_t gaps;       /*=0 => white card, >0 => black card*/
    struct node *next;
}t_node;

typedef struct list{
    int elem_cnt;
    t_node * first_element;
}t_list;

uint8_t gap_cnt(char*str);
t_node* add_new_back(t_list *list, char *answer);
t_list create_blank_list (t_list list);
t_list create_list(char *fn, t_list list);
t_node * find_elem(int n, t_list *list);
void add(t_list *list, t_node *node);
t_node* remove_node(t_list *list, t_node* node);
void change_pile(int n, t_list*list1, t_list *list2);
void print_list(t_list list);
t_list *delete_front(t_list *list);
void delete_list(t_list* list);
t_node * choose_random_card(t_list* draw_pile, t_list* discard_pile);
char** gimme(int needed_cards, t_list* draw, t_list* discard);
void print_strArray(int needed_cards, char** arr);

#endif