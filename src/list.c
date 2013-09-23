// -----------------------------------------------
// File: list.h
// Purpose: Implementation of a linked list, with inner
// data type of void.
// 
// Author: Lochlan Bunn
//         n8509719
//------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>
#include "list.h"


linked_list create_list(void)
{
    linked_list list;
    if ((list = (linked_list) malloc(sizeof(struct linked_list_s))) == NULL) 
    {
        printf("Error: malloc for create_list.\n");
        return NULL;
    }

    list->count = 0;
    list->head = NULL;

    return list;
}

void destroy_list(linked_list list)
{
    while (is_list_empty(list))
    {
        drop_head(list);
    }

    free(list);
}

int is_list_empty(linked_list list)
{
    return list->head == NULL;
}

void *head_data(linked_list list)
{
    return (list->head)->data;
}

int append_data(linked_list list, void *data)
{
    node pending = (node) malloc(sizeof(struct node_s));

    pending->data = data;
    pending->next = list->head;
    list->head = pending;
    list->count++;

    return 0;
}

void drop_head(linked_list list)
{
    node drop;
    drop = list->head;

    list->head = (list->head)->next;
    list->count--;

    free(drop);
}