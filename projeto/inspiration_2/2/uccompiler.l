

%{

	//Pedro Miguel Alves Miranda uc2012148969

	//Pedro Manuel Egas Simoes uc2016230351
#include <stdlib.h>
#include <stdio.h>
#include "y.tab.h"
#include "ast.h"

extern struct ast_node* ast;
int col=1,  option=0, line=1, com_c, com_l, error;
extern int print;

%}

number 								[0-9]

num8 								[0-7]

letter 								[a-zA-Z]


escape 								 	\\(\\|n|t|\'|\"|{num8}|{num8}{num8}|{num8}{num8}{num8})







reserved "["|"]"|"++"|"--"|"auto"|"break"|"case"|"const"|"continue"|"default"|"do"|"float"|"long"|"register"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"volatile"|"enum"|"extern"|"goto"|"_Packed"|"for"|"inline"|"restrict"|"_Bool"|"_Complex"|"_Imaginary"			

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



char											{if(option==1) printf("CHAR\n");col+=yyleng;if(option==2) return CHAR;}

else											{if(option==1) printf("ELSE\n");col+=yyleng;if(option==2) return ELSE;}

while											{if(option==1) printf("WHILE\n");col+=yyleng;if(option==2) return WHILE;}

if												{if(option==1) printf("IF\n");col+=yyleng;if(option==2) return IF;}

int												{if(option==1) printf("INT\n");col+=yyleng;if(option==2) return INT;}

short											{if(option==1) printf("SHORT\n");col+=yyleng;if(option==2) return SHORT;}

double											{if(option==1) printf("DOUBLE\n");col+=yyleng;if(option==2) return DOUBLE;}

return											{if(option==1) printf("RETURN\n");col+=yyleng;if(option==2) return RETURN;}

void											{if(option==1) printf("VOID\n");col+=yyleng;if(option==2) return VOID;}

"&"												{if(option==1) printf("BITWISEAND\n");col+=yyleng;if(option==2) return BITWISEAND;}

"|"												{if(option==1) printf("BITWISEOR\n");col+=yyleng;if(option==2) return BITWISEOR;}

"^"												{if(option==1) printf("BITWISEXOR\n");col+=yyleng;if(option==2) return BITWISEXOR;}

"&&"											{if(option==1) printf("AND\n");col+=yyleng;if(option==2) return AND;}

"="												{if(option==1) printf("ASSIGN\n");col+=yyleng;if(option==2) return ASSIGN;}

"*"												{if(option==1) printf("MUL\n");col+=yyleng;if(option==2) return MUL;}

","												{if(option==1) printf("COMMA\n");col+=yyleng;if(option==2) return COMMA;}

"/"												{if(option==1) printf("DIV\n");col+=yyleng;if(option==2) return DIV;}

"=="											{if(option==1) printf("EQ\n");col+=yyleng;if(option==2) return EQ;}

">="											{if(option==1) printf("GE\n");col+=yyleng;if(option==2) return GE;}

">"												{if(option==1) printf("GT\n");col+=yyleng;if(option==2) return GT;}

"{"												{if(option==1) printf("LBRACE\n");col+=yyleng;if(option==2) return LBRACE;}

"<="											{if(option==1) printf("LE\n");col+=yyleng;if(option==2) return LE;}

"("												{if(option==1) printf("LPAR\n");col+=yyleng;if(option==2) return LPAR;}

"<"												{if(option==1) printf("LT\n");col+=yyleng;if(option==2) return LT;}

"-"												{if(option==1) printf("MINUS\n");col+=yyleng;if(option==2) return MINUS;}

"%"												{if(option==1) printf("MOD\n");col+=yyleng;if(option==2) return MOD;}

"!="											{if(option==1) printf("NE\n");col+=yyleng;if(option==2) return NE;}

"!"												{if(option==1) printf("NOT\n");col+=yyleng;if(option==2) return NOT;}

"||"											{if(option==1) printf("OR\n");col+=yyleng;if(option==2) return OR;}

"+"												{if(option==1) printf("PLUS\n");col+=yyleng;if(option==2) return PLUS;}

"}"												{if(option==1) printf("RBRACE\n");col+=yyleng;if(option==2) return RBRACE;}

")"												{if(option==1) printf("RPAR\n");col+=yyleng;if(option==2) return RPAR;}

";"												{if(option==1) printf("SEMI\n");col+=yyleng;if(option==2) return SEMI;}


 

{reserved}           {if(option==1) printf("RESERVED(%s)\n",yytext);col+=yyleng;if(option==2){yylval.str = (char*)strdup(yytext);return RESERVED;}}

({letter}|"_")({number}|"_"|{letter})*			{if(option==1) printf("ID(%s)\n",yytext);col+=yyleng;if(option==2){yylval.str = (char*)strdup(yytext);return ID;}}

{number}+										{if(option==1) printf("INTLIT(%s)\n",yytext);col+=yyleng;if(option==2){yylval.str = (char*)strdup(yytext);return INTLIT;}}

{real_lit}										{if(option==1) printf("REALLIT(%s)\n", yytext);col += yyleng;if(option==2){yylval.str = (char*)strdup(yytext);return REALLIT;}}



\'([^\\\'\n\\]|{escape})\'					{if(option==1) printf("CHRLIT(%s)\n",yytext);col+=yyleng;if(option==2){yylval.str = (char*)strdup(yytext);return CHRLIT;}}

"\'"(([^\\\'\n]|{escape})*)[^\'\n]*		{printf("Line %d, col %d: unterminated char constant\n",line,col);col+=yyleng;}

"\'"(([^\\\'\n]|{escape})*)[^\'\n]*"\'"	{printf("Line %d, col %d: invalid char constant (%s)\n",line,col,yytext);col+=yyleng;}

.												{printf("Line %d, col %d: illegal character (%s)\n",line,col,yytext);col+=yyleng;}






%%	



int main(int argc, char const *argv[]){
	int i;
	if(argc>1){
		for(i = 1; i<argc; i++){
			if(strcmp(argv[i],"-l")==0){
        option = 1;
        yylex();
        return 0;
      }

      else if(strcmp(argv[i],"-e1")==0){
        option = 2;
        yylex();
        return 0;
      }
      else if(strcmp(argv[i],"-e2")==0){
        option = 2;
        yyparse();
        return 0;
      }
      else if (strcmp(argv[i],"-t")==0){
      	option = 2;
        yyparse();
        if(print==0)
          print_ast(ast,0);
        return 0;
      }


		}
  }
  	option = 2;
	yyparse();
	return 0;
}

int yywrap(){
	return 1;
}



void yyerror(char *s) {
      if(strcmp(yytext, "") != 0){
        error = col-yyleng;
        //printf("error: %d\n",error);
      }
      else{

        error = col;
        //printf("errorcol: %d\n",error);
        }
      printf("Line %d, col %d: %s: %s\n", line, error, s, yytext);
}
