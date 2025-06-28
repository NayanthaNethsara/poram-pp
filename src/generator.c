#include <stdio.h>
#include <string.h>
#include "model.h"

void generate_html(Form *form, FILE *out) {
    fprintf(out,
        "<!DOCTYPE html>\n"
        "<html lang=\"en\" class=\"dark\">\n"
        "<head>\n"
        "  <meta charset=\"UTF-8\">\n"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "  <title>%s</title>\n"
        "  <script src=\"https://cdn.tailwindcss.com\"></script>\n"
        "  <script>\n"
        "    tailwind.config = { darkMode: 'class' }\n"
        "  </script>\n"
        "</head>\n"
        "<body class=\"bg-gray-900 text-gray-100 font-sans p-6\">\n"
        "<form name=\"%s\" class=\"max-w-3xl mx-auto bg-gray-800 p-8 rounded-lg shadow-lg\">\n",
        form->name, form->name);

    for (int i = 0; i < form->section_count; i++) {
        Section *sec = &form->sections[i];
        fprintf(out,
            "  <fieldset class=\"mb-8 border border-gray-700 rounded-md p-6\">\n"
            "    <legend class=\"text-xl font-semibold mb-4 px-2\">%s</legend>\n",
            sec->name);

        // Loop over fields in the section
        for (int j = 0; j < sec->field_count; j++) {
            Field *f = &sec->fields[j];

            fprintf(out,
                "    <div class=\"mb-6\">\n"
                "      <label class=\"block mb-2 font-medium\" for=\"%s\">%s%s</label>\n",
                f->name, f->name, f->required ? " *" : "");

            if (strcmp(f->type, "radio") == 0) {
                for (int k = 0; k < f->option_count; k++) {
                    fprintf(out,
                        "      <div class=\"flex items-center mb-1\">\n"
                        "        <input type=\"radio\" id=\"%s_%d\" name=\"%s\" value=\"%s\" class=\"mr-2\"%s>\n"
                        "        <label for=\"%s_%d\" class=\"cursor-pointer\">%s</label>\n"
                        "      </div>\n",
                        f->name, k, f->name, f->options[k],
                        f->required ? " required" : "",
                        f->name, k, f->options[k]);
                }
            } else if (strcmp(f->type, "dropdown") == 0) {
                fprintf(out,
                    "      <select id=\"%s\" name=\"%s\" class=\"w-full p-2 rounded bg-gray-700 text-gray-100 border border-gray-600 focus:outline-none focus:ring-2 focus:ring-blue-500\"%s>\n",
                    f->name, f->name, f->required ? " required" : "");
                for (int k = 0; k < f->option_count; k++) {
                    int selected = (f->default_value && strcmp(f->default_value, f->options[k]) == 0);
                    fprintf(out,
                        "        <option value=\"%s\"%s>%s</option>\n",
                        f->options[k],
                        selected ? " selected" : "",
                        f->options[k]);
                }
                fprintf(out, "      </select>\n");
            } else if (strcmp(f->type, "checkbox") == 0) {
                fprintf(out,
                    "      <input type=\"checkbox\" id=\"%s\" name=\"%s\" class=\"mr-2\"%s%s>\n",
                    f->name,
                    f->name,
                    (f->checked) ? " checked" : "",
                    f->required ? " required" : "");
                fprintf(out,
                    "      <label for=\"%s\" class=\"cursor-pointer\">%s</label>\n",
                    f->name, f->name);
            } else if (strcmp(f->type, "textarea") == 0) {
                fprintf(out,
                    "      <textarea id=\"%s\" name=\"%s\" class=\"w-full p-2 rounded bg-gray-700 text-gray-100 border border-gray-600 focus:outline-none focus:ring-2 focus:ring-blue-500\" rows=\"4\"%s></textarea>\n",
                    f->name,
                    f->name,
                    f->required ? " required" : "");
            } else if (strcmp(f->type, "file") == 0) {
                fprintf(out,
                    "      <input type=\"file\" id=\"%s\" name=\"%s\" class=\"w-full p-2 rounded bg-gray-700 text-gray-100 border border-gray-600 focus:outline-none focus:ring-2 focus:ring-blue-500\"%s%s>\n",
                    f->name,
                    f->name,
                    (f->default_value) ? f->default_value : "",
                    f->required ? " required" : "");
            } else {
                fprintf(out,
                    "      <input type=\"%s\" id=\"%s\" name=\"%s\" class=\"w-full p-2 rounded bg-gray-700 text-gray-100 border border-gray-600 focus:outline-none focus:ring-2 focus:ring-blue-500\"",
                    f->type, f->name, f->name);

                if (f->required)
                    fprintf(out, " required");

                if (f->min)
                    fprintf(out, " min=\"%s\"", f->min);

                if (f->max)
                    fprintf(out, " max=\"%s\"", f->max);

                if (f->default_value)
                    fprintf(out, " value=\"%s\"", f->default_value);

                if (f->pattern)
                    fprintf(out, " pattern=\"%s\"", f->pattern);

                fprintf(out, ">\n");
            }

            fprintf(out, "    </div>\n");
        }

        fprintf(out, "  </fieldset>\n");
    }

    fprintf(out,
        "  <button type=\"submit\" class=\"w-full bg-blue-600 hover:bg-blue-700 text-white font-bold py-3 rounded\">Submit</button>\n"
        "</form>\n");

    if (form->validation_count > 0) {
        fprintf(out, "<script>\n");
        fprintf(out, "document.querySelector('form').addEventListener('submit', function(e) {\n");

        for (int i = 0; i < form->validation_count; i++) {
            Validation *v = &form->validations[i];
            const char *val = v->value;
            int needs_quotes = 1;
            if (val[0] == '"' || val[0] == '\'') {
                needs_quotes = 0;
            } else {
                int j = 0, is_num = 1;
                while (val[j] != '\0') {
                    if ((val[j] < '0' || val[j] > '9') && val[j] != '.') {
                        is_num = 0;
                        break;
                    }
                    j++;
                }
                if (is_num) needs_quotes = 0;
            }

fprintf(out,
    "  if (document.forms[0]['%s'].value %s %s) {\n"
    "    alert('%s'); e.preventDefault(); return; }\n",
    v->field_name,
    v->comparator,
    v->value,
    v->message
);

        }
        fprintf(out, "});\n</script>\n");
    }
    fprintf(out,
        "</body>\n"
        "</html>\n");
}


