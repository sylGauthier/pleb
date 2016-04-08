#ifndef GRAPH_H
#define GRAPH_H

#include "vector.h"
#include "list.h"

typedef struct Graph Graph;

Graph* graphNew();

int graphAddNode(Graph* g, void* attribute);
int graphAddEdge(Graph* g, int fromID, int toID, void* attribute);

List graphGetNeighbours(Graph* g, int nodeID);
void* graphGetNodeAttribute(Graph* g, int nodeID);

int graphGetNodeFrom(Graph* g, int edgeID);
int graphGetNodeTo(Graph* g, int edgeID);
void* graphGetEdgeAttribute(Graph* g, int edgeID);

void graphPrint(Graph* g);

void graphFree(Graph* g);

#endif
