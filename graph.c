#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "vector.h"

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

struct Graph
{
    int nbNodes;
    int nbEdges;
    Vector* nodes;
    Vector* edges;
};

static void mapDeleteEdge(void* edge, void* data)
{
    free(edge);
}

static void mapPrintEdge(void* edge, void* data)
{
    Vector* v = (Vector*) data;
    int to = ((Edge*) (vectorAt(v, *(int*) edge)))->to;
    printf(" -> %d\n", to);
}

static void mapListEdgeId(void* edge, void* list)
{
    listPush((List*) list, &(((Edge*) edge)->to));
}

List* neighbours(Graph* g, int nodeID)
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

int graphAddNode(Graph* g, void* attribute)
{
    int n = g->nbNodes;
    Node* newNode = malloc(sizeof(Node));
    newNode->ID = n;
    newNode->edges = NULL;
    newNode->attribute = attribute;

    vectorPush(g->nodes, newNode);
    g->nbNodes ++;

    return n;
}

int graphAddEdge(Graph* g, int fromID, int toID, void* attribute)
{
    int eid = g->nbEdges;
    Edge* e = malloc(sizeof(Edge));

    e->ID = eid;
    e->from = fromID;
    e->to = toID;
    e->attribute = attribute;

    listPush(neighbours(g, fromID), &e->ID);
    vectorPush(g->edges, e);
    g->nbEdges ++;

    return eid;
}

List graphGetNeighbours(Graph* g, int ID)
{
    List l = NULL;
    listMap(*neighbours(g, ID), mapListEdgeId, &l);
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

void* graphGetEdgeAttribute(Graph* g, int edgeID)
{
    return ((Edge*) g->edges->data[edgeID])->attribute;
}

void graphPrint(Graph* g)
{
    int i;

    for (i = 0; i < g->nbNodes; i++)
    {
        printf("Node %d :\n", ((Node*)(g->nodes->data[i]))->ID);
        listMap(*neighbours(g, i), mapPrintEdge, g->edges);
    }
}

void graphFree(Graph* g)
{
    int i = 0;

    for (i = 0; i < g->nbNodes; i++)
    {
        listMap(*neighbours(g, i), mapDeleteEdge, NULL);
        listFree(neighbours(g, i));
        free(g->nodes->data[i]);
    }

    vectorFree(g->nodes);
    vectorFree(g->edges);

    free(g);
}

