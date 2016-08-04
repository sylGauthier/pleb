#ifndef LIST_H
#define LIST_H

struct ListItem;

typedef struct ListItem* List;

void listPush(List *l, void* elem);
void* listPop(List *l);
int listSize(List l);

//Fills l2 with a reverted copy of l1
//Warning; copies the pointers in the list but not the memory they point to !!!
void listCopy(List l1, List* l2);

//Calls a free() on all elem of the list, assuming they are pointers to allocated memory
void listFlush(List l);
//Frees memory, if the elem are pointers to allocated memory, it WILL be lost.
void listFree(List* l);

void listMap(List l, void (*mapfun)(void* elem, void* data), void* dataIn); 
int listIsIn(List l, void* elem, int (*equalFunction)(void*, void*)); 


void printIntList(List l);

#endif
