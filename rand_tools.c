#include <stdlib.h>
#include <stdio.h>

#include "rand_tools.h"

int randUniform(int a, int b)
{    
    if (a >= b)
        return a;
    else
        return a + rand() % (b - a);
}

Vector* randRoute(int nb)
{
    Vector* v = vectorNew();
    int i = 0;

    for (i = 0; i < nb; i++)
    {
        int* newElem = malloc(sizeof(int));

        *newElem = i;
        vectorPush(v, newElem);
    }

    vectorShuffle(v);

    return v;
}
