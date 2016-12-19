#ifndef SOCIAL_GENERATION_H
#define SOCIAL_GENERATION_H

#include "social_graph.h"

//int generateNode(SocialGraph* SG);

void generatePeople(SocialGraph* SG, int nb);
int generateFamilies(SocialGraph* SG);
int generateCommunities(SocialGraph* SG);

#endif
