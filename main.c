#include <stdio.h>

#include "graph.h"

void printList(void* elem, void* data)
{
    printf("%p: %d\n", elem, *((int*) elem));
}

int main()
{
    Graph* G = graphNew();

    int a = graphAddNode(G, NULL);
    int b = graphAddNode(G, NULL);
    int c = graphAddNode(G, NULL);

    graphAddEdge(G, a, b, NULL);
    graphAddEdge(G, a, c, NULL);
    graphAddEdge(G, b, c, NULL);

    graphPrint(G);

    graphFree(G);

    return 0;
}
