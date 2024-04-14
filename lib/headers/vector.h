#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct Vector {
    int *data;
    size_t size;
    size_t capacity;
} Vector;

extern int v_get(Vector *vector, size_t pos);
extern void v_set(Vector *vector, int elem, size_t pos);
extern void v_update(Vector *vector, int elem, size_t pos);
extern void v_push_back(Vector *vector, int elem);
extern void v_insert(Vector *vector, int elem, size_t pos);
extern void v_erase(Vector *vector, size_t pos);
extern void v_print(Vector *vector);
extern Vector v_init(size_t size);
extern void v_free(Vector *vector);

#endif
