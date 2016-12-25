#ifndef SOCIAL_GRAPH_H
#define SOCIAL_GRAPH_H

#include "graph.h"
#include "name_manager.h"
#include "community_manager.h"

enum FamilyLink
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
    NameManager* NM;
    CommunityManager* CM;
    Graph* G;
} SocialGraph;

struct Identity
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

struct Personality
{
    int intelligence;
    int emotionality;
    int sexuality;
};

struct Perception
{
    int allegiance;
    int attachment;
    int love;
};

struct NodeAttrib
{
    int nodeID;
    struct Identity ID;
    struct Personality pers;
    Vector* positions;
};

struct RelationAttrib
{
    int edgeID;
    int familyRel;
    struct Identity knowAbout;
    struct Perception perc;
};

SocialGraph* social_new();

Node* social_add_node(SocialGraph* SG, struct NodeAttrib);
Edge* social_add_relation(SocialGraph* SG, Node* fromPtr, Node* toPtr, struct RelationAttrib);

/*Returns the ID of the soulmate or -1 if none*/
Node* social_is_mated(SocialGraph* SG, Node* n);

void social_print_people(SocialGraph* SG);
void social_print_node(SocialGraph* SG, Node* node);
void print_age_pyramid(SocialGraph* SG);

void social_free(SocialGraph* SG);

#endif
