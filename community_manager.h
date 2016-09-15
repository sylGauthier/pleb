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

    int nbPositions;
    struct position positions[20];

    char genericName[20];
    char specificName[20];

    //How many people in the network for one community of this type
    int quota;

    //If quota = 0, this indicates the absolute number of this kind of community
    int number;
};

struct communityManager
{
    Vector* communities;
    Vector* templates;
};

void communityNewManager(struct communityManager* CM);

int communityLoadTemplatesFromFile(struct communityManager* CM, char* filename);
void communityPrintTemplates(struct communityManager* CM);

void communityFree(struct communityManager* CM);
