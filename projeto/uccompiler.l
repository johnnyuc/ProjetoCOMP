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
    // Necessary includes
    #include <stdio.h>
    #include "y.tab.h"
    #include "ast.h"
    #include "semantics.h"

    // Macros
    #define LEXMODE 1
    #define SYNMODE 2
    #define SEMMODE 3
    #define YY_USER_ACTION { syn_line = line; syn_column = col;}

    // Lexing, parsing and AST variables
    int yyparse(void); 
    extern struct node *program;

    // Mode variable
    int mode = 0;

    // Counting variables
    int line = 1, col = 1; // line and column counters
    int line_aux = 0, col_aux=0; // aux vars for comments
    int errors = 0; // error counter
    int syn_line = 1, syn_column = 1; // line and column counters for syntax errors
%}

/* -------------------------------------------- RULES SECTION -------------------------------------------- */
%%
"/*"                            { BEGIN(COMMENT); col_aux = col; line_aux=line; col += yyleng; }
<COMMENT>{tab}                  { col += yyleng; } 
<COMMENT>{newline}              { line++; col = 1; }
<COMMENT>.           			{ col += yyleng; }
<COMMENT><<EOF>>                { BEGIN(0); printf("Line %d, column %d: unterminated comment\n", line_aux, col_aux); }
<COMMENT>"*/"                   { BEGIN(0); col += yyleng; }

"//"                            { BEGIN(COMMENT2); col_aux = col; line_aux = line; col += yyleng; }
<COMMENT2>{tab}                 { col += yyleng; }
<COMMENT2>.            			{ col += yyleng; }
<COMMENT2>{newline}             { BEGIN(0); line++; col = 1; }

'([^\n'\\]|{escape_seq})'       { col += yyleng; if(mode == LEXMODE) printf("CHRLIT(%s)\n", yytext); yylval.lexeme = strdup(yytext); return CHRLIT; }
'([^\n'\\]|\\.)*'               { printf("Line %d, column %d: invalid char constant (%s)\n", line, col, yytext); col += yyleng; }
'([^\n'\\]|\\.)*(\\)*           { printf("Line %d, column %d: unterminated char constant\n", line, col); col += yyleng; }
{reserved}                      { col += yyleng; if(mode == LEXMODE) printf("RESERVED(%s)\n", yytext); return RESERVED; }

"char"                          { col += yyleng; if(mode == LEXMODE) printf("CHAR\n");      return CHAR; }
"else"                          { col += yyleng; if(mode == LEXMODE) printf("ELSE\n");      return ELSE; }
"while"                         { col += yyleng; if(mode == LEXMODE) printf("WHILE\n");     return WHILE; }
"if"                            { col += yyleng; if(mode == LEXMODE) printf("IF\n");        return IF; }
"int"                           { col += yyleng; if(mode == LEXMODE) printf("INT\n");       return INT; }
"short"                         { col += yyleng; if(mode == LEXMODE) printf("SHORT\n");     return SHORT; }
"double"                        { col += yyleng; if(mode == LEXMODE) printf("DOUBLE\n");    return DOUBLE; }
"return"                        { col += yyleng; if(mode == LEXMODE) printf("RETURN\n");    return RETURN; }
"void"                          { col += yyleng; if(mode == LEXMODE) printf("VOID\n");      return VOID; }

"&"                             { col += yyleng; if(mode == LEXMODE) printf("BITWISEAND\n");    return BITWISEAND; }
"|"                             { col += yyleng; if(mode == LEXMODE) printf("BITWISEOR\n");     return BITWISEOR; }
"^"                             { col += yyleng; if(mode == LEXMODE) printf("BITWISEXOR\n");    return BITWISEXOR; }
"&&"                            { col += yyleng; if(mode == LEXMODE) printf("AND\n");           return AND; }
"="                             { col += yyleng; if(mode == LEXMODE) printf("ASSIGN\n");        return ASSIGN; }
"*"                             { col += yyleng; if(mode == LEXMODE) printf("MUL\n");           return MUL; }
","                             { col += yyleng; if(mode == LEXMODE) printf("COMMA\n");         return COMMA; }
"/"                             { col += yyleng; if(mode == LEXMODE) printf("DIV\n");           return DIV; }
"=="                            { col += yyleng; if(mode == LEXMODE) printf("EQ\n");            return EQ; }
">="                            { col += yyleng; if(mode == LEXMODE) printf("GE\n");            return GE; }
">"                             { col += yyleng; if(mode == LEXMODE) printf("GT\n");            return GT; }
"{"                             { col += yyleng; if(mode == LEXMODE) printf("LBRACE\n");        return LBRACE; }
"<="                            { col += yyleng; if(mode == LEXMODE) printf("LE\n");            return LE; }
"("                             { col += yyleng; if(mode == LEXMODE) printf("LPAR\n");          return LPAR; }
"<"                             { col += yyleng; if(mode == LEXMODE) printf("LT\n");            return LT; }
"-"                             { col += yyleng; if(mode == LEXMODE) printf("MINUS\n");         return MINUS; }
"%"                             { col += yyleng; if(mode == LEXMODE) printf("MOD\n");           return MOD; }
"!="                            { col += yyleng; if(mode == LEXMODE) printf("NE\n");            return NE; }
"!"                             { col += yyleng; if(mode == LEXMODE) printf("NOT\n");           return NOT; }
"||"                            { col += yyleng; if(mode == LEXMODE) printf("OR\n");            return OR; }
"+"                             { col += yyleng; if(mode == LEXMODE) printf("PLUS\n");          return PLUS; }
"}"                             { col += yyleng; if(mode == LEXMODE) printf("RBRACE\n");        return RBRACE; }
")"                             { col += yyleng; if(mode == LEXMODE) printf("RPAR\n");          return RPAR; }
";"                             { col += yyleng; if(mode == LEXMODE) printf("SEMI\n");          return SEMI; }


{newline}                       { line++; col = 1; syn_column =1; syn_line ++; }
{whitespaces}|{tab}             { col += yyleng; syn_column+= yyleng;}
{number}                        { col += yyleng; if(mode == LEXMODE) printf("NATURAL(%s)\n", yytext); yylval.lexeme = strdup(yytext); return NATURAL; }
{decimal}                       { col += yyleng; if(mode == LEXMODE) printf("DECIMAL(%s)\n", yytext); yylval.lexeme = strdup(yytext);return DECIMAL; } 

{identifier}                    { col += yyleng; if(mode == LEXMODE) printf("IDENTIFIER(%s)\n", yytext); yylval.lexeme = strdup(yytext); return IDENTIFIER; }
.                    			{ printf("Line %d, column %d: unrecognized character (%s)\n",line,col,yytext); col += yyleng; }
<<EOF>>							{ return 0; }
%%

/* -------------------------------------------- SUBROUTINES SECTION -------------------------------------------- */
void yyerror(char *error) {
    errors++;
    printf("Line %d, column %d: syntax error: %s\n", syn_line, syn_column, yytext);
}

extern int yylex();
int main(int argc, char* argv[]) {

    if (strcmp(argv[1], "-l") == 0) mode = LEXMODE;
    else if (strcmp(argv[1], "-t") == 0) mode = SYNMODE;
    else if (strcmp(argv[1], "-s") == 0) mode = SEMMODE;

    if (mode == LEXMODE) while (yylex());
	else if (mode == SYNMODE) {
        yyparse();
        if (!errors) show(program, 0);
    }
    else if (mode == SEMMODE) {
        yyparse();
        
        JohnnyTree(NULL, NULL, program);
        errors += check_program(program); // Prints errors if there is any
        show_symbol_table(); // Proceeds to show symbol table
        //show(program, 0); // Prints AST with annotations
    }

    deallocate(program); // Free AST
    yylex_destroy(); // Free lexer

    return 0;
}

int yywrap() { /* called on EOF, return 1 to terminate */
    return 1;
}