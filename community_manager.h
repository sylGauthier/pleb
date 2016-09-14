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

struct community
{
    int ID;
    struct position positions[20];
    char genericName[20];
    char specificName[20];

    //How many people in the network for one community of this type
    int quota;
};

struct communityManager
{
    Vector* communities;
    Vector* templates;
};

void communityNewManager(struct communityManager* CM);
int communityLoadTemplatesFromFile(struct communityManager* CM, char* filename);

void communityFree(struct communityManager* CM);
