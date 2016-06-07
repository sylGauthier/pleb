#include <stdlib.h>
#include <stdio.h>

#include "social_graph.h"

struct SocialGraph
{
    Graph* G;
};

SocialGraph* socialNew()
{
    SocialGraph* sg = malloc(sizeof(SocialGraph));
    sg->G = graphNew();
    return sg;
}

int socialAddNode(SocialGraph* SG, struct nodeAttrib attrib)
{
    struct nodeAttrib* na = malloc(sizeof(struct nodeAttrib));

    if (na)
    {
        memcpy(na, &attrib, sizeof(struct nodeAttrib));
        na->nodeID =  graphAddNode(SG->G, na);

        return na->nodeID;
    }
    else
    {
        fprintf(stderr, "Error ! Could not allocate memory for node attribute !\n");
        return -1;
    }
}

int socialAddEdge(SocialGraph* SG, struct relationAttrib attrib)
{
    struct relationAttrib* ra = malloc(sizeof(struct relationAttrib));

    if (na)
    {
        memcpy(ra, &attrib, sizeof(struct relationAttrib));
        ra->nodeID =  graphAddEdge(SG->G, ra);

        return ra->edgeID;
    }
    else
    {
        fprintf(stderr, "Error ! Could not allocate memory for edge attribute !\n");
        return -1;
    }
}

void printPeopleCallback(void* attr, void* data)
{
    struct nodeAttrib* na = attr;
    struct edgeAttrib* ea = NULL;
    SocialGraph* sg = data;

    printf("### Individual ###\nName: %d\nPhone: %d\nAddress: %d\nIntelligence: %d\nEmotivity: %d\n",
            na->ID.firstName, na->ID.phone, na->ID.address, na->pers.intelligence, na->pers.emotionality);

    List nb = graphGetEdgesFrom(sg->G, na->nodeID);

    int* curNB;

    while (nb)
    {
        curNB = listPop(&nb);
        edgeAttrib = graphGetEdgeAttribute(sg->G, *nb);
        printf("-> %d, knows phone: %d, knows address: %d, knows email: %d\n",
                graphGetNodeTo(sg->G, *curNB), edgeAttrib->KA->phone, edgeAttrib->KA->address, edgeAttrib->KA->email);
        printf("Allegiance: %d, Attachment: %d\n", edgeAttrib->P->allegiance, edgeAttrib->P->attachment);

        free(curNB);
    }

    printf("\n\n#############\n");
}

void socialPrintPeople(SocialGraph* SG)
{
    printf("Printing social graph...\n");
    graphMapNodes(SG->G, printPeopleCallback, SG);
}

void freeCallback(void* attr, void* data)
{
    free(attr);
}

void socialFree(SocialGraph* sg)
{
    printf("Freeing social graph...\n");
    graphMapNodes(sg->G, freeCallback, NULL);
    graphMapEdges(sg->G, freeCallback, NULL);
    graphFree(sg->G);
    vectorMap(sg->firstNames, freeCallback, NULL);
    vectorMap(sg->lastNames, freeCallback, NULL);
    free(sg);
    printf("Done\n");
}
