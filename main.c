#include <stdio.h>
#include <string.h>

#include "social_graph.h"

int main()
{
    SocialGraph* SG = socialNew();

    struct identity node1;
    strcpy(node1.name, "Glandalf");
    node1.phone = 1;
    node1.address = 1000;

    struct skills sk1;
    sk1.intelligence = 10;
    sk1.emotionality = 1;

    struct identity node2;
    strcpy(node2.name, "Frodo");
    node2.phone = 2;
    node2.address = 2000;

    struct skills sk2;
    sk2.intelligence = 3;
    sk2.emotionality = 6;

    struct identity node3;
    strcpy(node3.name, "Gollum");
    node3.phone = 3;
    node3.address = 3000;

    struct skills sk3;
    sk3.intelligence = 6;
    sk3.emotionality = 0;

    int a = socialAddNode(SG, node1, sk1);
    int b = socialAddNode(SG, node2, sk2);
    int c = socialAddNode(SG, node3, sk3);

    socialPrintPeople(SG);

    socialFree(SG);

    return 0;
}
