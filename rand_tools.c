#include <stdlib.h>

#include "rand_tools.h"

int randUniform(int a, int b)
{    
    if (a >= b)
        return a;
    else
        return a + rand() % (b - a);
}

List randRoute(int nb)
{
    List l1 = NULL;
    List l2 = NULL;
    int i = 0;

    //First we create a sorted list containing all numbers from 0 to nb
    for (i = 0; i < nb; i++)
    {
        int* newElem = malloc(sizeof(int));

        *newElem = i;
        listPush(&l1, nb);
    }

    //Then we pick a random element in this list and add it on top of the other list while
    //the first list isn't empty.
    for (i = nb; i > 0; i--)
    {
        int j = rand() % i;
        List cur, lst;

        for (; j >= 0; j--)
        {
            if (j == 0)
                lst = cur;
            cur = cur->next;
        }
    }
}
