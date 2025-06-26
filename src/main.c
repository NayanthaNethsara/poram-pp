#include <stdio.h>
#include <stdlib.h>
#include "model.h"

int yyparse(void);
void generate_html(Form *form, FILE *out);

extern Form current_form;

int main(int argc, char **argv) {
    if (yyparse() == 0) {
        FILE *fout = fopen("output/form.html", "w");
        if (!fout) {
            perror("Failed to open output file");
            return 1;
        }
        generate_html(&current_form, fout);
        fclose(fout);
    } else {
        fprintf(stderr, "Parsing failed.\n");
    }

    return 0;
}
 