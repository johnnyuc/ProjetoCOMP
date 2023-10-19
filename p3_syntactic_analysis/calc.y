/* FICHEIRO FORNECIDO PELO PROFESSOR [modificado] */

/* -------------------------------------------- DEFINITIONS SECTION -------------------------------------------- */
/* CODE VARS */
%{
    #include <stdio.h>
    extern int yylex(void);
    void yyerror(char *);
    extern char *yytext;
%}

/* -------------------------------------------- TOKENS SECTION -------------------------------------------- */
%token NATURAL IDENTIFIER IF THEN ELSE EQ

/* FROM HIGHER TO LOWER PRECEDENCE */
%left '+' '-'
%left '*' '/'

/* -------------------------------------------- RULES SECTION -------------------------------------------- */
%%
starting_point: calculation_list                    { printf(" <CALC_LIST>\n"); }
          | if_statement                            { printf("<IF_THEN_ELSE>\n"); }
          ;

calculation_list: expression                        { printf("%d", $1); }
          | calculation_list ',' expression         { printf(", %d", $3); }
          ;

if_statement: IF '(' expression ')' THEN expression ';' ELSE expression ';' 

expression: NATURAL                                 { $$ = $1; }
          | '(' expression ')'                      { $$ = $2; }
          | expression '+' expression               { $$ = $1 + $3; }
          | expression '-' expression               { $$ = $1 - $3; }
          | expression '*' expression               { $$ = $1 * $3; }
          | expression '/' expression               { $$ = $1 / $3; }
          | IDENTIFIER EQ expression                { }
          | IDENTIFIER '=' expression               { }
          ;
%%

/* -------------------------------------------- SUBROUTINES SECTION -------------------------------------------- */
void yyerror(char *error) {
    printf("%s '%s'\n", error, yytext);
}
