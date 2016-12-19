#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "vector.h"

static void mapDeleteEdge(void* edge, void* data)
{
    free(edge);
}

static void mapPrintEdge(void* edge, void* data)
{
    Vector* v = (Vector*) data;
    Node* to = ((Edge*) (vectorAt(v, *(int*) edge)))->to;
    printf(" -> %x\n", to);
}

static void mapListNodesIdTo(void* edge, void* list)
{
    listPush((List*) list, &(((Edge*) edge)->to));
}

/*static void mapListEdgeId(void* edge, void* list)
{
    int* to = malloc(sizeof(int));
    *to = ((Edge*) edge)->to;
    listPush((List*) list, to);
}*/

static List* neighbours(Graph* g, int nodeID)
{
    return &(((Node*)(g->nodes->data[nodeID]))->edges);
}

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

/*
List graphGetNeighbours(Graph* g, int ID)
{
    List l = NULL;
    listMap(*neighbours(g, ID), mapListNodesIdTo, &l);
    return l;
}

void* graphGetNodeAttribute(Graph* g, int nodeID)
{
    return ((Node*) g->nodes->data[nodeID])->attribute;
}

int graphGetNodeFrom(Graph* g, int edgeID)
{
    return ((Edge*) vectorAt(g->edges, edgeID))->from;
}

int graphGetNodeTo(Graph* g, int edgeID)
{
    return ((Edge*) vectorAt(g->edges, edgeID))->to;
}

List graphGetEdgesFrom(Graph* g, int nodeID)
{
    List l = NULL;

    listCopy(*neighbours(g, nodeID), &l);

    return l;
}

List graphGetEdgesTo(Graph* g, int nodeID)
{
    List l = NULL;
    int i = 0;
    int* id;

    for (i = 0; i < g->edges->count; i++)
    {
        if (((Edge*)(vectorAt(g->edges, i)))->to == nodeID)
        {
            id = malloc(sizeof(int));
            *id = ((Edge*) vectorAt(g->edges, i))->ID;
            listPush(&l, id);
        }
    }

    return l;
}

int graphGetEdgeBetween(Graph* g, int nodeFrom, int nodeTo)
{
    //List l = graphGetEdgesFrom(g, nodeFrom);
    return 0;
}

void* graphGetEdgeAttribute(Graph* g, int edgeID)
{
    return ((Edge*) g->edges->data[edgeID])->attribute;
}

*/

struct mapPackage
{
    void (*mapfun)(void* attr, void* dataIN);
    void* data;
};

void mapNodeCallback(void* elem, void* data)
{
    struct mapPackage* pkg = data;

    pkg->mapfun(((Node*)elem)->attribute, pkg->data);
}

void mapEdgeCallback(void* elem, void* data)
{
    struct mapPackage* pkg = data;

    pkg->mapfun(((Edge*)elem)->attribute, pkg->data);
}

void graphMapNodes(Graph* g, void (*mapfun)(void* attr, void* dataIN), void* data)
{
    struct mapPackage pkg;
    pkg.mapfun = mapfun;
    pkg.data = data;
    vectorMap(*g->nodes, mapNodeCallback, &pkg);
}

void graphMapEdges(Graph* g, void (*mapfun)(void* attr, void* dataIN), void* data)
{
    struct mapPackage pkg;
    pkg.mapfun = mapfun;
    pkg.data = data;
    vectorMap(*g->edges, mapEdgeCallback, &pkg);
}

void graphPrint(Graph* g)
{
    int i;

    for (i = 0; i < g->nbNodes; i++)
    {
    }
}
