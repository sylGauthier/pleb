#ifndef COMMUNITY_MANAGER_H
#define COMMUNITY_MANAGER_H

#include "vector.h"

struct Position
{
    int communityID;
    int hierarchy;
    int nbPeople;
    Vector* people;
    int salary;

    int minAge;
    int maxAge;

    /*Percentage of time this position occupies in the node's schedule*/
    int timeRatio;

    char name[25];

    int type;
};

struct PositionTemplate
{
    int hierarchy;
    int nbPeopleMax, nbPeopleMin;
    int salaryMax, salaryMin;

    int minAge;
    int maxAge;

    /*Percentage of time this position occupies in the node's schedule*/
    int timeRatioMin;
    int timeRatioMax;

    char name[25];

    int type;
};

enum PositionType
{
    WORK = 0,
    LEISURE
};

struct Community
{
    int ID;

    int nbPositions;
    struct Position positions[20];

    char genericName[20];
    char specificName[20];
};

struct CommunityTemplate
{
    int nbPositions;
    struct PositionTemplate positions[20];

    char genericName[20];

    /*How many people in the network for one community of this type*/
    int quota;

    /*If quota = 0, this indicates the absolute number of this kind of community*/
    int number;
};

typedef struct CommunityManager
{
    int nbPositions;
    Vector* communities;
    Vector* templates;
} CommunityManager;

CommunityManager*  community_new_manager();

int community_load_templates_from_file(CommunityManager* CM, char* filename);
void community_print_templates(CommunityManager* CM);
void community_print_communities(CommunityManager* CM);

/*Note: all we need to create empty, "abstract" communities is the number of people in
 *the network. Then we "fill" those communities with people in social_generation
 */
void community_generate_from_templates(CommunityManager* CM, int nbPeople);

void community_free(CommunityManager* CM);


#endif
