#ifndef RAND_TOOLS_H
#define RAND_TOOLS_H

#include "vector.h"
#include "social_graph.h"

enum REL_TYPE {
    COLLEAGUE = 0,
    SUPERIOR,
    SUBORDINATE,
    FRIEND
};

/* Simulates very approximately the french age pyramid of 2014 */ 
int rand_age();

/* Returns a random integer between a (incl) and b (excl) with an uniform probability */ 
int rand_uniform(int a, int b);

/* Returns a random integer drawn from a binomial distribution */ 
int rand_binom(int n);

/* Generates a shuffled vector containing all integers from 0 to nb-1
 * without redundancy
 */ 
Vector* rand_route(int nb);

/* Shuffles a given vector (in place) */ 
void rand_shuffle(Vector* v);

/* Generates a completely random identity */ 
void rand_id(SocialGraph* SG, struct Identity* ID);

/* Generates a random knowAbout attribute. The closeness parameter makes
 * the attribute more credible (ie it will be much less likely for someone
 * to know the mail address and age but not the name of someone).
 * Closeness should range from 1 (not close, will know only sporadic
 * information) to 10 (very close, likely to know everything).
 */ 
void rand_know_about(struct Identity*, int closeness);

/* Generates a completely random personality */ 
void rand_pers(struct Personality* pers);

/* Generates a random perception */ 
void rand_perc(struct Perception* perc);

/* Generates a random relation based on different templates */ 
void rand_rel(struct RelationAttrib* ra, int relType);

#endif
