#ifndef LIST_OF_MAP_NODES_H
#define LIST_OF_MAP_NODES_H

#include <stddef.h>
#include "map_node.h"

typedef struct ListOfMapNodes {
    MapNode data;
    struct ListOfMapNodes *next;
} ListOfMapNodes;

extern ListOfMapNodes *lmn_init(char *key, char *value);
extern void lmn_free(ListOfMapNodes *list);
extern size_t lmn_len(ListOfMapNodes *list);
extern ListOfMapNodes *lmn_push_back(ListOfMapNodes *list, char *key, char *value);
extern void lmn_set(ListOfMapNodes *list, char *key, char *new_val);
extern char *lmn_get(ListOfMapNodes *list, char *key);
extern ListOfMapNodes *lmn_erase(ListOfMapNodes *list, char *key);
extern ListOfMapNodes *lmn_find(ListOfMapNodes *list, char *key);
#endif
