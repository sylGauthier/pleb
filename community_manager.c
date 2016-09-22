#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "community_manager.h"
#include "rand_tools.h"

struct communityManager* communityNewManager()
{
    struct communityManager* CM = malloc(sizeof(struct communityManager));

    CM->communities = vectorNew();
    CM->templates = vectorNew();

    return CM;
}

static void parseDoubleArg(char* arg, int* n1, int* n2)
{
    char* end1 = arg;
    char* end2 = NULL;

    *n1 = strtol(arg, &end1, 10);

    if (*end1 != '\0')
    {
        *n2 = strtol(end1 + 1, &end2, 10);

        if (end2 == end1)
            *n2 = *n1;
    }
    else
        *n2 = *n1;
}

static void parsePosition(xmlDocPtr doc, xmlNodePtr cur, struct communityTemplate* com, int positionIndex)
{
    xmlChar* attr = xmlGetProp(cur, (xmlChar*) "hierarchy");
    com->positions[positionIndex].hierarchy = strtol((char*) attr, NULL, 10);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "nbPeople");
    parseDoubleArg((char*) attr, &(com->positions[positionIndex].nbPeopleMin), &(com->positions[positionIndex].nbPeopleMax));
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "salary");
    parseDoubleArg((char*) attr, &(com->positions[positionIndex].salaryMin), &(com->positions[positionIndex].salaryMax));
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "minAge");
    com->positions[positionIndex].minAge = strtol((char*) attr, NULL, 10);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "maxAge");
    com->positions[positionIndex].maxAge = strtol((char*) attr, NULL, 10);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "timeRatio");
    parseDoubleArg((char*) attr, &(com->positions[positionIndex].timeRatioMin), &(com->positions[positionIndex].timeRatioMax));
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "name");
    strncpy(com->positions[positionIndex].name, (char*) attr, 20);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "type");
    if (attr)
    {
        if (!xmlStrcmp(attr, (xmlChar*) "work"))
            com->positions[positionIndex].type = WORK;
        else if (!xmlStrcmp(attr, (xmlChar*) "leisure"))
            com->positions[positionIndex].type = LEISURE;
    }
    xmlFree(attr);
}

static void parseCommunity(xmlDocPtr doc, xmlNodePtr cur, struct communityManager* CM)
{
    struct communityTemplate* newTemplate = malloc(sizeof(struct communityTemplate));

    newTemplate->quota = 0;
    newTemplate->number = 0;

    xmlChar* gn = xmlGetProp(cur, (xmlChar*) "genericName");
    if (gn)
        strncpy(newTemplate->genericName, (char*) gn, 20);
    xmlFree(gn);

    xmlChar* qu = xmlGetProp(cur, (xmlChar*) "quota");
    if (qu)
        newTemplate->quota = strtol((char*) qu, NULL, 10);
    xmlFree(qu);

    xmlChar* nb = xmlGetProp(cur, (xmlChar*) "number");
    if (nb)
        newTemplate->number = strtol((char*) nb, NULL, 10);
    xmlFree(nb);

    printf("New template: %s, quota: %d\n", newTemplate->genericName, newTemplate->quota);

    cur = cur->xmlChildrenNode;
    int pn = 0;

    while (cur != NULL && pn < 20)
    {
        if (!xmlStrcmp(cur->name, (xmlChar*) "position"))
        {
            parsePosition(doc, cur, newTemplate, pn);
            pn++;
        }

        cur = cur->next;
    }

    newTemplate->nbPositions = pn;

    vectorPush(CM->templates, newTemplate);
}

int communityLoadTemplatesFromFile(struct communityManager* CM, char* filename)
{
    xmlDocPtr doc;
    xmlNodePtr cur;

    printf("Loading community templates...\n");

    doc = xmlParseFile(filename);

    if (doc == NULL)
    {
        printf("Error reading XML file...\n");
        return 0;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        printf("Error, empty or invalid XML file...\n");
        return 0;
    }

    if (xmlStrcmp(cur->name, (xmlChar*) "communities"))
    {
        printf("Error, XML file of wrong type...\n");
        return 0;
    }

    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
        if (!xmlStrcmp(cur->name, (xmlChar*) "community"))
            parseCommunity(doc, cur, CM);

        cur = cur->next;
    }

    xmlFreeDoc(doc);

    return 1;
}

static void printTemplateClbk(void* elem, void* data)
{
    struct communityTemplate* C = elem;

    printf("Template: %s\n  - quota: %d\n  - nbPositions: %d\n", C->genericName, C->quota, C->nbPositions);
    printf("  - Positions:\n");

    int i = 0;

    for (i = 0; i < C->nbPositions; i++)
    {
        printf("    - name: %s - salary: %d/%d - nbPeople: %d/%d - hierarchy: %d - minAge: %d"
                " - maxAge: %d - timeRatio: %d/%d - type: %s\n",
                C->positions[i].name, C->positions[i].salaryMin, C->positions[i].salaryMax,
                C->positions[i].nbPeopleMin, C->positions[i].nbPeopleMax,
                C->positions[i].hierarchy, C->positions[i].minAge, C->positions[i].maxAge,
                C->positions[i].timeRatioMin, C->positions[i].timeRatioMax, C->positions[i].type ? "leisure" : "work");
    }
}

static void printCommunityClbk(void* elem, void* data)
{
    struct community* C = elem;

    printf("Community: %s\n - nbPositions: %d\n", C->specificName, C->nbPositions);
    printf("  - Positions:\n");

    int i = 0;

    for (i = 0; i < C->nbPositions; i++)
    {
        printf("    - name: %s - salary: %d - nbPeople: %d - hierarchy: %d - minAge: %d"
                " - maxAge: %d - timeRatio: %d - type: %s\n",
                C->positions[i].name, C->positions[i].salary, C->positions[i].nbPeople,
                C->positions[i].hierarchy, C->positions[i].minAge, C->positions[i].maxAge,
                C->positions[i].timeRatio, C->positions[i].type ? "leisure" : "work");
    }
}

void communityPrintTemplates(struct communityManager* CM)
{
    vectorMap(*(CM->templates), printTemplateClbk, NULL);
}

void communityPrintCommunities(struct communityManager* CM)
{
    vectorMap(*(CM->communities), printCommunityClbk, NULL);
}

void communityGenerateFromTemplates(struct communityManager* CM, int nbPeople)
{
    int i = 0;
    int communityID = 0;
    int totPositions = 0;

    for (i = 0; i < CM->templates->count; i++)
    {
        struct communityTemplate* curT = CM->templates->data[i];

        int nbCom = 0;

        if (curT->quota > 0)
            nbCom = nbPeople/curT->quota;
        else
            if (curT->number > 0)
                nbCom = curT->number;

        printf("Generating %d communities of type %s\n", nbCom, curT->genericName);

        int j = 0;

        for (j = 0; j < nbCom; j++)
        {
            struct community* newC = malloc(sizeof(struct community));

            if (!newC)
            {
                printf("Error allocating memory while generating communities, aborting...\n");
                return;
            }

            int k = 0;

            //*newC = *curT;
            newC->nbPositions = curT->nbPositions;

            for (k = 0; k < newC->nbPositions; k++)
            {
                newC->positions[k].hierarchy = curT->positions[k].hierarchy;
                newC->positions[k].nbPeople = randUniform(curT->positions[k].nbPeopleMin, curT->positions[k].nbPeopleMax);
                newC->positions[k].salary = randUniform(curT->positions[k].salaryMin, curT->positions[k].salaryMax);
                newC->positions[k].minAge = curT->positions[k].minAge;
                newC->positions[k].maxAge = curT->positions[k].maxAge;
                newC->positions[k].timeRatio = randUniform(curT->positions[k].timeRatioMin, curT->positions[k].timeRatioMax);
                strncpy(newC->positions[k].name, curT->positions[k].name, 25);
                totPositions += newC->positions[k].nbPeople;
                newC->positions[k].type = curT->positions[k].type;
            }

            snprintf(newC->specificName, 20, "%s%d", curT->genericName, j);
            newC->ID = communityID;

            vectorPush(CM->communities, newC);
            communityID++;
        }
    }

    printf("There are now %d positions for %d people in the network\n", totPositions, nbPeople);
}

void communityFree(struct communityManager* CM)
{
    vectorFlush(CM->communities);
    vectorFlush(CM->templates);

    vectorFree(CM->communities);
    vectorFree(CM->templates);

    free(CM);
}
