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

"char"                          { col += yyleng; if(lexanalysis) printf("CHAR\n");       if(!lexanalysis) return CHAR; }
"else"                          { col += yyleng; if(lexanalysis) printf("ELSE\n");       if(!lexanalysis) return ELSE; }
"while"                         { col += yyleng; if(lexanalysis) printf("WHILE\n");      if(!lexanalysis) return WHILE; }
"if"                            { col += yyleng; if(lexanalysis) printf("IF\n");         if(!lexanalysis) return IF; }
"int"                           { col += yyleng; if(lexanalysis) printf("INT\n");        if(!lexanalysis) return INT; }
"short"                         { col += yyleng; if(lexanalysis) printf("SHORT\n");      if(!lexanalysis) return SHORT; }
"double"                        { col += yyleng; if(lexanalysis) printf("DOUBLE\n");     if(!lexanalysis) return DOUBLE; }
"return"                        { col += yyleng; if(lexanalysis) printf("RETURN\n");     if(!lexanalysis) return RETURN; }
"void"                          { col += yyleng; if(lexanalysis) printf("VOID\n");       if(!lexanalysis) return VOID; }

"&"                             { col += yyleng; if(lexanalysis) printf("BITWISEAND\n"); if(!lexanalysis) return BITWISEAND; }
"|"                             { col += yyleng; if(lexanalysis) printf("BITWISEOR\n");  if(!lexanalysis) return BITWISEOR; }
"^"                             { col += yyleng; if(lexanalysis) printf("BITWISEXOR\n"); if(!lexanalysis) return BITWISEXOR; }
"&&"                            { col += yyleng; if(lexanalysis) printf("AND\n");        if(!lexanalysis) return AND; }
"="                             { col += yyleng; if(lexanalysis) printf("ASSIGN\n");     if(!lexanalysis) return ASSIGN; }
"*"                             { col += yyleng; if(lexanalysis) printf("MUL\n");        if(!lexanalysis) return MUL; }
","                             { col += yyleng; if(lexanalysis) printf("COMMA\n");      if(!lexanalysis) return COMMA; }
"/"                             { col += yyleng; if(lexanalysis) printf("DIV\n");        if(!lexanalysis) return DIV; }
"=="                            { col += yyleng; if(lexanalysis) printf("EQ\n");         if(!lexanalysis) return EQ; }
">="                            { col += yyleng; if(lexanalysis) printf("GE\n");         if(!lexanalysis) return GE; }
">"                             { col += yyleng; if(lexanalysis) printf("GT\n");         if(!lexanalysis) return GT; }
"{"                             { col += yyleng; if(lexanalysis) printf("LBRACE\n");     if(!lexanalysis) return LBRACE; }
"<="                            { col += yyleng; if(lexanalysis) printf("LE\n");         if(!lexanalysis) return LE; }
"("                             { col += yyleng; if(lexanalysis) printf("LPAR\n");       if(!lexanalysis) return LPAR; }
"<"                             { col += yyleng; if(lexanalysis) printf("LT\n");         if(!lexanalysis) return LT; }
"-"                             { col += yyleng; if(lexanalysis) printf("MINUS\n");      if(!lexanalysis) return MINUS; }
"%"                             { col += yyleng; if(lexanalysis) printf("MOD\n");        if(!lexanalysis) return MOD; }
"!="                            { col += yyleng; if(lexanalysis) printf("NE\n");         if(!lexanalysis) return NE; }
"!"                             { col += yyleng; if(lexanalysis) printf("NOT\n");        if(!lexanalysis) return NOT; }
"||"                            { col += yyleng; if(lexanalysis) printf("OR\n");         if(!lexanalysis) return OR; }
"+"                             { col += yyleng; if(lexanalysis) printf("PLUS\n");       if(!lexanalysis) return PLUS; }
"}"                             { col += yyleng; if(lexanalysis) printf("RBRACE\n");     if(!lexanalysis) return RBRACE; }
")"                             { col += yyleng; if(lexanalysis) printf("RPAR\n");       if(!lexanalysis) return RPAR; }
";"                             { col += yyleng; if(lexanalysis) printf("SEMI\n");       if(!lexanalysis) return SEMI; }

{newline}                       { line++; col = 1; }
{whitespaces}|{tab}             { col += yyleng; }  
{number}                        { col += yyleng; if(lexanalysis) printf("NATURAL(%s)\n", yytext); }
{decimal}                       { col += yyleng; if(lexanalysis) printf("DECIMAL(%s)\n", yytext); }

{identifier}                    { col += yyleng; if(lexanalysis) printf("IDENTIFIER(%s)\n", yytext); if(!lexanalysis) return IDENTIFIER; }
{any_char}                      { printf("Line %d, column %d: unrecognized character (%s)\n", line, col, yytext); col += yyleng; }
%%

/* -------------------------------------------- SUBROUTINES SECTION -------------------------------------------- */
void yyerror(char *error) {
    errors++;
    printf("Line %d, column %d :%s :%s\n", line, col, error, yytext);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        if (strcmp(argv[2], "debug") == 0) yydebug = 1; // debug mode
        else {
            // handle error or print usage message
            printf("Usage: %s [-l|-t]\n", argv[0]);
            return 1;
        }
    }
    
    if (strcmp(argv[1], "-l") == 0) {
        lexanalysis = 1;
        yylex(); // starts the lexical analysis
        yylex_destroy(); // clean up lexer
    } else if (strcmp(argv[1], "-t") == 0) {
        yyparse(); // starts the parsing process
        if (errors == 0) { // only if there are no errors
            show(program, 0); // print AST
        }
        deallocate(program); // free AST
    } else {
        // handle unknown argument
        printf("Unknown argument: %s\n", argv[1]);
        return 1;
    }
    
    return 0;
}

int yywrap() { /* called on EOF, return 1 to terminate */
    return 1;
}