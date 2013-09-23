// -----------------------------------------------
// File: list.h
// Purpose: Implementation of a linked list, with inner
// data type of void.
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
    void *data;
    node next;
};
struct linked_list_s {
    int count;
    node head;
    node tail;
};

linked_list create_list(void);
void destroy_list(linked_list list);

int is_list_empty(linked_list list);
void *head_data(linked_list list);
int append_data(linked_list list, void *data);
void drop_head(linked_list list);

#endif
