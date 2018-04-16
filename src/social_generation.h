#ifndef SOCIAL_GENERATION_H
#define SOCIAL_GENERATION_H

#include "social_graph.h"

void generate_people(SocialGraph* SG, int nb);
int generate_families(SocialGraph* SG);
int generate_communities(SocialGraph* SG);

#endif
