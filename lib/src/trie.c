#include "trie.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const static char *__valid_chars = "`,',-,a-z,A-Z,0-9";             // can be changed

int 
char_to_ind(char c) {
    int n = strlen(__valid_chars);
    int cur_ind = 0;
    signed char prev_c = -1, cur_c = -1;
    int range = 0;
    for (int i = 0; i < n; ++i) {
        if (i & 1) {
            if (__valid_chars[i] == '-') {
                range = 1;
            } else {
                range = 0;
            }
        } else {
            prev_c = cur_c;
            cur_c = __valid_chars[i];
            if (range) {
                if (cur_c <= prev_c) {
                    fprintf(stderr, "%s: incorrect valid chars string format\n", __func__);
                    fflush(stderr);
                    exit(1);
                }
                if (prev_c <= c && c <= cur_c) {
                    return cur_ind + c - prev_c - 1;
                }
                cur_ind += cur_c - prev_c;
            } else {
                if (cur_c == c) {
                    return cur_ind;
                }
                cur_ind++;
            }
        }
    }
    fprintf(stderr, "%s: invalid symbol '%c'\n", __func__, c);
    fflush(stderr);
    exit(1);
    return -1;
}

static TrieNode *
add_node() {
    TrieNode *res = calloc(1, sizeof(*res));
    for (size_t i = 0; i < MAX_NEXT_NUM; ++i) {
        res->next[i] = NULL;
    }
    res->is_terminal = 0;
    res->data = v_init(0);
    return res;
}

Trie
t_init() {
    Trie trie;
    trie.root = add_node();
    return trie;
}

static void
free_node(TrieNode *node) {
    if (node == NULL) {
        fprintf(stderr, "%s: trie node ptr is NULL", __func__);
        fflush(stderr);
        exit(1);
    }
    for (int i = 0; i < MAX_NEXT_NUM; ++i) {
        if (node->next[i] != NULL) {
            free_node(node->next[i]);
        }
    }
    v_free(&node->data);
    free(node);
}

void
t_free(Trie *trie) {
    if (trie == NULL) {
        fprintf(stderr, "%s: trie ptr is NULL", __func__);
        fflush(stderr);
        exit(1);
    }
    if (trie->root != NULL) {
        free_node(trie->root);
    }
    return;
}

void 
t_add(Trie *trie, char *word, Vector docs) {
    if (trie->root == NULL) {
        *trie = t_init();
    }
    size_t word_len = strlen(word);
    TrieNode *cur = trie->root;
    for (int i = 0; i < word_len; ++i) {
        if (cur->next[char_to_ind(word[i])] == NULL) {
            cur->next[char_to_ind(word[i])] = add_node();
        }
        cur = cur->next[char_to_ind(word[i])];
    }
    cur->is_terminal = 1;
    v_copy(&docs, &cur->data);
    return;
}

Vector
t_get(Trie *trie, char *word) {
    Vector res = v_init(0);
    if (trie->root == NULL) {
        return res;
    }
    size_t word_len = strlen(word);
    TrieNode *cur = trie->root;
    for (int i = 0; i < word_len; ++i) {
        if (cur->next[char_to_ind(word[i])] == NULL) {
            return res;
        }
        cur = cur->next[char_to_ind(word[i])];
    }
    if (cur->is_terminal) {
        v_copy(&cur->data, &res);
    }
    return res;
}

Vector *
t_get_ptr(Trie *trie, char *word) {
    if (trie->root == NULL) {
        return NULL;
    }
    size_t word_len = strlen(word);
    TrieNode *cur = trie->root;
    for (int i = 0; i < word_len; ++i) {
        if (cur->next[char_to_ind(word[i])] == NULL) {
            return NULL;
        }
        cur = cur->next[char_to_ind(word[i])];
    }
    if (cur->is_terminal) {
        return &cur->data;
    }
    return NULL;
}

void
t_push_back(Trie *trie, char *word, int doc) {
    Vector *v = t_get_ptr(trie, word);
    if (v == NULL) {
        t_add(trie, word, v_init(0));
        v = t_get_ptr(trie, word);
    }
    for (int i = 0; i < v->size; ++i) {
        if (v_get(v, i) == doc) {
            return;
        }
    }
    v_push_back(v, doc);
    return;
}