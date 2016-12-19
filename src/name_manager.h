#ifndef NAME_MANAGER_H
#define NAME_MANAGER_H

#include "vector.h"

#define FIRST_NAME 1
#define LAST_NAME 0

enum _sex_
{
    MALE = 0,
    FEMALE
};

enum _race_ 
{
    FRENCH = 0,
    ARAB,
    JEW
};

struct nameRaceSelector
{
    int start;
    int nb;
};

struct nameManager
{
    struct nameRaceSelector selectors[2][20];
    Vector* firstNames;
    Vector* lastNames;
};


struct nameManager* nameNewManager();

/*Loads names of the given race and sex from a file, FN = 1 for first names, any other
 *value for last names.
 */
void nameLoadFromFile(struct nameManager* NM, char* fileName, int race, int sex, int FN);
void namePrintAll(struct nameManager* NM, int race, int sex, int FN);

char* nameGetFirstName(struct nameManager* NM, int ID);
char* nameGetLastName(struct nameManager* NM, int ID);

void nameRandName(struct nameManager* NM, int race, int sex, int* firstName, int* lastName);

void nameFreeManager(struct nameManager* NM);

#endif
