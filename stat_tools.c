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
