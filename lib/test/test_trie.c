#include <stdio.h>
#include "trie.h"
#include "vector.h"

enum {
    N = 5,
    M = 5,
};

char *words[N] = {"abba", "abaka", "abbabba", "aber", "abero"};
Vector vals[N] = {0};

int
main(void) {
    for (int i = 0; i < N; ++i) {
        vals[i] = v_init(M);
        for (int j = 0; j < M; ++j) {
            v_set(&vals[i], i, j);
        }
    }

    Trie t = t_init();
    for (int i = 0; i < N; ++i) {
        t_add(&t, words[i], vals[i]);
    }

    for (int i = 0; i < N; ++i) {
        v_free(&vals[i]);                      // we can do anithing with vals
    }

    for (int i = 0; i < N; ++i) {
        Vector res = {0};
        res = t_get(&t, words[i]);
        printf("Get %s: ", words[i]);
        v_print(&res);
        v_free(&res);
    }
    t_free(&t);
    return 0;
}