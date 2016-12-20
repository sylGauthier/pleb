#include <stdlib.h>
#include <stdio.h>

#include "rand_tools.h"

int randAge()
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

void randKnowAbout(struct identity* ID, int closeness)
{
}

void randPers(struct personality* pers)
{
    *pers = (struct personality) {(rand()%5 + rand()%6) + 1, rand()%10 + 1, rand()%10 + 1};
}

void randPerc(struct perception* perc)
{
    *perc = (struct perception) {1,2};
}

int randUniform(int a, int b)
{    
    if (a >= b)
        return a;
    else
        return a + rand() % (b - a);
}

Vector* randRoute(int nb)
{
    Vector* v = vectorNew();
    int i = 0;

    for (i = 0; i < nb; i++)
    {
        int* newElem = malloc(sizeof(int));

        *newElem = i;
        vectorPush(v, newElem);
    }

    randShuffle(v);

    return v;
}

void randShuffle(Vector* v)
{
    int i = 0;

    for (i = v->count - 1; i > 0; i--)
    {
        int j = rand() % (i+1);
        void* tmp = v->data[i];
        v->data[i] = v->data[j];
        v->data[j] = tmp;
    }
}

void randRel(struct relationAttrib* ra, int relType)
{
}
