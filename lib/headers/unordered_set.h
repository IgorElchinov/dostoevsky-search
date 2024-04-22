#ifndef UNORDERED_SET_H
#define UNORDERED_SET_H

#include "hash_table.h"

typedef HashTable UnorderedSet;

enum {
    UNORDERED_SET_SEED = 0xa2d7, 
};

extern void us_init(UnorderedSet *set);
extern void us_free(UnorderedSet *set);
extern void us_insert(UnorderedSet *set, char *str);
extern void us_erase(UnorderedSet *set, char *str);
extern int us_find(UnorderedSet *set, char *str);

#endif
