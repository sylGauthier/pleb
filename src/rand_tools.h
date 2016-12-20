#ifndef RAND_TOOLS_H
#define RAND_TOOLS_H

#include "vector.h"
#include "social_graph.h"

enum REL_TYPE
{
    COLLEAGUE = 0,
    SUPERIOR,
    SUBORDINATE,
    FRIEND
};

/*Simulates very approximately the french age pyramid of 2014*/
int randAge();

/*Returns a random integer between a (incl) and b (excl) with an uniform probability*/
int randUniform(int a, int b);

/*Generates a shuffled vector containing all integers from 0 to nb-1 without redundancy*/
Vector* randRoute(int nb);

/*Shuffles a given vector (in place)*/
void randShuffle(Vector* v);

/*Generates a completely random identity*/
void randID(SocialGraph* SG, struct identity* ID);

/*Generates a random knowAbout attribute. The closeness parameter makes the attribute more
 *credible (ie it will be much less likely for someone to know the mail address and age but
 *not the name of someone).
 *Closeness should range from 1 (not close, will know only sporadic information) to 10 (very
 *close, likely to know everything).
 */
void randKnowAbout(struct identity*, int closeness);

/*Generates a completely random personality*/
void randPers(struct personality* pers);

/*Generates a random perception*/
void randPerc(struct perception* perc);

/*Generates a random relation based on different templates*/
void randRel(struct relationAttrib* ra, int relType);

#endif
