#include <stdio.h>
#include <string.h>

#include "social_graph.h"
#include "social_generation.h"
#include "save_graph.h"

int main()
{
    int nbPeople = 100;
    SocialGraph* SG = socialNew();

   /* struct nodeAttrib node1;
    node1.ID.firstName = 0;
    node1.ID.lastName = 0;
    node1.ID.sex = 0;
    node1.ID.phone = 1;
    node1.ID.address = 1000;

    node1.pers.intelligence = 10;
    node1.pers.emotionality = 1;

    struct nodeAttrib node2;
    node2.ID.firstName = 1;
    node2.ID.lastName = 1;
    node2.ID.sex = 0;
    node2.ID.phone = 2;
    node2.ID.address = 2000;

    node2.pers.intelligence = 3;
    node2.pers.emotionality = 6;

    struct nodeAttrib node3;
    node3.ID.firstName = 3;
    node3.ID.lastName = 3;
    node3.ID.sex = 3;
    node3.ID.phone = 3;
    node3.ID.address = 3000;

    node3.pers.intelligence = 6;
    node3.pers.emotionality = 0;

    struct perception RPerc;
    randPerc(&RPerc);

    struct identity RIdent;
    randID(&RIdent);
    
    struct relationAttrib dontCare;
    dontCare.edgeID = 0;
    dontCare.knowAbout = RIdent;
    dontCare.perc = RPerc;

    int a = socialAddNode(SG, node1);
    int b = socialAddNode(SG, node2);
    int c = socialAddNode(SG, node3);

    socialAddRelation(SG, 0, 1, dontCare);
    socialAddRelation(SG, 0, 2, dontCare);
    socialAddRelation(SG, 2, 1, dontCare);*/

    generatePeople(SG, nbPeople);
    //printAgePyramid(SG);
    printf("Generated %d couples out of %d people\n", generateFamilies(SG), nbPeople);

    //socialPrintPeople(SG);

    saveGraph(SG, "test.graphml");

    socialFree(SG);

    return 0;
}
