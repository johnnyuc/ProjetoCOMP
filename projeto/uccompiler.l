/* PROJETO META 2 */
/* Johnny Fernandes 2021190668 */
/* Stefanos Lucena  2021115090 */

/* -------------------------------------------- DEFINITIONS SECTION -------------------------------------------- */
/* ATOMIC VARS */
newline         "\n"|"\r\n"|"\r"
tab             "\t"
escape_seq      \\([nt\\'"]|[0-7]{1,3})
digit           [0-9]
letter          [a-zA-Z_]
whitespaces     " "
reserved        "["|"]"|"++"|"--"|auto|break|case|const|continue|default|do|enum|extern|float|for|goto|long|register|signed|sizeof|static|struct|switch|typedef|union|unsigned|volatile|inline|restrict|_Bool|_Complex|_Imaginary

/* COMPOSITE VARS */
number          {digit}+
decimal         {digit}+\.{digit}*([eE][+-]?{digit}+)?|\.{digit}+([eE][+-]?{digit}+)?|{digit}+[eE][+-]?{digit}+
identifier      {letter}({letter}|{digit})*

/* STATES */    
%X COMMENT
%X COMMENT2

/* CODE VARS */
%{
    // necessary includes
    #include <stdio.h>
    #include "y.tab.h"
	#include <stdbool.h>
    #define YY_USER_ACTION { syn_line = line; syn_column = col;}
    int yyparse(void); // parser

    bool lexanalysis; //flag para análise lexical
    bool syntaxanalysis = true; //flag para a análise sintática
	bool tree; //flag para a tree

    int line = 1, col = 1; // line and column counters
    int line_aux = 0, col_aux=0; // aux vars for comments
    int errors = 0; // error counter
    int syn_line = 1, syn_column = 1; // line and column counters for syntax errors

%}

/* -------------------------------------------- RULES SECTION -------------------------------------------- */
%%
"/*"                            { BEGIN(COMMENT); col_aux = col; line_aux=line; col += yyleng;}
<COMMENT>{tab}                  { col += yyleng; } 
<COMMENT>{newline}              { line++; col = 1;}
<COMMENT>.           			{ col += yyleng; }
<COMMENT><<EOF>>                { BEGIN(0); if(syntaxanalysis) printf("Line %d, column %d: unterminated comment\n", line_aux, col_aux); }
<COMMENT>"*/"                   { BEGIN(0); col += yyleng;}

"//"                            { BEGIN(COMMENT2); col_aux = col; line_aux = line; col += yyleng;}
<COMMENT2>{tab}                 {col += yyleng;}
<COMMENT2>.            			{col += yyleng;}
<COMMENT2>{newline}             { BEGIN(0); line++; col = 1; }

'([^\n'\\]|{escape_seq})'       { col += yyleng; if(lexanalysis) printf("CHRLIT(%s)\n", yytext);if(syntaxanalysis) return CHRLIT; }
'([^\n'\\]|\\.)*'               { printf("Line %d, column %d: invalid char constant (%s)\n", line, col, yytext); col += yyleng; }
'([^\n'\\]|\\.)*(\\)*           { printf("Line %d, column %d: unterminated char constant\n", line, col); col += yyleng;}
{reserved}                      { col += yyleng; if(lexanalysis) printf("RESERVED(%s)\n", yytext); if(syntaxanalysis) return RESERVED; }

"char"                          { col += yyleng; if(lexanalysis) printf("CHAR\n");       if(syntaxanalysis) return CHAR; }
"else"                          { col += yyleng; if(lexanalysis) printf("ELSE\n");       if(syntaxanalysis) return ELSE; }
"while"                         { col += yyleng; if(lexanalysis) printf("WHILE\n");      if(syntaxanalysis) return WHILE; }
"if"                            { col += yyleng; if(lexanalysis) printf("IF\n");         if(syntaxanalysis) return IF; }
"int"                           { col += yyleng; if(lexanalysis) printf("INT\n");        if(syntaxanalysis) return INT; }
"short"                         { col += yyleng; if(lexanalysis) printf("SHORT\n");      if(syntaxanalysis) return SHORT; }
"double"                        { col += yyleng; if(lexanalysis) printf("DOUBLE\n");     if(syntaxanalysis) return DOUBLE; }
"return"                        { col += yyleng; if(lexanalysis) printf("RETURN\n");     if(syntaxanalysis) return RETURN; }
"void"                          { col += yyleng; if(lexanalysis) printf("VOID\n");       if(syntaxanalysis) return VOID; }


"&"                             { col += yyleng; if(lexanalysis) printf("BITWISEAND\n"); if(syntaxanalysis) return BITWISEAND; }
"|"                             { col += yyleng; if(lexanalysis) printf("BITWISEOR\n");  if(syntaxanalysis) return BITWISEOR; }
[\^]                             { col += yyleng; if(lexanalysis) printf("BITWISEXOR\n"); if(syntaxanalysis) return BITWISEXOR; }
"&&"                            { col += yyleng; if(lexanalysis) printf("AND\n");        if(syntaxanalysis) return AND; }
"="                             { col += yyleng; if(lexanalysis) printf("ASSIGN\n");     if(syntaxanalysis) return ASSIGN; }
"*"                             { col += yyleng; if(lexanalysis) printf("MUL\n");        if(syntaxanalysis) return MUL; }
","                             { col += yyleng; if(lexanalysis) printf("COMMA\n");      if(syntaxanalysis) return COMMA; }
"/"                             { col += yyleng; if(lexanalysis) printf("DIV\n");        if(syntaxanalysis) return DIV; }
"=="                            { col += yyleng; if(lexanalysis) printf("EQ\n");         if(syntaxanalysis) return EQ; }
">="                            { col += yyleng; if(lexanalysis) printf("GE\n");         if(syntaxanalysis) return GE; }
">"                             { col += yyleng; if(lexanalysis) printf("GT\n");         if(syntaxanalysis) return GT; }
"{"                             { col += yyleng; if(lexanalysis) printf("LBRACE\n");     if(syntaxanalysis) return LBRACE; }
"<="                            { col += yyleng; if(lexanalysis) printf("LE\n");         if(syntaxanalysis) return LE; }
"("                             { col += yyleng; if(lexanalysis) printf("LPAR\n");       if(syntaxanalysis) return LPAR; }
"<"                             { col += yyleng; if(lexanalysis) printf("LT\n");         if(syntaxanalysis) return LT; }
"-"                             { col += yyleng; if(lexanalysis) printf("MINUS\n");      if(syntaxanalysis) return MINUS; }
"%"                             { col += yyleng; if(lexanalysis) printf("MOD\n");        if(syntaxanalysis) return MOD; }
"!="                            { col += yyleng; if(lexanalysis) printf("NE\n");         if(syntaxanalysis) return NE; }
"!"                             { col += yyleng; if(lexanalysis) printf("NOT\n");        if(syntaxanalysis) return NOT; }
"||"                            { col += yyleng; if(lexanalysis) printf("OR\n");         if(syntaxanalysis) return OR; }
"+"                             { col += yyleng; if(lexanalysis) printf("PLUS\n");       if(syntaxanalysis) return PLUS; }
"}"                             { col += yyleng; if(lexanalysis) printf("RBRACE\n");     if(syntaxanalysis) return RBRACE; }
")"                             { col += yyleng; if(lexanalysis) printf("RPAR\n");       if(syntaxanalysis) return RPAR; }
";"                             { col += yyleng; if(lexanalysis) printf("SEMI\n");       if(syntaxanalysis) return SEMI; }


{newline}                       { line++; col = 1; syn_column =1; syn_line ++; }
{whitespaces}|{tab}             { col += yyleng; syn_column+= yyleng;}
{number}                        { col += yyleng; yylval = atoi(yytext); if(lexanalysis) printf("NATURAL(%s)\n", yytext); if(syntaxanalysis) return NATURAL;}
{decimal}                       { col += yyleng; if(lexanalysis) printf("DECIMAL(%s)\n", yytext);return DECIMAL; }

{identifier}                    { col += yyleng; if(lexanalysis) printf("IDENTIFIER(%s)\n", yytext); if(syntaxanalysis) return IDENTIFIER; }
.                    			{ printf("Line %d, column %d: unrecognized character (%s)\n",line,col,yytext); col += yyleng; }
<<EOF>>							{if(syntaxanalysis) return 0;}

%%

/* -------------------------------------------- SUBROUTINES SECTION -------------------------------------------- */
void yyerror(char *error) {
    errors++;
    printf("Line %d, column %d: syntax error: %s\n", syn_line, syn_column, yytext);
}

extern int yylex();
int main(int argc, char *argv[]) {


    // Process command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            lexanalysis = true;
        } else if (strcmp(argv[i], "-t") == 0) {
            tree = true;
        }
    }

    if (lexanalysis) {
        // Realizar apenas análise léxica
        while (yylex());
    } 
	
	else {
        // Realizar análise sintática
        yyparse();

        if (tree) {
            // Realizar alguma ação para construir a árvore, se necessário
            // A lógica para construir a árvore seria aqui
        }
    }

    return 0;
}

int yywrap() { /* called on EOF, return 1 to terminate */
    return 1;
}