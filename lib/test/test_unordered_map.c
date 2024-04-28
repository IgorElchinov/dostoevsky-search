#include <stdio.h>
#include "unordered_map.h"

enum { N = 7 };

void 
print_map(UnorderedMap m) {
    printf("Printing map...\n");
    int empty = 1;
    for (int i = 0; i < MAX_MAP_SIZE; ++i) {
        if (m.arr[i] != NULL) {
            empty = 0;
            printf("(%d) ", i);
            for (ListOfMapNodes *cur = m.arr[i]; cur != NULL; cur = cur->next) {
                printf("%s : %s\n", cur->data.key, cur->data.value);
            }
        }
    }
    if (empty) {
        printf("Empty map\n");
    }
    printf("\n");
    return;
}

char *keys[N] = {"one", "two", "three", "four", "five", "six", "seven"};
char *values[N] = {"value one", "value two", "value three",
        "value four", "value five", "value six", "value seven"};
char *add_key = "qwerty123";
char *add_value = "additional value";

int
main(void) {
    UnorderedMap m;
    um_init(&m);
    for (int i = 0; i < N; ++i) {
       um_insert(&m, keys[i], values[i]);
    }
    print_map(m);
    for (int i = N - 1; i >= 0; --i) {
        printf("Find %s: %d\n", keys[i], um_find(&m, keys[i]));
    }
    printf("Find %s: %d\n", add_key, um_find(&m, add_key));
    printf("\n");
//    printf("Erasing non-existing key\n");
    fflush(stdout);
//    um_erase(&m, add_key);
    for (int i = 0; i < N; ++i) {
        printf("Erasing '%s':\n", keys[i]);
        um_erase(&m, keys[i]);
    	print_map(m);
    }
    for (int i = 0; i < N; ++i) {
       um_insert(&m, keys[i], values[i]);
    }
    print_map(m);
    printf("Set '%s' (value: '%s') to '%s':\n", keys[N / 2], um_get(&m, keys[N / 2]), add_value);
    um_set(&m, keys[N / 2], add_value);
    for (int i = 0; i < N; ++i) {
        printf("Get %s: %s\n", keys[i], um_get(&m, keys[i]));
    }
    printf("\nFree map:\n");
    um_free(&m); 
//    um_erase(&m, add_key);
    print_map(m);
    return 0;
}
