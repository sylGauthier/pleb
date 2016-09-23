#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "name_manager.h"

struct nameManager* nameNewManager()
{
    struct nameManager* NM = malloc(sizeof(struct nameManager));

    int i = 0;

    for (i = 0; i < 20; i++)
    {
        NM->selectors[0][i].start = -1;
        NM->selectors[0][i].nb = -1;
        NM->selectors[1][i].start = -1;
        NM->selectors[1][i].nb = -1;
    }

    NM->firstNames = vectorNew();
    NM->lastNames = vectorNew();

    return NM;
}

void nameLoadFromFile(struct nameManager* NM, char* fileName, int race, int sex, int FN)
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

        //To avoid valgrind annoying error of "cond jump depends on uninitialized value"
        strncpy(curName, "--------------------", 20);
        vectorPush(target, curName);

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

    free(vectorPop(target)); //The last element corresponds to an empty line so we don't want to keep it.
    count--;
    NM->selectors[FN][race*2 + sex].nb = count;

    printf("%d names loaded from %s\n", count, fileName);
    printf("Names range from %d to %d\n", NM->selectors[FN][race*2 + sex].start, NM->selectors[FN][race*2 + sex].start + count - 1);

    fclose(f);
}

static void printNamesClbk(void* name, void* data)
{
    printf("%s\n", (char*) name);
}

void namePrintAll(struct nameManager* NM, int race, int sex, int FN)
{
    Vector* target = (FN == 1 ? NM->firstNames : NM->lastNames);

    vectorMap(*target, printNamesClbk, NULL);
}

char* nameGetFirstName(struct nameManager* NM, int ID)
{
    return vectorAt(NM->firstNames, ID);
}

char* nameGetLastName(struct nameManager* NM, int ID)
{
    return vectorAt(NM->lastNames, ID);
}

void nameRandName(struct nameManager* NM, int race, int sex, int* firstName, int* lastName)
{
    *firstName = NM->selectors[1][2*race + sex].start + rand() % NM->selectors[1][2*race + sex].nb;
    *lastName = NM->selectors[0][2*race + sex].start + rand() % NM->selectors[0][2*race + sex].nb;
}

void nameFreeManager(struct nameManager* NM)
{
    vectorFlush(NM->firstNames);
    vectorFree(NM->firstNames);

    vectorFlush(NM->lastNames);
    vectorFree(NM->lastNames);

    free(NM);
}
