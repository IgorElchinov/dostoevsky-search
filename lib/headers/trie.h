#ifndef TRIE_H
#define TRIE_H

#include "vector.h"

enum {
    MAX_NEXT_NUM = 65,
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

// the set of valid characters can be changed in trie.c
extern int char_to_ind(char c);                              // converts char to index

extern Trie t_init();                                        // initializes a trie
extern void t_free(Trie *trie);                              // frees a trie
extern void t_add(Trie *trie, char *word, Vector docs);      // assigns whole vector to word
extern void t_push_back(Trie *trie, char *word, int doc);    // adds one document to the vector assigned for word
extern Vector t_get(Trie *trie, char *word);                 // returns a copy if a vector or empty vector if the word isn't found
extern Vector *t_get_ptr(Trie *trie, char *word);            // returns a pointer to the vector or NULL if the word isn't found

#endif