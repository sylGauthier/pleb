#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "social_graph.h"

SocialGraph* social_new() {
    SocialGraph* sg = NULL;

    if (!(sg = malloc(sizeof(SocialGraph)))) {
        fprintf(stderr, "Error: SocialGraph: could not allocate memory\n");
        return NULL;
    }

    if (!(sg->G = graph_new())) {
        fprintf(stderr, "Error: SocialGraph: graph creation failed\n");
        free(sg);
        return NULL;
    }

    sg->NM = name_new_manager();
    sg->CM = community_new_manager();
    sg->MM = map_new_manager();
    return sg;
}

Node* social_add_node(SocialGraph* SG, struct NodeAttrib attrib) {
    struct NodeAttrib* na = NULL;
    
    if (!(na = malloc(sizeof(struct NodeAttrib)))) {
        fprintf(stderr, "Error: SocialGraph: "
                        "could not allocate memory for node attribute\n");
        return NULL;
    }

    if (memcpy(na, &attrib, sizeof(struct NodeAttrib)) != na) {
        fprintf(stderr, "Error: SocialGraph: copying attribute failed\n");
        free(na);
        return NULL;
    }

    if (!(na->positions = vector_new())) {
        fprintf(stderr, "Error: SocialGraph: vector creation failed\n");
        free(na);
        return NULL;
    }

    return graph_add_node(SG->G, na);
}

Edge* social_add_relation(SocialGraph* SG, Node* fromPtr, Node* toPtr,
                          struct RelationAttrib attrib) {
    struct RelationAttrib* ra = NULL;
    
    if (!(ra = malloc(sizeof(struct RelationAttrib)))) {
        fprintf(stderr, "Error: SocialGraph: could not allocate memory for relation\n");
        return NULL;
    }

    if (memcpy(ra, &attrib, sizeof(struct RelationAttrib)) != ra) {
        fprintf(stderr, "Error: SocialGraph: memcpy failed\n");
        free(ra);
        return NULL;
    }

    return graph_add_edge(SG->G, fromPtr, toPtr, ra);
}

Node* social_is_mated(SocialGraph* SG, Node* node) {
    List nbs = node->edges;

    while (nbs) {
        Edge* curEdge = nbs->elem;
        struct RelationAttrib* ea = curEdge->attribute;

        if (ea->familyRel == COUPLE) {
            return curEdge->to;
        }
        nbs = nbs->next;
    }

    return NULL;
}

static void print_people_clbk(Node* node, void* data) {
    struct NodeAttrib* na = node->attribute;
    SocialGraph* sg = data;

    printf("### Individual ###\nID: %d\nName: %s %s\nAge: %d\nSex: %s\n",
           na->nodeID,
           name_get_first_name(sg->NM, na->ID.firstName),
           name_get_last_name(sg->NM, na->ID.lastName),
           na->ID.age, 
           na->ID.sex ? "Female" : "Male");

    if (na->positions->count > 0) {
        struct Position* pos = na->positions->data[0];
        struct Community* com = vector_at(sg->CM->communities, pos->communityID);

        printf("Works at %s as %s\n", com->specificName, pos->name);
    }
    /*
       List nb = graphGetEdgesFrom(sg->G, na->nodeID);

       int* curNB;

       while (nb) {
       curNB = listPop(&nb);
       ea = graphGetEdgeAttribute(sg->G, *curNB);
       printf("-> %d as ", graphGetNodeTo(sg->G, *curNB));

       switch (ea->familyRel) {
       case PARENT :
       printf("Parent\n");
       break;

       case SIBLING :
       printf("Sibling\n");
       break;

       case CHILD :
       printf("Child\n");
       break;

       case COUPLE :
       printf("Couple\n");
       break;

       default :
       printf("Acquaintance\n");
       break;
       }

       printf("Allegiance: %d, Attachment: %d\n", ea->perc.allegiance, ea->perc.attachment);
       }
       */
    printf("##################\n\n");
}

void social_print_people(SocialGraph* SG) {
    printf("Printing social graph...\n\n");
    graph_map_nodes(SG->G, print_people_clbk, SG);
}

void social_print_node(SocialGraph* SG, Node* node) {
    print_people_clbk(node, SG);
}

static void build_age_pyramid_clbk(Node* node, void* pyramid) {
    int* pyr = pyramid;
    struct NodeAttrib* na = node->attribute;

    pyr[na->ID.age]++;
}

void print_age_pyramid(SocialGraph* SG) {
    int pyramid[110] = {0};
    int i = 0;

    graph_map_nodes(SG->G, build_age_pyramid_clbk, pyramid);

    for (i = 0; i < 110; i++) {
        int j = 0;

        printf("%2d : ", i);
        for (j = 0; j < pyramid[i]; j++) {
            printf("*");
        }
        printf("\n");
    }
}

static void free_edge_attr_clbk(Edge* edge, void* data) {
    struct RelationAttrib* cur = edge->attribute;
    free(cur);
}

static void free_node_attr_clbk(Node* node, void* data) {
    struct NodeAttrib* cur = node->attribute;
    /*No listFlush because positions are freed with the freeCommunities method*/
    vector_free(cur->positions);
    free(cur);
}

void social_free(SocialGraph* sg) {
    printf("Freeing social graph...\n");
    printf("  ->Freeing nodes...\n");
    graph_map_nodes(sg->G, free_node_attr_clbk, NULL);
    printf("  ->Freeing edges...\n");
    graph_map_edges(sg->G, free_edge_attr_clbk, NULL);
    printf("  ->Freeing graph...\n");
    graph_free(sg->G);

    printf("  ->Freeing names...\n");
    name_free_manager(sg->NM);
    printf("  ->Freeing communities...\n");
    community_free(sg->CM);
    printf("  ->Freeing map...\n");
    map_free_manager(sg->MM);

    free(sg);
    printf("Done\n");
}
