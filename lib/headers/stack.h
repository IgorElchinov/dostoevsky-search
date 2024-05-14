#ifndef STACK_H
#define STACK_H

#include "trie.h"

typedef struct Stack {
    struct Stack *next;
    TrieNode *data;
} Stack;

extern Stack *st_push(Stack *stack, TrieNode *data);
extern Stack *st_pop(Stack *stack);
extern void st_free(Stack *stack);

#endif