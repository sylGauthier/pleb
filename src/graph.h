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
    struct Node* from;
    struct Node* to;
    void* attribute;
} Edge;

Graph* graphNew();

Node* graphAddNode(Graph* g, void* attribute);
Edge* graphAddEdge(Graph* g, struct Node* fromPtr, struct Node* toPtr, void* attribute);

void graphMapNodes(Graph* g, void (*mapfun)(Node* node, void* dataIn), void* data);
void graphMapEdges(Graph* g, void (*mapfun)(Edge* edge, void* dataIn), void* data);

void graphFree(Graph* g);

#endif
