#include <stdio.h>

#include "save_graph.h"

static void writeHeader(FILE *f)
{
    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\""
            " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
            "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n"
            "<key id=\"d0\" for=\"node\" attr.name=\"color\" attr.type=\"string\">\n"
            "<default>yellow</default>\n</key>\n<key id=\"d1\" for=\"edge\" attr.name=\"weight\" attr.type=\"double\"/>\n"
            "<graph id=\"G\" edgedefault=\"directed\">\n");
}

static void writeFooter(FILE *f)
{
    fprintf(f, "</graph>\n</graphml>");
}

static void writeNode(FILE *f, int ID)
{
    fprintf(f, "<node id=\"n%d\">\n<data key=\"plop\">plip</data>\n</node>\n", ID);
}

static void writeEdge(FILE *f, int ID, int from, int to)
{
    fprintf(f, "<edge id=\"e%d\" source=\"n%d\" target=\"n%d\"/>\n", ID, from, to);
}

void saveGraph(Graph* G, const char* fileName)
{
    printf("Writing graph to %s\n", fileName);
    FILE* f = fopen(fileName, "w");

    writeHeader(f);

    int i = 0;

    for (i = 0; i < G->nbNodes; i++)
    {
        writeNode(f, i);
    }

    for (i = 0; i < G->nbEdges; i++)
    {
        writeEdge(f, i, graphGetNodeFrom(G, i), graphGetNodeTo(G, i));
    }

    writeFooter(f);

    fclose(f);
}
