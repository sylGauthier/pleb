#include <stdlib.h>
#include <stdio.h>

#include "social_generation.h"
#include "rand_tools.h"

//Simulates very approximately the french age pyramid of 2014
static int randAge()
{
    int a = rand()%1001;

    if (a <= 840)
        return a / 12;
    else
    {
        int r = (a*a*96*8 - (a*97)*1536*8 + 3675*1536*96)/(1536*96*8);
        return r;
    }
}

void randID(SocialGraph* SG, struct identity* ID)
{
    ID->age = randAge();
    ID->sex = rand() % 2;

    nameRandName(SG->NM, FRENCH, ID->sex, &(ID->firstName), &(ID->lastName));
}

void randPers(struct personality* pers)
{
    *pers = (struct personality) {(rand()%5 + rand()%6) + 1, rand()%10 + 1, rand()%10 + 1};
}

void randPerc(struct perception* perc)
{
    *perc = (struct perception) {1,2};
}

static void createCouple(SocialGraph* SG, int n1, int n2)
{
    struct relationAttrib ra1;
    struct relationAttrib ra2;

    struct nodeAttrib* na1 = graphGetNodeAttribute(SG->G, n1);
    struct nodeAttrib* na2 = graphGetNodeAttribute(SG->G, n2);
 
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

int generateNode(SocialGraph* SG)
{
    struct nodeAttrib* na = malloc(sizeof(struct nodeAttrib));
    randID(SG, &na->ID);
    randPers(&na->pers);

    return graphAddNode(SG->G, na);
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

static void affiliate(SocialGraph* SG, int childID, int parentID)
{
    struct nodeAttrib* naParent = graphGetNodeAttribute(SG->G, parentID);
    struct nodeAttrib* naChild = graphGetNodeAttribute(SG->G, childID);

    //Here we propagate the name through all the generations starting with the
    //child who's being affiliated
    List progeny = NULL;

    int* ch = malloc(sizeof(int));
    *ch = childID;

    listPush(&progeny, ch);

    while (progeny)
    {
        int* cur = listPop(&progeny);
        int m8 = socialIsMated(SG, *cur);
        struct nodeAttrib* naCur = graphGetNodeAttribute(SG->G, *cur);

        if (m8 == -1 || naChild->ID.sex == MALE)
        {
            naCur->ID.lastName = naParent->ID.lastName;

            if (m8 != -1)
            {
                struct nodeAttrib* naWife = graphGetNodeAttribute(SG->G, m8);
                naWife->ID.lastName = naCur->ID.lastName;
            }

            //TODO here, update knowAbout
        }

        List relations = graphGetEdgesFrom(SG->G, *cur);

        //We add all children of the current child to update their name.
        while (relations)
        {
            int curEdge = *((int*) listPop(&relations));
            struct relationAttrib* ra = graphGetEdgeAttribute(SG->G, curEdge);

            if (ra->familyRel == CHILD)
            {
                int* e = malloc(sizeof(int));
                *e = graphGetNodeTo(SG->G, curEdge);
                listPush(&progeny, e);
            }
        }

        free(cur);
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

    socialAddRelation(SG, childID, parentID, childToParent);
    socialAddRelation(SG, parentID, childID, parentToChild);
}

static void integrateInSiblings(SocialGraph* SG, int childID, int parentID)
{
    List l = graphGetEdgesFrom(SG->G, parentID);
    struct nodeAttrib* newSib = graphGetNodeAttribute(SG->G, childID);

    //We search every other child of the parent...
    while (l)
    {
        int* e = listPop(&l);
        struct relationAttrib* ra = graphGetEdgeAttribute(SG->G, *e);

        //And when we find one we create a link with the new child
        if (ra->familyRel == CHILD)
        {
            int i = 0;

            struct nodeAttrib* curSib = graphGetNodeAttribute(SG->G, graphGetNodeTo(SG->G, ra->edgeID));

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
                    socialAddRelation(SG, curSib->nodeID, childID, newra);
                }
                else
                {
                    newra.knowAbout = curSib->ID;
                    socialAddRelation(SG, childID, curSib->nodeID, newra);
                }
            }
        }
    }
}

static int canBeParent(SocialGraph* SG, int childID, int parentID)
{
    struct nodeAttrib* naParent = graphGetNodeAttribute(SG->G, parentID);
    struct nodeAttrib* naChild = graphGetNodeAttribute(SG->G, childID);

    if (naParent->ID.age < 18)
        return 0;

    if (naParent->ID.age - naChild->ID.age < 18 || naParent->ID.age - naChild->ID.age > 45)
        return 0;

    List l = graphGetEdgesFrom(SG->G, childID);

    //To avoid having multiple unrelated parents
    while (l)
    {
        int* e = listPop(&l);
        struct relationAttrib* ra = graphGetEdgeAttribute(SG->G, *e);

        //If the node already has a parent...
        if (ra->familyRel == PARENT)
        {
            listFree(&l);
            return 0;
        }
    }

    int m8 = socialIsMated(SG, childID);

    //To avoid interbreeding...
    if (m8 != -1) //If the child has a soulmate
    {
        l = graphGetEdgesFrom(SG->G, parentID);

        //Well we better make sure it ain't one of his/her siblings...
        while (l)
        {
            int* e = listPop(&l);
            struct relationAttrib* ra = graphGetEdgeAttribute(SG->G, *e);

            //If the current neighbour of the parent happens to be one of his/her child
            //And if this child happens to be the soulmate of the current tested child...
            if (ra->familyRel == CHILD && graphGetNodeTo(SG->G, *e) == m8)
            {
                listFree(&l);
                return 0; //Youre goddamn right they can't be related, hell this isn't England.
            }
        }
    }

    return 1;
}

int generateFamilies(SocialGraph* SG)
{
    printf("Creating couples...\n");
    //int i = 0;
    int count = 0;
    int collid = 0;

    int nbPeople = SG->G->nbNodes;

    Vector* path1 = randRoute(nbPeople);
    int pathIndex = 0;

    //for (i = 0; i < 6*nbPeople; i++)
    //TODO optimize this. Around 150,000 rejected solutions for 2500 couples...
    //TODO make sure the loop actually stops at some point lol.
    while (count < nbPeople/4) //About 50% of french population is in a relationship according to INSEE
    {
        int j = *((int*)path1->data[pathIndex % nbPeople]);
        int k = *((int*)path1->data[rand() % nbPeople]);

        struct nodeAttrib* na1 = graphGetNodeAttribute(SG->G, k);
        struct nodeAttrib* na2 = graphGetNodeAttribute(SG->G, j);

        //A healthy family can only exist with a dad AND a mom :^)
        if (na1->ID.sex != na2->ID.sex && abs(na1->ID.age - na2->ID.age) + randBinom(3) < 10
                && na1->ID.age - randBinom(6) >= 18 && na2->ID.age - randBinom(6) >= 18
                && socialIsMated(SG, j) == -1 && socialIsMated(SG, k) == -1)
        {
            createCouple(SG, k, j);
            count++;
        }
        else
            collid++;

        pathIndex++;
    }

    vectorFlush(path1);
    vectorFree(path1);

    printf("%d colliding matches\n", collid);
    printf("Creating families...\n");

    int i = 0;
    //int j = 0;

    for (i = 0; i < nbPeople; i++)
    {
        //int i = rand() % nbPeople;
        struct nodeAttrib* na = graphGetNodeAttribute(SG->G, i);
        int affiliated = 0;
        int parentID = rand()%nbPeople;

        do
        {
            if (canBeParent(SG, i, parentID))
            {
                int m8 = socialIsMated(SG, parentID);

                integrateInSiblings(SG, i, parentID);
                affiliate(SG, i, parentID);
                affiliated = 1;

                //If the selected potential parent is in couple, we also affiliate his/her partner to the child
                if (m8 != -1)
                    affiliate(SG, i, m8);
            }
            //So we are sure we find a suitable parent for children in a finite time
            parentID = (parentID + 1) % nbPeople;

        } while (na->ID.age <= 18 && !affiliated); //That way there can't be any orphan child.
    }

    return count;
}
