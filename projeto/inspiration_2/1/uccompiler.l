
%{

	//Pedro Miguel Alves Miranda uc2012148969
	// Pedro Manuel Egas Simões uc2016230351

int col=1,  option=0, line=1, com_c, com_l;
%}
number 								[0-9]
num8 								[0-7]
letter 								[a-zA-Z]

escape 								 	\\(\\|n|t|\'|\"|{num8}|{num8}{num8}|{num8}{num8}{num8})




expoente ((e|E)(\+|\-)?{number}+)
real_int {number}+(\.)({number}*{expoente}?)?
real_frac (\.){number}+{expoente}?
real_exp {number}+{expoente}
real_lit {real_int}|{real_frac}|{real_exp}



%X COMMENT
%X COMMENT2


%%

"/*"				{com_l=line; com_c=col; col+= yyleng;BEGIN COMMENT;}
<COMMENT>"\n"		{line++;col=1;} 	
<COMMENT>.  		{col+=yyleng;}
<COMMENT>"*/" 		{col+=yyleng; BEGIN 0;}
<COMMENT><<EOF>>	{printf("Line %d, col %d: unterminated comment\n",com_l,com_c);BEGIN 0;}



"//" 				{col += yyleng; BEGIN COMMENT2;}
<COMMENT2>.			{col += yyleng;}
<COMMENT2>"\n" 		{line++ ; BEGIN 0; col=1;}



"\r"|"\n"|"\r\n"								{line++;col=1;}
"\t"|" "                   						{col++;}

char											{if(option) printf("CHAR\n");col+=yyleng;}
else											{if(option) printf("ELSE\n");col+=yyleng;}
while											{if(option) printf("WHILE\n");col+=yyleng;}
if												{if(option) printf("IF\n");col+=yyleng;}
int												{if(option) printf("INT\n");col+=yyleng;}
short											{if(option) printf("SHORT\n");col+=yyleng;}
double											{if(option) printf("DOUBLE\n");col+=yyleng;}
return											{if(option) printf("RETURN\n");col+=yyleng;}
void											{if(option) printf("VOID\n");col+=yyleng;}
"&"												{if(option) printf("BITWISEAND\n");col+=yyleng;}
"|"												{if(option) printf("BITWISEOR\n");col+=yyleng;}
"^"												{if(option) printf("BITWISEXOR\n");col+=yyleng;}
"&&"											{if(option) printf("AND\n");col+=yyleng;}
"="												{if(option) printf("ASSIGN\n");col+=yyleng;}
"*"												{if(option) printf("MUL\n");col+=yyleng;}
","												{if(option) printf("COMMA\n");col+=yyleng;}
"/"												{if(option) printf("DIV\n");col+=yyleng;}
"=="											{if(option) printf("EQ\n");col+=yyleng;}
">="											{if(option) printf("GE\n");col+=yyleng;}
">"												{if(option) printf("GT\n");col+=yyleng;}
"{"												{if(option) printf("LBRACE\n");col+=yyleng;}
"<="											{if(option) printf("LE\n");col+=yyleng;}
"("												{if(option) printf("LPAR\n");col+=yyleng;}
"<"												{if(option) printf("LT\n");col+=yyleng;}
"-"												{if(option) printf("MINUS\n");col+=yyleng;}
"%"												{if(option) printf("MOD\n");col+=yyleng;}
"!="											{if(option) printf("NE\n");col+=yyleng;}
"!"												{if(option) printf("NOT\n");col+=yyleng;}
"||"											{if(option) printf("OR\n");col+=yyleng;}
"+"												{if(option) printf("PLUS\n");col+=yyleng;}
"}"												{if(option) printf("RBRACE\n");col+=yyleng;}
")"												{if(option) printf("RPAR\n");col+=yyleng;}
";"												{if(option) printf("SEMI\n");col+=yyleng;}


"["|"]"|"++"|"--"|"auto"|"break"|"case"|"const"|"continue"|"default"|"do"|"float"|"long"|"register"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"volatile"|"enum"|"extern"|"goto"|"_Packed"|"for"|"inline"|"restrict"|"_Bool"|"_Complex"|"_Imaginary"			{if(option) printf("RESERVED(%s)\n",yytext);col+=yyleng;}
({letter}|"_")({number}|"_"|{letter})*			{if(option) printf("ID(%s)\n",yytext);col+=yyleng;}
{number}+										{if(option) printf("INTLIT(%s)\n",yytext);col+=yyleng;}
{real_lit}										{if(option) printf("REALLIT(%s)\n", yytext);col += yyleng;}

\'([^\\\'\n\\]|{escape})\'					{if(option) printf("CHRLIT(%s)\n",yytext);col+=yyleng;}
"\'"(([^\\\'\n]|{escape})*)[^\'\n]*		{printf("Line %d, col %d: unterminated char constant\n",line,col);col+=yyleng;}
"\'"(([^\\\'\n]|{escape})*)[^\'\n]*"\'"	{printf("Line %d, col %d: invalid char constant (%s)\n",line,col,yytext);col+=yyleng;}
.												{printf("Line %d, col %d: illegal character (%s)\n",line,col,yytext);col+=yyleng;}



%%	
int main(int argc, char *argv[]){
		int i;
	if(argc>1){
		for(i = 0; i<argc; i++){
			if(strcmp(argv[i],"-l") == 1){
				option = 1;
			}
		}
	}
yylex();
return 0;
}

int yywrap()
{
return 1;
}