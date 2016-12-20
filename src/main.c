#include <stdio.h>
#include <string.h>

#include "social_graph.h"
#include "social_generation.h"
#include "save_graph.h"
#include "stat_tools.h"
#include "rand_tools.h"

int main()
{
    int nbPeople = 100000;
    SocialGraph* SG = socialNew();

    communityLoadTemplatesFromFile(SG->CM, "plop.xml");
    communityGenerateFromTemplates(SG->CM, nbPeople);

    nameLoadFromFile(SG->NM, "firstNamesFRFemales.txt", FRENCH, FEMALE, FIRST_NAME);
    nameLoadFromFile(SG->NM, "firstNamesFRMales.txt", FRENCH, MALE, FIRST_NAME);
    nameLoadFromFile(SG->NM, "lastNamesFR.txt", FRENCH, FEMALE, LAST_NAME);
    nameLoadFromFile(SG->NM, "lastNamesFR.txt", FRENCH, MALE, LAST_NAME);

    generatePeople(SG, nbPeople);

    printf("Generated %d couples out of %d people\n", generateFamilies(SG), nbPeople);

    generateCommunities(SG);

    int i = 0;

    /*for (i = 0; i < 10; i++)
    {
        socialPrintNode(SG, i);
    }*/

    /*saveGraph(SG, "test.graphml");*/
    
    for (i = 0; i < 100; i += 4)
    {
        printf("Inactive people (%d-%d y.o): %d\n", i, i+3, nbInactives(SG, i, i+3));
    }

    printUnassignedPositions(SG);

    socialFree(SG);

    return 0;
}
