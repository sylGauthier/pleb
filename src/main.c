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
    SocialGraph* SG = social_new();

    community_load_templates_from_file(SG->CM, "data/plop.xml");
    community_generate_from_templates(SG->CM, nbPeople);

    name_load_from_file(SG->NM, "data/firstNamesFRFemales.txt", FRENCH, FEMALE, FIRST_NAME);
    name_load_from_file(SG->NM, "data/firstNamesFRMales.txt", FRENCH, MALE, FIRST_NAME);
    name_load_from_file(SG->NM, "data/lastNamesFR.txt", FRENCH, FEMALE, LAST_NAME);
    name_load_from_file(SG->NM, "data/lastNamesFR.txt", FRENCH, MALE, LAST_NAME);

    generate_people(SG, nbPeople);

    printf("Generated %d couples out of %d people\n", generate_families(SG), nbPeople);

    generate_communities(SG);

    int i = 0;

    /*for (i = 0; i < 10; i++)
    {
        socialPrintNode(SG, i);
    }*/

    /*saveGraph(SG, "test.graphml");*/
    
    for (i = 0; i < 100; i += 4)
    {
        printf("Inactive people (%d-%d y.o): %d\n", i, i+3, nb_inactives(SG, i, i+3));
    }

    print_unassigned_positions(SG);

    social_free(SG);

    return 0;
}
