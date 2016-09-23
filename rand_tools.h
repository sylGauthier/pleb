#ifndef RAND_TOOLS_H
#define RAND_TOOLS_H

#include "vector.h"

int randUniform(int a, int b);

//Thank you Fisher-Yates...
Vector* randRoute(int nb);

#endif
