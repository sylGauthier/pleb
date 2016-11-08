#ifndef PLACE_MANAGER_H
#define PLACE_MANAGER_H

#include "graph.h"

enum placeType
{
    BUILDING = 0,
    STREET = 1
};

struct placeNode
{
    int ID;
    enum placeType type;
    void* data;
};

struct placeBuilding
{
    int open;
    int security;
};

struct placeStreet
{
    char name[25];
    int length;
};

struct placeManager
{
    Graph places;
};

#endif
