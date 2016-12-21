#include <stdio.h>

#include "stat_tools.h"

static void countOrphansClbk(Node* curNode, void* cpt)
{
    int* c = cpt;
    struct nodeAttrib* na = curNode->attribute;

    if (na->ID.age <= 18)
    {
        List l = curNode->edges;

        (*c)++;

        while (l)
        {
            Edge* e = l->elem;
            struct relationAttrib* ra = e->attribute;

            if (ra->familyRel == PARENT)
            {
                (*c)--;
                break;
            }

            l = l->next;
        }
    }
}

int statCountOrphanKids(SocialGraph* SG)
{
    int c = 0;
    graphMapNodes(SG->G,countOrphansClbk, &c);
    return c;
}

void printUnassignedPositions(SocialGraph* SG)
{
    printf("Printing unassigned positions...\n");
    int i = 0;

    for (i = 0; i < SG->CM->communities->count; i++)
    {
        struct community* curCom = SG->CM->communities->data[i];

        int j = 0;

        for (j = 0; j < curCom->nbPositions; j++)
        {
            struct position curPos = curCom->positions[j];

            /*printf("Position: %s in community %s, %d people\n", curPos.name, curCom->genericName, curPos.people->count);*/

            if (curPos.people->count < curPos.nbPeople)
            {
                printf("Unassigned position: %s in community %s (%d/%d)\n",
                        curPos.name, curCom->genericName, curPos.people->count, curPos.nbPeople);
            }
        }
    }
}

struct countInactivesClbkData
{
    int* cpt;
    int minAge;
    int maxAge;
};

static void countInactivesClbk(Node* curNode, void* data)
{
    struct nodeAttrib* na = curNode->attribute;
    struct countInactivesClbkData* d = data;

    if (na->positions->count == 0 && na->ID.age >= d->minAge && na->ID.age <= d->maxAge)
        *(d->cpt) = *(d->cpt) + 1;
}

int nbInactives(SocialGraph* SG, int minAge, int maxAge)
{
    int cpt = 0;
    struct countInactivesClbkData data;

    data.cpt = &cpt;
    data.minAge = minAge;
    data.maxAge = maxAge;

    graphMapNodes(SG->G, countInactivesClbk, &data);
    return cpt;
}
