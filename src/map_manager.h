#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "graph.h"

enum PlaceType
{
    BUILDING = 0,
    STREET
};

struct MapNode
{
    int ID;
    enum PlaceType type;
    void* data;
};

struct MapBuilding
{
    int number;
    int buildingType;
    int security;
};

struct MapStreet
{
    char* name;
    int length;
};

typedef struct MapManager
{
    Graph* map;
} MapManager;

MapManager* map_new_manager();

void map_free_manager(MapManager* MM);
#endif
