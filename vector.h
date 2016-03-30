#ifndef VECTOR_H
#define VECTOR_H

struct Vector
{
    int size;
    int count;
    void** data;
};

typedef struct Vector Vector;

Vector* vectorNew();
void vectorPush(Vector*, void*);
void vectorMap(Vector vec, void (*mapfun)(void* elem, void* dataIn), void* data);

void* vectorAt(Vector*, int);

void vectorFree(Vector*);

#endif
