#ifndef STAT_TOOLS_H
#define STAT_TOOLS_H

#include "social_graph.h"

int stat_count_orphan_kids(SocialGraph* SG);
void print_unassigned_positions(SocialGraph* SG);
int nb_inactives(SocialGraph* SG, int minAge, int maxAge);

#endif
