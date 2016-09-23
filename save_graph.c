#include <stdio.h>

#include "save_graph.h"

char* familyRelNames[10];

static void writeHeader(FILE *f)
{
    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\""
            " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
            "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n"

            //Attributes of nodes and edges
            "<key id=\"d0\" for=\"node\" attr.name=\"color\" attr.type=\"string\">\n<default>yellow</default>\n</key>\n"
            "<key id=\"d1\" for=\"node\" attr.name=\"age\" attr.type=\"int\"/>\n"
            "<key id=\"d2\" for=\"edge\" attr.name=\"type\" attr.type=\"string\"/>\n"
            "<key id=\"d3\" for=\"node\" attr.name=\"firstName\" attr.type=\"string\"/>\n"
            "<key id=\"d4\" for=\"node\" attr.name=\"lastName\" attr.type=\"string\"/>\n"

            "<graph id=\"G\" edgedefault=\"directed\">\n");
}

static void writeFooter(FILE *f)
{
    fprintf(f, "</graph>\n</graphml>");
}

static void writeNode(FILE *f, SocialGraph* SG, int ID)
{
    struct nodeAttrib* na = graphGetNodeAttribute(SG->G, ID);

    fprintf(f, "<node id=\"n%d\">\n"
            "<data key=\"d0\">blue</data>\n"
            "<data key=\"d1\">%d</data>\n"
            "<data key=\"d3\">%s</data>\n"
            "<data key=\"d4\">%s</data>\n"
            "</node>\n", na->nodeID, na->ID.age, nameGetFirstName(SG->NM, na->ID.firstName), nameGetLastName(SG->NM, na->ID.lastName));
}

static void writeEdge(FILE *f, int ID, int from, int to, int type)
{
    if (type != CHILD)
        fprintf(f, "<edge id=\"e%d\" source=\"n%d\" target=\"n%d\">\n  <data key=\"d2\"> %s </data>\n</edge>\n", ID, from, to, familyRelNames[type]);
}

void saveGraph(SocialGraph* SG, const char* fileName)
{
    familyRelNames[0] = "NONE";
    familyRelNames[1] = "GRANDPARENT";
    familyRelNames[2] = "PARENT";
    familyRelNames[3] = "COUPLE";
    familyRelNames[4] = "SIBLING";
    familyRelNames[5] = "CHILD";
    familyRelNames[0] = "NONE";
    familyRelNames[0] = "NONE";
    familyRelNames[0] = "NONE";
    familyRelNames[0] = "NONE";

    printf("Writing graph to %s\n", fileName);
    FILE* f = fopen(fileName, "w");

    writeHeader(f);

    int i = 0;

    for (i = 0; i < SG->G->nbNodes; i++)
    {
        writeNode(f, SG, i);
    }

    for (i = 0; i < SG->G->nbEdges; i++)
    {
        struct relationAttrib* ra = graphGetEdgeAttribute(SG->G, i);
        writeEdge(f, i, graphGetNodeFrom(SG->G, i), graphGetNodeTo(SG->G, i), ra->familyRel);
    }

    writeFooter(f);

    fclose(f);
}
