#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "model.h"

int yyparse(void);
void generate_html(Form *form, FILE *out, OutputOptions *opts);
void free_form(Form *form);

extern Form current_form;

int main(int argc, char **argv) {
    char *output_path = NULL;
    FILE *fout = NULL;
    int opt;

    OutputOptions options = {
        .theme = "dark",      // default values
        .shape = "rounded",
        .style = "minimal"
    };

    // Parse command-line options
    while ((opt = getopt(argc, argv, "o:t:s:m:")) != -1) {
        switch (opt) {
            case 'o':
                output_path = optarg;
                break;
            case 't':
                options.theme = optarg;
                break;
            case 's':
                options.shape = optarg;
                break;
            case 'm':
                options.style = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-o output.html] [-t theme] [-s shape] [-m style]\n", argv[0]);
                fprintf(stderr, "  -t theme: light or dark\n");
                fprintf(stderr, "  -s shape: rounded or square\n");
                fprintf(stderr, "  -m style: minimal or fancy\n");
                return 1;
        }
    }

    if (yyparse() == 0) {
        fout = output_path ? fopen(output_path, "w") : stdout;
        if (!fout) {
            perror("Failed to open output file");
            return 1;
        }

        generate_html(&current_form, fout, &options);
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
