#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

Vector* vector_new() {
    Vector* vec = NULL;
    
    if (!(vec = malloc(sizeof(Vector)))) {
        fprintf(stderr, "Error: Vector: could not allocate memory\n");
        return NULL;
    }

    vec->size = CHUNK_SIZE;
    vec->count = 0;
    if (!(vec->data = malloc(CHUNK_SIZE*sizeof(void*)))) {
        fprintf(stderr, "Error: Vector: could not allocate memory\n");
        free(vec);
        return NULL;
    }

    return vec;
}

void vector_push(Vector* vec, void* elem) {
    if (vec->count >= vec->size) {
        if (!(vec->data = realloc(vec->data, (vec->size + CHUNK_SIZE) * sizeof(void*)))) {
            fprintf(stderr, "Error: Vector: could not reallocate memory\n");
            return;
        }
        vec->size += CHUNK_SIZE;
    }

    vec->data[vec->count] = elem;
    vec->count ++;
}

void* vector_pop(Vector* vec) {
    if (vec->count > 0) {
        vec->count --;
        return vec->data[vec->count];
    } else {
        return NULL;
    }
}

void vector_map(Vector vec, void (*mapfun)(void* elem, void* dataIn), void* data) {
    int i = 0;

    for (i = 0; i < vec.count; i++) {
        mapfun(vec.data[i], data);
    }
}

void* vector_at(Vector* v, int i) {
    if (v) {
        if (i >= 0 && i < v->count)
            return v->data[i];
        else
            return NULL;
    } else {
        return NULL;
    }
}

static void free_clbk(void* elem, void* data) {
    free(elem);
}

void vector_flush(Vector* vec) {
    vector_map(*vec, free_clbk, NULL);
}

void vector_free(Vector* vec) {
    free(vec->data);
    free(vec);
}

static void print_int_clbk(void* elem, void* data) {
    printf("%d\n", *(int*)elem);
}

void print_int_vector(Vector* v) {
    vector_map(*v, print_int_clbk, NULL);
}
