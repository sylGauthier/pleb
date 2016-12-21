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

Node* socialAddNode(SocialGraph* SG, struct nodeAttrib attrib)
{
    struct nodeAttrib* na = malloc(sizeof(struct nodeAttrib));

    if (na)
    {
        memcpy(na, &attrib, sizeof(struct nodeAttrib));
        na->positions = vectorNew();

        return graphAddNode(SG->G, na);
    }
    else
    {
        fprintf(stderr, "Error ! Could not allocate memory for node attribute !\n");
        return NULL;
    }
}

Edge* socialAddRelation(SocialGraph* SG, Node* fromPtr, Node* toPtr, struct relationAttrib attrib)
{
    struct relationAttrib* ra = malloc(sizeof(struct relationAttrib));

    if (ra)
    {
        memcpy(ra, &attrib, sizeof(struct relationAttrib));

        return graphAddEdge(SG->G, fromPtr, toPtr, ra);
    }
    else
    {
        fprintf(stderr, "Error ! Could not allocate memory for edge attribute !\n");
        return NULL;
    }
}

Node* socialIsMated(SocialGraph* SG, Node* node)
{
    List nbs = node->edges;

    while (nbs)
    {
        Edge* curEdge = nbs->elem;
        struct relationAttrib* ea = curEdge->attribute;

        if (ea->familyRel == COUPLE)
        {
            return curEdge->to;
        }

        nbs = nbs->next;
    }

    return NULL;
}

static void printPeopleClbk(Node* node, void* data)
{
    struct nodeAttrib* na = node->attribute;
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
/*
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
*/
    printf("##################\n\n");
}

void socialPrintPeople(SocialGraph* SG)
{
    printf("Printing social graph...\n\n");
    graphMapNodes(SG->G, printPeopleClbk, SG);
}

void socialPrintNode(SocialGraph* SG, Node* node)
{
    printPeopleClbk(node, SG);
}

static void buildAgePyramidClbk(Node* node, void* pyramid)
{
    int* pyr = pyramid;
    struct nodeAttrib* na = node->attribute;

    pyr[na->ID.age]++;
}

void printAgePyramid(SocialGraph* SG)
{
    int pyramid[110] = {0};
    int i = 0;

    graphMapNodes(SG->G, buildAgePyramidClbk, pyramid);

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

static void freeEdgeAttrClbk(Edge* edge, void* data)
{
    struct relationAttrib* cur = edge->attribute;
    free(cur);
}

static void freeNodeAttrClbk(Node* node, void* data)
{
    struct nodeAttrib* cur = node->attribute;
    /*No listFlush because positions are freed with the freeCommunities method*/
    vectorFree(cur->positions);
    free(cur);
}

void socialFree(SocialGraph* sg)
{
    printf("Freeing social graph...\n");
    printf("  ->Freeing nodes...\n");
    graphMapNodes(sg->G, freeNodeAttrClbk, NULL);
    printf("  ->Freeing edges...\n");
    graphMapEdges(sg->G, freeEdgeAttrClbk, NULL);
    printf("  ->Freeing graph...\n");
    graphFree(sg->G);
    
    nameFreeManager(sg->NM);
    communityFree(sg->CM);

    free(sg);
    printf("Done\n");
}
