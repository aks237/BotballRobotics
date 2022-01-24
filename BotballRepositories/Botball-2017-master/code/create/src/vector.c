/*
vector.c

has code for recreating vectors in c

based off of http://eddmann.com/posts/implementing-a-dynamic-vector-array-in-c/
*/

#include <stdlib.h>
#include "vector.h"

void vector_init(vector *v)
{
    v->capacity = VECTOR_INIT_CAPACITY;
    v->total = 0;
    v->items = malloc(sizeof(int) * v->capacity);
}

int vector_total(vector *v)
{
    return v->total;
}

static void vector_resize(vector *v, int capacity)
{
    void **items = realloc(v->items, sizeof(int) * capacity);
    if (items) {
        v->items = items;
        v->capacity = capacity;
    }
}

void vector_add(vector *v, void *item)
{
    if (v->capacity == v->total)
        vector_resize(v, v->capacity * 2);
    v->items[v->total++] = item;
}

void vector_set(vector *v, int index, int item)
{
    if (index >= 0 && index < v->total){
		v->items[index] = item;
    }
	else if (index < 0){
		//for going from back of list
        v->items[v->total + (v->total % (index))] = item;
	}
}

void *vector_get(vector *v, int index)
{
    if (index >= 0 && index < v->total){
        return v->items[index];
    }
    else if (index < 0){
		//for going from back of list
        return v->items[v->total + (v->total % (index))];
    }
    return -1;
}

void vector_delete(vector *v, int index)
{
    if (index < 0 || index >= v->total)
        return;

    v->items[index] = NULL;
    
	int i;
    for (i = 0; i < v->total - 1; i++) {
        v->items[i] = v->items[i + 1];
        v->items[i + 1] = NULL;
    }

    v->total--;

    if (v->total > 0 && v->total == v->capacity / 4)
        vector_resize(v, v->capacity / 2);
}

void vector_free(vector *v)
{
    free(v->items);
}

int vector_in(vector *v, int value){
	int i;
	for (i = 0;i <= v->total; i++){
		if ((int) vector_get(&v, i) == value){
			return 1;
		}
	}
	return 0;
}