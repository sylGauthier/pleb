#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

Vector* vectorNew()
{
    Vector* vec = malloc(sizeof(Vector));

    vec->size = CHUNK_SIZE;
    vec->count = 0;
    vec->data = malloc(CHUNK_SIZE*sizeof(void*));

    return vec;
}

void vectorPush(Vector* vec, void* elem)
{
    if (vec->count >= vec->size)
    {
        vec->data = realloc(vec->data, (vec->size + CHUNK_SIZE) * sizeof(void*));
        vec->size += CHUNK_SIZE;
    }

    vec->data[vec->count] = elem;
    vec->count ++;
}

void* vectorPop(Vector* vec)
{
    if (vec->count > 0)
    {
        vec->count --;
        return vec->data[vec->count];
    }
    else
    {
        return NULL;
    }
}

void vectorMap(Vector vec, void (*mapfun)(void* elem, void* dataIn), void* data)
{
    int i = 0;

    for (i = 0; i < vec.count; i++)
    {
        mapfun(vec.data[i], data);
    }
}

void vectorShuffle(Vector* v)
{
    int i = 0;

    for (i = v->count - 1; i > 0; i--)
    {
        int j = rand() % (i+1);
        void* tmp = v->data[i];
        v->data[i] = v->data[j];
        v->data[j] = tmp;
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
    else
    {
        return NULL;
    }
}

static void vecFreeClbk(void* elem, void* data)
{
    free(elem);
}

void vectorFlush(Vector* vec)
{
    vectorMap(*vec, vecFreeClbk, NULL);
}

void vectorFree(Vector* vec)
{
    free(vec->data);
    free(vec);
}

static void printIntClbk(void* elem, void* data)
{
    printf("%d\n", *(int*)elem);
}

void printIntVector(Vector* v)
{
    vectorMap(*v, printIntClbk, NULL);
}
