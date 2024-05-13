#include <stdio.h>
#include "trie.h"
#include "vector.h"

enum {
    N = 5,
    M = 5,
    new_val = 1000000000,
};

char *words[N] = {"abba", "abaka", "abba-abba", "ab'er", "ab`e-RO"};
Vector vals[N] = {0};
char *add_word = "aaaa";

int
main(void) {
    for (int i = 0; i < N; ++i) {
        vals[i] = v_init(M);
        for (int j = 0; j < M; ++j) {
            v_set(&vals[i], i * M + j, j);
        }
    }

    Trie t = t_init();
    for (int i = 0; i < N; ++i) {
        t_add(&t, words[i], vals[i]);                          // assigns whole vector
    }

    for (int i = 0; i < N; ++i) {
        v_free(&vals[i]);                                      // we can do anything with vals
    }

    for (int i = 0; i < N; ++i) {
        Vector res = {0};
        res = t_get(&t, words[i]);                             // returns a copy of a vector
        printf("Get %s: ", words[i]);
        v_print(&res);
        v_free(&res);
    }
    printf("\n");

    for (int i = 0; i < N; ++i) {
        t_push_back(&t, words[i], new_val);                    // adds one value
    }
    
    printf("After push_back:\n");
    for (int i = 0; i < N; ++i) {
        Vector res = {0};
        res = t_get(&t, words[i]);
        printf("Get %s: ", words[i]);
        v_print(&res);
        v_free(&res);
    }
    printf("\n");

    for (int i = 0; i < N; ++i) {
        Vector *res = t_get_ptr(&t, words[i]);                 // returns a pointer
        printf("Get pointer to %s: %p :\n", words[i], res);
        v_set(res, new_val, M >> 1);                           // if we modify it, original vector will also be modified
        v_print(res);
    }
    printf("\n");

    Vector res = {0};
    res = t_get(&t, add_word);
    printf("Get %s: ", add_word);
    v_print(&res);
    v_free(&res);

    printf("Number of words: %zu\n", t_num_of_words(&t));     // returns number of different words in trie

    t_free(&t); 
    return 0;
}