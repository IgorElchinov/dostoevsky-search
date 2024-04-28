#include "unordered_set.h"
#include <stdio.h>

enum { N = 7 };

char *arr[N] = {"one", "two", "three", "four", "five", "six", "seven"};
char *add = "qwerty123";

void
u_set_print(UnorderedSet h) {
    printf("Printing set:\n");
    for (int i = 0; i < MAX_HASH_TABLE_SIZE; ++i) {
        if (h.arr[i] != NULL) {
            printf("%d: ", i);
            ListStr *cur = h.arr[i];
            while (cur != NULL) {
                printf("%s; ", cur->data);
                cur = cur->next;
            }
            printf("\n");
        }
    }
    return;
}

int
main(void) {
    UnorderedSet set;
    us_init(&set);
    for (int i = 0; i < N; ++i) {
        us_insert(&set, arr[i]);
    }
    for (int i = 0; i < N; ++i) {
        printf("%s: %d\n", arr[i], us_find(&set, arr[i]));
    }
    u_set_print(set);
    printf("%s: %d\n", add, us_find(&set, add));
    
    for (int i = 0; i < N; ++i) {
        us_erase(&set, arr[i]);
        if (i == N / 2) {
            u_set_print(set);
        }
    }
    for (int i = 0; i < N; ++i) {
        printf("%s: %d\n", arr[i], us_find(&set, arr[i]));
    }
    printf("%s: %d\n", add, us_find(&set, add));

    us_free(&set);
    return 0;
}
