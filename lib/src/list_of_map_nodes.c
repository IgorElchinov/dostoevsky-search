#include "list_of_map_nodes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ListOfMapNodes *
lmn_init(char *key, char *value) {
    ListOfMapNodes *list = calloc(1, sizeof(*list));
    list->data.key = strdup(key);
    list->data.value = strdup(value);
    if (!list->data.key || !list->data.value) {
        fprintf(stderr, "%s: memory allocation problem\n", __func__);
        exit(1);
    }    
    list->next = NULL;
    return list;
}

void
lmn_free(ListOfMapNodes *list) {
    while (list != NULL) {
        ListOfMapNodes *next = list->next;
        if (list->data.key != NULL) {
            free(list->data.key);
        }
        if (list->data.value != NULL) {
            free(list->data.value);
        }
        free(list);
        list = next;
    }
    return;
}

size_t
lmn_len(ListOfMapNodes *list) {
    if (list == NULL) {
        return 0;
    }
    size_t size = 1;
    while (list->next != NULL) {
        list = list->next;
        ++size;
    }
    return size;
}

ListOfMapNodes *
lmn_push_back(ListOfMapNodes *list, char *key, char *value) {
    if (list == NULL) {
        return lmn_init(key, value);
    }
    ListOfMapNodes *cur = list;
    while (cur->next != NULL) {
        if (strcmp(cur->data.key, key) == 0) {
            return list;
        }
        cur = cur->next;
    }
    cur->next = lmn_init(key, value);
    return list;
}

void
lmn_set(ListOfMapNodes *list, char *key, char *new_val) {
    ListOfMapNodes *cur = list;
    while (cur != NULL) {
        if (strcmp(cur->data.key, key) == 0) {
            if (cur->data.value != NULL) {
                free(cur->data.value);
            }
            cur->data.value = strdup(new_val);
            return;
        }
        cur = cur->next;
    }
    lmn_free(list);
    fprintf(stderr, "%s: %s -- no such key\n", __func__, key);
    exit(1);
    return;
}

char *
lmn_get(ListOfMapNodes *list, char *key) {
    ListOfMapNodes *cur = list;
    while (cur != NULL) {
        if (strcmp(cur->data.key, key) == 0) {
            return cur->data.value;
        }
        cur = cur->next;
    }
    lmn_free(list);
    fprintf(stderr, "%s: %s -- no such key\n", __func__, key);
    exit(1);
    return NULL;
}

ListOfMapNodes *
lmn_erase(ListOfMapNodes *list, char *key) {
    if (list == NULL) {
        lmn_free(list);
        fprintf(stderr, "%s: %s -- no such key\n", __func__, key);
        exit(1);
        return NULL;
    }
    if (strcmp(list->data.key, key) == 0) {
        ListOfMapNodes *cur = list->next;
        if (list->data.key != NULL) {
            free(list->data.key);
        }
        if (list->data.value != NULL) {
            free(list->data.value);
        }
        free(list);
        return cur;
    }
    ListOfMapNodes *cur = list;
    while (cur->next != NULL) {
        if (strcmp(cur->next->data.key, key) == 0) {
	    ListOfMapNodes *new_next = cur->next->next;
            if (cur->next->data.key != NULL) {
                free(cur->next->data.key);
            }
            if (cur->next->data.value != NULL) {
                free(cur->next->data.value);
            }
            free(cur->next);
            cur->next = new_next;
            return list; 
        }
        cur = cur->next;
    }
    lmn_free(list);
    fprintf(stderr, "%s: %s -- no such key\n", __func__, key);
    exit(1);
    return list;        
}

ListOfMapNodes *
lmn_find(ListOfMapNodes *list, char *key) {
    ListOfMapNodes *cur = list;
    while (cur != NULL) {
        if (strcmp(cur->data.key, key) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

