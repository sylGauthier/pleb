#ifndef LIST_H
#define LIST_H

struct ListItem
{
    void* elem;
    struct ListItem* next;
};

typedef struct ListItem* List;

void list_push(List *l, void* elem);
void* list_pop(List *l);
int list_size(List l);

/*Fills l2 with a reverted copy of l1
 *Warning; copies the pointers in the list but not the memory they point to !!!
 */
void list_copy(List l1, List* l2);

/*Calls a free() on all elem of the list, assuming they are pointers to allocated memory*/
void list_flush(List l);
/*Frees memory, if the elem are pointers to allocated memory, it WILL be lost.*/
void list_free(List* l);

void list_map(List l, void (*map_fun)(void* elem, void* data), void* dataIn); 
int list_is_in(List l, void* elem, int (*equal_function)(void*, void*)); 


void print_int_list(List l);

#endif
