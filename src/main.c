#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include "model.h"
#include <bits/getopt_core.h>

int yyparse(void);
void generate_html(Form *form, FILE *out);

extern Form current_form;

int main(int argc, char **argv) {
    char *output_path = NULL;
    FILE *fout = NULL;
    int opt;

    // Parse command-line options
    while ((opt = getopt(argc, argv, "o:")) != -1) {
        switch (opt) {
            case 'o':
                output_path = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-o output_file.html]\n", argv[0]);
                return 1;
        }
    }

    // Run parser
    if (yyparse() == 0) {
        // Decide output destination
        if (output_path) {
            fout = fopen(output_path, "w");
            if (!fout) {
                perror("Failed to open output file");
                return 1;
            }
        } else {
            fout = stdout;
        }

        generate_html(&current_form, fout);
        free_form(&current_form);


        if (fout != stdout) {
            fclose(fout);
        }
    } else {
        fprintf(stderr, "Parsing failed.\n");
        return 1;
    }

    return 0;
}
