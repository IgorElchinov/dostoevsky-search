#include <stdio.h>
#include "map_node.h"

int
main(void) {
    MapNode node;
    node.key = "example key";
    node.value = "example value";
    printf("%s : %s\n", node_key(node), node_value(node));
    return 0;
}
