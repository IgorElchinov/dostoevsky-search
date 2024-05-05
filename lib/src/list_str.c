#include "list_str.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <string.h>

ListStr *
ls_init(char *val) {
    ListStr *list = calloc(1, sizeof(*list));
    list->data = strdup(val);
    if (list->data == NULL) {
        fprintf(stderr, "%s: memory allocation problem\n", __func__);
        exit(1);
    }
    list->next = NULL;
    return list;
}

void
ls_free(ListStr *list) {
    if (list == NULL) {
        return;
    }
    while (list != NULL) {
        ListStr *next = list->next;
        if (list->data != NULL) {
            free(list->data);
            list->data = NULL;
        }
        free(list);
        list = next;
    }
    return;
}

size_t
ls_len(ListStr *list) {
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

ListStr *
ls_push_back(ListStr *list, char *val) {
    if (list == NULL) {
        return ls_init(val);
    }
    ListStr *cur = list;
    while (cur->next != NULL) {
        if (strcmp(cur->data, val) == 0) {
            return list;
        }
        cur = cur->next;
    }
    cur->next = ls_init(val);
    return list;
}

ListStr *
ls_erase(ListStr *list, char *val) {
    if (list == NULL) {
        return NULL;
    }
    if (strcmp(list->data, val) == 0) {
        ListStr *cur = list->next;
        if (list->data != NULL) {
            free(list->data);
            list->data = NULL;
        }
        free(list);
        return cur;
    }
    ListStr *cur = list;
    while (cur->next != NULL) {
        if (strcmp(cur->next->data, val) == 0) {
	    ListStr *new_next = cur->next->next;
            if (cur->next->data != NULL) {
                free(cur->next->data);
                cur->next->data = NULL;
            }
            free(cur->next);
            cur->next = new_next;
            return list; 
        }
        cur = cur->next;
    }
    ls_free(list);
    fprintf(stderr, "%s: %s -- no such value\n", __func__, val);
    exit(1);
    return list;        
}

ListStr *
ls_find(ListStr *list, char *val) {
    ListStr *cur = list;
    while (cur != NULL) {
        if (strcmp(cur->data, val) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

