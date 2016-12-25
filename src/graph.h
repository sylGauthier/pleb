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

Graph* graph_new();

Node* graph_add_node(Graph* g, void* attribute);
Edge* graph_add_edge(Graph* g, Node* fromPtr, Node* toPtr, void* attribute);

void graph_map_nodes(Graph* g, void (*mapfun)(Node* node, void* dataIn), void* data);
void graph_map_edges(Graph* g, void (*mapfun)(Edge* edge, void* dataIn), void* data);

void graph_free(Graph* g);

#endif
