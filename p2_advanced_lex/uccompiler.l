/* Johnny Fernandes 2021190668 */
/* Stefanos Lucena  2021115090 */

/* ATOMIC VARS */
digit           [0-9]
letter          [a-zA-Z]
keyword         integer|double|if|then|else
whitespaces     " " 
newline         (\n|\r\n)
pmark           \(|\)|=|,|\*|\/|\+|-
any_char        .
comment_start   "/\\*"
comment_end     "\\*/"

/* COMPOSITE VARS */
number          {digit}+
decimal         {digit}*"."{digit}+
identifier      {letter}({letter}|{digit})*

/* STATES */
%X COMMENT

/* CODE VARS */
%{
    #include <ctype.h>
    int line = 1, col = 1;
    void upper(char *s) {
        for(int i = 0; s[i]; i++) {
            s[i] = toupper(s[i]);
        }
    }
    void incCol(int num_chars) {
        col += num_chars;
    }
%}

%%
{comment_start}                 {   BEGIN(COMMENT);incCol(yyleng);                                                 }
<COMMENT>{newline}              {   line++; col = 1;                                                               }
<COMMENT>{any_char}             {   incCol(yyleng);                                                                }
<COMMENT>{comment_end}          {   BEGIN(0);incCol(yyleng);                                                       }
{newline}                       {   line++; col = 1;                                                               }
{whitespaces}                   {   incCol(yyleng);                                                                }
{pmark}                         {   printf("%s\n", yytext); incCol(yyleng);                                        }
{number}                        {   printf("NATURAL(%s)\n", yytext); incCol(yyleng);                               }
{decimal}                       {   printf("DECIMAL(%s)\n", yytext); incCol(yyleng);                               }
{keyword}                       {   upper(yytext); printf("%s\n", yytext); incCol(yyleng);                         }
{identifier}                    {   printf("IDENTIFIER(%s)\n", yytext); incCol(yyleng);                            }
{any_char}                      {   printf("Line %d, column %d: unrecognized character (%s)\n", line, col, yytext);}
%%

extern int yylex();
int main() {
    yylex();    /* run the lexical analysis automaton */
    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}