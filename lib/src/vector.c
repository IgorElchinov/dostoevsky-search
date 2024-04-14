#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

Vector
v_init(size_t size) {
    Vector vector;
    vector.size = size;
    if (size == 0) {
        size = 1;
    }
    vector.data = calloc(size, sizeof(*vector.data));
    if (vector.data == NULL) {
        fprintf(stderr, "%s: memory allocation error\n", __func__);
        exit(1);
    }
    vector.capacity = size;
    return vector;
}

void
v_free(Vector *vector) {
    if (vector->capacity != 0) {
        free(vector->data);
        vector->size = 0;
        vector->capacity = 0;
    }
    return;
}

void
v_check_pos(Vector *vector, size_t pos, const char func_name[]) {
    if (pos < 0 || pos >= vector->size) {
        fprintf(stderr, "%s: list index is out of range\n", func_name);
        fflush(stderr);
        exit(1);
    }
    return;
}

int
v_get(Vector *vector, size_t pos) {
    v_check_pos(vector, pos, __func__);
    return vector->data[pos];
}

void
v_set(Vector *vector, int elem, size_t pos) {
    v_check_pos(vector, pos, __func__);
    vector->data[pos] = elem;
    return;
}

inline void
v_update(Vector *vector, int elem, size_t pos) {
    v_set(vector, elem, pos);
    return;
}

void
v_push_back(Vector *vector, int elem) {
    if (vector->capacity == 0) {
        vector->capacity = 1;
        vector->size = 1;
        vector->data = calloc(1, sizeof(*vector->data));
        return;
    }
    if (vector->size >= vector->capacity) {
        vector->data = realloc(vector->data, 2 * vector->capacity * sizeof(*vector->data));
        if (vector->data == NULL) {
            fprintf(stderr, "%s: memory allocation error\n", __func__);
            exit(1);
        }
        vector->capacity *= 2;
    }
    vector->data[vector->size] = elem;
    vector->size++;
    return;
}


void
v_insert(Vector *vector, int elem, size_t pos) {
    v_push_back(vector, elem);
    v_check_pos(vector, pos, __func__);
    for (size_t i = vector->size - 1; i > pos; --i) {
        v_set(vector, v_get(vector, i - 1), i);
    }
    v_set(vector, elem, pos);
    return;
}

void
v_erase(Vector *vector, size_t pos) {
    v_check_pos(vector, pos, __func__);
    for (size_t i = pos; i < vector->size - 1; ++i) {
        v_set(vector, v_get(vector, i + 1), i);
    }
    vector->size--;
    return;
}

void
v_print(Vector *vector) {
    for (size_t i = 0; i < vector->size; ++i) {
        printf("%d ", v_get(vector, i));
    }
    printf("\n");
    return;
}

void
v_copy(const Vector *src, Vector *dest) {
    if (src == NULL) {
        fprintf(stderr, "%s: source pointer is NULL\n", __func__);
        exit(1);
    }
    if (dest == NULL) {
        fprintf(stderr, "%s: destination pointer is NULL\n", __func__);
        exit(1);
    }
    if (dest->data != NULL) {
        free(dest->data);
    }
    *dest = v_init(src->capacity);
    dest->size = dest->size;
    memcpy(dest->data, src->data, src->size);
    return;
}
