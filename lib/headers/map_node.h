#ifndef MAP_NODE_H
#define MAP_NODE_H

typedef struct MapNode {
    char *key;
    char *value;
} MapNode;

extern char *node_key(MapNode node);
extern char *node_value(MapNode node);
#endif
