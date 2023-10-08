/* Johnny Fernandes 2021190668 */
/* Stefanos Lucena  2021115090 */

/* -------------------------------------------- DEFINITIONS SECTION -------------------------------------------- */
/* ATOMIC VARS */
digit           [0-9]
letter          [a-z|A-Z|_]
whitespaces     " "
newline         (\n|\r\n)
any_char        .

/* COMPOSITE VARS */
number          {digit}+
decimal         {digit}+\.{digit}*([eE][+-]?{digit}+)?|\.{digit}+([eE][+-]?{digit}+)?|{digit}+[eE][+-]?{digit}+
identifier      {letter}({letter}|{digit})*

/* STATES */    
%X COMMENT

/* CODE VARS */
%{
    #include <ctype.h> // To include toupper
    #include <string.h> // To include string compare
    int line = 1, col = 1;
    void upper(char *s) {
        for(int i = 0; s[i]; i++) {
            s[i] = toupper(s[i]);
        }
    }
%}

/* -------------------------------------------- RULES SECTION -------------------------------------------- */
%%
"/*"                            {   BEGIN(COMMENT); col += yyleng;                                                  }
<COMMENT>{newline}              {   line++; col++;                                                                  }
<COMMENT>{any_char}             {   col += yyleng;                                                                  }
<COMMENT>"*/"                   {   BEGIN(0); col += yyleng;                                                        }

{newline}                       {   line++; col++;                                                                  }
{whitespaces}                   {   col += yyleng;                                                                  }
{number}                        {   printf("NATURAL(%s)\n", yytext); col += yyleng;                                 }
{decimal}                       {   printf("DECIMAL(%s)\n", yytext); col += yyleng;                                 }

char                            {   printf("CHAR\n");                                                               }
else                            {   printf("ELSE\n");                                                               }
while                           {   printf("WHILE\n");                                                              }
if                              {   printf("IF\n");                                                                 }
int                             {   printf("INT\n");                                                                }
short                           {   printf("SHORT\n");                                                              }
double                          {   printf("DOUBLE\n");                                                             }
return                          {   printf("RETURN\n");                                                             }
void                            {   printf("VOID\n");                                                               }

&                               {   printf("BITWISEAND\n");                                                         }
\|                              {   printf("BITWISEOR\n");                                                          }
\^                              {   printf("BITWISEXOR\n");                                                         }
&&                              {   printf("AND\n");                                                                }
=                               {   printf("ASSIGN\n");                                                             }
\*                              {   printf("MUL\n");                                                                }
,                               {   printf("COMMA\n");                                                              }
\/                              {   printf("DIV\n");                                                                }
==                              {   printf("EQ\n");                                                                 }
!=                              {   printf("NE\n");                                                                 }
!                               {   printf("NOT\n");                                                                }
\|\|                            {   printf("OR\n");                                                                 }                                
>=                              {   printf("GE\n");                                                                 }
>                               {   printf("GT\n");                                                                 }
\<=                             {   printf("LE\n");                                                                 }
\<                              {   printf("LT\n");                                                                 }
\(                              {   printf("LPAR\n");                                                               }
\)                              {   printf("RPAR\n");                                                               }
\{                              {   printf("LBRACE\n");                                                             }
\}                              {   printf("RBRACE\n");                                                             }
\+                              {   printf("PLUS\n");                                                               }
-                               {   printf("MINUS\n");                                                              }
\%                              {   printf("MOD\n");                                                                }
;                               {   printf("SEMI\n");                                                               }

{identifier}                    {   printf("IDENTIFIER(%s)\n", yytext); col += yyleng;                              }
{any_char}                      {   printf("Line %d, column %d: unrecognized character (%s)\n", line, col, yytext); }
%%

/* -------------------------------------------- SUBROUTINES SECTION -------------------------------------------- */
extern int yylex();
int main(int argc, char *argv[]) {
    if (argc < 2) {
        //printf("NORMAL MODE\n");
    } else if (argc == 2 && strcmp(argv[1], "-l") == 0) {
        //printf("MINUS ONE MODE\n");
    }
    yylex();    /* run the lexical analysis automaton */
    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}
