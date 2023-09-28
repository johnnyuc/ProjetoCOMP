/* ATOMIC VARS */
digit           [0-9]
letter          [a-zA-Z]
keyword         integer|double|if|then|else
whitespaces     " " 
newline         \n
comment_begin   "/*"
comment_end     "*/"
pmark           \(|\)|=|,|\*|\/|\+|-
else            .

/* COMPOSITE VARS */
number          {digit}+
decimal         {digit}*"."{digit}+
identifier      {letter}({letter}|{digit})*

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

%X COMMENT

%%
{newline}                       {   line++; col = 1;                                                    }
{whitespaces}                   {   incCol(1);                                                          }
{comment_begin}                 {   BEGIN(COMMENT); incCol(yyleng);                                     }
<COMMENT>{comment_end}          {   BEGIN(INITIAL); incCol(yyleng);                                     }
<COMMENT>{newline}              {   line++; col = 1;                                                    }
<COMMENT>.                      {   incCol(yyleng);                                                     }
{pmark}                         {   printf("%s\n", yytext); incCol(yyleng);                             }
{number}                        {   printf("NATURAL(%s)\n", yytext); incCol(yyleng);                    }
{decimal}                       {   printf("DECIMAL(%s)\n", yytext); incCol(yyleng);                    }
{keyword}                       {   upper(yytext); printf("%s\n", yytext); incCol(yyleng);              }
{identifier}                    {   printf("IDENTIFIER(%s)\n", yytext); incCol(yyleng);                 }
{else}                          {   printf("Unrecognized character '%s' (line %d, column %d)\n", yytext, line, col); }
%%

extern int yylex();
int main() {
    yylex();    /* run the lexical analysis automaton */
    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}
