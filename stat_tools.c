#include <stdio.h>

#include "stat_tools.h"

int statCountOrphanKids(SocialGraph* SG)
{
    int i = 0;
    int c = 0;

    for (i = 0; i < SG->G->nbNodes; i++)
    {
        struct nodeAttrib* na = graphGetNodeAttribute(SG->G, i);

        if (na->ID.age <= 18)
        {
            List l = graphGetEdgesFrom(SG->G, i);

            c++;

            while (l)
            {
                int* e = listPop(&l);
                struct relationAttrib* ra = graphGetEdgeAttribute(SG->G, *e);

                if (ra->familyRel == PARENT)
                {
                    c--;
                    listFree(&l);
                    break;
                }
            }
        }
    }

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

            //printf("Position: %s in community %s, %d people\n", curPos.name, curCom->genericName, curPos.people->count);

            if (curPos.people->count == 0)//< curPos.nbPeople)
            {
                printf("Unassigned position: %s in community %s\n", curPos.name, curCom->genericName);
            }
        }
    }
}
