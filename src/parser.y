%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "model.h"
#include "field_rules.h"


int yylex(void);
void yyerror(const char *s);

Form current_form;
Section *current_section = NULL;
Field current_field;
bool has_options = false;

%}

%union {
    char *str;
}

%token <str> IDENTIFIER STRING NUMBER
%token FORM META SECTION FIELD VALIDATE IF ERROR
%token TEXT TEXTAREA NUMBER_TYPE EMAIL DATE CHECKBOX RADIO DROPDOWN PASSWORD FILETYPE
%token REQUIRED DEFAULT PATTERN MIN MAX ROWS COLS ACCEPT PLACEHOLDER
%token LE GE EQ NE LT GT ASSIGN
%token LBRACE RBRACE LBRACKET RBRACKET COLON SEMICOLON COMMA
%token TRUE FALSE

%type <str> field_type
%type <str> comparator
%type <str> value

%%

form : FORM IDENTIFIER LBRACE form_body RBRACE {
    current_form.name = $2;
}
;

form_body : form_body form_item
          | 
;

form_item : section
          | meta_block
          | validate_block
;

meta_block : META IDENTIFIER ASSIGN STRING SEMICOLON {
    if (strcmp($2, "author") == 0) {
        current_form.author = $4;
    }
}
;

section : SECTION IDENTIFIER LBRACE {
        current_section = calloc(1, sizeof(Section));
    } field_list RBRACE {
        strcpy(current_form.sections[current_form.section_count].name, $2);
        current_form.sections[current_form.section_count].field_count = current_section->field_count;
        memcpy(current_form.sections[current_form.section_count].fields, current_section->fields,
               sizeof(Field) * current_section->field_count);
        current_form.section_count++;
        free(current_section);
    }
;

field_list : field_list field
           | field
;

field : FIELD IDENTIFIER COLON field_type attributes_opt SEMICOLON {
    
    current_field.name = $2;
    current_field.type = $4;

    if ((strcmp($4, "radio") == 0 || strcmp($4, "dropdown") == 0) && !has_options) {
        fprintf(stderr, "Error: '%s' field must have options.\n", $4);
        exit(1);
    }

    current_section->fields[current_section->field_count++] = current_field;
    memset(&current_field, 0, sizeof(Field));
    has_options = false;
}
;

field_type : TEXT        { $$ = strdup("text"); }
           | TEXTAREA    { $$ = strdup("textarea"); }
           | NUMBER_TYPE { $$ = strdup("number"); }
           | EMAIL       { $$ = strdup("email"); }
           | DATE        { $$ = strdup("date"); }
           | CHECKBOX    { $$ = strdup("checkbox"); }
           | RADIO       { $$ = strdup("radio"); }
           | DROPDOWN    { $$ = strdup("dropdown"); }
           | PASSWORD    { $$ = strdup("password"); }
           | FILETYPE    { $$ = strdup("file"); }
;

attributes_opt : attributes
               | 
;

attributes : attributes attribute
           | attribute
;

attribute
    : REQUIRED {
        current_field.required = 1;
    }
    | DEFAULT ASSIGN STRING {
        current_field.default_value = $3;
    }
    | DEFAULT ASSIGN TRUE {
        current_field.checked = 1;
    }
    | DEFAULT ASSIGN FALSE {
        current_field.checked = 0;
    }
    | MIN ASSIGN value {
        current_field.min = strdup($3);
    }
    | MAX ASSIGN value {
        current_field.max = strdup($3);
    }
    | LBRACKET option_list RBRACKET {
        has_options = true;
    }
    | ACCEPT ASSIGN STRING {
        current_field.accept = $3; 
    }
    | PATTERN ASSIGN STRING {
        current_field.pattern = $3;
    }
    | PLACEHOLDER ASSIGN STRING {
        current_field.placeholder = $3;
    }
    | ROWS ASSIGN NUMBER {
        current_field.rows = atoi($3);
    }
    | COLS ASSIGN NUMBER {
        current_field.cols = atoi($3);
    }

;

value
    : NUMBER { $$ = $1; }
    | STRING { $$ = $1; }
    ;


option_list
    : STRING {
        current_field.options[0] = $1;
        current_field.option_count = 1;
    }
    | option_list COMMA STRING {
        current_field.options[current_field.option_count++] = $3;
    }
;

validate_block : VALIDATE LBRACE validation_rules RBRACE ;

validation_rules
    : validation_rules validation_rule
    | validation_rule
;

validation_rule
    : IF IDENTIFIER comparator value LBRACE ERROR STRING SEMICOLON RBRACE {
        Validation *v = &current_form.validations[current_form.validation_count++];
        v->field_name = $2;
        v->comparator = $3;
        v->value = $4;
        v->message = $7;
    }
;

comparator
    : LT { $$ = strdup("<"); }
    | GT { $$ = strdup(">"); }
    | EQ { $$ = strdup("=="); }
    | NE { $$ = strdup("!="); }
    | LE { $$ = strdup("<="); }
    | GE { $$ = strdup(">="); }
;



%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error: %s\n", s);
}
