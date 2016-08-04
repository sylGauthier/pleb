#ifndef GRAPH_H
#define GRAPH_H

#include "vector.h"
#include "list.h"

typedef struct Graph
{
    int nbNodes;
    int nbEdges;
    Vector* nodes;
    Vector* edges;
} Graph;

typedef struct Node
{
    int ID;
    List edges;
    void* attribute;
} Node;

typedef struct Edge
{
    int ID;
    int from;
    int to;
    void* attribute;
} Edge;

Graph* graphNew();

int graphAddNode(Graph* g, void* attribute);
int graphAddEdge(Graph* g, int fromID, int toID, void* attribute);

List graphGetNeighbours(Graph* g, int nodeID);

void* graphGetNodeAttribute(Graph* g, int nodeID);
void* graphGetEdgeAttribute(Graph* g, int edgeID);

int graphGetNodeFrom(Graph* g, int edgeID);
int graphGetNodeTo(Graph* g, int edgeID);

List graphGetEdgesFrom(Graph* g, int nodeID);
List graphGetEdgesTo(Graph* g, int nodeID);
int graphGetEdgeBetween(Graph* g, int nodeFrom, int nodeTo);

void graphMapNodes(Graph* g, void (*mapfun)(void* attr, void* dataIn), void* data);
void graphMapEdges(Graph* g, void (*mapfun)(void* attr, void* dataIn), void* data);

void graphPrint(Graph* g);
void graphPrintNeighbours(Graph* g, int nodeID);

void graphFree(Graph* g);

#endif
