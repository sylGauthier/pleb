#include <stdlib.h>
#include <stdio.h>

#include "social_generation.h"
#include "rand_tools.h"

static void createCouple(SocialGraph* SG, Node* n1, Node* n2)
{
    struct relationAttrib ra1;
    struct relationAttrib ra2;

    struct nodeAttrib* na1 = n1->attribute;
    struct nodeAttrib* na2 = n2->attribute;
 
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

    socialAddRelation(SG, n1, n2, ra1);
    socialAddRelation(SG, n2, n1, ra2);
}

static int randBinom(int n)
{
    return rand()%n + rand()%n;
}

void generatePeople(SocialGraph* SG, int nb)
{
    int i = 0;
    struct nodeAttrib na;

    for (i = 0; i < nb; i++)
    {
        randID(SG, &na.ID);
        randPers(&na.pers);

        socialAddNode(SG, na);
    }
}

static void affiliate(SocialGraph* SG, Node* child, Node* parent)
{
    struct nodeAttrib* naParent = parent->attribute;
    struct nodeAttrib* naChild = child->attribute;

    /*Here we propagate the name through all the generations starting with the
     *child who's being affiliated
     */
    List progeny = NULL;

    Node* ch = child;
    listPush(&progeny, ch);

    while (progeny)
    {
        Node* cur = listPop(&progeny);
        Node* m8 = socialIsMated(SG, cur);
        struct nodeAttrib* naCur = cur->attribute;

        if (m8 == NULL || naChild->ID.sex == MALE)
        {
            naCur->ID.lastName = naParent->ID.lastName;

            if (m8 != NULL)
            {
                struct nodeAttrib* naWife = m8->attribute;
                naWife->ID.lastName = naCur->ID.lastName;
            }

            /*TODO here, update knowAbout*/
        }

        List relations = cur->edges;

        /*We add all children of the current child to update their name.*/
        while (relations)
        {
            Edge* curEdge = relations->elem;
            struct relationAttrib* ra = curEdge->attribute;

            if (ra->familyRel == CHILD)
            {
                Node* e = curEdge->to;
                listPush(&progeny, e);
            }

            relations = relations->next;
        }
    }

    struct relationAttrib childToParent;
    struct relationAttrib parentToChild;

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

    socialAddRelation(SG, child, parent, childToParent);
    socialAddRelation(SG, parent, child, parentToChild);
}

static void integrateInSiblings(SocialGraph* SG, Node* child, Node* parent)
{
    List l = parent->edges;
    struct nodeAttrib* newSib = child->attribute;

    /*We search every other child of the parent...*/
    while (l)
    {
        Edge* e = listPop(&l);
        struct relationAttrib* ra = e->attribute;

        /*And when we find one we create a link with the new child*/
        if (ra->familyRel == CHILD)
        {
            int i = 0;

            Node* curSib = e->to;
            struct nodeAttrib* curSibAttr = curSib->attribute;

            for (i = 0; i < 2; i++)
            {
                struct relationAttrib newra;

                newra.familyRel = SIBLING;
                newra.perc.allegiance = rand()%4;
                newra.perc.attachment = 5 + rand()%6;
                newra.perc.love = 0;

                if (i)
                {
                    newra.knowAbout = newSib->ID;
                    socialAddRelation(SG, curSib, child, newra);
                }
                else
                {
                    newra.knowAbout = curSibAttr->ID;
                    socialAddRelation(SG, child, curSib, newra);
                }
            }
        }
    }
}

static int canBeParent(SocialGraph* SG, Node* child, Node* parent)
{
    struct nodeAttrib* naParent = parent->attribute;
    struct nodeAttrib* naChild = child->attribute;

    if (naParent->ID.age < 18)
        return 0;

    if (naParent->ID.age - naChild->ID.age < 18 || naParent->ID.age - naChild->ID.age > 45)
        return 0;

    List l = child->edges;

    /*To avoid having multiple unrelated parents*/
    while (l)
    {
        Edge* e = l->elem;
        struct relationAttrib* ra = e->attribute;

        /*If the node already has a parent...*/
        if (ra->familyRel == PARENT)
        {
            return 0;
        }

        l = l->next;
    }

    Node* m8 = socialIsMated(SG, child);

    /*To avoid interbreeding...*/
    if (m8) /*If the child has a soulmate*/
    {
        l = parent->edges;

        /*Well we better make sure it ain't one of his/her siblings...*/
        while (l)
        {
            Edge* e = l->elem;
            struct relationAttrib* ra = e->attribute;

            /*If the current neighbour of the parent happens to be one of his/her child*/
            /*And if this child happens to be the soulmate of the current tested child...*/
            if (ra->familyRel == CHILD && e->to == m8)
            {
                return 0; /*Youre goddamn right they can't be related, hell this isn't England.*/
            }
        }
    }

    return 1;
}

static void generateFamiliesClbk(Node* child, void* socialGraph)
{
    SocialGraph* SG = socialGraph;
    struct nodeAttrib* na = child->attribute;
    int affiliated = 0;
    int nbPeople = SG->G->nbNodes;
    int parentID = rand()%nbPeople;

    do
    {
        Node* parent = SG->G->nodes->data[parentID];

        if (canBeParent(SG, child, parent))
        {
            Node* m8 = socialIsMated(SG, parent);

            integrateInSiblings(SG, child, parent);
            affiliate(SG, child, parent);
            affiliated = 1;

            /*If the selected potential parent is in couple, we also affiliate his/her partner to the child*/
            if (m8)
                affiliate(SG, child, m8);
        }
        /*So we are sure we find a suitable parent for children in a finite time*/
        parentID = (parentID + 1) % nbPeople;

    } while (na->ID.age <= 18 && !affiliated); /*That way there can't be any orphan child.*/
}

int generateFamilies(SocialGraph* SG)
{
    printf("Creating couples...\n");

    int count = 0;
    int collid = 0;
    int nbPeople = SG->G->nbNodes;
    int pathIndex = 0;

    randShuffle(SG->G->nodes);

    /*Creates couples*/
    /*TODO optimize this. Around 150,000 rejected solutions for 2500 couples...*/
    /*TODO make sure the loop actually stops at some point lol.*/
    while (count < nbPeople/4) /*About 50% of french population is in a relationship according to INSEE*/
    {
        Node* j = vectorAt(SG->G->nodes, pathIndex%nbPeople);
        Node* k = vectorAt(SG->G->nodes, rand()%nbPeople);
        struct nodeAttrib* na1 = j->attribute;
        struct nodeAttrib* na2 = k->attribute;

        /*A healthy family can only exist with a dad AND a mom :^)*/
        if (na1->ID.sex != na2->ID.sex && abs(na1->ID.age - na2->ID.age) + randBinom(3) < 10
                && na1->ID.age - randBinom(6) >= 18 && na2->ID.age - randBinom(6) >= 18
                && socialIsMated(SG, j) == NULL && socialIsMated(SG, k) == NULL)
        {
            createCouple(SG, k, j);
            count++;
        }
        else
            collid++;

        pathIndex++;
    }

    printf("%d colliding matches\n", collid);
    printf("Creating families...\n");

    /*Creates families, ie associates some children with some parents according to a set of constraints*/
    /*There can be single parents*/
    graphMapNodes(SG->G, generateFamiliesClbk, SG);

    return count;
}

static void assignPositionClbk(Node* n, void* socialGraph)
{
    SocialGraph* SG = socialGraph;

    struct nodeAttrib* curNode = n->attribute;
    int nbCommunities = SG->CM->communities->count;

    /*For each communities, we scan all the associated positions and find one that matches*/
    /*the current node*/

    int i = 0;
    int positionned = 0; /*Temporary, we start with only one position, of type "work"*/

    for (i = 0; i < nbCommunities && !positionned; i++)
    {
        struct community* curCom = vectorAt(SG->CM->communities, rand()%nbCommunities);

        int j = 0;

        for (j = 0; j < curCom->nbPositions && !positionned; j++)
        {
            struct position* curPos = &(curCom->positions[j]);

            if (curPos->people->count < curPos->nbPeople
                    && curNode->ID.age >= curPos->minAge
                    && curNode->ID.age <= curPos->maxAge)
            {
                vectorPush(curPos->people, n);
                vectorPush(curNode->positions, curPos);

                positionned = 1; /*Temporary*/
            }
        }
    }
}

int generateCommunities(SocialGraph* SG)
{
    printf("Starting communities generation...\n");

    printf("Assigning positions to nodes...\n");

    /*We shuffle the node vector to avoid any statistical bias*/
    randShuffle(SG->G->nodes);
    /*We map the previous callback function to every node*/
    graphMapNodes(SG->G, assignPositionClbk, SG);

    return 0;
}
