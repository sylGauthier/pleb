#include <stdio.h>

#include "stat_tools.h"

static void count_orphans_clbk(Node* curNode, void* cpt) {
    int* c = cpt;
    struct NodeAttrib* na = curNode->attribute;

    if (na->ID.age <= 18) {
        List l = curNode->edges;

        (*c)++;

        while (l) {
            Edge* e = l->elem;
            struct RelationAttrib* ra = e->attribute;

            if (ra->familyRel == PARENT) {
                (*c)--;
                break;
            }

            l = l->next;
        }
    }
}

int stat_count_orphan_kids(SocialGraph* SG) {
    int c = 0;

    graph_map_nodes(SG->G,count_orphans_clbk, &c);
    return c;
}

void print_unassigned_positions(SocialGraph* SG) {
    int i = 0;

    printf("Printing unassigned positions...\n");

    for (i = 0; i < SG->CM->communities->count; i++) {
        struct Community* curCom = vector_at(SG->CM->communities, i);
        int j = 0;

        for (j = 0; j < curCom->nbPositions; j++) {
            struct Position curPos = curCom->positions[j];

            if (curPos.people->count < curPos.nbPeople) {
                printf("Unassigned position: %s in community %s (%d/%d)\n",
                        curPos.name, curCom->genericName, curPos.people->count,
                        curPos.nbPeople);
            }
        }
    }
}

struct CountInactivesClbkData {
    int* cpt;
    int minAge;
    int maxAge;
};

static void count_inactives_clbk(Node* curNode, void* data) {
    struct NodeAttrib* na = curNode->attribute;
    struct CountInactivesClbkData* d = data;

    if (na->positions->count == 0 && na->ID.age >= d->minAge && na->ID.age <= d->maxAge)
        *(d->cpt) = *(d->cpt) + 1;
}

int nb_inactives(SocialGraph* SG, int minAge, int maxAge) {
    int cpt = 0;
    struct CountInactivesClbkData data;

    data.cpt = &cpt;
    data.minAge = minAge;
    data.maxAge = maxAge;

    graph_map_nodes(SG->G, count_inactives_clbk, &data);
    return cpt;
}
