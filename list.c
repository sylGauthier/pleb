#include <stdlib.h>
#include <stdio.h>

#include "list.h"

struct ListItem
{
    void* elem;
    struct ListItem* next;
};

void listPush(List* l, void* elem)
{
    List head = malloc(sizeof(struct ListItem));
    head->elem = elem;
    head->next = *l;
    *l = head;
}

void* listPop(List* l)
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

void count(void* elem, void* cpt)
{
    (*(int*) cpt) ++;
}

int listSize(List l)
{
    int cpt = 0;
    listMap(l,count,(void*) &cpt);
    return cpt;
}

static void freeCallback(void* elem, void* data)
{
    free(elem);
}

void listFlush(List l)
{
    listMap(l, freeCallback, NULL);
}

void listFree(List* l)
{
    while (*l)
        listPop(l);
}

void listMap(List l, void (*mapfun)(void*, void*), void* data)
{
    List cursor = l;

    while (cursor)
    {
        mapfun(cursor->elem, data);
        cursor = cursor->next;
    }
}

int listIsIn(List l, void* elem, int (*equalFunction)(void*, void*))
{
    List cursor = l;

    while (cursor)
    {
        if (equalFunction != NULL)
        {
            if (equalFunction(elem, cursor->elem))
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
