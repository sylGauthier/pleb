#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "social_graph.h"

SocialGraph* socialNew()
{
    SocialGraph* sg = malloc(sizeof(SocialGraph));
    sg->G = graphNew();

    sg->NM = nameNewManager();
    sg->CM = communityNewManager();

    return sg;
}

int socialAddNode(SocialGraph* SG, struct nodeAttrib attrib)
{
    struct nodeAttrib* na = malloc(sizeof(struct nodeAttrib));

    if (na)
    {
        memcpy(na, &attrib, sizeof(struct nodeAttrib));
        na->nodeID =  graphAddNode(SG->G, na);
        na->positions = vectorNew();

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

    printf("### Individual ###\nID: %d\nName: %s %s\nAge: %d\nSex: %s\n",
            na->nodeID, nameGetFirstName(sg->NM, na->ID.firstName), nameGetLastName(sg->NM, na->ID.lastName),
            na->ID.age, na->ID.sex ? "Female" : "Male");

    if (na->positions->count > 0)
    {
        struct position* pos = na->positions->data[0];
        struct community* com = sg->CM->communities->data[pos->communityID];

        printf("Works at %s as %s\n", com->specificName, pos->name);
    }

    List nb = graphGetEdgesFrom(sg->G, na->nodeID);

    int* curNB;

    while (nb)
    {
        curNB = listPop(&nb);
        ea = graphGetEdgeAttribute(sg->G, *curNB);
        printf("-> %d as ", graphGetNodeTo(sg->G, *curNB));

        switch (ea->familyRel)
        {
            case PARENT :
                printf("Parent\n");
                break;

            case SIBLING :
                printf("Sibling\n");
                break;

            case CHILD :
                printf("Child\n");
                break;

            case COUPLE :
                printf("Couple\n");
                break;

            default :
                printf("Acquaintance\n");
                break;
        }

        printf("Allegiance: %d, Attachment: %d\n", ea->perc.allegiance, ea->perc.attachment);
    }

    printf("##################\n\n");
}

void socialPrintPeople(SocialGraph* SG)
{
    printf("Printing social graph...\n\n");
    graphMapNodes(SG->G, printPeopleCallback, SG);
}

void socialPrintNode(SocialGraph* SG, int ID)
{
    struct nodeAttrib* na = graphGetNodeAttribute(SG->G, ID);

    printPeopleCallback(na, SG);
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

void freeNodeCallback(void* attr, void* data)
{
    struct nodeAttrib* cur = attr;
    //No listFlush because positions are freed with the freeCommunities method
    vectorFree(cur->positions);
    free(cur);
}

void socialFree(SocialGraph* sg)
{
    printf("Freeing social graph...\n");
    graphMapNodes(sg->G, freeNodeCallback, NULL);
    graphMapEdges(sg->G, freeCallback, NULL);
    graphFree(sg->G);
    
    nameFreeManager(sg->NM);
    communityFree(sg->CM);

    free(sg);
    printf("Done\n");
}
