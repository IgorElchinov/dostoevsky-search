#include <stdio.h>
#include "stack.h"

void
st_print(Stack *st) {
    printf("Printing stack...\n");
    if (st == NULL) {
        printf("Stack is empty\n");
        return;
    }
    int i = 0;
    for (; st != NULL; st = st->next) {
        printf("%d %p->%p : %p\n", i++, st, st->next, st->data);
    }
    return;
}

int
main(void) {
    int n;
    scanf("%d", &n);
    Stack *st = {0};
    int x;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &x);
        st = st_push(st, x);
    }
    st_print(st);
    while (st != NULL) {
        x = st->data;
        printf("Top: %d\n", x);
        st = st_pop(st);
        st_print(st);
    }
    return 0;
}