#include <stdlib.h>
#include <stdio.h>

#include "list.h"

void list_push(List* l, void* elem)
{
    List head = malloc(sizeof(struct ListItem));
    head->elem = elem;
    head->next = *l;
    *l = head;
}

void* list_pop(List* l)
{
    if (*l)
    {
        void* elem = (*l)->elem;
        List newl = (*l)->next;
        free(*l);
        *l = newl;

        return elem;
    }
    else
    {
        printf("Error: trying to pop empty list\n");
        return NULL;
    }
}

static void count_clbk(void* elem, void* cpt)
{
    (*(int*) cpt) ++;
}

int list_size(List l)
{
    int cpt = 0;
    list_map(l, count_clbk, (void*) &cpt);
    return cpt;
}

static void list_copy_clbk(void* elem, void* data)
{
    List* l = data;
    list_push(l, elem);
}

void list_copy(List l1, List* l2)
{
    list_map(l1, list_copy_clbk, l2);
}

static void free_clbk(void* elem, void* data)
{
    free(elem);
}

void list_flush(List l)
{
    list_map(l, free_clbk, NULL);
}

void list_free(List* l)
{
    while (*l)
        list_pop(l);
}

void list_map(List l, void (*map_fun)(void*, void*), void* data)
{
    List cursor = l;

    while (cursor)
    {
        map_fun(cursor->elem, data);
        cursor = cursor->next;
    }
}

int list_is_in(List l, void* elem, int (*equal_function)(void*, void*))
{
    List cursor = l;

    while (cursor)
    {
        if (equal_function != NULL)
        {
            if (equal_function(elem, cursor->elem))
                return 1;
        }
        else
        {
            if (elem == cursor->elem)
                return 1;
        }
        cursor = cursor->next;
    }

    return 0;
}

static void print_int(void* elem, void* data)
{
    printf("%d\n", *(int*)elem);
}

void print_int_list(List l)
{
    list_map(l, print_int, NULL);
}
