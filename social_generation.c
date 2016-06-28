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

static void createChild(SocialGraph* SG, int p1, int p2)
{
    struct nodeAttrib* np1 = ((struct nodeAttrib*)graphGetNodeAttribute(SG->G, p1));
    struct nodeAttrib* np2 = ((struct nodeAttrib*)graphGetNodeAttribute(SG->G, p2));

    struct nodeAttrib child;
    struct relationAttrib childRA1;
    struct relationAttrib childRA2;
    struct relationAttrib childRA3;

    int minAge = np1->ID.age < np2->ID.age ? np1->ID.age : np2->ID.age;

    int childAge = (rand() % (minAge - 17)) + (minAge > 40 ? (minAge - 40) : 0);

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
    socialAddRelation(SG, p1, childID, childRA3);
    socialAddRelation(SG, p2, childID, childRA3);
}

static void createFamily(SocialGraph* SG, int n1, int n2)
{
    struct relationAttrib ra1;
    struct relationAttrib ra2;

    ra1.knowAbout = ((struct nodeAttrib*)graphGetNodeAttribute(SG->G, n2))->ID;
    ra2.knowAbout = ((struct nodeAttrib*)graphGetNodeAttribute(SG->G, n1))->ID;

    ra1.perc.allegiance = rand()%7 + 4;
    ra2.perc.allegiance = rand()%7 + 4;

    ra1.perc.attachment = rand()%4 + 7;
    ra2.perc.attachment = rand()%4 + 7;

    socialAddRelation(SG, n1, n2, ra1);
    socialAddRelation(SG, n2, n1, ra2);

    int nbChildren = rand()%3 + rand()%4;
    printf("Creating %d children...\n", nbChildren);

    int i = 0;

    for (i = 0; i < nbChildren; i++)
        createChild(SG, n1, n2);
}

int generateNode(SocialGraph* SG)
{
    struct nodeAttrib* na = malloc(sizeof(struct nodeAttrib));
    randID(&na->ID);
    randPers(&na->pers);

    return graphAddNode(SG->G, na);
}

void generateActives(SocialGraph* SG, int nb)
{
    int i = 0;
    struct nodeAttrib na;

    for (i = 0; i < nb; i++)
    {
        randID(&na.ID);
        randPers(&na.pers);

        na.ID.age = 18 + rand()%30;
        na.ID.sex = rand() % 2;

        socialAddNode(SG, na);
    }
}

int generateFamilies(SocialGraph* SG)
{
    printf("Creating couples...\n");
    List matched = NULL;
    int i = 0;
    int k = 0;
    int count = 0;

    for (k = 0; k < 5; k++)
    {
        for (i = 0; i < SG->G->nbNodes; i++)
        {
            int j = rand() % SG->G->nbNodes;


            struct nodeAttrib* na1 = (struct nodeAttrib*)graphGetNodeAttribute(SG->G, i);
            struct nodeAttrib* na2 = (struct nodeAttrib*)graphGetNodeAttribute(SG->G, j);

            //A healthy family can only exist with a dad AND a mom :^)
            if (na1->ID.sex != na2->ID.sex && !listIsIn(matched, na1, NULL) && !listIsIn(matched, na2, NULL)
                    && abs(na1->ID.age - na2->ID.age) < 10)
            {
                printf("Creating couple between %d and %d (%d yo and %d yo)...\n", i, j, na1->ID.age, na2->ID.age);
                createFamily(SG, i, j);
                listPush(&matched, na1);
                listPush(&matched, na2);
                count++;
            }
        }
    }

    listFree(&matched);
    return count;
}
