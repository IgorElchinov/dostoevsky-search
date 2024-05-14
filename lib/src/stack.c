#include "stack.h"
#include <stdlib.h>

Stack *
st_push(Stack *stack, TrieNode *data) {
    if (stack == NULL) {
        stack = calloc(1, sizeof(*stack));
        stack->next = NULL;
        stack->data = data;
        return stack;
    }
    Stack *new_head = calloc(1, sizeof(*stack));
    new_head->next = stack;
    new_head->data = data;
    return new_head;
}

Stack *
st_pop(Stack *stack) {
    if (stack == NULL) {
        return NULL;
    }
    if (stack->next == NULL) {
        free(stack);
        return NULL;
    }
    Stack *next = stack->next;
    free(stack);
    return next;
}

void 
st_free(Stack *stack) {
    while (stack != NULL) {
        Stack *next = stack->next;
        free(stack);
        stack = next;
    }
    return;
}