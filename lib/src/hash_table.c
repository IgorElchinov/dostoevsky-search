#include "hash_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint16_t SEED = 0;
uint32_t MASK = 0;

uint16_t
APHash(char *str) {
    uint64_t length = strlen(str);
    uint16_t hash = 0xAAAA;
    uint16_t i = 0;
    for (; i < length; ++str, ++i) {
        hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :
                (~((hash << 11) + ((*str) ^ (hash >> 5))));
    }
    return hash;
}

static inline uint16_t
murmur_16_scramble(uint16_t k) {
    k *= 0xc9e1;
    k = (k << 7) | (k >> 9);
    k *= 0x1b873593;
    return k;
}

uint16_t 
murmur3_16(char *key) {
    size_t len = strlen(key);
    uint16_t h = SEED;
    printf("SEED: %d", SEED);
    uint16_t k;
    for (size_t i = len >> 1; i; i--) {
        memcpy(&k, key, sizeof(k));
        key += sizeof(k);
        h ^= murmur_16_scramble(k);
        h = (h << 5) | (h >> 11);
        h = h * 5 + 0xe644;
    }
    k = 0;
    for (size_t i = len & 1; i; i--) {
        k <<= 4;
        k |= key[i - 1];
    }
    h ^= murmur_16_scramble(k);
    h ^= len;
    h ^= h >> 8;
    h *= 0x8bcb;
    h ^= h >> 5;
    h *= 0xc2a5;
    h ^= h >> 8;
    return h;
}

static inline uint8_t
get_bit(uint32_t n, size_t bit) {
    return (n >> bit) & 1;
}

static inline uint32_t
murmur_32_scramble(uint32_t k) {
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}

uint16_t 
murmur3_fake32(char *key) {
    size_t len = strlen(key);
    uint32_t h = SEED;
    uint32_t k;
    for (size_t i = len >> 2; i; i--) {
        memcpy(&k, key, sizeof(k));
        key += sizeof(k);
        h ^= murmur_32_scramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }
    k = 0;
    for (size_t i = len & 3; i; i--) {
        k <<= 8;
        k |= key[i - 1];
    }
    h ^= murmur_32_scramble(k);
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    uint16_t ans = 0;
    for (int i = 0; i < 16; ++i) {
        ans |= get_bit(h, (i << 1) + get_bit(MASK, i));
        ans <<= 1;
    }
    // fprintf(stderr, "%u %u %u\n", h, MASK, ans);
    return ans;
}

uint16_t
constant_hash(char *str) {
    return 2;
}

void
h_init(HashTable *hash_table, HashType type, uint16_t seed) {
    if (SEED == 0) {
        SEED = seed;
    }
    if (MASK == 0) {
        MASK = rand() / (RAND_MAX / MAX_MASK_VAL);
    }
    switch (type) {
        case NONE:
            hash_table->hash = NULL;
            break;
        case APHASH:
            hash_table->hash = APHash;
            break;
        case MURMUR_3_16:
            hash_table->hash = murmur3_16;
            break;
        case MURMUR_3_FAKE32:
            hash_table->hash = murmur3_fake32;
            break;
        case MOCK_COMPRESS:
            hash_table->hash = constant_hash;
            break;
        default:
            fprintf(stderr, "%s: unknown hash type\n", __func__);
            fflush(stderr);
            exit(1);
    }
    for (size_t i = 0; i < MAX_HASH_TABLE_SIZE; ++i) {
        hash_table->arr[i] = 0;
    }
    return;
}

void
h_free(HashTable *hash_table) {
    for (size_t i = 0; i < MAX_HASH_TABLE_SIZE; ++i) {
        ls_free(hash_table->arr[i]);
        hash_table->arr[i] = 0;
    }
    return;
}

void
h_insert(HashTable *hash_table, char *str) {
    uint16_t hash = hash_table->hash(str);
    hash_table->arr[hash] = ls_push_back(hash_table->arr[hash], str);
    return;
}

void
h_erase(HashTable *hash_table, char *str) {
    uint16_t hash = hash_table->hash(str);
    hash_table->arr[hash] = ls_erase(hash_table->arr[hash], str);
    return;
}

int
h_find(HashTable *hash_table, char *str) {
    uint16_t hash = hash_table->hash(str);
    return ls_find(hash_table->arr[hash], str) != NULL;
}
