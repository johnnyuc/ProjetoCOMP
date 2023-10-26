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
'([^\n'\\]|\\.)*(\\)*           { printf("Line %d, column %d: unterminated char constant\n", line, col); col += yyleng;}
'([^\n'\\]|\\.)*'               { printf("Line %d, column %d: invalid char constant (%s)\n", line, col, yytext); col += yyleng; }
{reserved}                      { if(mode) printf("RESERVED(%s)\n", yytext); col += yyleng; }

"char"                          { if(mode) printf("CHAR\n"); col += yyleng; return CHAR;}
"else"                          { if(mode) printf("ELSE\n"); col += yyleng; return ELSE;}
"while"                         { if(mode) printf("WHILE\n"); col += yyleng; return WHILE;}
"if"                            { if(mode) printf("IF\n"); col += yyleng; return IF;}
"int"                           { if(mode) printf("INT\n"); col += yyleng; return INT;}
"short"                         { if(mode) printf("SHORT\n"); col += yyleng; return SHORT;}
"double"                        { if(mode) printf("DOUBLE\n"); col += yyleng; return DOUBLE;}
"return"                        { if(mode) printf("RETURN\n"); col += yyleng; return RETURN;}
"void"                          { if(mode) printf("VOID\n"); col += yyleng; return VOID;}

"&"                             { if(mode) printf("BITWISEAND\n"); col += yyleng; return BITWISEAND;}
"|"                             { if(mode) printf("BITWISEOR\n"); col += yyleng; return BITWISEOR;}
"^"                             { if(mode) printf("BITWISEXOR\n"); col += yyleng; return BITWISEXOR; }
"&&"                            { if(mode) printf("AND\n"); col += yyleng; return AND; }
"="                             { if(mode) printf("ASSIGN\n"); col += yyleng; return ASSIGN; }
"*"                             { if(mode) printf("MUL\n"); col += yyleng; return MUL; }
","                             { if(mode) printf("COMMA\n"); col += yyleng; return COMMA; }
"/"                             { if(mode) printf("DIV\n"); col += yyleng; return DIV; }
"=="                            { if(mode) printf("EQ\n"); col += yyleng; return EQ; }
">="                            { if(mode) printf("GE\n"); col += yyleng; return GE; }
">"                             { if(mode) printf("GT\n"); col += yyleng; return GT; }
"{"                             { if(mode) printf("LBRACE\n"); col += yyleng; return LBRACE; }
"<="                            { if(mode) printf("LE\n"); col += yyleng; return LE; }
"("                             { if(mode) printf("LPAR\n"); col += yyleng; return LPAR; }
"<"                             { if(mode) printf("LT\n"); col += yyleng; return LT; }
"-"                             { if(mode) printf("MINUS\n"); col += yyleng; return MINUS; }
"%"                             { if(mode) printf("MOD\n"); col += yyleng; return MOD; }
"!="                            { if(mode) printf("NE\n"); col += yyleng; return NE; }
"!"                             { if(mode) printf("NOT\n"); col += yyleng; return NOT; }
"||"                            { if(mode) printf("OR\n"); col += yyleng; return OR; }
"+"                             { if(mode) printf("PLUS\n"); col += yyleng; return PLUS; }
"}"                             { if(mode) printf("RBRACE\n"); col += yyleng; return RBRACE; }
")"                             { if(mode) printf("RPAR\n"); col += yyleng; return RPAR; }
";"                             { if(mode) printf("SEMI\n"); col += yyleng; return SEMI; }


{newline}                       { line++; col = 1; }
{whitespaces}|{tab}             { col += yyleng; }
{number}                        { if(mode) printf("NATURAL(%s)\n", yytext); col += yyleng; }
{decimal}                       { if(mode) printf("DECIMAL(%s)\n", yytext); col += yyleng; }

{identifier}                    { if(mode) printf("IDENTIFIER(%s)\n", yytext); col += yyleng; return IDENTIFIER; }
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