#include <stdlib.h>
#include <stdio.h>

#include "map_generation.h"

static struct MapNode* new_building_node(int number, int type, int size) {
    struct MapNode* newNode = NULL;
    struct MapBuilding* buildingData;

    if (!(newNode = malloc(sizeof(struct MapNode)))) {
        fprintf(stderr, "Error: map_generation: could not allocate memory\n");
        return NULL;
    }
    if (!(newNode->data = malloc(sizeof(struct MapBuilding)))) {
        fprintf(stderr, "Error: map_generation: could not allocate memory\n");
        free(newNode);
        return NULL;
    }

    newNode->type = BUILDING;
    buildingData = newNode->data;
    buildingData->number = number;
    buildingData->buildingType = type;
    buildingData->security = 0;

    return newNode;
}

static struct MapNode* new_street_node(char* name, int length) {
    struct MapNode* newNode = NULL;
    struct MapStreet* streetData;

    if (!(newNode = malloc(sizeof(struct MapNode)))) {
        fprintf(stderr, "Error: map_generation: could not allocate memory\n");
        return NULL;
    }
    if (!(newNode->data =  malloc(sizeof(struct MapStreet)))) {
        fprintf(stderr, "Error: map_generation: could not allocate memory\n");
        free(newNode);
        return NULL;
    }

    newNode->type = STREET;
    streetData = newNode->data;
    streetData->name = name;
    streetData->length = length;
    return newNode;
}

void generate_map(SocialGraph* SG) {
    int i;
    struct MapNode* newStreet = new_street_node("Avenue du Maréchal", 4);
    Node* curStreetNode = graph_add_node(SG->MM->map, newStreet);

    for (i = 0; i < SG->CM->communities->count; i++) {
        struct Community* curCom = vector_at(SG->CM->communities, i);
        struct MapNode* newBuilding = new_building_node(i, 0, curCom->nbPositions);
        Node* n = graph_add_node(SG->MM->map, newBuilding);

        graph_add_edge(SG->MM->map, curStreetNode, n, NULL);

        if (list_size(curStreetNode->edges) > 4) {
            Node* tmp = curStreetNode;
            newStreet = new_street_node("Avenue du Maréchal", 4);
            curStreetNode = graph_add_node(SG->MM->map, newStreet);
            graph_add_edge(SG->MM->map, tmp, curStreetNode, NULL);
            graph_add_edge(SG->MM->map, curStreetNode, tmp, NULL);
        }
    }
}
