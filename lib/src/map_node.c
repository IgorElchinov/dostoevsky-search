#include "map_node.h"

inline char *
node_key(MapNode node) {
    return node.key;
}

inline char *
node_value(MapNode node) {
    return node.value;
}
