#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "social_graph.h"

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

int socialAddRelation(SocialGraph* SG, int nodeFrom, int nodeTo, struct relationAttrib attrib)
{
    struct relationAttrib* ra = malloc(sizeof(struct relationAttrib));

    if (ra)
    {
        memcpy(ra, &attrib, sizeof(struct relationAttrib));
        ra->edgeID = graphAddEdge(SG->G, nodeFrom, nodeTo, ra);

        return ra->edgeID;
    }
    else
    {
        fprintf(stderr, "Error ! Could not allocate memory for edge attribute !\n");
        return -1;
    }
}

int socialIsMated(SocialGraph* SG, int n)
{
    List nbs = graphGetEdgesFrom(SG->G, n);

    while (nbs)
    {
        int* edgeIndex = listPop(&nbs);
        struct relationAttrib* ea = graphGetEdgeAttribute(SG->G, *edgeIndex);

        if (ea->familyRel == COUPLE)
        {
            listFree(&nbs);
            return graphGetNodeTo(SG->G, ea->edgeID);
        }
    }

    return -1;
}

static void printPeopleCallback(void* attr, void* data)
{
    struct nodeAttrib* na = attr;
    struct relationAttrib* ea = NULL;
    SocialGraph* sg = data;

    printf("### Individual ###\nID: %d\nName: %d\nAge: %d\nSex: %s\nPhone: %d\nAddress: %d\nIntelligence: %d\nEmotivity: %d\n",
            na->nodeID, na->ID.firstName, na->ID.age, na->ID.sex ? "Female" : "Male", na->ID.phone, na->ID.address,
            na->pers.intelligence, na->pers.emotionality);

    List nb = graphGetEdgesFrom(sg->G, na->nodeID);

    int* curNB;

    while (nb)
    {
        curNB = listPop(&nb);
        ea = graphGetEdgeAttribute(sg->G, *curNB);
        printf("-> %d, knows phone: %d, knows address: %d, knows email: %d\n",
                graphGetNodeTo(sg->G, *curNB), ea->knowAbout.phone, ea->knowAbout.address, ea->knowAbout.email);
        printf("Allegiance: %d, Attachment: %d\n", ea->perc.allegiance, ea->perc.attachment);

        free(curNB);
    }

    printf("##################\n\n");
}

void socialPrintPeople(SocialGraph* SG)
{
    printf("Printing social graph...\n\n");
    graphMapNodes(SG->G, printPeopleCallback, SG);
}

static void buildAgePyramidCallback(void* attr, void* pyramid)
{
    int* pyr = pyramid;
    struct nodeAttrib* na = attr;

    pyr[na->ID.age]++;
}

void printAgePyramid(SocialGraph* SG)
{
    int pyramid[110] = {0};
    int i = 0;

    graphMapNodes(SG->G, buildAgePyramidCallback, pyramid);

    for (i = 0; i < 110; i++)
    {
        printf("%2d : ", i);
        int j = 0;
        for (j = 0; j < pyramid[i]; j++)
        {
            printf("*");
        }
        printf("\n");
    }
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
    //vectorMap(sg->firstNames, freeCallback, NULL);
    //vectorMap(sg->lastNames, freeCallback, NULL);
    free(sg);
    printf("Done\n");
}
