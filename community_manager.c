#include <stdio.h>
#include <stdlib.h>
#include <libxml2/libxml/xmlmemory.h>
#include <libxml2/libxml/parser.h>

#include "community_manager.h"

void communityNewManager(struct communityManager* CM)
{
    CM->communities = vectorNew();
    CM->templates = vectorNew();
}

static void parseCommunity(xmlDocPtr doc, xmlNodePtr cur)
{
    struct community* newTemplate = malloc(sizeof(struct community));

    xmlChar *name = xmlGetProp(cur, "name");
    printf("Name: %s\n", name);
    xmlFree(name);
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
            parseCommunity(doc, cur);

        cur = cur->next;
    }

    xmlFreeDoc(doc);

    return 1;
}

void communityFree(struct communityManager* CM)
{
    vectorFlush(CM->communities);
    vectorFlush(CM->templates);

    vectorFree(CM->communities);
    vectorFree(CM->templates);
}
