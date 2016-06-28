#ifndef SOCIAL_GENERATION_H
#define SOCIAL_GENERATION_H

#include "social_graph.h"

void randID(struct identity* ID);
void randPers(struct personality* pers);
void randPerc(struct perception* perc);

int generateNode(SocialGraph* SG);

void generateActives(SocialGraph* SG, int nb);
int generateFamilies(SocialGraph* SG);

#endif
