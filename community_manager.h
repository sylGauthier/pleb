#include "vector.h"

struct position
{
    int hierarchy;
    int nbPeople;
    int salary;

    int minAge;
    int maxAge;

    //Percentage of time this position occupies in the node's schedule
    int timeRatio;

    char name[25];
};

enum COMMUNITY_TYPE
{
    WORK,
    LEISURE
};

struct community
{
    int ID;

    int nbPositions;
    struct position positions[20];

    char genericName[20];
    char specificName[20];

    //How many people in the network for one community of this type
    int quota;

    //If quota = 0, this indicates the absolute number of this kind of community
    int number;

    int type;
};

struct communityManager
{
    Vector* communities;
    Vector* templates;
};

struct communityManager*  communityNewManager();

int communityLoadTemplatesFromFile(struct communityManager* CM, char* filename);
void communityPrintTemplates(struct communityManager* CM);
void communityPrintCommunities(struct communityManager* CM);

//Note: all we need to create empty, "abstract" communities is the number of people in
//the network. Then we "fill" those communities with people in social_generation
void communityGenerateFromTemplates(struct communityManager* CM, int nbPeople);

void communityFree(struct communityManager* CM);
