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
    #include <string.h> // strcmp();
    int mode = 0, line = 1, col = 1, line_aux = 1, col_aux=1; 
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

'([^\n'\\]|{escape_seq})'       { if(mode) printf("CHRLIT(%s)\n", yytext); col += yyleng; }
'([^\n'\\]|\\.)*(\\)*            { printf("Line %d, column %d: unterminated char constant\n", line, col); col += yyleng;}
'([^\n'\\]|\\.)*'               { printf("Line %d, column %d: invalid char constant (%s)\n", line, col, yytext); col += yyleng; }
{reserved}                      { if(mode) printf("RESERVED(%s)\n", yytext); col += yyleng; }


char                            { if(mode) printf("CHAR\n"); col += yyleng; }
else                            { if(mode) printf("ELSE\n"); col += yyleng;}
while                           { if(mode) printf("WHILE\n"); col += yyleng;}
if                              { if(mode) printf("IF\n"); col += yyleng;}
int                             { if(mode) printf("INT\n"); col += yyleng;}
short                           { if(mode) printf("SHORT\n"); col += yyleng;}
double                          { if(mode) printf("DOUBLE\n"); col += yyleng;}
return                          { if(mode) printf("RETURN\n"); col += yyleng;}
void                            { if(mode) printf("VOID\n"); col += yyleng;}

"&"                             { if(mode) printf("BITWISEAND\n"); col += yyleng; }
"|"                             { if(mode) printf("BITWISEOR\n"); col += yyleng;}
"^"                             { if(mode) printf("BITWISEXOR\n"); col += yyleng; }
"&&"                            { if(mode) printf("AND\n"); col += yyleng; }
"="                             { if(mode) printf("ASSIGN\n"); col += yyleng; }
"*"                             { if(mode) printf("MUL\n"); col += yyleng; }
","                             { if(mode) printf("COMMA\n"); col += yyleng; }
"/"                             { if(mode) printf("DIV\n"); col += yyleng; }
"=="                            { if(mode) printf("EQ\n"); col += yyleng; }
">="                            { if(mode) printf("GE\n"); col += yyleng; }
">"                             { if(mode) printf("GT\n"); col += yyleng; }
"{"                             { if(mode) printf("LBRACE\n"); col += yyleng; }
"<="                            { if(mode) printf("LE\n"); col += yyleng; }
"("                             { if(mode) printf("LPAR\n"); col += yyleng; }
"<"                             { if(mode) printf("LT\n"); col += yyleng; }
"-"                             { if(mode) printf("MINUS\n"); col += yyleng; }
"%"                             { if(mode) printf("MOD\n"); col += yyleng; }
"!="                            { if(mode) printf("NE\n"); col += yyleng; }
"!"                             { if(mode) printf("NOT\n"); col += yyleng; }
"||"                            { if(mode) printf("OR\n"); col += yyleng; }
"+"                             { if(mode) printf("PLUS\n"); col += yyleng; }
"}"                             { if(mode) printf("RBRACE\n"); col += yyleng; }
")"                             { if(mode) printf("RPAR\n"); col += yyleng; }
";"                             { if(mode) printf("SEMI\n"); col += yyleng; }

{newline}                       { line++; col = 1; }
{whitespaces}|{tab}             { col += yyleng; }
{number}                        { if(mode) printf("NATURAL(%s)\n", yytext); col += yyleng; }
{decimal}                       { if(mode) printf("DECIMAL(%s)\n", yytext); col += yyleng; }

{identifier}                    { if(mode) printf("IDENTIFIER(%s)\n", yytext); col += yyleng; }
{any_char}                      { printf("Line %d, column %d: unrecognized character (%s)\n", line, col, yytext); col += yyleng; }
%%

/* -------------------------------------------- SUBROUTINES SECTION -------------------------------------------- */
extern int yylex();
int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "-l") == 0) {
        mode = 1;
    }
    yylex();    /* run the lexical analysis automaton */
    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}
