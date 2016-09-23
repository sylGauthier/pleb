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
void* vectorPop(Vector*);
void vectorMap(Vector vec, void (*mapfun)(void* elem, void* dataIn), void* data);

void vectorShuffle(Vector*);

void* vectorAt(Vector*, int);

void vectorFlush(Vector*);
void vectorFree(Vector*);


void printIntVector(Vector*);

#endif
