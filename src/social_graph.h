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
    Vector* positions;
};

struct relationAttrib
{
    int edgeID;
    int familyRel;
    struct identity knowAbout;
    struct perception perc;
};

SocialGraph* socialNew();

Node* socialAddNode(SocialGraph* SG, struct nodeAttrib);
Edge* socialAddRelation(SocialGraph* SG, Node* fromPtr, Node* toPtr, struct relationAttrib);

/*Returns the ID of the soulmate or -1 if none*/
Node* socialIsMated(SocialGraph* SG, Node* n);

void socialPrintPeople(SocialGraph* SG);
void socialPrintNode(SocialGraph* SG, Node* node);
void printAgePyramid(SocialGraph* SG);

void socialFree(SocialGraph* SG);

#endif
