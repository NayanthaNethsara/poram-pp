#ifndef MODEL_H
#define MODEL_H

#include <stddef.h>

#define MAX_FIELDS       100
#define MAX_SECTIONS     50
#define MAX_OPTIONS      20
#define MAX_VALIDATIONS  50

typedef struct {
    char *name;
    char *type;
    int required;
    char *default_value;
    char *options[MAX_OPTIONS];
    int option_count;
    char *min;
    char *max;
    char *pattern;
    char *placeholder;
    char *accept;
    int checked;
    int rows;
    int cols;
} Field;

typedef struct {
    char *field_name;
    char *comparator;
    char *value;
    char *message;
} Validation;

typedef struct {
    char name[100];
    Field fields[MAX_FIELDS];
    int field_count;
} Section;

typedef struct {
    char *name;
    char *author;
    Section sections[MAX_SECTIONS];  
    int section_count;
    Validation validations[MAX_VALIDATIONS];
    int validation_count;
} Form;

extern Form current_form;
void free_form(Form *form);

#endif
