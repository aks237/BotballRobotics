/*
vector.h

has code for recreating vectors in c

based off of http://eddmann.com/posts/implementing-a-dynamic-vector-array-in-c/
*/

#ifndef VECTOR_H
#define VECTOR_H

#define VECTOR_INIT_CAPACITY 4

typedef struct vector {
    void **items;
    int capacity;
    int total;
} vector;

void vector_init(vector *);
int vector_total(vector *);
static void vector_resize(vector *, int);
void vector_add(vector *, void *);
void vector_set(vector *, int, int);
void *vector_get(vector *, int);
void vector_delete(vector *, int);
void vector_free(vector *);
int vector_in(vector *, int);

#endif