// -----------------------------------------------
// File: list.h
// Purpose: Implementation of a linked list, with inner
// var type of void.
// 
// Author: Lochlan Bunn
//         n8509719
//------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>
#include "list.h"


int init_list(linked_list list)
{

    if ((list = (linked_list) malloc(sizeof(struct linked_list_s))) == NULL) 
    {
        printf("Error: malloc for init_list.\n");
        return -1;
    }

    list->count = 0;
    list->first = NULL;
    list->last = NULL;

    return 0;
}

void destroy_list(linked_list list)
{
    while (is_list_empty(list))
    {
        drop_first(list);
    }

    free(list);
}

int is_list_empty(linked_list list)
{
    return list->first == NULL;
}

void *first_var(linked_list list)
{
    return (list->first)->var;
}

void *last_var(linked_list list)
{
    return (list->last)->var;
}

int append_var(linked_list list, void *var)
{
    node append;
    if ((append = (node) malloc(sizeof(struct node_s))) == NULL) 
    {
        printf("Error: malloc for append_var.\n");
        return -1;
    }
    if (append->var = var)
    {
        printf("Error: assign var for append_var.\n");
        return -1;
    }
    append->next = NULL;

    if (is_list_empty)
    {
        list->first = append;
        list->last = append;
        list->count = 1;
    } else
    {
        (list->last)->next = append;
        list->last = append;
        list->count++;
    }

    return 0;
}

void drop_first(linked_list list)
{
    node temp;
    temp = list->first;

    list->first = (list->first)->next;
    list->count--;

    free(temp);
}