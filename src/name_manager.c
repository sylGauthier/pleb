#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "name_manager.h"

struct NameRaceSelector
{
    int start;
    int nb;
};

struct NameManager
{
    struct NameRaceSelector selectors[2][20];
    Vector* firstNames;
    Vector* lastNames;
};

NameManager* name_new_manager()
{
    NameManager* NM = malloc(sizeof(NameManager));

    int i = 0;

    for (i = 0; i < 20; i++)
    {
        NM->selectors[0][i].start = -1;
        NM->selectors[0][i].nb = -1;
        NM->selectors[1][i].start = -1;
        NM->selectors[1][i].nb = -1;
    }

    NM->firstNames = vector_new();
    NM->lastNames = vector_new();

    return NM;
}

void name_load_from_file(NameManager* NM, char* fileName, int race, int sex, int FN)
{
    Vector* target = (FN == 1 ? NM->firstNames : NM->lastNames);

    FILE* f = fopen(fileName, "r");

    char* curName;
    char* res = NULL;
    int count = 0;
    NM->selectors[FN][race*2 + sex].start = target->count;

    do
    {
        curName = malloc(20*sizeof(char));

        /*To avoid valgrind annoying error of "cond jump depends on uninitialized value"*/
        strncpy(curName, "--------------------", 20);
        vector_push(target, curName);

        res = fgets(curName, 20, f);

        int i = 0;

        for (i = 0; i < 20; i++)
        {
            if (curName[i] == '\n')
            {
                curName[i] = '\0';
                break;
            }
        }

        count++;

    } while (res != NULL);

    free(vector_pop(target)); /*The last element corresponds to an empty line so we don't want to keep it.*/
    count--;
    NM->selectors[FN][race*2 + sex].nb = count;

    fclose(f);
}

static void print_names_clbk(void* name, void* data)
{
    printf("%s\n", (char*) name);
}

void name_print_all(NameManager* NM, int race, int sex, int FN)
{
    Vector* target = (FN == 1 ? NM->firstNames : NM->lastNames);

    vector_map(*target, print_names_clbk, NULL);
}

char* name_get_first_name(NameManager* NM, int ID)
{
    return vector_at(NM->firstNames, ID);
}

char* name_get_last_name(NameManager* NM, int ID)
{
    return vector_at(NM->lastNames, ID);
}

void name_rand_name(NameManager* NM, int race, int sex, int* firstName, int* lastName)
{
    *firstName = NM->selectors[1][2*race + sex].start + rand() % NM->selectors[1][2*race + sex].nb;
    *lastName = NM->selectors[0][2*race + sex].start + rand() % NM->selectors[0][2*race + sex].nb;
}

void name_free_manager(NameManager* NM)
{
    vector_flush(NM->firstNames);
    vector_free(NM->firstNames);

    vector_flush(NM->lastNames);
    vector_free(NM->lastNames);

    free(NM);
}
