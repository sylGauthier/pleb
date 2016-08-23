#include <stdlib.h>
#include <stdio.h>

#include "social_generation.h"

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

    nameRandName(&(SG->NM), 0, ID->sex, &(ID->firstName), &(ID->lastName));
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

    ra1.knowAbout = ((struct nodeAttrib*)graphGetNodeAttribute(SG->G, n2))->ID;
    ra2.knowAbout = ((struct nodeAttrib*)graphGetNodeAttribute(SG->G, n1))->ID;

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

    int nbActives = SG->G->nbNodes;

    //for (i = 0; i < 6*nbActives; i++)
    //TODO optimize this. Around 150,000 rejected solutions for 2500 couples...
    //TODO make sure the loop actually stops at some point lol.
    while (count < nbActives/4) //About 50% of french population is in a relationship according to INSEE
    {
        int j = rand() % nbActives;
        int k = rand() % nbActives;


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
    }

    printf("Creating families...\n");

    int k = 0;
    int j = 0;

    for (j = 0; j < 3; j++)
    {
        for (k = 0; k < nbActives; k++)
        {
            int i = rand() % nbActives;
            struct nodeAttrib* na = graphGetNodeAttribute(SG->G, i);
            int count = 0;

            do
            {
                int parentID = rand()%nbActives;
                struct nodeAttrib* na2 = graphGetNodeAttribute(SG->G, parentID);

                if (canBeParent(SG, i, parentID))
                {
                    int m8 = socialIsMated(SG, parentID);

                    integrateInSiblings(SG, i, parentID);
                    affiliate(SG, i, parentID);

                    //If the selected potential parent is in couple, we also affiliate his/her partner to the child
                    if (m8 != -1)
                        affiliate(SG, i, m8);
                }

                count++;
            } while (na->ID.age <= 18 && count < 10); //Gives underage kids 10 times more chance to have parents...
        }
    }

    printf("%d colliding matches\n", collid);
    return count;
}
