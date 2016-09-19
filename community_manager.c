#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "community_manager.h"

struct communityManager* communityNewManager()
{
    struct communityManager* CM = malloc(sizeof(struct communityManager));

    CM->communities = vectorNew();
    CM->templates = vectorNew();

    return CM;
}

static void parsePosition(xmlDocPtr doc, xmlNodePtr cur, struct community* com, int positionIndex)
{
    xmlChar* attr = xmlGetProp(cur, (xmlChar*) "hierarchy");
    com->positions[positionIndex].hierarchy = strtol((char*) attr, NULL, 10);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "nbPeople");
    com->positions[positionIndex].nbPeople = strtol((char*) attr, NULL, 10);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "salary");
    com->positions[positionIndex].salary = strtol((char*) attr, NULL, 10);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "minAge");
    com->positions[positionIndex].minAge = strtol((char*) attr, NULL, 10);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "maxAge");
    com->positions[positionIndex].maxAge = strtol((char*) attr, NULL, 10);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "timeRatio");
    com->positions[positionIndex].timeRatio = strtol((char*) attr, NULL, 10);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "name");
    strncpy(com->positions[positionIndex].name, (char*) attr, 20);
    xmlFree(attr);
}

static void parseCommunity(xmlDocPtr doc, xmlNodePtr cur, struct communityManager* CM)
{
    struct community* newTemplate = malloc(sizeof(struct community));

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

    xmlChar* type = xmlGetProp(cur, (xmlChar*) "type");
    if (type)
    {
        if (!xmlStrcmp(type, (xmlChar*) "work"))
            newTemplate->type = WORK;
        else if (!xmlStrcmp(type, (xmlChar*) "leisure"))
            newTemplate->type = LEISURE;
    }
    xmlFree(type);

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
    struct community* C = elem;

    printf("Template: %s\n  - quota: %d\n  - nbPositions: %d\n", C->genericName, C->quota, C->nbPositions);
    printf("  - Positions:\n");

    int i = 0;

    for (i = 0; i < C->nbPositions; i++)
    {
        printf("    - name: %s - salary: %d - nbPeople: %d - hierarchy: %d - minAge: %d"
                " - maxAge: %d - timeRatio: %d\n",
                C->positions[i].name, C->positions[i].salary, C->positions[i].nbPeople,
                C->positions[i].hierarchy, C->positions[i].minAge, C->positions[i].maxAge,
                C->positions[i].timeRatio);
    }
}

static void printCommunityClbk(void* elem, void* data)
{
    struct community* C = elem;

    printf("Community: %s\n  - quota: %d\n  - nbPositions: %d\n", C->specificName, C->quota, C->nbPositions);
    printf("  - Positions:\n");

    int i = 0;

    for (i = 0; i < C->nbPositions; i++)
    {
        printf("    - name: %s - salary: %d - nbPeople: %d - hierarchy: %d - minAge: %d"
                " - maxAge: %d - timeRatio: %d\n",
                C->positions[i].name, C->positions[i].salary, C->positions[i].nbPeople,
                C->positions[i].hierarchy, C->positions[i].minAge, C->positions[i].maxAge,
                C->positions[i].timeRatio);
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
        struct community* curT = CM->templates->data[i];

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

            *newC = *curT;

            for (k = 0; k < newC->nbPositions; k++)
            {
                totPositions += newC->positions[k].nbPeople;
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
