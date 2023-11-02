/* LEX - LEXICAL | Autores: Dario Felix 2018275530 e Jose Marcelo Marques da Cruz 2017255151 | COMP PL3 2020/21 | Meta 2 */


%X MULTICOMMENT CHARLIT


WHITESPACE					[\x20\x09]
NEWLINE						\n|\r\n|\r
Chrlit						[^\n\r'\\]|\\'|\\n|\\t|\\\\|\\\"|\\[0-7]{1,3}
Char						"char"
Else						"else"
While						"while"
If							"if"
Int							"int"
Short						"short"
Double						"double"
Return						"return"
Void						"void"
Bitwiseand					"&"
Bitwiseor					"|"
Bitwisexor					"\^"
And							"&&"
Assign						"="
Mul							"*"
Comma						","
Div							"/"
Eq							"=="
Ge							">="
Gt							">"
Lbrace						"{"
Le							"<="
Lpar						"("
Lt							"<"
Minus						"-"
Mod							"%"
Ne							"!="
Not							"!"
Or							"||"
Plus						"+"
Rbrace						"}"
Rpar						")"
Semi						";"
Reserved					"["|"]"|"++"|"--"|"auto"|"break"|"case"|"const"|"continue"|"default"|"do"|"enum"|"extern"|"float"|"for"|"goto"|"inline"|"register"|"restrict"|"long"|"sizeof"|"signed"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"volatile"|"_Bool"|"_Complex"|"_Imaginary"
Reallit						(([0-9]+"."[0-9]*|"."[0-9]+)(("E"|"e")("+"|"-")?[0-9]+)?)|([0-9]+("E"|"e")("+"|"-")?[0-9]+)
Intlit						[0-9]+
Id							[a-zA-Z_][a-zA-Z0-9_]*
SINGLE_COMMENT				"//".*
	
		
%{
		#include "structures.h"
		#include "y.tab.h"
		unsigned int column_count = 1;
		unsigned int new_line_count = 0;
		unsigned int character_count = 0;
		extern int fator;
		extern int ocorreu_erro;
%}


%%

{NEWLINE}					{ yylineno++; column_count = 1; }
{Char}						{ column_count += yyleng; if(fator == 1) { printf("CHAR\n"); } if(fator != 1 && fator != 2){ return CHAR;} }
{Else}						{ column_count += yyleng; if(fator == 1) { printf("ELSE\n"); } if(fator != 1 && fator != 2){return ELSE;} }
{While}						{ column_count += yyleng; if(fator == 1) { printf("WHILE\n"); } if(fator != 1 && fator != 2){return WHILE;} }
{If}						{ column_count += yyleng; if(fator == 1) { printf("IF\n"); } if(fator != 1 && fator != 2){return IF;} }
{Int}						{ column_count += yyleng; if(fator == 1) { printf("INT\n"); } if(fator != 1 && fator != 2){return INT;} }
{Short}						{ column_count += yyleng; if(fator == 1) { printf("SHORT\n"); } if(fator != 1 && fator != 2){return SHORT;} }
{Double}					{ column_count += yyleng; if(fator == 1) { printf("DOUBLE\n"); } if(fator != 1 && fator != 2){return DOUBLE;} }
{Return}					{ column_count += yyleng; if(fator == 1) { printf("RETURN\n"); } if(fator != 1 && fator != 2){return RETURN;} }
{Void}						{ column_count += yyleng; if(fator == 1) { printf("VOID\n"); } if(fator != 1 && fator != 2){return VOID;} }
{Bitwiseand}				{ column_count += yyleng; if(fator == 1) { printf("BITWISEAND\n"); } if(fator != 1 && fator != 2){return BITWISEAND;} }
{Bitwiseor}					{ column_count += yyleng; if(fator == 1) { printf("BITWISEOR\n"); } if(fator != 1 && fator != 2){return BITWISEOR;} }
{Bitwisexor}				{ column_count += yyleng; if(fator == 1) { printf("BITWISEXOR\n"); } if(fator != 1 && fator != 2){return BITWISEXOR;} }
{And}						{ column_count += yyleng; if(fator == 1) { printf("AND\n"); } if(fator != 1 && fator != 2){return AND;} }
{Assign}					{ column_count += yyleng; if(fator == 1) { printf("ASSIGN\n"); } if(fator != 1 && fator != 2){return ASSIGN;} }
{Mul}						{ column_count += yyleng; if(fator == 1) { printf("MUL\n"); } if(fator != 1 && fator != 2){return MUL;} }
{Comma}						{ column_count += yyleng; if(fator == 1) { printf("COMMA\n"); } if(fator != 1 && fator != 2){return COMMA;} }
{Div}						{ column_count += yyleng; if(fator == 1) { printf("DIV\n"); } if(fator != 1 && fator != 2){return DIV;} }
{Eq}						{ column_count += yyleng; if(fator == 1) { printf("EQ\n"); } if(fator != 1 && fator != 2){return EQ;} }
{Ge}						{ column_count += yyleng; if(fator == 1) { printf("GE\n"); } if(fator != 1 && fator != 2){return GE;} }
{Gt}						{ column_count += yyleng; if(fator == 1) { printf("GT\n"); } if(fator != 1 && fator != 2){return GT;} }
{Lbrace}					{ column_count += yyleng; if(fator == 1) { printf("LBRACE\n"); } if(fator != 1 && fator != 2){return LBRACE;} }
{Le}						{ column_count += yyleng; if(fator == 1) { printf("LE\n"); } if(fator != 1 && fator != 2){return LE;} }
{Lpar}						{ column_count += yyleng; if(fator == 1) { printf("LPAR\n"); } if(fator != 1 && fator != 2){return LPAR;} }
{Lt}						{ column_count += yyleng; if(fator == 1) { printf("LT\n"); } if(fator != 1 && fator != 2){return LT;} }
{Minus}						{ column_count += yyleng; if(fator == 1) { printf("MINUS\n"); } if(fator != 1 && fator != 2){return MINUS;} }
{Mod}						{ column_count += yyleng; if(fator == 1) { printf("MOD\n"); } if(fator != 1 && fator != 2){return MOD;} }
{Ne}						{ column_count += yyleng; if(fator == 1) { printf("NE\n"); } if(fator != 1 && fator != 2){return NE;} }
{Not}						{ column_count += yyleng; if(fator == 1) { printf("NOT\n"); } if(fator != 1 && fator != 2){return NOT;} }
{Or}						{ column_count += yyleng; if(fator == 1) { printf("OR\n"); } if(fator != 1 && fator != 2){return OR;} }
{Plus}						{ column_count += yyleng; if(fator == 1) { printf("PLUS\n"); } if(fator != 1 && fator != 2){return PLUS;} }
{Rbrace}					{ column_count += yyleng; if(fator == 1) { printf("RBRACE\n"); } if(fator != 1 && fator != 2){return RBRACE;} }
{Rpar}						{ column_count += yyleng; if(fator == 1) { printf("RPAR\n"); } if(fator != 1 && fator != 2){return RPAR;} }
{Semi}						{ column_count += yyleng; if(fator == 1) { printf("SEMI\n"); } if(fator != 1 && fator != 2){return SEMI;} }
{Reserved}					{ column_count += yyleng; if(fator == 1) { printf("RESERVED(%s)\n", yytext); } if(fator != 1 && fator != 2){return RESERVED;} }
{Reallit}					{ column_count += yyleng; if(fator == 1) { printf("REALLIT(%s)\n", yytext); } yylval.stringValue = strdup(yytext); if(fator != 1 && fator != 2){return REALLIT;} }
{Intlit}					{ column_count += yyleng; if(fator == 1) { printf("INTLIT(%s)\n", yytext); } yylval.stringValue = strdup(yytext); if(fator != 1 && fator != 2){return INTLIT;} }
'							{ character_count = yyleng; BEGIN CHARLIT; }

<CHARLIT>{Chrlit}'									{ if(fator == 1) { printf("CHRLIT('%s)\n", yytext);} column_count += (character_count + yyleng); BEGIN 0; yylval.stringValue = strdup(yytext); if(fator != 1 && fator != 2){return CHRLIT;} }
<CHARLIT>((\\\\)*(\\')*([^\n\r'\\](\\\\)*(\\')*|[^\n\r'])*){NEWLINE}		{ { printf("Line %d, col %d: unterminated char constant\n", yylineno, column_count); } column_count = 1; yylineno++; BEGIN 0;}
<CHARLIT>((\\\\)*(\\')*([^\n\r'\\](\\\\)*(\\')*|[^\n\r'])*)'						{  { printf("Line %d, col %d: invalid char constant ('%s)\n", yylineno, column_count, yytext); } column_count += (character_count + yyleng); BEGIN 0;}
<CHARLIT>((\\\\)*(\\')*([^\n\r'\\](\\\\)*(\\')*|[^\n\r'])*)		{ { printf("Line %d, col %d: unterminated char constant\n", yylineno, column_count); } column_count = 1; yylineno++; BEGIN 0;}

{Id}                        { column_count += yyleng; if(fator == 1) { printf("ID(%s)\n", yytext); } yylval.stringValue = strdup(yytext); if(fator != 1 && fator != 2){return ID;} }
{WHITESPACE}				{ column_count += yyleng; }
{SINGLE_COMMENT}			{ column_count += yyleng; }
"/*"						{ new_line_count = 0; character_count = yyleng; BEGIN MULTICOMMENT; }

<MULTICOMMENT>{NEWLINE}		{ column_count = 1; character_count = 0; new_line_count++; }
<MULTICOMMENT>.				{ character_count += yyleng; }
<MULTICOMMENT><<EOF>>		{ { printf("Line %d, col %d: unterminated comment\n", yylineno, column_count); } return 0; }
<MULTICOMMENT>"*/"			{ column_count += (character_count + yyleng); yylineno += new_line_count; BEGIN 0; }

.							{ { printf("Line %d, col %d: illegal character (%s)\n", yylineno, column_count, yytext); } column_count += yyleng;}


%%


void yyerror(char *s) {		// Analise Sintatica
	if(fator > 2) { printf("Line %d, col %d: %s: %s\n", yylineno, column_count - (int) strlen(yytext), s, yytext); }
	ocorreu_erro = 1;
}


/*
int main(int argc, char *argv[ ]) {
	if (argc == 2) {
		if (strcmp (argv[1], "-l") == 0) {
			fator = 1;  // -l -> deve emitir os tokens e as mensagens de erro para o stdout e terminar
		}
		// ELSE: (fator = 0) -> Na ausencia de qualquer opcao, ou se invocado com a opcao -e1, deve escrever no stdout apenas as mensagens de erro
	}
	
	yylex(); 
	return 0;
}

int yywrap() {
	return 1;
}

*/


