#include <stdio.h>

#include "save_graph.h"

char* familyRelNames[10];

static void writeHeader(FILE *f)
{
    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\""
            " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
            "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n"

            /*Attributes of nodes and edges*/
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

struct writeClbkData
{
    FILE* file;
    SocialGraph* SG;
};

static void writeNodeClbk(Node* node, void* data)
{
    struct writeClbkData* cd = data;
    struct nodeAttrib* na = node->attribute;

    fprintf(cd->file, "<node id=\"n%d\">\n"
            "<data key=\"d0\">blue</data>\n"
            "<data key=\"d1\">%d</data>\n"
            "<data key=\"d3\">%s</data>\n"
            "<data key=\"d4\">%s</data>\n"
            "</node>\n", na->nodeID, na->ID.age, nameGetFirstName(cd->SG->NM, na->ID.firstName), nameGetLastName(cd->SG->NM, na->ID.lastName));
}

static void writeEdgeClbk(Edge* edge, void* data)
{
    struct writeClbkData* cd = data;
    struct relationAttrib* ra = edge->attribute;
    if (ra->familyRel != CHILD)
        fprintf(cd->file, "<edge id=\"e%d\" source=\"n%d\" target=\"n%d\">\n  <data key=\"d2\"> %s </data>\n</edge>\n",
                ra->edgeID, edge->from->ID, edge->to->ID, familyRelNames[ra->familyRel]);
}

void saveGraph(SocialGraph* SG, const char* fileName)
{
    familyRelNames[0] = "NONE";
    familyRelNames[1] = "GRANDPARENT";
    familyRelNames[2] = "PARENT";
    familyRelNames[3] = "COUPLE";
    familyRelNames[4] = "SIBLING";
    familyRelNames[5] = "CHILD";
    familyRelNames[6] = "NONE";
    familyRelNames[7] = "NONE";
    familyRelNames[8] = "NONE";
    familyRelNames[9] = "NONE";

    printf("Writing graph to %s\n", fileName);
    FILE* f = fopen(fileName, "w");
    struct writeClbkData clbkData;

    clbkData.file = f;
    clbkData.SG = SG;

    writeHeader(f);

    graphMapNodes(SG->G, writeNodeClbk, &clbkData);
    graphMapEdges(SG->G, writeEdgeClbk, &clbkData);

    writeFooter(f);

    fclose(f);
}
