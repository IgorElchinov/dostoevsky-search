#include "unordered_set.h"

inline void
us_init(UnorderedSet *set) {
    h_init(set, MURMUR_3_16, UNORDERED_SET_SEED);
    return;
}

inline void
us_free(UnorderedSet *set) {
    h_free(set);
    return;
}

inline void
us_insert(UnorderedSet *set, char *str) {
    h_insert(set, str);
    return;
}

inline void
us_erase(UnorderedSet *set, char *str) {
    h_erase(set, str);
    return;
}

inline int
us_find(UnorderedSet *set, char *str) {
    return h_find(set, str);
}
