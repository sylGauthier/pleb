#include <stdio.h>
#include <string.h>

#include "social_graph.h"
#include "social_generation.h"
#include "save_graph.h"
#include "stat_tools.h"
#include "community_manager.h"

int main()
{
    /* int nbPeople = 100;
    SocialGraph* SG = socialNew();

    nameLoadFromFile(&(SG->NM), "firstNamesFRFemales.txt", FRENCH, FEMALE, FIRST_NAME);
    nameLoadFromFile(&(SG->NM), "firstNamesFRMales.txt", FRENCH, MALE, FIRST_NAME);
    nameLoadFromFile(&(SG->NM), "lastNamesFR.txt", FRENCH, FEMALE, LAST_NAME);
    nameLoadFromFile(&(SG->NM), "lastNamesFR.txt", FRENCH, MALE, LAST_NAME);

    generatePeople(SG, nbPeople);

    printf("Generated %d couples out of %d people\n", generateFamilies(SG), nbPeople);
    printf("There are still %d underage orphans\n", statCountOrphanKids(SG));

    saveGraph(SG, "test.graphml");

    socialFree(SG);*/

    struct communityManager* CM = communityNewManager();

    communityLoadTemplatesFromFile(CM, "plop.xml");
    communityPrintTemplates(CM);

    communityGenerateFromTemplates(CM, 100000);
    //communityPrintCommunities(CM);

    communityFree(CM);

    return 0;
}
