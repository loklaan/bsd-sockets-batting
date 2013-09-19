// -----------------------------------------------
// File: list.h
// Purpose: Implementation of a linked list, with inner
// var type of void.
// 
// Author: Lochlan Bunn
//         n8509719
//------------------------------------------------
//

#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct node_s *node;
typedef struct linked_list_s *linked_list;
struct node_s {
    void *var;
    node next;
};
struct linked_list_s {
    int count;
    node first;
    node last;
};

int init_list(linked_list list);
void destroy_list(linked_list list);

int is_list_empty(linked_list list);
void *first_var(linked_list list);
void *last_var(linked_list list);
int append_var(linked_list list, void *var);
void drop_first(linked_list list);

#endif
