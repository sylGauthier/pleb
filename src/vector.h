#ifndef VECTOR_H
#define VECTOR_H

#define CHUNK_SIZE 200

struct Vector {
    int size;
    int count;
    void** data;
};

typedef struct Vector Vector;

Vector* vector_new();
void vector_push(Vector*, void*);
void* vector_pop(Vector*);
void vector_map(Vector vec, void (*mapfun)(void* elem, void* dataIn), void* data);

void* vector_at(Vector*, int);

void vector_flush(Vector*);
void vector_free(Vector*);


void print_int_vector(Vector*);

#endif
