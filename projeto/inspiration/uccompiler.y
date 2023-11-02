/* variaveis, declaracoes, bibliotecas, prototipos */
%{
	#include <stdio.h>
	#include <string.h>
	#include "functions.h"
	extern int yylex();
	
	ast_node* myprogram;

	void yyerror (char *s);
	int fator = 0;
	int ocorreu_erro = 0;
	int parametros_extra = 0;
%}


/* %token <tipo de token> TOKEN | %type <tipo> simboloNaoTerminal */
%token <stringValue> CHRLIT
%token <stringValue> INTLIT
%token <stringValue> REALLIT
%token <stringValue> ID
%token VOID CHAR INT SHORT DOUBLE
%token SEMI COMMA LBRACE RBRACE LPAR RPAR ASSIGN
%token PLUS MINUS MUL DIV MOD
%token OR AND BITWISEAND BITWISEOR BITWISEXOR EQ NE LE GE LT GT NOT
%token WHILE IF ELSE RETURN RESERVED

%type <ast> Start FunctionsAndDeclarations FunctionDeclaration TypeSpec FunctionDeclarator ParameterList ParameterDeclaration
%type <ast> Declaration DeclarationPrime Declarator Expr
%type <ast> FunctionDefinition FunctionBody DeclarationsAndStatements Statement StatementRecursivo CallParameters


/* yylval.xxx (tipo de token) */
%union{
int intValue;
double doubleValue;
char* stringValue;

ast_node* ast;
}


/* precedências */
%left COMMA
%right ASSIGN
%left OR
%left AND
%left BITWISEOR
%left BITWISEXOR
%left BITWISEAND
%left EQ NE
%left LT LE GT GE
%left PLUS MINUS
%left MUL DIV MOD
%right NOT /* neste nível de prioridade incluí-se também os operadores MINUS e PLUS se forem unários */
%left LPAR RPAR
%nonassoc ELSE

%%
Start							: FunctionsAndDeclarations							{$$ = myprogram = new_ast_program($1);}
								;
FunctionsAndDeclarations		: FunctionDefinition FunctionsAndDeclarations		{$$ = new_ast_functions_and_declarations($1, $2);}
								| FunctionDefinition								{$$ = new_ast_functions_and_declarations($1, NULL);}
								| FunctionDeclaration FunctionsAndDeclarations		{$$ = new_ast_functions_and_declarations($1, $2);}	
								| FunctionDeclaration								{$$ = new_ast_functions_and_declarations($1, NULL);}
								| Declaration FunctionsAndDeclarations				{$$ = new_ast_functions_and_declarations($1, $2);}
								| Declaration										{$$ = new_ast_functions_and_declarations($1, NULL);}
								| error SEMI FunctionsAndDeclarations				{$$ = NULL; }
								| error SEMI										{$$ = NULL;}
								;
FunctionDefinition				: TypeSpec FunctionDeclarator FunctionBody			{$$ = new_ast_function_definition($1, $2, $3);}
								;
FunctionBody					: LBRACE DeclarationsAndStatements RBRACE			{$$ = new_ast_function_body($2);}
								;
DeclarationsAndStatements		: Declaration DeclarationsAndStatements				{$$ = new_ast_declarations_and_statements($1, $2);}
								| Statement DeclarationsAndStatements				{$$ = new_ast_declarations_and_statements($1, $2);}
								|													{$$ = NULL;}
								;
FunctionDeclaration				: TypeSpec FunctionDeclarator SEMI					{$$ = new_ast_function_declaration($1, $2);}
								;
FunctionDeclarator				: ID LPAR ParameterList RPAR						{$$ = new_ast_function_declarator(new_ast_id($1), $3);}
								;
ParameterList					: ParameterDeclaration								{$$ = new_ast_parameter_list($1, NULL);}
								| ParameterDeclaration COMMA ParameterList			{$$ = new_ast_parameter_list($1, $3);}
								;
ParameterDeclaration			: TypeSpec											{$$ = new_ast_parameter_declaration($1, NULL);}
								| TypeSpec ID										{$$ = new_ast_parameter_declaration($1, new_ast_id($2));}
								;
Declaration						: TypeSpec DeclarationPrime SEMI					{$$ = new_ast_declaration($1, $2);}
								| TypeSpec error SEMI								{$$ = NULL;}
								;
DeclarationPrime 				: Declarator COMMA DeclarationPrime					{$$ = new_ast_declarator_list($1, $3);}
								| Declarator										{$$ = new_ast_declarator_list($1, NULL);}
								;
TypeSpec						: CHAR												{$$ = new_ast_type(sym_char);}
								| INT												{$$ = new_ast_type(sym_int);}
								| VOID												{$$ = new_ast_type(sym_void);}
								| SHORT												{$$ = new_ast_type(sym_short);}
								| DOUBLE											{$$ = new_ast_type(sym_double);}
								;
Declarator 						: ID ASSIGN Expr									{$$ = new_ast_declarator(new_ast_id($1), $3);}
								| ID												{$$ = new_ast_declarator(new_ast_id($1), NULL);}
								;
Statement						: Expr SEMI											{$$ = $1;}
								| SEMI												{$$ = NULL;}
								| LBRACE StatementRecursivo RBRACE					{$$ = $2;}
								| IF LPAR Expr RPAR Statement						{$$ = new_ast_if($3, $5, NULL);}
								| IF LPAR Expr RPAR Statement ELSE Statement		{$$ = new_ast_if($3, $5, $7);}
								| WHILE LPAR Expr RPAR Statement					{$$ = new_ast_while($3, $5);}
								| RETURN Expr SEMI									{$$ = new_ast_return($2);}
								| RETURN SEMI										{$$ = new_ast_return(NULL);}
								| LBRACE error RBRACE								{$$ = NULL;}
								| error SEMI										{$$ = NULL;}
								;
StatementRecursivo              : Statement StatementRecursivo						{$$ = new_ast_statlist($1, $2);}
								|													{$$ = NULL;}
								;
Expr							: INTLIT											{$$ = new_ast_intlit($1);}
								| CHRLIT											{$$ = new_ast_chrlit($1);}
								| REALLIT											{$$ = new_ast_reallit($1);}
								| PLUS Expr %prec NOT								{$$ = new_ast_unary(op_plus, $2);}
								| MINUS Expr %prec NOT								{$$ = new_ast_unary(op_minus, $2);}
								| NOT Expr											{$$ = new_ast_unary(op_not, $2);}
								| ID												{$$ = new_ast_id($1);}
								| ID LPAR RPAR										{$$ = new_ast_call(new_ast_id($1), NULL, NULL);}
								| CallParameters RPAR 								{$$ = $1;}	/* pode gerar as COMMA Expr seguindo por Expr -> Expr COMMA Expr */
								| ID LPAR error RPAR								{$$ = NULL;}
								| LPAR Expr RPAR									{$$ = $2;}
								| LPAR error RPAR									{$$ = NULL;}
								| Expr ASSIGN Expr									{$$ = new_ast_binary(op_store, $1, $3);}
								| Expr COMMA Expr									{$$ = new_ast_binary(op_comma, $1, $3);}
								| Expr PLUS Expr									{$$ = new_ast_binary(op_add, $1, $3);}
								| Expr MINUS Expr									{$$ = new_ast_binary(op_sub, $1, $3);}
								| Expr MUL Expr										{$$ = new_ast_binary(op_mul, $1, $3);}
								| Expr DIV Expr										{$$ = new_ast_binary(op_div, $1, $3);}
								| Expr MOD Expr										{$$ = new_ast_binary(op_mod, $1, $3);}
								| Expr OR Expr										{$$ = new_ast_binary(op_or, $1, $3);}
								| Expr AND Expr										{$$ = new_ast_binary(op_and, $1, $3);}
								| Expr BITWISEAND Expr								{$$ = new_ast_binary(op_bitwiseand, $1, $3);}
								| Expr BITWISEOR Expr								{$$ = new_ast_binary(op_bitwiseor, $1, $3);}
								| Expr BITWISEXOR Expr								{$$ = new_ast_binary(op_bitwisexor, $1, $3);}
								| Expr EQ Expr										{$$ = new_ast_binary(op_eq, $1, $3);}
								| Expr NE Expr										{$$ = new_ast_binary(op_ne, $1, $3);}
								| Expr LE Expr										{$$ = new_ast_binary(op_le, $1, $3);}
								| Expr GE Expr										{$$ = new_ast_binary(op_ge, $1, $3);}
								| Expr LT Expr										{$$ = new_ast_binary(op_lt, $1, $3);}
								| Expr GT Expr										{$$ = new_ast_binary(op_gt, $1, $3);}
								;
CallParameters					: CallParameters COMMA Expr							{$$ = new_ast_call_parameter($1, $3); parametros_extra = 1;}
								| ID LPAR Expr										{if(parametros_extra){
																						$$ = new_ast_call(new_ast_id($1), $3, $$);
																						parametros_extra = 0;
																						}
																					else{
																						$$ = new_ast_call(new_ast_id($1), $3, NULL);
																					}
																					}
								;
%%


/* funcoes */
int main(int argc, char *argv[ ]){
	if (argc == 2) {
	/* FLAG | DESCRICAO | FATOR
	* <sem flag> | Apenas escrever no stdout as mensagens de erro correspondentes aos erros lexicais e de sintaxe | fator=0 == fator=4 == -e2
	* -l | Apenas realiza analise lexical - emitir os tokens e as mensagens de erro | fator=1
	* -e1 | Apenas realiza analise lexical - apenas as mensagens de erro | fator=2
	* -t | Imprimir a arvore de sintaxe | fator=3
	* -e2 | Apenas as mensagens de erro relativas aos erros sintáticos e lexicais | fator=4
	*/
		if (strcmp (argv[1], "-l") == 0) {
			fator = 1;  /* -l */
			yylex();
		} else {
			if (strcmp (argv[1], "-e1") == 0) {
				fator = 2;  /* -e1 */
				yylex();
			} else {
				if (strcmp (argv[1], "-t") == 0) {
					fator = 3;  /* -t */
					yyparse();
					if(!ocorreu_erro)
						print_ast(myprogram, 0);
					clean_ast(myprogram);
				} else {
					if (strcmp (argv[1], "-e2") == 0) {
						fator = 4;  /* -e2 */
						yyparse();
						clean_ast(myprogram);
					} else {
						/* ERRO NA FLAG == <sem flag>, fator=0 == fator=4 == -e2 */
						fator = 4;
						yyparse();
						clean_ast(myprogram);
					}
				}
			}
		}
	} else {
		/* ELSE: (fator = 0), <sem flag>, fator=0 == fator=4 == -e2 */
		fator = 4;
		yyparse();
		clean_ast(myprogram);
	}
	

	return 0;
}


int yywrap() {
	return 1;
}


