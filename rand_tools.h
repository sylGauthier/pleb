#ifndef RAND_TOOLS_H
#define RAND_TOOLS_H

#include "vector.h"

//Returns a random integer between a (incl) and b (excl) with an uniform probability
int randUniform(int a, int b);

//Generates a shuffled vector containing all integers from 0 to nb-1 without redundancy
Vector* randRoute(int nb);

#endif
