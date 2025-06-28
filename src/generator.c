#include <stdio.h>
#include <string.h>
#include "model.h"

void generate_html(Form *form, FILE *out, OutputOptions *opts) {
    // Theme styles
    const char *bg = strcmp(opts->theme, "light") == 0 ? "#ffffff" : "#121212";
    const char *fg = strcmp(opts->theme, "light") == 0 ? "#111111" : "#eeeeee";
    const char *input_bg = strcmp(opts->theme, "light") == 0 ? "#f4f4f4" : "#2a2a2a";

    // Shape styles
    const char *radius = strcmp(opts->shape, "square") == 0 ? "0" : "6px";

    // Style tweaks
    const char *form_shadow = strcmp(opts->style, "fancy") == 0 ? "box-shadow: 0 0 10px rgba(0,0,0,0.5);" : "";
    const char *hover = strcmp(opts->style, "fancy") == 0 ? "button:hover { background: #1b61c1; }" : "";

    fprintf(out,
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "  <meta charset=\"UTF-8\">\n"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "  <title>%s</title>\n"
        "  <style>\n"
        "    body { background: %s; color: %s; font-family: sans-serif; padding: 20px; }\n"
        "    form { max-width: 800px; margin: auto; background: %s; padding: 20px; border-radius: %s; %s }\n"
        "    fieldset { border: 1px solid #444; margin-bottom: 20px; padding: 15px; border-radius: %s; }\n"
        "    legend { font-size: 1.2em; margin-bottom: 10px; }\n"
        "    label { display: block; margin-bottom: 6px; font-weight: bold; }\n"
        "    input, select, textarea {\n"
        "      width: 100%%; padding: 8px; border: 1px solid #555;\n"
        "      border-radius: %s; background: %s; color: %s; margin-bottom: 15px;\n"
        "    }\n"
        "    input[type=radio], input[type=checkbox] {\n"
        "      width: auto; margin-right: 8px;\n"
        "    }\n"
        "    button {\n"
        "      width: 100%%; padding: 12px; background: #2d7ddf; color: white;\n"
        "      font-weight: bold; border: none; border-radius: %s; cursor: pointer;\n"
        "    }\n"
        "    %s\n"
        "  </style>\n"
        "</head>\n"
        "<body>\n"
        "<form name=\"%s\">\n",
        form->name, bg, fg, strcmp(opts->theme, "light") == 0 ? "#f9f9f9" : "#1e1e1e",
        radius, form_shadow, radius, radius, input_bg, fg, radius, hover, form->name);

    for (int i = 0; i < form->section_count; i++) {
        Section *sec = &form->sections[i];
        fprintf(out, "  <fieldset>\n    <legend>%s</legend>\n", sec->name);

        for (int j = 0; j < sec->field_count; j++) {
            Field *f = &sec->fields[j];
            fprintf(out,
                "    <div>\n"
                "      <label for=\"%s\">%s%s</label>\n",
                f->name, f->name, f->required ? " *" : "");

            if (strcmp(f->type, "radio") == 0) {
                for (int k = 0; k < f->option_count; k++) {
                    fprintf(out,
                        "      <div>\n"
                        "        <input type=\"radio\" id=\"%s_%d\" name=\"%s\" value=\"%s\"%s>\n"
                        "        <label for=\"%s_%d\">%s</label>\n"
                        "      </div>\n",
                        f->name, k, f->name, f->options[k],
                        f->required ? " required" : "",
                        f->name, k, f->options[k]);
                }
            } else if (strcmp(f->type, "dropdown") == 0) {
                fprintf(out, "      <select id=\"%s\" name=\"%s\"%s>\n",
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
                    "      <input type=\"checkbox\" id=\"%s\" name=\"%s\"%s%s>\n"
                    "      <label for=\"%s\">%s</label>\n",
                    f->name, f->name,
                    f->checked ? " checked" : "",
                    f->required ? " required" : "",
                    f->name, f->name);
            } else if (strcmp(f->type, "textarea") == 0) {
                fprintf(out,
                    "      <textarea id=\"%s\" name=\"%s\" rows=\"4\"%s></textarea>\n",
                    f->name, f->name, f->required ? " required" : "");
            } else {
                fprintf(out,
                    "      <input type=\"%s\" id=\"%s\" name=\"%s\"",
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
        "  <button type=\"submit\">Submit</button>\n"
        "</form>\n");

    if (form->validation_count > 0) {
        fprintf(out, "<script>\n");
        fprintf(out, "document.querySelector('form').addEventListener('submit', function(e) {\n");
        for (int i = 0; i < form->validation_count; i++) {
            Validation *v = &form->validations[i];
            fprintf(out,
                "  if (document.forms[0]['%s'].value %s %s) {\n"
                "    alert('%s'); e.preventDefault(); return; }\n",
                v->field_name, v->comparator, v->value, v->message);
        }
        fprintf(out, "});\n</script>\n");
    }

    fprintf(out, "</body>\n</html>\n");
}
