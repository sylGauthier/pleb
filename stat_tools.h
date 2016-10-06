#ifndef STAT_TOOLS_H
#define STAT_TOOLS_H

#include "social_graph.h"

int statCountOrphanKids(SocialGraph* SG);
void printUnassignedPositions(SocialGraph* SG);
int nbInactives(SocialGraph* SG, int minAge, int maxAge);

#endif
