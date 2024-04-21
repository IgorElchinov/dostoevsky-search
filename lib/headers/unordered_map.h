#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include <stdint.h>
#include "list_of_map_nodes.h"
#include "hash_table.h"

enum {
    MAX_MAP_SIZE = 1 << 16,
    UNORDERED_MAP_SEED = 0xa2d7,
};

typedef struct UnorderedMap {
    uint16_t (*hash) (char *str);
    ListOfMapNodes *arr[MAX_MAP_SIZE];
} UnorderedMap;

extern void um_init_params(UnorderedMap *map, HashType type, uint16_t seed);
extern void um_init(UnorderedMap *map);
extern void um_free(UnorderedMap *map);
extern void um_insert(UnorderedMap *map, char *key, char *value);
extern void um_set(UnorderedMap *map, char *key, char *value);
extern char *um_get(UnorderedMap *map, char *key);
extern void um_erase(UnorderedMap *map, char *key);
extern int um_find(UnorderedMap *map, char *key);

#endif
