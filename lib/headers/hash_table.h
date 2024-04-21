#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>
#include "list_str.h"

enum {
    MAX_HASH_TABLE_SIZE = 1 << 16,
    MAX_MASK_VAL = 0b1111111111111111,
};

typedef struct HashTable {
    uint16_t (*hash) (char *str);
    ListStr *arr[MAX_HASH_TABLE_SIZE];
} HashTable;

typedef enum HashType {
    NONE,
    APHASH,
    MURMUR_3_16,
    MURMUR_3_FAKE32,
    MOCK_COMPRESS
} HashType;

// different hash functions
extern uint16_t APHash(char *str);
extern uint16_t murmur3_16(char *str);
extern uint16_t murmur3_fake32(char *str);
extern uint16_t constant_hash(char *str);

extern void h_init(HashTable *hash_table, HashType type, uint16_t seed);
extern void h_free(HashTable *hash_table);
extern void h_insert(HashTable *hash_table, char *str);
extern void h_erase(HashTable *hash_table, char *str);
extern int h_find(HashTable *hash_table, char *str);

#endif
