/* FICHEIRO FORNECIDO PELO PROFESSOR */

%{
#include <stdio.h>
extern int yylex(void);
void yyerror(char *);
extern char *yytext;
%}

%token NATURAL

/* FROM HIGHER TO LOWER PRECEDENCE */
%left '+' '-'
%left '*' '/'

%%

calculator: calculation
          | calculator ',' calculation

calculation: expression                  { printf("%d\n", $1); }

expression: NATURAL                     { $$ = $1; }
          | '(' expression ')'          { $$ = $2; }
          | expression '+' expression   { $$ = $1 + $3; }
          | expression '-' expression   { $$ = $1 - $3; }
          | expression '*' expression   { $$ = $1 * $3; }
          | expression '/' expression   { $$ = $1 / $3; }
          ;

%%

void yyerror(char *error) {
    printf("%s '%s'\n", error, yytext);
}
