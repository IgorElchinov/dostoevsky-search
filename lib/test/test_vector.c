#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "vector.h"

enum { N = 5 };

int
main(void) {
    Vector v1;
    v1.data = malloc(N * sizeof(*v1.data));
    v1.size = 0;
    v1.capacity = N;
    for (int i = 0; i < 2 * N + 1; ++i) {
        v_push_back(&v1, i*i);
    }
    for (int i = 0; i < v1.size; ++i) {
        assert(v1.data[i] == v_get(&v1, i));
    }
    
    assert(v1.capacity == 4 * N);
    assert(v1.size == 2 * N + 1);

    v_erase(&v1, 1);
    v_insert(&v1, N, N / 2);
    assert(v_get(&v1, 0) == 0);
    for (int i = 1; i < N / 2; ++i) {
        assert(v_get(&v1, i) == i * i + 2 * i + 1);
    }
    assert(v_get(&v1, N/2) == N);
    for (int i = N / 2 + 1; i < v1.size; ++i) {
        assert(v_get(&v1, i) == i * i);
    }

    v_update(&v1, 0, N/2);
    assert(v_get(&v1, N/2) == 0); 

    Vector v2 = {0};
    v_copy(&v1, &v2);
    assert(v1.size == v2.size);
    assert(v1.capacity = v2.capacity);
    for (int i = 0; i < v1.size; ++i) {
        assert(v_get(&v1, i) == v_get(&v2, i));
    }

    free(v1.data);
    free(v2.data);

    printf("Ok!\n");
    return 0;
}
