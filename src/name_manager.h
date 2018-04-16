#ifndef NAME_MANAGER_H
#define NAME_MANAGER_H

#include "vector.h"

#define FIRST_NAME 1
#define LAST_NAME 0

enum Sex {
    MALE = 0,
    FEMALE
};

enum Race {
    FRENCH = 0,
    ARAB,
    JEW
};

typedef struct NameManager NameManager;

NameManager* name_new_manager();

/*Loads names of the given race and sex from a file, FN = 1 for first names, any other
 *value for last names.
 */
void name_load_from_file(NameManager* NM, char* fileName, int race, int sex, int FN);
void name_print_all(NameManager* NM, int race, int sex, int FN);

char* name_get_first_name(NameManager* NM, int ID);
char* name_get_last_name(NameManager* NM, int ID);

void name_rand_name(NameManager* NM, int race, int sex, int* firstName, int* lastName);

void name_free_manager(NameManager* NM);

#endif
