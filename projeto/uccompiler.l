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
any_char        .
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
    #include "y.tab.h"
    #include "ast.h"
    
    int yyparse(void); // parser
    void yyerror(char *); // error handler

    int lexanalysis = 0; // 0 = normal, 1 = lexical analysis
    int line = 1, col = 1; // line and column counters
    int line_aux = 1, col_aux=1; // aux vars for comments
    int errors = 0; // error counter
    int syn_line = 1, syn_column = 1; // line and column counters for syntax errors

    extern struct node *program; // AST root
%}

/* -------------------------------------------- RULES SECTION -------------------------------------------- */
%%
"/*"                            { BEGIN(COMMENT); col_aux = col; line_aux=line; col += yyleng; }
<COMMENT>{tab}                  { col += yyleng; } 
<COMMENT>{newline}              { line++; col = 1;}
<COMMENT>{any_char}             { col += yyleng; }
<COMMENT><<EOF>>                { BEGIN(0); printf("Line %d, column %d: unterminated comment\n", line_aux, col_aux); }
<COMMENT>"*/"                   { BEGIN(0); col += yyleng; }

"//"                            { BEGIN(COMMENT2); col += yyleng; }
<COMMENT2>{tab}                  
<COMMENT2>{any_char}            
<COMMENT2>{newline}             { BEGIN(0); line++; col = 1; }

'([^\n'\\]|{escape_seq})'       { if(lexanalysis) printf("CHRLIT(%s)\n", yytext); col += yyleng; }
'([^\n'\\]|\\.)*(\\)*           { printf("Line %d, column %d: unterminated char constant\n", line, col); col += yyleng;}
'([^\n'\\]|\\.)*'               { printf("Line %d, column %d: invalid char constant (%s)\n", line, col, yytext); col += yyleng; }
{reserved}                      { if(lexanalysis) printf("RESERVED(%s)\n", yytext); col += yyleng; }

"char"                          { if(lexanalysis) printf("CHAR\n"); col += yyleng; return CHAR; }
"else"                          { if(lexanalysis) printf("ELSE\n"); col += yyleng; return ELSE; }
"while"                         { if(lexanalysis) printf("WHILE\n"); col += yyleng; return WHILE; }
"if"                            { if(lexanalysis) printf("IF\n"); col += yyleng; return IF; }
"int"                           { if(lexanalysis) printf("INT\n"); col += yyleng; return INT; }
"short"                         { if(lexanalysis) printf("SHORT\n"); col += yyleng; return SHORT; }
"double"                        { if(lexanalysis) printf("DOUBLE\n"); col += yyleng; return DOUBLE; }
"return"                        { if(lexanalysis) printf("RETURN\n"); col += yyleng; return RETURN; }
"void"                          { if(lexanalysis) printf("VOID\n"); col += yyleng; return VOID; }

"&"                             { if(lexanalysis) printf("BITWISEAND\n"); col += yyleng; return BITWISEAND; }
"|"                             { if(lexanalysis) printf("BITWISEOR\n"); col += yyleng; return BITWISEOR; }
"^"                             { if(lexanalysis) printf("BITWISEXOR\n"); col += yyleng; return BITWISEXOR; }
"&&"                            { if(lexanalysis) printf("AND\n"); col += yyleng; return AND; }
"="                             { if(lexanalysis) printf("ASSIGN\n"); col += yyleng; return ASSIGN; }
"*"                             { if(lexanalysis) printf("MUL\n"); col += yyleng; return MUL; }
","                             { if(lexanalysis) printf("COMMA\n"); col += yyleng; return COMMA; }
"/"                             { if(lexanalysis) printf("DIV\n"); col += yyleng; return DIV; }
"=="                            { if(lexanalysis) printf("EQ\n"); col += yyleng; return EQ; }
">="                            { if(lexanalysis) printf("GE\n"); col += yyleng; return GE; }
">"                             { if(lexanalysis) printf("GT\n"); col += yyleng; return GT; }
"{"                             { if(lexanalysis) printf("LBRACE\n"); col += yyleng; return LBRACE; }
"<="                            { if(lexanalysis) printf("LE\n"); col += yyleng; return LE; }
"("                             { if(lexanalysis) printf("LPAR\n"); col += yyleng; return LPAR; }
"<"                             { if(lexanalysis) printf("LT\n"); col += yyleng; return LT; }
"-"                             { if(lexanalysis) printf("MINUS\n"); col += yyleng; return MINUS; }
"%"                             { if(lexanalysis) printf("MOD\n"); col += yyleng; return MOD; }
"!="                            { if(lexanalysis) printf("NE\n"); col += yyleng; return NE; }
"!"                             { if(lexanalysis) printf("NOT\n"); col += yyleng; return NOT; }
"||"                            { if(lexanalysis) printf("OR\n"); col += yyleng; return OR; }
"+"                             { if(lexanalysis) printf("PLUS\n"); col += yyleng; return PLUS; }
"}"                             { if(lexanalysis) printf("RBRACE\n"); col += yyleng; return RBRACE; }
")"                             { if(lexanalysis) printf("RPAR\n"); col += yyleng; return RPAR; }
";"                             { if(lexanalysis) printf("SEMI\n"); col += yyleng; return SEMI; }

{newline}                       { line++; col = 1; }
{whitespaces}|{tab}             { col += yyleng; }  
{number}                        { if(lexanalysis) printf("NATURAL(%s)\n", yytext); col += yyleng; }
{decimal}                       { if(lexanalysis) printf("DECIMAL(%s)\n", yytext); col += yyleng; }

{identifier}                    { if(lexanalysis) printf("IDENTIFIER(%s)\n", yytext); col += yyleng; return IDENTIFIER; }
{any_char}                      { printf("Line %d, column %d: unrecognized character (%s)\n", line, col, yytext); col += yyleng; }
%%

/* -------------------------------------------- SUBROUTINES SECTION -------------------------------------------- */
void yyerror(char *error) {
    errors++;
    printf ("Line %d, column %d :%s :%s\n", line, col, error, yytext);
}

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "-l") == 0) {
        lexanalysis = 1; // set lexical analysis flag
    } else if(argc == 2 && strcmp(argv[1], "-t") == 0 && errors == 0) {
        show(program, 0); // prints AST
    }

    yylex(); // runs the lexer
    yyparse(); // starts the parsing proccess

    deallocate(program); // frees AST
    yylex_destroy(); // frees lexer
    return 0;
}

int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}