#include <stdlib.h>
#include <stdio.h>

#include "rand_tools.h"

int rand_age()
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

void rand_id(SocialGraph* SG, struct Identity* ID)
{
    ID->age = rand_age();
    ID->sex = rand() % 2;

    name_rand_name(SG->NM, FRENCH, ID->sex, &(ID->firstName), &(ID->lastName));
}

void rand_know_about(struct Identity* ID, int closeness)
{
}

void rand_pers(struct Personality* pers)
{
    *pers = (struct Personality) {(rand()%5 + rand()%6) + 1, rand()%10 + 1, rand()%10 + 1};
}

void rand_perc(struct Perception* perc)
{
    *perc = (struct Perception) {1,2};
}

int rand_uniform(int a, int b)
{    
    if (a >= b)
        return a;
    else
        return a + rand() % (b - a);
}

Vector* rand_route(int nb)
{
    Vector* v = vector_new();
    int i = 0;

    for (i = 0; i < nb; i++)
    {
        int* newElem = malloc(sizeof(int));

        *newElem = i;
        vector_push(v, newElem);
    }

    rand_shuffle(v);

    return v;
}

void rand_shuffle(Vector* v)
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

void rand_rel(struct RelationAttrib* ra, int relType)
{
}
