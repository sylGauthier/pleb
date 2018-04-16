#include <stdlib.h>
#include <stdio.h>

#include "map_manager.h"


MapManager* map_new_manager() {
    MapManager* newMM = malloc(sizeof(MapManager));

    if (newMM) {
        newMM->map = graph_new();
        return newMM;
    } else {
        printf("Error\n");
        return NULL;
    }
}

static void free_node_clbk(Node* node, void* data) {
    struct MapNode* curNode = node->attribute;

    switch (curNode->type) {
        case BUILDING :
        case STREET :
            free(curNode->data);
            break;
        default :
            break;
    }
    free(curNode);
}

void map_free_manager(MapManager* MM) {
    graph_map_nodes(MM->map, free_node_clbk, NULL);
    graph_free(MM->map);
    free(MM);
}
