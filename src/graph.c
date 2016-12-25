#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "vector.h"

Graph* graph_new()
{
    Graph* g = malloc(sizeof(Graph));
    g->nodes = vector_new();
    g->nbNodes = 0;
    g->edges = vector_new();
    g->nbEdges = 0;
    return g;
}

Node* graph_add_node(Graph* g, void* attribute)
{
    int n = g->nbNodes;
    Node* newNode = malloc(sizeof(Node));
    newNode->ID = n;
    newNode->edges = NULL;
    newNode->attribute = attribute;

    vector_push(g->nodes, newNode);
    g->nbNodes ++;

    return newNode;
}

Edge* graph_add_edge(Graph* g, Node* fromPtr, Node* toPtr, void* attribute)
{
    int eid = g->nbEdges;
    Edge* e = malloc(sizeof(Edge));

    e->ID = eid;
    e->from = fromPtr;
    e->to = toPtr;
    e->attribute = attribute;

    list_push(&(fromPtr->edges), e);

    vector_push(g->edges, e);
    g->nbEdges ++;

    return e;
}

Node* graph_get_node_by_index(Graph* g, int idx)
{
    if (idx >= 0 && idx < g->nbNodes)
        return vector_at(g->nodes, idx);
    else
        return NULL;
}

struct MapNodePackage
{
    void (*mapfun)(Node* node, void* dataIN);
    void* data;
};

struct MapEdgePackage
{
    void (*mapfun)(Edge* edge, void* dataIN);
    void* data;
};

void map_node_clbk(void* elem, void* data)
{
    struct MapNodePackage* pkg = data;

    pkg->mapfun((Node*) elem, pkg->data);
}

void map_edge_clbk(void* elem, void* data)
{
    struct MapEdgePackage* pkg = data;

    pkg->mapfun((Edge*) elem, pkg->data);
}

void graph_map_nodes(Graph* g, void (*mapfun)(Node* node, void* dataIN), void* data)
{
    struct MapNodePackage pkg;
    pkg.mapfun = mapfun;
    pkg.data = data;
    vector_map(*g->nodes, map_node_clbk, &pkg);
}

void graph_map_edges(Graph* g, void (*mapfun)(Edge* edge, void* dataIN), void* data)
{
    struct MapEdgePackage pkg;
    pkg.mapfun = mapfun;
    pkg.data = data;
    vector_map(*g->edges, map_edge_clbk, &pkg);
}

static void free_node_clbk(Node* node, void* data)
{
    list_flush(node->edges);
    list_free(&node->edges);
    free(node);
}

void graph_free(Graph* g)
{
    graph_map_nodes(g, free_node_clbk, NULL);
    vector_free(g->nodes);
    vector_free(g->edges);

    free(g);
}
