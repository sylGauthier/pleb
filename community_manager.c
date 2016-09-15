#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "community_manager.h"

void communityNewManager(struct communityManager* CM)
{
    CM->communities = vectorNew();
    CM->templates = vectorNew();
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

    xmlChar* gn = xmlGetProp(cur, (xmlChar*) "genericName");
    strncpy(newTemplate->genericName, (char*) gn, 20);
    xmlFree(gn);

    xmlChar* qu = xmlGetProp(cur, (xmlChar*) "quota");
    newTemplate->quota = strtol((char*) qu, NULL, 10);
    xmlFree(qu);

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

void communityPrintTemplates(struct communityManager* CM)
{
    vectorMap(*(CM->templates), printTemplateClbk, NULL);
}

void communityFree(struct communityManager* CM)
{
    vectorFlush(CM->communities);
    vectorFlush(CM->templates);

    vectorFree(CM->communities);
    vectorFree(CM->templates);
}
