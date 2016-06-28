#ifndef SOCIAL_GRAPH_H
#define SOCIAL_GRAPH_H

#include "graph.h"

enum _sex_
{
    MALE = 0,
    FEMALE
};

typedef struct SocialGraph
{
    Graph* G;
} SocialGraph;

struct identity
{
    int firstName;
    int lastName;
    int sex;
    int age;
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
    int love;
};

struct nodeAttrib
{
    int nodeID;
    struct identity ID;
    struct personality pers;
};

struct relationAttrib
{
    int edgeID;
    struct identity knowAbout;
    struct perception perc;
};

SocialGraph* socialNew();

int socialAddNode(SocialGraph* SG, struct nodeAttrib);
int socialAddRelation(SocialGraph* SG, int nf, int nt, struct relationAttrib);

void socialPrintPeople(SocialGraph* SG);

void socialFree(SocialGraph* SG);

#endif
