/* FICHEIRO FORNECIDO PELO PROFESSOR [modificado] */

/* -------------------------------------------- DEFINITIONS SECTION -------------------------------------------- */
%{

#include "y.tab.h"
#include "ast.h"

int yyparse(void);
void yyerror(char *);

int lex_line = 1, lex_column = 1;
int syn_line = -1, syn_column = -1;
int errors = 0;

extern struct node *program;

%}

/* ends after the first end-of-file is reached | only reads one input file */
%option noyywrap

%X COMMENT

digit   [0-9]
letter  [a-zA-Z]

/* -------------------------------------------- RULES SECTION -------------------------------------------- */
%%
integer                     { return INTEGER; }
double                      { return DOUBLE; }
if                          { return IF; }
then                        { return THEN; }
else                        { return ELSE; }
{letter}({letter}|{digit})* { yylval.lexeme = strdup(yytext); return IDENTIFIER; }
{digit}+                    { yylval.lexeme = strdup(yytext); return NATURAL; }
{digit}*"."{digit}+         { yylval.lexeme = strdup(yytext); return DECIMAL; }

"/*"                        { BEGIN(COMMENT); }
<COMMENT>"*/"               { BEGIN(INITIAL); }
<COMMENT>\n|\r|\r\n         { lex_line++; lex_column = 1; }
<COMMENT>.                  {;}
<COMMENT><<EOF>>            { return 0; }

[\t ]+                      {;}
\n|\r|\r\n                  { lex_line++; lex_column = 1; }
[()=,*/+-]                  { return yytext[0]; }
.                           { yyerror("unrecognized character"); }
<<EOF>>                     { return 0; }
%%

/* -------------------------------------------- SUBROUTINES SECTION -------------------------------------------- */
void yyerror(char *error) {
    errors++;
    printf("Found %s '%s' (%d,%d)\n", error, yytext, syn_line, syn_column);
}

int main(int argc, char *argv[]) {
    yyparse();
    if(argc >= 2 && strcmp(argv[1], "-t") == 0 && errors == 0) {   /* "-t" prints the AST */
        show(program, 0);
    }
    deallocate(program); // free the AST
    yylex_destroy(); // free the lexer
    return 0;
}
