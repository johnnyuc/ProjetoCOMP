/* YACC - SINTATICA | Autores: Dario Felix 2018275530 e Jose Marcelo Marques da Cruz 2017255151 | COMP PL3 2020/21 | Meta 3 */


/* variaveis, declaracoes, bibliotecas, prototipos */
%{
	#include <stdio.h>
	#include <string.h>
	#include "functions.h"

	#define MANDATORY(N) (N == NULL) ? new_typeSpec(is_null) : N
	#define DATA_TYPE(N) N->data_type
	#define TYPE_CONVERSION(T1, T2) 

	extern int yylex();
	
	_program* myprogram;
	elemento_tabela * raiz_tabela;

	void yyerror (char *s);
	int fator = 0;
	int ocorreu_erro = 0;
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

%type <program> Start FunctionsAndDeclarations FunctionDeclaration TypeSpec ParameterList ParameterDeclaration
%type <program> Declaration DeclaratorList Declarator Expr
%type <program> FunctionDefinition FunctionBody DeclarationsAndStatements Statement StatementList CallParameters

/* yylval.xxx (tipo de token) */
%union{
int intValue;
double doubleValue;
char* stringValue;

_program* program;
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
Start							: FunctionsAndDeclarations								{if(!ocorreu_erro){$$ = myprogram = new_program((void*) $1);}}
								;

FunctionsAndDeclarations		: FunctionsAndDeclarations FunctionDefinition			{if(!ocorreu_erro){$$ = append_list((void*)$1, (void*)$2);}}
								| FunctionsAndDeclarations FunctionDeclaration			{if(!ocorreu_erro){$$ = append_list((void*)$1, (void*)$2);}}
								| FunctionsAndDeclarations Declaration					{if(!ocorreu_erro){$$ = append_list((void*)$1, (void*)$2);}}
								| FunctionsAndDeclarations error SEMI
								| FunctionDefinition									{if(!ocorreu_erro){$$ = $1;}}
								| FunctionDeclaration									{if(!ocorreu_erro){$$ = $1;}}
								| Declaration											{if(!ocorreu_erro){$$ = $1;}}
								| error SEMI											{/*erro*/}
								;
									
FunctionDefinition				: TypeSpec ID LPAR ParameterList RPAR FunctionBody		{if(!ocorreu_erro){$$ = new_functionDefinition((_typeSpec*) $1, (_terminal*) new_terminal(Id, $2), (_parameterList*) $4, (_functionBody*) $6);}}
								;
FunctionBody					: LBRACE DeclarationsAndStatements RBRACE				{if(!ocorreu_erro){$$ = new_functionBody((void*) $2);}}
								| LBRACE RBRACE											{if(!ocorreu_erro){$$ = new_functionBody(NULL);}}
								;
DeclarationsAndStatements		: DeclarationsAndStatements Declaration					{if(!ocorreu_erro){$$ = append_list((void*)$1, (void*)$2);}}
								| DeclarationsAndStatements	 Statement					{if(!ocorreu_erro){$$ = append_list((void*)$1, (void*)$2);}}
								| Statement												{if(!ocorreu_erro){$$ = $1;}}
								| Declaration											{if(!ocorreu_erro){$$ = $1;}}
								;
FunctionDeclaration				: TypeSpec ID LPAR ParameterList RPAR SEMI				{if(!ocorreu_erro){$$ = new_functionDeclaration((_typeSpec*) $1, (_terminal*) new_terminal(Id, $2), (_parameterList*) $4);}}		
								;					
ParameterList					: ParameterDeclaration									{if(!ocorreu_erro){$$ = new_parameterList((_parameterDeclaration*) $1);}}
								| ParameterList COMMA ParameterDeclaration				{if(!ocorreu_erro){append_list((void*)((_parameterList*)$$)->first_parameter, (void*) $3);}}
								;
ParameterDeclaration			: TypeSpec												{if(!ocorreu_erro){$$ = new_parameterDeclaration((_typeSpec*) $1, NULL);}}
								| TypeSpec ID											{if(!ocorreu_erro){$$ = new_parameterDeclaration((_typeSpec*) $1, (_terminal*) new_terminal(Id, $2));}}
								;
Declaration						: TypeSpec DeclaratorList SEMI							{if(!ocorreu_erro){$$ = set_typeSpec((_typeSpec*) $1, (_declaration*) $2);}}
								| TypeSpec error SEMI									{/*erro*/}
								//| error SEMI
								;
DeclaratorList	 				: Declarator											{if(!ocorreu_erro){$$ = $1;}}
								| DeclaratorList COMMA Declarator						{if(!ocorreu_erro){$$ = append_list((void*)$1, (void*)$3);}}
								;
TypeSpec						: CHAR													{if(!ocorreu_erro){$$ = new_typeSpec(Char);}}
								| INT													{if(!ocorreu_erro){$$ = new_typeSpec(Int);}}
								| VOID													{if(!ocorreu_erro){$$ = new_typeSpec(Void);}}
								| SHORT													{if(!ocorreu_erro){$$ = new_typeSpec(Short);}}
								| DOUBLE												{if(!ocorreu_erro){$$ = new_typeSpec(Double);}}
								;
Declarator 						: ID ASSIGN Expr										{if(!ocorreu_erro){$$ = new_declaration((_terminal*) new_terminal(Id, $1), (void*) $3);}}
								| ID													{if(!ocorreu_erro){$$ = new_declaration((_terminal*) new_terminal(Id, $1), NULL);}}
								;
Statement						: Expr SEMI												{if(!ocorreu_erro){$$ = $1;}}
								| SEMI													{if(!ocorreu_erro){$$ = NULL;}}
								| LBRACE StatementList RBRACE							{if(!ocorreu_erro){if($2 && $2->next){$$ = new_statementList((void*) $2);} else{$$ = $2;}}}
								| LBRACE RBRACE											{if(!ocorreu_erro){$$ = NULL;}}
								| IF LPAR Expr RPAR Statement							{if(!ocorreu_erro){$$ = new_if((void*) $3, MANDATORY((void*) $5), (void*) new_typeSpec(is_null));}}
								| IF LPAR Expr RPAR Statement ELSE Statement			{if(!ocorreu_erro){$$ = new_if((void*) $3, MANDATORY((void*) $5), MANDATORY((void*) $7));}}
								| WHILE LPAR Expr RPAR Statement						{if(!ocorreu_erro){$$ = new_while((void*)$3, MANDATORY((void*)$5));}}
								| RETURN Expr SEMI										{if(!ocorreu_erro){$$ = new_return((void*)$2);}}
								| RETURN SEMI											{if(!ocorreu_erro){$$ = new_return((void*) new_typeSpec(is_null));}}
								| LBRACE error RBRACE									{;/*erro*/}
								| error SEMI											{;/*erro*/}
								;
StatementList					: StatementList Statement								{if(!ocorreu_erro){$$ = append_list((void*)$1, (void*)$2);}}
								| Statement												{if(!ocorreu_erro){$$ = $1;}}
								;
Expr							: INTLIT												{if(!ocorreu_erro){$$ = new_terminal(is_intLit, $1);}}
								| CHRLIT												{if(!ocorreu_erro){$$ = new_terminal(is_chrLit, $1);}}
								| REALLIT												{if(!ocorreu_erro){$$ = new_terminal(is_realLit, $1);}}
								| PLUS Expr %prec NOT									{if(!ocorreu_erro){$$ = new_unaryOp(is_plus, $2);}}
								| MINUS Expr %prec NOT									{if(!ocorreu_erro){$$ = new_unaryOp(is_minus, $2);}}
								| NOT Expr												{if(!ocorreu_erro){$$ = new_unaryOp(is_not, $2);}}
								| ID													{if(!ocorreu_erro){$$ = new_terminal(Id, $1);}}
								| ID LPAR RPAR											{if(!ocorreu_erro){$$ = new_call((_terminal*) new_terminal(Id, $1), NULL);}}
								| CallParameters RPAR 									{if(!ocorreu_erro){$$ = $1;}}
								| ID LPAR error RPAR									{/*erro*/}
								| LPAR Expr RPAR										{if(!ocorreu_erro){$$ = $2;}}
								| LPAR error RPAR										{/*erro*/}
								| Expr ASSIGN Expr										{if(!ocorreu_erro){$$ = new_binaryOp(is_store, (void*) $1, (void*) $3);}}
								| Expr COMMA Expr										{if(!ocorreu_erro){$$ = new_binaryOp(is_comma, (void*) $1, (void*) $3);}}
								| Expr PLUS Expr										{if(!ocorreu_erro){$$ = new_binaryOp(is_add, (void*) $1, (void*) $3);}}
								| Expr MINUS Expr										{if(!ocorreu_erro){$$ = new_binaryOp(is_sub, (void*) $1, (void*) $3);}}
								| Expr MUL Expr											{if(!ocorreu_erro){$$ = new_binaryOp(is_mul, (void*) $1, (void*) $3);}}
								| Expr DIV Expr											{if(!ocorreu_erro){$$ = new_binaryOp(is_div, (void*) $1, (void*) $3);}}
								| Expr MOD Expr											{if(!ocorreu_erro){$$ = new_binaryOp(is_mod, (void*) $1, (void*) $3);}}
								| Expr OR Expr											{if(!ocorreu_erro){$$ = new_binaryOp(is_or, (void*) $1, (void*) $3);}}
								| Expr AND Expr											{if(!ocorreu_erro){$$ = new_binaryOp(is_and, (void*) $1, (void*) $3);}}
								| Expr BITWISEAND Expr									{if(!ocorreu_erro){$$ = new_binaryOp(is_bitwiseand, (void*) $1, (void*) $3);}}
								| Expr BITWISEOR Expr									{if(!ocorreu_erro){$$ = new_binaryOp(is_bitwiseor, (void*) $1, (void*) $3);}}
								| Expr BITWISEXOR Expr									{if(!ocorreu_erro){$$ = new_binaryOp(is_bitwisexor, (void*) $1, (void*) $3);}}
								| Expr EQ Expr											{if(!ocorreu_erro){$$ = new_binaryOp(is_eq, (void*) $1, (void*) $3);}}
								| Expr NE Expr											{if(!ocorreu_erro){$$ = new_binaryOp(is_ne, (void*) $1, (void*) $3);}}
								| Expr LE Expr											{if(!ocorreu_erro){$$ = new_binaryOp(is_le, (void*) $1, (void*) $3);}}
								| Expr GE Expr											{if(!ocorreu_erro){$$ = new_binaryOp(is_ge, (void*) $1, (void*) $3);}}
								| Expr LT Expr											{if(!ocorreu_erro){$$ = new_binaryOp(is_lt, (void*) $1, (void*) $3);}}
								| Expr GT Expr											{if(!ocorreu_erro){$$ = new_binaryOp(is_gt, (void*) $1, (void*) $3);}}
								//| error
								;
CallParameters					: CallParameters COMMA Expr								{if(!ocorreu_erro){append_list((void*)((_call*)$$)->first_parameter, (void*)$3);}} // call -> id -> expr
								| ID LPAR Expr											{if(!ocorreu_erro){$$ = new_call((_terminal*) new_terminal(Id, $1), $3);}} 
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
						printTree(myprogram, 0);
					cleanTree(myprogram);
				} else {
					if (strcmp (argv[1], "-e2") == 0) {
						fator = 4;  /* -e2 */
						yyparse();
						cleanTree(myprogram);
					} else {
						if(strcmp (argv[1], "-s") == 0) {
							fator = 4;
							yyparse();
							if(!ocorreu_erro){
								check_program(myprogram, NULL);
								tabela_print();
								printTreeAnotada(myprogram, 0, 0);
							}
						}
						else {
							/* ERRO NA FLAG == <sem flag>, fator=0 == fator=4 == -e2 */
							fator = 4;
							yyparse();
							cleanTree(myprogram);
						}
					}
				}
			}
		}
	} else {
		/* ELSE: (fator = 0), <sem flag>, fator=0 == fator=4 == -e2 */
		fator = 4;
		yyparse();
		cleanTree(myprogram);
	}
	

	return 0;
}


int yywrap() {
	return 1;
}


