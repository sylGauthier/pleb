#include <stdlib.h>
#include <stdio.h>

#include "social_generation.h"
#include "rand_tools.h"

static void create_couple(SocialGraph* SG, Node* n1, Node* n2) {
    struct RelationAttrib ra1;
    struct RelationAttrib ra2;
    struct NodeAttrib* na1 = n1->attribute;
    struct NodeAttrib* na2 = n2->attribute;
 
    if (na1->ID.sex == MALE)
        na2->ID.lastName = na1->ID.lastName;
    else
        na1->ID.lastName = na2->ID.lastName;
    
    ra1.knowAbout = na2->ID;
    ra2.knowAbout = na1->ID;

    ra1.familyRel = COUPLE;
    ra2.familyRel = COUPLE;

    ra1.perc.allegiance = rand()%7 + 4;
    ra2.perc.allegiance = rand()%7 + 4;

    ra1.perc.attachment = rand()%4 + 7;
    ra2.perc.attachment = rand()%4 + 7;

    social_add_relation(SG, n1, n2, ra1);
    social_add_relation(SG, n2, n1, ra2);
}

void generate_people(SocialGraph* SG, int nb) {
    int i = 0;
    struct NodeAttrib na;

    for (i = 0; i < nb; i++) {
        rand_id(SG, &na.ID);
        rand_pers(&na.pers);

        social_add_node(SG, na);
    }
}

static void affiliate(SocialGraph* SG, Node* child, Node* parent) {
    struct NodeAttrib* naParent = parent->attribute;
    struct NodeAttrib* naChild = child->attribute;
    List progeny = NULL;
    Node* ch = child;

    list_push(&progeny, ch);

    /* Here we propagate the name through all the generations starting with the
     * child who's being affiliated
     */
    while (progeny) {
        Node* cur = list_pop(&progeny);
        Node* m8 = social_is_mated(SG, cur);
        struct NodeAttrib* naCur = cur->attribute;
        List relations = cur->edges;

        if (m8 == NULL || naChild->ID.sex == MALE) {
            naCur->ID.lastName = naParent->ID.lastName;

            if (m8 != NULL) {
                struct NodeAttrib* naWife = m8->attribute;
                naWife->ID.lastName = naCur->ID.lastName;
            }
            /* TODO here, update knowAbout */
        }

        /* We add all children of the current child to update their name. */
        while (relations) {
            Edge* curEdge = relations->elem;
            struct RelationAttrib* ra = curEdge->attribute;

            if (ra->familyRel == CHILD) {
                Node* e = curEdge->to;
                list_push(&progeny, e);
            }

            relations = relations->next;
        }
    }

    { /* Setting up parent <-> child relations */
        struct RelationAttrib childToParent;
        struct RelationAttrib parentToChild;

        childToParent.familyRel = PARENT;
        childToParent.knowAbout = naParent->ID;
        childToParent.perc.allegiance = 4 + rand()%7;
        childToParent.perc.attachment = 7 + rand()%4;
        childToParent.perc.love = 0;

        parentToChild.familyRel = CHILD;
        parentToChild.knowAbout = naChild->ID;
        parentToChild.perc.allegiance = rand()%3;
        parentToChild.perc.attachment = 7 + rand()%4;
        parentToChild.perc.love = 0;

        social_add_relation(SG, child, parent, childToParent);
        social_add_relation(SG, parent, child, parentToChild);
    }
}

static void integrate_in_siblings(SocialGraph* SG, Node* child, Node* parent) {
    List l = parent->edges;
    struct NodeAttrib* newSib = child->attribute;

    /* We search every other child of the parent... */
    while (l) {
        Edge* e = l->elem;
        struct RelationAttrib* ra = e->attribute;

        /* And when we find one we create a link with the new child */
        if (ra->familyRel == CHILD) {
            int i = 0;
            Node* curSib = e->to;
            struct NodeAttrib* curSibAttr = curSib->attribute;

            for (i = 0; i < 2; i++) {
                struct RelationAttrib newra;

                newra.familyRel = SIBLING;
                newra.perc.allegiance = rand()%4;
                newra.perc.attachment = 5 + rand()%6;
                newra.perc.love = 0;

                if (i) {
                    newra.knowAbout = newSib->ID;
                    social_add_relation(SG, curSib, child, newra);
                } else {
                    newra.knowAbout = curSibAttr->ID;
                    social_add_relation(SG, child, curSib, newra);
                }
            }
        }
        l = l->next;
    }
}

static int can_be_parent(SocialGraph* SG, Node* child, Node* parent) {
    struct NodeAttrib* naParent = parent->attribute;
    struct NodeAttrib* naChild = child->attribute;
    List l = child->edges;
    Node* m8 = social_is_mated(SG, child);

    if (naParent->ID.age < 18)
        return 0;

    if (naParent->ID.age - naChild->ID.age < 18
            || naParent->ID.age - naChild->ID.age > 45)
        return 0;

    /* To avoid having multiple unrelated parents */
    while (l) {
        Edge* e = l->elem;
        struct RelationAttrib* ra = e->attribute;

        /* If the node already has a parent... */
        if (ra->familyRel == PARENT) {
            return 0;
        }
        l = l->next;
    }

    /* To avoid interbreeding...
     * If the child has a soulmate
     */
    if (m8) { 
        l = parent->edges;

        /* Well we better make sure it ain't one of his/her siblings... */
        while (l) {
            Edge* e = l->elem;
            struct RelationAttrib* ra = e->attribute;

            /* If the current neighbour of the parent happens to be one of
             * his/her child and if this child happens to be the soulmate
             * of the current tested child...
             */
            if (ra->familyRel == CHILD && e->to == m8) {
                /* Youre goddamn right they can't be related, hell this isn't England. */
                return 0; 
            }
            l = l->next;
        }
    }
    return 1;
}

static void generate_families_clbk(Node* child, void* socialGraph) {
    SocialGraph* SG = socialGraph;
    struct NodeAttrib* na = child->attribute;
    int affiliated = 0;
    int nbPeople = SG->G->nbNodes;
    int parentID = rand()%nbPeople;

    do {
        Node* parent = vector_at(SG->G->nodes, parentID);

        if (can_be_parent(SG, child, parent)) {
            Node* m8 = social_is_mated(SG, parent);

            integrate_in_siblings(SG, child, parent);
            affiliate(SG, child, parent);
            affiliated = 1;

            /* If the selected potential parent is in couple, we also
             * affiliate his/her partner to the child
             */
            if (m8)
                affiliate(SG, child, m8);
        }
        /* So we are sure we find a suitable parent for children in a finite time */
        parentID = (parentID + 1) % nbPeople;
    } while (na->ID.age <= 18 && !affiliated);
    /* That way there can't be any orphan child. */
}

int generate_families(SocialGraph* SG) {
    int count = 0;
    int collid = 0;
    int nbPeople = SG->G->nbNodes;
    int pathIndex = 0;

    printf("Creating couples...\n");
    rand_shuffle(SG->G->nodes);

    /* Creates couples
     * TODO optimize this. Around 150,000 rejected solutions for 2500 couples...
     * TODO make sure the loop actually stops at some point lol.
     */
    while (count < nbPeople/4) {
        Node* j = vector_at(SG->G->nodes, pathIndex%nbPeople);
        Node* k = vector_at(SG->G->nodes, rand()%nbPeople);
        struct NodeAttrib* na1 = j->attribute;
        struct NodeAttrib* na2 = k->attribute;

        /* A healthy family can only exist with a dad AND a mom :^) */
        if (na1->ID.sex != na2->ID.sex
                && abs(na1->ID.age - na2->ID.age) + rand_binom(3) < 10
                && na1->ID.age - rand_binom(6) >= 18 && na2->ID.age - rand_binom(6) >= 18
                && social_is_mated(SG, j) == NULL && social_is_mated(SG, k) == NULL) {
            create_couple(SG, k, j);
            count++;
        } else
            collid++;
        pathIndex++;
    }
    printf("%d colliding matches\n", collid);
    printf("Creating families...\n");

    /* Creates families, ie associates some children with some parents according
     * to a set of constraints. There can be single parents
     */
    graph_map_nodes(SG->G, generate_families_clbk, SG);
    printf("Done\n");
    return count;
}

static void assign_position_clbk(Node* n, void* socialGraph) {
    SocialGraph* SG = socialGraph;
    struct NodeAttrib* curNode = n->attribute;
    int nbCommunities = SG->CM->communities->count;
    int i = 0;
    int positionned = 0;

    /* For each communities, we scan all the associated positions and
     * find one that matches the current node
     */

    for (i = 0; i < nbCommunities && !positionned; i++) {
        struct Community* curCom = vector_at(SG->CM->communities, rand()%nbCommunities);
        int j = 0;

        for (j = 0; j < curCom->nbPositions && !positionned; j++) {
            struct Position* curPos = &(curCom->positions[j]);

            if (curPos->people->count < curPos->nbPeople
                    && curNode->ID.age >= curPos->minAge
                    && curNode->ID.age <= curPos->maxAge) {
                vector_push(curPos->people, n);
                vector_push(curNode->positions, curPos);
                positionned = 1; /* Temporary */
            }
        }
    }
}

int generate_communities(SocialGraph* SG) {
    printf("Starting communities generation...\n");
    printf("Assigning positions to nodes...\n");

    /* We shuffle the node vector to avoid any statistical bias */
    rand_shuffle(SG->G->nodes);
    /* We map the previous callback function to every node */
    graph_map_nodes(SG->G, assign_position_clbk, SG);

    return 0;
}
