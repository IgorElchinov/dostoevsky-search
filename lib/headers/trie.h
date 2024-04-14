#ifndef TRIE_H
#define TRIE_H

#include "vector.h"

enum {
    MAX_NEXT_NUM = 26,
};

typedef struct TrieNode {
    struct TrieNode *next[MAX_NEXT_NUM];
    int is_terminal;
    Vector data;
} TrieNode;

typedef struct Trie
{
    TrieNode *root;
} Trie;

extern int char_to_num(char c);

extern Trie t_init();
extern void t_free();
extern void t_add(Trie *trie, char *word, Vector data);
extern Vector t_get(Trie *trie, char *word);

#endif