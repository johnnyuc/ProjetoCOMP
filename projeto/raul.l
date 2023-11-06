/*
Raul Cardoso Serra Rodrigues 2021234487
Tiago Milheiro Da Conceição 2021233341
*/

%{
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "y.tab.h"
#define YY_USER_ACTION { syn_line = linha; syn_column = coluna;}
bool flag;
bool flagy=true;
bool flagarvore;
int yyparse(void);
int linha = 1;
int coluna = 1;
int syn_line= -1;
int syn_column=-1;
int com_linha = 0;
int com_coluna = 0;

void up(char *x, int y){
	int i = 0;
	for(i=0;i<y;i++){
		x[i] = toupper(x[i]);
	}
	printf("%s\n",x);
}
%}

digit  		[0-9]
letter		[a-zA-Z]
reserved 	(auto|break|case|const|continue|default|do|long|register|signed|sizeof|static|struct|switch|typedef|union|unsigned|volatile|enum|extern|float|for|goto|restrict|_Bool|_Complex|_Imaginary|inline|[[]|[]]|"++"|"--")
escape		(\n|\t|\r|\f|\\|\)
exp 		[eE][+-]?{digit}+
decimal 	(\.{0,1}{digit}+({exp})?)|({digit}+\.{0,1}{digit}*({exp})?)
chrlit		([^\'\n\\]|\\([nt\\'\"]{1}|[0-7]{1,3}))
newline		 "\n"|"\r\n"|"\r"
escape_seq 	\\([nt\\'\"]{1}|[0-7]{1,3})

%X COMMENT1 COMMENT2

%%

{newline}							{ linha++; coluna = 1;}
[ ]									{ coluna += yyleng;}
\t									{ coluna += 1;}


"char"								{ if(flag) up(yytext, yyleng); coluna = coluna + yyleng; if(flagy) return CHAR;}
"int"								{ if(flag) up(yytext, yyleng); coluna = coluna + yyleng; if(flagy) return INT;}
"void"								{ if(flag) up(yytext, yyleng); coluna = coluna + yyleng; if(flagy) return VOID;}
"short"								{ if(flag) up(yytext, yyleng); coluna = coluna + yyleng; if(flagy) return SHORT;}
"double"							{ if(flag) up(yytext, yyleng); coluna = coluna + yyleng; if(flagy) return DOUBLE;}
"if"								{ if(flag) up(yytext, yyleng); coluna = coluna + yyleng; if(flagy) return IF;}
"else"								{ if(flag) up(yytext, yyleng); coluna = coluna + yyleng; if(flagy) return ELSE;}
"while"								{ if(flag) up(yytext, yyleng); coluna = coluna + yyleng; if(flagy) return WHILE;}
"return"							{ if(flag) up(yytext, yyleng); coluna = coluna + yyleng; if(flagy) return RETURN;}

{reserved}							{ if(flag) printf("RESERVED(%s)\n",yytext); coluna += yyleng; if(flagy) return RESERVED;}

'{chrlit}'							{ if(flag) printf("CHRLIT(%s)\n",yytext); coluna = coluna + yyleng; if(flagy) return CHRLIT; }
'([^'\n\\]|\\[^'\n]|\\[^\n\\])*'	{ printf("Line %d, column %d: invalid char constant (%s)\n",linha,coluna,yytext); coluna = coluna + yyleng; }
'([^'\n]*|\\[^'\n]*)'				{ printf("Line %d, column %d: unterminated char constant\n",linha,coluna); coluna = coluna + yyleng;}
'([^'\n]*|\\[^'\n]*)				{ printf("Line %d, column %d: unterminated char constant\n",linha,coluna); coluna = coluna + yyleng;}
'([^\n'\\]|{escape_seq})*(\\)* 		{ printf("Line %d, column %d: unterminated char constant\n",linha,coluna); coluna = coluna + yyleng;}	

({letter}|_)+({digit}|{letter}|_)*	{ if(flag) printf("IDENTIFIER(%s)\n",yytext); coluna = coluna + yyleng; if(flagy) return IDENTIFIER;}	
{digit}+                			{ if(flag) printf("NATURAL(%s)\n",yytext); coluna = coluna + yyleng; yylval = atoi(yytext); if(flagy) return NATURAL;}
{decimal} 							{ if(flag) printf("DECIMAL(%s)\n",yytext); coluna = coluna + yyleng; if(flagy) return DECIMAL;}

[&]    								{ if(flag) printf("BITWISEAND\n"); coluna += yyleng; if(flagy) return BITWISEAND; }
[|]    								{ if(flag) printf("BITWISEOR\n"); coluna += yyleng; if(flagy) return BITWISEOR; }
[\^]   								{ if(flag) printf("BITWISEXOR\n"); coluna += yyleng; if(flagy) return BITWISEXOR;}
"&&"    							{ if(flag) printf("AND\n"); coluna += yyleng; if(flagy) return AND;}
[=]									{ if(flag) printf("ASSIGN\n"); coluna += yyleng; if(flagy) return ASSIGN; }
[*]									{ if(flag) printf("MUL\n"); coluna += yyleng; if(flagy) return MUL;}
[,]									{ if(flag) printf("COMMA\n"); coluna += yyleng; if(flagy) return COMMA; }
[/]									{ if(flag) printf("DIV\n"); coluna += yyleng; if(flagy) return DIV; }
"=="								{ if(flag) printf("EQ\n"); coluna += yyleng; if(flagy) return EQ; }
">="								{ if(flag) printf("GE\n"); coluna += yyleng; if(flagy) return GE; }
[>]									{ if(flag) printf("GT\n"); coluna += yyleng; if(flagy) return GT; }
[{]									{ if(flag) printf("LBRACE\n"); coluna += yyleng; if(flagy) return LBRACE; }
"<="								{ if(flag) printf("LE\n"); coluna += yyleng; if(flagy) return LE; }
[(]									{ if(flag) printf("LPAR\n"); coluna += yyleng; if(flagy) return LPAR; }
[<]									{ if(flag) printf("LT\n"); coluna += yyleng; if(flagy) return LT; }
[-]									{ if(flag) printf("MINUS\n"); coluna += yyleng; if(flagy) return MINUS; }
[%]									{ if(flag) printf("MOD\n"); coluna += yyleng; if(flagy) return MOD; }
"!="								{ if(flag) printf("NE\n"); coluna += yyleng; if(flagy) return NE; }
[!]									{ if(flag) printf("NOT\n"); coluna += yyleng; if(flagy) return NOT; }
"||"								{ if(flag) printf("OR\n"); coluna += yyleng; if(flagy) return OR; }
[+]									{ if(flag) printf("PLUS\n"); coluna += yyleng; if(flagy) return PLUS; }
[}]									{ if(flag) printf("RBRACE\n"); coluna += yyleng; if(flagy) return RBRACE; }
[)]									{ if(flag) printf("RPAR\n"); coluna += yyleng; if(flagy) return RPAR; }
[;]									{ if(flag) printf("SEMI\n"); coluna += yyleng; if(flagy) return SEMI; }


"/*" 								{ BEGIN(COMMENT1); com_linha = linha; com_coluna = coluna; coluna += yyleng; }
<COMMENT1><<EOF>>					{ if(flagy)printf("Line %d, column %d: unterminated comment\n",com_linha,com_coluna); return 1; }
<COMMENT1>. 						{ ; coluna += yyleng; }
<COMMENT1>\n 						{ ; linha++; coluna = 1; }
<COMMENT1>"*/" 						{ BEGIN(INITIAL); coluna += yyleng; }

"//" 								{ BEGIN(COMMENT2); coluna += yyleng; }
<COMMENT2>. 						{ ; coluna += yyleng; }
<COMMENT2>\n 						{ BEGIN(INITIAL); linha++; coluna = 1; }

. 									{ printf("Line %d, column %d: unrecognized character (%s)\n",linha,coluna,yytext); coluna = coluna + yyleng; }

%%

void yyerror(char *error) {
	printf("Line %d, column %d: syntax error: %s\n", syn_line, syn_column, yytext);
}


extern int yylex();
int main(int argc, char** argv) {
	int i;
  	for (i = 0; i < argc; i++) {
    	if (strcmp(argv[i], "-l") == 0) {
      		flag = true;
			flagy=false;
      		break;
    	}
    }

	for (i = 0; i < argc; i++) {
    	if (strcmp(argv[i], "-t") == 0) {
      		flagarvore = false;
      		break;
    	}
	}
	if(flag){
   		yylex();    /* run the lexical analysis automaton */
	}
	if(flagarvore==true){ /*Construir a árvore*/
		yyparse();
	}
	
	yyparse();
	yylex();

    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}


