#include <stdio.h>

#include "save_graph.h"

char* familyRelNames[10];

static void write_header(FILE *f)
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

static void write_footer(FILE *f)
{
    fprintf(f, "</graph>\n</graphml>");
}

struct WriteClbkData
{
    FILE* file;
    SocialGraph* SG;
};

static void write_node_clbk(Node* node, void* data)
{
    struct WriteClbkData* cd = data;
    struct NodeAttrib* na = node->attribute;

    fprintf(cd->file, "<node id=\"n%d\">\n"
            "<data key=\"d0\">blue</data>\n"
            "<data key=\"d1\">%d</data>\n"
            "<data key=\"d3\">%s</data>\n"
            "<data key=\"d4\">%s</data>\n"
            "</node>\n", na->nodeID, na->ID.age, name_get_first_name(cd->SG->NM, na->ID.firstName), name_get_last_name(cd->SG->NM, na->ID.lastName));
}

static void write_edge_clbk(Edge* edge, void* data)
{
    struct WriteClbkData* cd = data;
    struct RelationAttrib* ra = edge->attribute;
    if (ra->familyRel != CHILD)
        fprintf(cd->file, "<edge id=\"e%d\" source=\"n%d\" target=\"n%d\">\n  <data key=\"d2\"> %s </data>\n</edge>\n",
                ra->edgeID, edge->from->ID, edge->to->ID, familyRelNames[ra->familyRel]);
}

void save_graph(SocialGraph* SG, const char* fileName)
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
    struct WriteClbkData clbkData;

    clbkData.file = f;
    clbkData.SG = SG;

    write_header(f);

    graph_map_nodes(SG->G, write_node_clbk, &clbkData);
    graph_map_edges(SG->G, write_edge_clbk, &clbkData);

    write_footer(f);

    fclose(f);
}
