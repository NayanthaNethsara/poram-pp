%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>

int yywrap(void) {
  return 1;
}
%}

%x FIELD_STATE   

digit       [0-9]
id          [a-zA-Z_][a-zA-Z0-9_]*
string      \"([^\\\"]|\\.)*\"


%%

"form"              { return FORM; }
"meta"              { return META; }
"section"           { return SECTION; }
"field"             { BEGIN(FIELD_STATE); return FIELD; } 
"validate"          { return VALIDATE; }
"if"                { return IF; }
"error"             { return ERROR; }

"text"              { return TEXT; }
"textarea"          { return TEXTAREA; }
"number"            { return NUMBER_TYPE; }
"email"             { return EMAIL; }
"date"              { return DATE; }
"checkbox"          { return CHECKBOX; }
"radio"             { return RADIO; }
"dropdown"          { return DROPDOWN; }
"password"          { return PASSWORD; }
"file"              { return FILETYPE; }

"required"          { return REQUIRED; }
"default"           { return DEFAULT; }
"pattern"           { return PATTERN; }
"min"               { return MIN; }
"max"               { return MAX; }
"rows"              { return ROWS; }
"cols"              { return COLS; }
"accept"            { return ACCEPT; }
"placeholder"       { return PLACEHOLDER; }

"<="                { return LE; }
">="                { return GE; }
"=="                { return EQ; }
"!="                { return NE; }
"<"                 { return LT; }
">"                 { return GT; }
"="                 { return ASSIGN; }

"true"              { return TRUE; }
"false"             { return FALSE; }

<FIELD_STATE>{id}   { BEGIN(INITIAL); yylval.str = strdup(yytext); return IDENTIFIER; }
<FIELD_STATE>[ \t\r\n]+ ;  

{id}                { yylval.str = strdup(yytext); return IDENTIFIER; }
{string}            {
                        yylval.str = strdup(yytext);
                        yylval.str[strlen(yylval.str) - 1] = '\0';
                        memmove(yylval.str, yylval.str + 1, strlen(yylval.str)); 
                        return STRING;
                    }

{digit}+            { yylval.str = strdup(yytext); return NUMBER; }



"{"                 { return LBRACE; }
"}"                 { return RBRACE; }
"["                 { return LBRACKET; }
"]"                 { return RBRACKET; }
":"                 { return COLON; }
";"                 { return SEMICOLON; }
","                 { return COMMA; }

"//"[^\n]*          ;   
[ \t\r\n]+          ;   

.                   { return yytext[0]; }

%%
