#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "vector.h"

Graph* graphNew()
{
    Graph* g = malloc(sizeof(Graph));
    g->nodes = vectorNew();
    g->nbNodes = 0;
    g->edges = vectorNew();
    g->nbEdges = 0;
    return g;
}

Node* graphAddNode(Graph* g, void* attribute)
{
    int n = g->nbNodes;
    Node* newNode = malloc(sizeof(Node));
    newNode->ID = n;
    newNode->edges = NULL;
    newNode->attribute = attribute;

    vectorPush(g->nodes, newNode);
    g->nbNodes ++;

    return newNode;
}

Edge* graphAddEdge(Graph* g, Node* fromPtr, Node* toPtr, void* attribute)
{
    int eid = g->nbEdges;
    Edge* e = malloc(sizeof(Edge));

    e->ID = eid;
    e->from = fromPtr;
    e->to = toPtr;
    e->attribute = attribute;

    listPush(&(fromPtr->edges), e);

    vectorPush(g->edges, e);
    g->nbEdges ++;

    return e;
}

Node* graphGetNodeByIndex(Graph* g, int idx)
{
    if (idx >= 0 && idx < g->nbNodes)
        return vectorAt(g->nodes, idx);
    else
        return NULL;
}

struct mapNodePackage
{
    void (*mapfun)(Node* node, void* dataIN);
    void* data;
};

struct mapEdgePackage
{
    void (*mapfun)(Edge* edge, void* dataIN);
    void* data;
};

void mapNodeCallback(void* elem, void* data)
{
    struct mapNodePackage* pkg = data;

    pkg->mapfun((Node*) elem, pkg->data);
}

void mapEdgeCallback(void* elem, void* data)
{
    struct mapEdgePackage* pkg = data;

    pkg->mapfun((Edge*) elem, pkg->data);
}

void graphMapNodes(Graph* g, void (*mapfun)(Node* node, void* dataIN), void* data)
{
    struct mapNodePackage pkg;
    pkg.mapfun = mapfun;
    pkg.data = data;
    vectorMap(*g->nodes, mapNodeCallback, &pkg);
}

void graphMapEdges(Graph* g, void (*mapfun)(Edge* edge, void* dataIN), void* data)
{
    struct mapEdgePackage pkg;
    pkg.mapfun = mapfun;
    pkg.data = data;
    vectorMap(*g->edges, mapEdgeCallback, &pkg);
}

static void freeNodeClbk(Node* node, void* data)
{
    listFlush(node->edges);
    listFree(&node->edges);
    free(node);
}

void graphFree(Graph* g)
{
    graphMapNodes(g, freeNodeClbk, NULL);
    vectorFlush(g->edges);
    vectorFree(g->edges);
}
