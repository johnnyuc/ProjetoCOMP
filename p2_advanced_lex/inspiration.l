%{
int line = 1;
int column = 1;

int line_start = 1;
int column_start = 1;

int check = 0;
%}

digit   [0-9]
letter  [a-zA-Z_]
expo    [eE][+-]?[0-9]+
reserved "["|"]"|"++"|"--"|auto|break|case|const|continue|default|do|enum|extern|float|for|goto|inline|long|register|restrict|signed|sizeof|static|struct|switch|typedef|union|unsigned|volatile|_Bool|_Complex|_Imaginary
escape_sequences \\n|\\t|\\\\|\\'|\\\"|\\[0-7]{1,3}

%X COMMENT
%X COMMENTLINE
%%
"/*"                                         { BEGIN(COMMENT); line_start = line; column_start = column; column += yyleng;}
<COMMENT>"\t"                                { column++ ; }
<COMMENT>"\r\n"                              { line++; column = 1; }
<COMMENT>"\n"                                { line++; column = 1; }
<COMMENT>"\r"                                { line++; column = 1; }
<COMMENT>.                                   { column += yyleng; }
<COMMENT><<EOF>>                             { BEGIN(INITIAL); printf("Line %d, column %d: unterminated comment\n", line_start, column_start); }
<COMMENT>"*/"                                { BEGIN(INITIAL); column += yyleng; }

"//"                                         { BEGIN(COMMENTLINE); column += yyleng; }
<COMMENTLINE>"\t"                            { column++; }
<COMMENTLINE>.                               { column += yyleng; }
<COMMENTLINE>("\r\n"|"\n"|"\r")              { BEGIN(INITIAL); line++; column = 1; }

'([^'\n\\]|{escape_sequences})'              { if(check) printf("CHRLIT(%s)\n", yytext); column += yyleng; }
'([^'\n\\]|\\.)*'                            { printf("Line %d, column %d: invalid char constant (%s)\n", line, column, yytext); column += yyleng; }
'[^']                                      { printf("Line %d, column %d: unterminated char constant\n", line, column); column = 1; line++; }

{reserved}                                   { if(check) printf("RESERVED(%s)\n", yytext); column += yyleng; }

else                                         { if(check) printf("ELSE\n"); column += yyleng; }
char                                         { if(check) printf("CHAR\n"); column += yyleng; }
while                                        { if(check) printf("WHILE\n"); column += yyleng; }
if                                           { if(check) printf("IF\n"); column += yyleng; }  
int                                          { if(check) printf("INT\n"); column += yyleng; }
short                                        { if(check) printf("SHORT\n"); column += yyleng; }
double                                       { if(check) printf("DOUBLE\n"); column += yyleng; }
return                                       { if(check) printf("RETURN\n"); column += yyleng; }
void                                         { if(check) printf("VOID\n"); column += yyleng; }
"&"                                          { if(check) printf("BITWISEAND\n"); column += yyleng; }
"|"                                          { if(check) printf("BITWISEOR\n"); column += yyleng; }
"^"                                          { if(check) printf("BITWISEXOR\n"); column += yyleng; }
"&&"                                         { if(check) printf("AND\n"); column += yyleng; }
"="                                          { if(check) printf("ASSIGN\n"); column += yyleng; }                              
"*"                                          { if(check) printf("MUL\n"); column += yyleng; }
","                                          { if(check) printf("COMMA\n"); column += yyleng; }
"/"                                          { if(check) printf("DIV\n"); column += yyleng; }
"=="                                         { if(check) printf("EQ\n"); column += yyleng; }
">="                                         { if(check) printf("GE\n"); column += yyleng; }
">"                                          { if(check) printf("GT\n"); column += yyleng; }
"{"                                          { if(check) printf("LBRACE\n"); column += yyleng; }
"<="                                         { if(check) printf("LE\n"); column += yyleng; }
"("                                          { if(check) printf("LPAR\n"); column += yyleng; }
"<"                                          { if(check) printf("LT\n"); column += yyleng; }
"-"                                          { if(check) printf("MINUS\n"); column += yyleng; }
"%"                                          { if(check) printf("MOD\n"); column += yyleng; }
"!="                                         { if(check) printf("NE\n"); column += yyleng; }
"!"                                          { if(check) printf("NOT\n"); column += yyleng; }
"||"                                         { if(check) printf("OR\n"); column += yyleng; }
"+"                                          { if(check) printf("PLUS\n"); column += yyleng; }
"}"                                          { if(check) printf("RBRACE\n"); column += yyleng; }
")"                                          { if(check) printf("RPAR\n"); column += yyleng; }
";"                                          { if(check) printf("SEMI\n"); column += yyleng; }

{digit}+                                     { if(check) printf("NATURAL(%s)\n",yytext); column+=yyleng; }
     
{digit}+"."{digit}+?{expo}?                  { if(check) printf("DECIMAL(%s)\n",yytext); column+=yyleng; }
"."{digit}+{expo}?                           { if(check) printf("DECIMAL(%s)\n",yytext); column+=yyleng; }
{digit}+{expo}                               { if(check) printf("DECIMAL(%s)\n",yytext); column+=yyleng; }

{letter}({letter}|{digit})*                  { if(check) printf("IDENTIFIER(%s)\n",yytext); column+=yyleng; }
" "+                                         {column += yyleng; }
"\t"                                         {column++ ; }
("\r\n"|"\n"|"\r")                           {line++; column = 1; }

.                                            { printf("Line %d, column %d: unrecognized character (%s)\n", line,column,yytext); column += yyleng; }

%%
extern int yylex();
int main(int argc, char* argv[]) {
    if(argc == 2 && strcmp(argv[1],"-l")==0){
        check = 1;
    }
    yylex();    /* run the lexical analysis automaton */
    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}
