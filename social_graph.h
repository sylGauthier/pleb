#ifndef SOCIAL_GRAPH_H
#define SOCIAL_GRAPH_H

#include "graph.h"

typedef struct SocialGraph SocialGraph;

struct identity
{
    int firstName;
    int lastName;
    int sex;
    int phone;
    int address;
    int email;
};

struct personality
{
    int intelligence;
    int emotionality;
    int sexuality;
};

struct perception
{
    int allegiance;
    int attachment;
};

struct nodeAttrib
{
    int nodeID;
    struct identity ID;
    struct personality pers;
};

struct relationAttrib
{
    struct identity knowAbout;
    struct perception perc;
};

SocialGraph* socialNew();

int socialAddNode(SocialGraph* SG, struct nodeAttrib);
int socialAddRelation(SocialGraph* SG, struct relationAttrib);

void socialPrintPeople(SocialGraph* SG);

void socialFree(SocialGraph* SG);

#endif
