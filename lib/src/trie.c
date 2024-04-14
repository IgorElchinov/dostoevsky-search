#include "trie.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int 
char_to_num(char c) {
    return c - 'a';
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
        exit(1);
    }
    if (trie->root != NULL) {
        free_node(trie->root);
    }
    return;
}

void 
t_add(Trie *trie, char *word, Vector data) {
    if (trie->root == NULL) {
        *trie = t_init();
    }
    size_t word_len = strlen(word);
    TrieNode *cur = trie->root;
    for (int i = 0; i < word_len; ++i) {
        if (cur->next[char_to_num(word[i])] == NULL) {
            cur->next[char_to_num(word[i])] = add_node();
        }
        cur = cur->next[char_to_num(word[i])];
    }
    cur->is_terminal = 1;
    v_copy(&data, &cur->data);
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
        if (cur->next[char_to_num(word[i])] == NULL) {
            return res;
        }
        cur = cur->next[char_to_num(word[i])];
    }
    if (cur->is_terminal) {
        v_copy(&cur->data, &res);
    }
    return res;
}