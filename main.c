#include <stdio.h>
#include <string.h>

#include "social_graph.h"
#include "social_generation.h"
#include "save_graph.h"
#include "stat_tools.h"

int main()
{
    int nbPeople = 100000;
    SocialGraph* SG = socialNew();

    nameLoadFromFile(&(SG->NM), "firstNamesFRFemales.txt", FRENCH, FEMALE, FIRST_NAME);
    nameLoadFromFile(&(SG->NM), "firstNamesFRMales.txt", FRENCH, MALE, FIRST_NAME);
    nameLoadFromFile(&(SG->NM), "lastNamesFR.txt", FRENCH, MALE, LAST_NAME);
    /*generatePeople(SG, nbPeople);

    printf("Generated %d couples out of %d people\n", generateFamilies(SG), nbPeople);
    printf("There are still %d underage orphans\n", statCountOrphanKids(SG));
*/
    //saveGraph(SG, "test.graphml");

    socialFree(SG);



    return 0;
}
