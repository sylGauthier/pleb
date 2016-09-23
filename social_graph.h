#ifndef SOCIAL_GRAPH_H
#define SOCIAL_GRAPH_H

#include "graph.h"
#include "name_manager.h"
#include "community_manager.h"

enum _familyLink_
{
    NONE = 0,
    GRANDPARENT,
    PARENT,
    COUPLE,
    SIBLING,
    CHILD,
    GRANDCHILD,
    UNCLE,
    COUSIN,
    NEPHEW
};


typedef struct SocialGraph
{
    struct nameManager* NM;
    struct communityManager* CM;
    Graph* G;
} SocialGraph;

struct identity
{
    int firstName;
    int lastName;
    int familyID;
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
    int familyRel;
    struct identity knowAbout;
    struct perception perc;
};

SocialGraph* socialNew();

int socialAddNode(SocialGraph* SG, struct nodeAttrib);
int socialAddRelation(SocialGraph* SG, int nf, int nt, struct relationAttrib);

//Returns the ID of the soulmate or -1 if none
int socialIsMated(SocialGraph* SG, int n);

void socialPrintPeople(SocialGraph* SG);
void printAgePyramid(SocialGraph* SG);

void socialFree(SocialGraph* SG);

#endif
