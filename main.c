#include <stdio.h>
#include <string.h>

#include "graph.h"

struct nodeAttrib
{
    char name[100];
};

struct edgeAttrib
{
    char type[100];
};

void explainEdge(Graph* G, int eid)
{
    struct edgeAttrib* ea = graphGetEdgeAttribute(G, eid);

    struct nodeAttrib* nf = graphGetNodeAttribute(G, graphGetNodeFrom(G, eid));
    struct nodeAttrib* nt = graphGetNodeAttribute(G, graphGetNodeTo(G, eid));

    printf("%s %s %s\n", nf->name, ea->type, nt->name);
}

int main()
{
    Graph* G = graphNew();

    struct nodeAttrib node1;
    strcpy(node1.name, "Glandalf");

    struct nodeAttrib node2;
    strcpy(node2.name, "Frodo");

    struct nodeAttrib node3;
    strcpy(node3.name, "Gollum");

    struct edgeAttrib edge1;
    strcpy(edge1.type, "whispers sweet words to");

    int a = graphAddNode(G, &node1);
    int b = graphAddNode(G, &node2);
    int c = graphAddNode(G, NULL);

    int ab = graphAddEdge(G, a, b, &edge1);

    graphAddEdge(G, a, c, NULL);
    graphAddEdge(G, b, c, NULL);
    graphAddEdge(G, c, a, NULL);

    graphPrint(G);

    explainEdge(G, ab);

    graphFree(G);

    return 0;
}
