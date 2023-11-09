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
    #include "ast.h"
    #define YY_USER_ACTION { syn_line = line; syn_column = col;}
    int yyparse(void); // parser
    extern struct node *program;

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

'([^\n'\\]|{escape_seq})'       { col += yyleng; if(lexanalysis) printf("CHRLIT(%s)\n", yytext);if(syntaxanalysis){yylval.lexeme = strdup(yytext); return CHRLIT; }}
'([^\n'\\]|\\.)*'               { printf("Line %d, column %d: invalid char constant (%s)\n", line, col, yytext); col += yyleng; }
'([^\n'\\]|\\.)*(\\)*           { printf("Line %d, column %d: unterminated char constant\n", line, col); col += yyleng;}
{reserved}                      { col += yyleng; if(lexanalysis) printf("RESERVED(%s)\n", yytext); return RESERVED; }

"char"                          { col += yyleng; if(lexanalysis) printf("CHAR\n");      return CHAR; }
"else"                          { col += yyleng; if(lexanalysis) printf("ELSE\n");      return ELSE; }
"while"                         { col += yyleng; if(lexanalysis) printf("WHILE\n");     return WHILE; }
"if"                            { col += yyleng; if(lexanalysis) printf("IF\n");        return IF; }
"int"                           { col += yyleng; if(lexanalysis) printf("INT\n");       return INT; }
"short"                         { col += yyleng; if(lexanalysis) printf("SHORT\n");     return SHORT; }
"double"                        { col += yyleng; if(lexanalysis) printf("DOUBLE\n");    return DOUBLE; }
"return"                        { col += yyleng; if(lexanalysis) printf("RETURN\n");    return RETURN; }
"void"                          { col += yyleng; if(lexanalysis) printf("VOID\n");      return VOID; }


"&"                             { col += yyleng; if(lexanalysis) printf("BITWISEAND\n");    return BITWISEAND; }
"|"                             { col += yyleng; if(lexanalysis) printf("BITWISEOR\n");     return BITWISEOR; }
"^"                             { col += yyleng; if(lexanalysis) printf("BITWISEXOR\n");   return BITWISEXOR; }
"&&"                            { col += yyleng; if(lexanalysis) printf("AND\n");           return AND; }
"="                             { col += yyleng; if(lexanalysis) printf("ASSIGN\n");        return ASSIGN; }
"*"                             { col += yyleng; if(lexanalysis) printf("MUL\n");           return MUL; }
","                             { col += yyleng; if(lexanalysis) printf("COMMA\n");         return COMMA; }
"/"                             { col += yyleng; if(lexanalysis) printf("DIV\n");           return DIV; }
"=="                            { col += yyleng; if(lexanalysis) printf("EQ\n");            return EQ; }
">="                            { col += yyleng; if(lexanalysis) printf("GE\n");            return GE; }
">"                             { col += yyleng; if(lexanalysis) printf("GT\n");            return GT; }
"{"                             { col += yyleng; if(lexanalysis) printf("LBRACE\n");        return LBRACE; }
"<="                            { col += yyleng; if(lexanalysis) printf("LE\n");            return LE; }
"("                             { col += yyleng; if(lexanalysis) printf("LPAR\n");          return LPAR; }
"<"                             { col += yyleng; if(lexanalysis) printf("LT\n");            return LT; }
"-"                             { col += yyleng; if(lexanalysis) printf("MINUS\n");         return MINUS; }
"%"                             { col += yyleng; if(lexanalysis) printf("MOD\n");           return MOD; }
"!="                            { col += yyleng; if(lexanalysis) printf("NE\n");            return NE; }
"!"                             { col += yyleng; if(lexanalysis) printf("NOT\n");           return NOT; }
"||"                            { col += yyleng; if(lexanalysis) printf("OR\n");            return OR; }
"+"                             { col += yyleng; if(lexanalysis) printf("PLUS\n");          return PLUS; }
"}"                             { col += yyleng; if(lexanalysis) printf("RBRACE\n");        return RBRACE; }
")"                             { col += yyleng; if(lexanalysis) printf("RPAR\n");          return RPAR; }
";"                             { col += yyleng; if(lexanalysis) printf("SEMI\n");          return SEMI; }


{newline}                       { line++; col = 1; syn_column =1; syn_line ++; }
{whitespaces}|{tab}             { col += yyleng; syn_column+= yyleng;}
{number}                        { col += yyleng; if(lexanalysis) printf("NATURAL(%s)\n", yytext); yylval.lexeme = strdup(yytext); return NATURAL; }
{decimal}                       { col += yyleng; if(lexanalysis) printf("DECIMAL(%s)\n", yytext); yylval.lexeme = strdup(yytext);return DECIMAL; } 

{identifier}                    { col += yyleng; if(lexanalysis) printf("IDENTIFIER(%s)\n", yytext); yylval.lexeme = strdup(yytext); return IDENTIFIER; }
.                    			{ printf("Line %d, column %d: unrecognized character (%s)\n",line,col,yytext); col += yyleng; }
<<EOF>>							{ return 0; }

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

    if (lexanalysis) while (yylex());
	
	else {
        // Syntax analysis (parsing)
        yyparse();

        if (tree && errors==0) {
            show(program, 0);
        }
    }

    deallocate(program); // free the AST
    yylex_destroy(); // free the lexer

    return 0;
}

int yywrap() { /* called on EOF, return 1 to terminate */
    return 1;
}