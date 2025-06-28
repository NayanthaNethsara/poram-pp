#include <stdlib.h>
#include <string.h>
#include "model.h"

void free_form(Form *form) {
    free(form->name);
    free(form->author);

    for (int i = 0; i < form->section_count; i++) {
        Section *sec = &form->sections[i];
        for (int j = 0; j < sec->field_count; j++) {
            Field *f = &sec->fields[j];
            free(f->name);
            free(f->type);
            free(f->default_value);
            free(f->min);
            free(f->max);
            free(f->pattern);
            free(f->placeholder);
            free(f->accept);

            for (int k = 0; k < f->option_count; k++) {
                free(f->options[k]);
            }
        }
    }

    for (int i = 0; i < form->validation_count; i++) {
        Validation *v = &form->validations[i];
        free(v->field_name);
        free(v->comparator);
        free(v->value);
        free(v->message);
    }

    *form = (Form){0};
}
