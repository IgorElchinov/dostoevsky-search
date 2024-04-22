#include "unordered_map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

uint16_t SEED;
uint32_t MASK;

void
um_init_params(UnorderedMap *map, HashType type, uint16_t seed) {
    if (SEED == 0) {
        SEED = seed;
    }
    if (MASK == 0) {
        MASK = rand() / (RAND_MAX / MAX_MASK_VAL);
    }
    switch (type) {
        case NONE:
            map->hash = NULL;
            break;
        case APHASH:
            map->hash = APHash;
            break;
        case MURMUR_3_16:
            map->hash = murmur3_16;
            break;
        case MURMUR_3_FAKE32:
            map->hash = murmur3_fake32;
            break;
        case MOCK_COMPRESS:
            map->hash = constant_hash;
            break;
        default:
            fprintf(stderr, "%s: unknown hash type\n", __func__);
            fflush(stderr);
            exit(1);
    }
    for (size_t i = 0; i < MAX_MAP_SIZE; ++i) {
        map->arr[i] = 0;
    }
    return;
}

inline void
um_init(UnorderedMap *map) {
    um_init_params(map, MURMUR_3_16, UNORDERED_MAP_SEED);
    return;
}

void
um_free(UnorderedMap *map) {
    for (size_t i = 0; i < MAX_MAP_SIZE; ++i) {
        lmn_free(map->arr[i]);
        map->arr[i] = NULL;
    }
    return;
}

void
um_insert(UnorderedMap *map, char *key, char *value) {
    uint16_t hash = map->hash(key);
    map->arr[hash] = lmn_push_back(map->arr[hash], key, value);
    return;
}

void
um_set(UnorderedMap *map, char *key, char *value) {
    uint16_t hash = map->hash(key);
    lmn_set(map->arr[hash], key, value);
    return;
}

char *
um_get(UnorderedMap *map, char *key) {
    uint16_t hash = map->hash(key);
    return lmn_get(map->arr[hash], key);
}

void
um_erase(UnorderedMap *map, char *key) {
    uint16_t hash = map->hash(key);
    map->arr[hash] = lmn_erase(map->arr[hash], key);
    return;
}

int
um_find(UnorderedMap *map, char *key) {
    uint16_t hash = map->hash(key);
    return lmn_find(map->arr[hash], key) != NULL;
}
