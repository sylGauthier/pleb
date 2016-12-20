#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "graph.h"

enum placeType
{
    BUILDING = 0,
    STREET
};

struct mapNode
{
    int ID;
    enum placeType type;
    void* data;
};

struct mapBuilding
{
    int number;
    int buildingType;
    int security;
};

struct mapStreet
{
    char name[25];
    int length;
};

struct mapManager
{
    Graph map;
};

struct mapManager* mapNewManager();

void mapGenerate(struct mapManager* MM);

#endif
