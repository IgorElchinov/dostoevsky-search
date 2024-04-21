#ifndef LIST_STR_H
#define LIST_STR_H

#include <stddef.h>

typedef struct ListStr {
    char *data;
    struct ListStr *next;
} ListStr;

extern ListStr *ls_init(char *val);
extern void ls_free(ListStr *list);
extern size_t ls_len(ListStr *list);
extern ListStr *ls_push_back(ListStr *list, char *val);
extern ListStr *ls_erase(ListStr *list, char *val);
extern ListStr *ls_find(ListStr *list, char *val);
#endif
