/* FICHEIRO FORNECIDO PELO PROFESSOR [modificado] */

/* -------------------------------------------- DEFINITIONS SECTION -------------------------------------------- */
/* ATOMIC VARS */
digit           [0-9]
letter          [a-zA-Z_]
identifier      {letter}({letter}|{digit})*
/* CODE VARS */
%{
#include "y.tab.h"
int yyparse(void);
%}

/* -------------------------------------------- RULES SECTION -------------------------------------------- */
%%
if                      { return IF; }
then                    { return THEN; }
else                    { return ELSE; }

"=="                    { return EQ; }

{digit}+                { yylval = atoi(yytext); return NATURAL; }
{identifier}            { return IDENTIFIER; }
[()=,*/+-;]             { return yytext[0]; }

\n                      { return 0; }
" "|"\t"                { }
.                       { printf("Unrecognized character: %s\n", yytext); }
%%

/* -------------------------------------------- SUBROUTINES SECTION -------------------------------------------- */
int main() {
    yyparse();
    return 0;
}
int yywrap() {
    return 1;
}
