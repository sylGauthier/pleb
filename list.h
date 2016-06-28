#ifndef LIST_H
#define LIST_H

struct ListItem;

typedef struct ListItem* List;

void listPush(List *l, void* elem);
void* listPop(List *l);
int listSize(List l);

void listFlush(List l);
void listFree(List* l);

void listMap(List l, void (*mapfun)(void* elem, void* data), void* dataIn); 
int listIsIn(List l, void* elem, int (*equalFunction)(void*, void*)); 

#endif
