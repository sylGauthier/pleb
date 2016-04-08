#include <stdlib.h>

#include "vector.h"

Vector* vectorNew()
{
    Vector* vec = malloc(sizeof(Vector));

    vec->size = 20;
    vec->count = 0;
    vec->data = malloc(20*sizeof(void*));

    return vec;
}

void vectorPush(Vector* vec, void* elem)
{
    if (vec->count >= vec->size)
    {
        vec->data = realloc(vec->data, (vec->size + 20) * sizeof(void*));
        vec->size += 20;
    }

    vec->data[vec->count] = elem;
    vec->count ++;
}

void vectorMap(Vector vec, void (*mapfun)(void* elem, void* dataIn), void* data)
{
    int i = 0;

    for (i = 0; i < vec.count; i++)
    {
        mapfun(vec.data[i], data);
    }
}

void* vectorAt(Vector* v, int i)
{
    if (v)
    {
        if (i >= 0 && i < v->count)
            return v->data[i];
        else
            return NULL;
    }
}

void vectorFree(Vector* vec)
{
    free(vec->data);
    free(vec);
}
