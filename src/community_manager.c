#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "community_manager.h"
#include "rand_tools.h"

CommunityManager* community_new_manager() {
    CommunityManager* CM = NULL;
    
    if (!(CM = malloc(sizeof(CommunityManager)))) {
        fprintf(stderr, "Error: CommunityManager: could not allocate memory\n");
        return NULL;
    }

    CM->communities = vector_new();
    CM->templates = vector_new();

    return CM;
}

static void parse_double_arg(char* arg, int* n1, int* n2) {
    char* end1 = arg;
    char* end2 = NULL;

    *n1 = strtol(arg, &end1, 10);

    if (*end1 != '\0') {
        *n2 = strtol(end1 + 1, &end2, 10);

        if (end2 == end1)
            *n2 = *n1;
    } else
        *n2 = *n1;
}

static void parse_position(xmlDocPtr doc,
                           xmlNodePtr cur,
                           struct CommunityTemplate* com,
                           int positionIndex) {
    xmlChar* attr;
    
    attr = xmlGetProp(cur, (xmlChar*) "hierarchy");
    com->positions[positionIndex].hierarchy = strtol((char*) attr, NULL, 10);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "nbPeople");
    parse_double_arg((char*) attr, &(com->positions[positionIndex].nbPeopleMin),
                     &(com->positions[positionIndex].nbPeopleMax));
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "salary");
    parse_double_arg((char*) attr, &(com->positions[positionIndex].salaryMin),
                     &(com->positions[positionIndex].salaryMax));
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "minAge");
    com->positions[positionIndex].minAge = strtol((char*) attr, NULL, 10);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "maxAge");
    com->positions[positionIndex].maxAge = strtol((char*) attr, NULL, 10);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "timeRatio");
    parse_double_arg((char*) attr, &(com->positions[positionIndex].timeRatioMin),
                     &(com->positions[positionIndex].timeRatioMax));
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "name");
    strncpy(com->positions[positionIndex].name, (char*) attr, 20);
    xmlFree(attr);

    attr = xmlGetProp(cur, (xmlChar*) "type");
    if (attr) {
        if (!xmlStrcmp(attr, (xmlChar*) "work"))
            com->positions[positionIndex].type = WORK;
        else if (!xmlStrcmp(attr, (xmlChar*) "leisure"))
            com->positions[positionIndex].type = LEISURE;
    }
    xmlFree(attr);
}

static void parse_community(xmlDocPtr doc, xmlNodePtr cur, CommunityManager* CM) {
    struct CommunityTemplate* newTemplate = NULL;
    xmlChar* gn;
    xmlChar* qu;
    xmlChar* nb;
    int pn = 0;
    
    if (!(newTemplate = malloc(sizeof(struct CommunityTemplate)))) {
        fprintf(stderr, "Error: CommunityManager: could not allocate memory\n");
        return;
    }

    newTemplate->quota = 0;
    newTemplate->number = 0;

    gn = xmlGetProp(cur, (xmlChar*) "genericName");
    if (gn)
        strncpy(newTemplate->genericName, (char*) gn, 20);
    xmlFree(gn);

    qu = xmlGetProp(cur, (xmlChar*) "quota");
    if (qu)
        newTemplate->quota = strtol((char*) qu, NULL, 10);
    xmlFree(qu);

    nb = xmlGetProp(cur, (xmlChar*) "number");
    if (nb)
        newTemplate->number = strtol((char*) nb, NULL, 10);
    xmlFree(nb);

    printf("New template: %s, quota: %d\n", newTemplate->genericName, newTemplate->quota);

    cur = cur->xmlChildrenNode;
    while (cur != NULL && pn < 20) {
        if (!xmlStrcmp(cur->name, (xmlChar*) "position")) {
            parse_position(doc, cur, newTemplate, pn);
            pn++;
        }
        cur = cur->next;
    }

    newTemplate->nbPositions = pn;
    vector_push(CM->templates, newTemplate);
}

int community_load_templates_from_file(CommunityManager* CM, char* filename) {
    xmlDocPtr doc;
    xmlNodePtr cur;

    printf("Loading community templates...\n");
    doc = xmlParseFile(filename);

    if (doc == NULL) {
        printf("Error reading XML file...\n");
        return 0;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        printf("Error, empty or invalid XML file...\n");
        return 0;
    }

    if (xmlStrcmp(cur->name, (xmlChar*) "communities")) {
        printf("Error, XML file of wrong type...\n");
        return 0;
    }

    cur = cur->xmlChildrenNode;

    while (cur != NULL) {
        if (!xmlStrcmp(cur->name, (xmlChar*) "community"))
            parse_community(doc, cur, CM);

        cur = cur->next;
    }

    xmlFreeDoc(doc);

    return 1;
}

static void print_template_clbk(void* elem, void* data) {
    struct CommunityTemplate* C = elem;
    int i = 0;

    printf("Template: %s\n  - quota: %d\n  - nbPositions: %d\n",
            C->genericName, C->quota, C->nbPositions);
    printf("  - Positions:\n");

    for (i = 0; i < C->nbPositions; i++) {
        printf("    - name: %s - salary: %d/%d - nbPeople: %d/%d - hierarchy: %d"
               " - minAge: %d - maxAge: %d - timeRatio: %d/%d - type: %s\n",
               C->positions[i].name, C->positions[i].salaryMin, C->positions[i].salaryMax,
               C->positions[i].nbPeopleMin, C->positions[i].nbPeopleMax,
               C->positions[i].hierarchy, C->positions[i].minAge, C->positions[i].maxAge,
               C->positions[i].timeRatioMin, C->positions[i].timeRatioMax,
               C->positions[i].type ? "leisure" : "work");
    }
}

static void print_community_clbk(void* elem, void* data) {
    struct Community* C = elem;
    int i = 0;

    printf("Community: %s\n - nbPositions: %d\n", C->specificName, C->nbPositions);
    printf("  - Positions:\n");

    for (i = 0; i < C->nbPositions; i++) {
        printf("    - name: %s - salary: %d - nbPeople: %d - hierarchy: %d - minAge: %d"
               " - maxAge: %d - timeRatio: %d - type: %s\n",
               C->positions[i].name, C->positions[i].salary, C->positions[i].nbPeople,
               C->positions[i].hierarchy, C->positions[i].minAge, C->positions[i].maxAge,
               C->positions[i].timeRatio, C->positions[i].type ? "leisure" : "work");
    }
}

void community_print_templates(CommunityManager* CM) {
    vector_map(*(CM->templates), print_template_clbk, NULL);
}

void community_print_communities(CommunityManager* CM) {
    vector_map(*(CM->communities), print_community_clbk, NULL);
}

void community_generate_from_templates(CommunityManager* CM, int nbPeople) {
    int i = 0;
    int communityID = 0;
    int totPositions = 0;

    for (i = 0; i < CM->templates->count; i++) {
        struct CommunityTemplate* curT = CM->templates->data[i];
        int nbCom = 0;
        int j = 0;

        if (curT->quota > 0)
            nbCom = nbPeople/curT->quota;
        else
            if (curT->number > 0)
                nbCom = curT->number;

        printf("Generating %d communities of type %s\n", nbCom, curT->genericName);

        for (j = 0; j < nbCom; j++) {
            struct Community* newC = NULL;
            int k = 0;
            
            if (!(newC = malloc(sizeof(struct Community)))) {
                fprintf(stderr, "Error: CommunityManager: could not allocate memory\n");
                return;
            }

            newC->ID = communityID;
            newC->nbPositions = curT->nbPositions;

            for (k = 0; k < newC->nbPositions; k++) {
                newC->positions[k].communityID = communityID;
                newC->positions[k].hierarchy = curT->positions[k].hierarchy;
                newC->positions[k].nbPeople =
                    rand_uniform(curT->positions[k].nbPeopleMin,
                                 curT->positions[k].nbPeopleMax);
                newC->positions[k].salary = rand_uniform(curT->positions[k].salaryMin,
                                                         curT->positions[k].salaryMax);
                newC->positions[k].minAge = curT->positions[k].minAge;
                newC->positions[k].maxAge = curT->positions[k].maxAge;
                newC->positions[k].timeRatio =
                    rand_uniform(curT->positions[k].timeRatioMin,
                                 curT->positions[k].timeRatioMax);
                strncpy(newC->positions[k].name, curT->positions[k].name, 25);
                totPositions += newC->positions[k].nbPeople;
                newC->positions[k].type = curT->positions[k].type;

                newC->positions[k].people = vector_new();
            }

            strncpy(newC->genericName, curT->genericName, 20);
            sprintf(newC->specificName, "%s%d", curT->genericName, j);

            vector_push(CM->communities, newC);
            communityID++;
        }
    }

    CM->nbPositions = totPositions;

    printf("There are now %d positions for %d people in the network\n",
           totPositions, nbPeople);
}

static void free_pos_clbk(void* com, void* data) {
    struct Community* cur = com;
    int i = 0;

    for (i = 0; i < cur->nbPositions; i++) {
        vector_free(cur->positions[i].people);
    }
}

void community_free(CommunityManager* CM) {
    vector_map(*(CM->communities), free_pos_clbk, NULL);
    vector_flush(CM->communities);
    vector_flush(CM->templates);

    vector_free(CM->communities);
    vector_free(CM->templates);

    free(CM);
}
