#include <stdlib.h>
#include <stdio.h>

#include "social_generation.h"

void randID(struct identity* ID)
{
    *ID = (struct identity) {1,2,3,4,5,6,7};
}

void randPers(struct personality* pers)
{
    *pers = (struct personality) {(rand()%5 + rand()%6) + 1, rand()%10 + 1, rand()%10 + 1};
}

void randPerc(struct perception* perc)
{
    *perc = (struct perception) {1,2};
}

static void createChild(SocialGraph* SG, int p1, int p2, int childAge)
{
    struct nodeAttrib* np1 = ((struct nodeAttrib*)graphGetNodeAttribute(SG->G, p1));
    struct nodeAttrib* np2 = ((struct nodeAttrib*)graphGetNodeAttribute(SG->G, p2));

    struct nodeAttrib child;
    struct relationAttrib childRA1;
    struct relationAttrib childRA2;
    struct relationAttrib childRA3;

    child.ID.age = childAge;
    child.ID.sex = rand() % 2;
    child.ID.firstName = 3; //TODO
    child.ID.lastName = 3;  //TODO
    child.pers.intelligence = rand() % 4;
    child.pers.emotionality = rand() % 7;
    child.pers.sexuality = 0;

    childRA1.knowAbout = np1->ID;
    childRA1.perc.attachment = rand()%3 + 7;
    childRA1.perc.allegiance = rand()%3 + 7;

    childRA2.knowAbout = np2->ID;
    childRA2.perc.attachment = rand()%3 + 7;
    childRA2.perc.allegiance = rand()%3 + 7;

    childRA3.knowAbout = child.ID;
    childRA2.perc.attachment = rand()%3 + 7;
    childRA2.perc.allegiance = rand()%3;

    int childID = socialAddNode(SG, child);
    printf("Added child %d, aged %d, of sex %d\n", childID, child.ID.age, child.ID.sex);
    socialAddRelation(SG, childID, p1, childRA1);
    socialAddRelation(SG, childID, p2, childRA2);
    /*socialAddRelation(SG, p1, childID, childRA3);
    socialAddRelation(SG, p2, childID, childRA3);*/
}

static void createCouple(SocialGraph* SG, int n1, int n2)
{
    struct nodeAttrib* np1 = ((struct nodeAttrib*)graphGetNodeAttribute(SG->G, n1));
    struct nodeAttrib* np2 = ((struct nodeAttrib*)graphGetNodeAttribute(SG->G, n2));

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
    randID(&na->ID);
    randPers(&na->pers);

    return graphAddNode(SG->G, na);
}

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
        randID(&na.ID);
        randPers(&na.pers);

        na.ID.age = randAge();
        na.ID.sex = rand() % 2;

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

    //We search every other child of the parent...
    while (l)
    {
        int* e = listPop(&l);
        struct relationAttrib* ra = graphGetEdgeAttribute(SG->G, *e);

        //And when we find one we create a link with the new child
        if (ra->familyRel == CHILD)
        {
            int i = 0;

            for (i = 0; i < 2; i++)
            {
                struct relationAttrib newra;

                newra.familyRel = SIBLING;
                newra.knowAbout = naParent->ID;
                newra.perc.allegiance = rand()%4;
                newra.perc.attachment = 5 + rand()%6;
                newra.perc.love = 0;

                if (i)
                    socialAddRelation(SG, graphGetNodeTo(SG->G, ra->edgeID), childID, newra);
                else
                    socialAddRelation(SG, childID, graphGetNodeTo(SG->G, ra->edgeID), newra);
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

    while (l)
    {
        int* e = listPop(&l);
        struct relationAttrib* ra = graphGetEdgeAttribute(SG->G, *e);

        //If the node already has a parent...
        if (ra->familyRel == PARENT)
        {
            printf("Node already has parents\n");
            listFree(&l);
            return 0;
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
            //printf("PLop\n");
            createCouple(SG, k, j);
            count++;
        }
        else
            collid++;
    }

    printf("Creating families...\n");

    int i = 0;
    int j = 0;

    for (j = 0; j < 3; j++)
    {
        for (i = 0; i < nbActives; i++)
        {
            struct nodeAttrib* na = graphGetNodeAttribute(SG->G, i);

            int parentID = rand()%nbActives;
            struct nodeAttrib* na2 = graphGetNodeAttribute(SG->G, parentID);

            if (canBeParent(SG, i, parentID))
            {
                int m8 = socialIsMated(SG, parentID);
                affiliate(SG, i, parentID);

                //If the selected potential parent is in couple, we also affiliate his/her partner to the child
                if (m8 != -1)
                    affiliate(SG, i, m8);
            }
        }
    }

    printf("%d colliding matches\n", collid);
    return count;
}
