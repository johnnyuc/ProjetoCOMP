%{

	//Pedro Miguel Alves Miranda uc2012148969
	//Pedro Manuel Egas Simoes uc2016230351
 #include <stdio.h>
 #include <string.h>
#include "ast.h"
 #include "y.tab.h"

struct ast_node* ast;
struct ast_node* aux;
int yylex(void);
extern void yyerror(char* s);
char tipo[10];
int func=0;
extern int col,  option, line, com_c, com_l, error;

int print = 0;
%}
%union {
	struct Inf* inf;
	struct ast_node* tree;
};
//criar struct com char lin e col
%token <inf> CHAR
%token ELSE
%token WHILE
%token IF
%token <inf> INT
%token <inf> SHORT
%token <inf> DOUBLE
%token RETURN
%token <inf> VOID
%token BITWISEAND
%token BITWISEOR
%token BITWISEXOR
%token AND
%token ASSIGN
%token MUL
%token COMMA
%token DIV
%token EQ
%token GE
%token GT
%token LBRACE
%token LE
%token LPAR
%token LT
%token MINUS
%token MOD
%token NE
%token NOT
%token OR
%token PLUS
%token RBRACE
%token RPAR
%token SEMI
%token <inf> RESERVED ID INTLIT REALLIT CHRLIT
//mudar este str

%type <tree> Begin FunctionsAndDeclarations FunctionDefinition FunctionDeclaration Declaration TypeSpec FunctionDeclarator FunctionBody DeclarationsAndStatements Statement ParameterDeclaration CommaParam_rep Declarator CommaDec_rep Expr Expr_rep RepeatExp StatementIncludingError RepeatStatError ParameterList


%left COMMA
%right ASSIGN

%left OR
%left AND
%left BITWISEOR
%left BITWISEAND
%left BITWISEXOR
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left DIV MUL MOD

%right NOT
%left LPAR RPAR

%nonassoc IF ELSE
%nonassoc LBRACE RBRACE


%%





Begin: FunctionsAndDeclarations 				{ast=register_ast("Program",0,"null",false,0,0);$$=ast;add_son($$,$1);}
						;

FunctionsAndDeclarations :

						 FunctionsAndDeclarations FunctionDefinition {$$ = $1; add_brother($1, $2);}
						| FunctionsAndDeclarations FunctionDeclaration {$$ = $1; add_brother($1, $2);}
						| FunctionsAndDeclarations Declaration {$$=$1; add_brother($1, $2);}
						| FunctionDefinition	{$$ = $1;}
						| FunctionDeclaration  {$$=$1;}
						| Declaration 		{$$=$1;}
						;

FunctionDefinition: TypeSpec	FunctionDeclarator	FunctionBody  {$$=register_ast("FuncDefinition",0,"null",false,0,0);add_son($$,$1);add_son($$,$2);$2->type=$1->type;if($3!=NULL){add_son($$,$3);}}
						;

FunctionBody: LBRACE	DeclarationsAndStatements	RBRACE {if($2!=NULL){$$=register_ast("FuncBody",0,"null",false,0,0);add_son($$,$2);}else{$$=register_ast("FuncBody",0,"null",false,0,0);}}
						| LBRACE RBRACE {$$=register_ast("FuncBody",0,"null",false,0,0);}

						;

/*vvvvvvvvvvvvvvvvvvvvvvv REDUCE REDUCE vvvvvvvvvvvvvvvvvvvvvv*/

DeclarationsAndStatements : Statement DeclarationsAndStatements {$$=$1; add_brother($$, $2);}
						| Declaration DeclarationsAndStatements {$$ = $1; add_brother($$, $2);}
						| Statement   {$$=$1;}
						| Declaration {$$=$1;}
						;
/*^^^^^^^^^^^^^^^^^^^^^ REDUCE REDUCE ^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
FunctionDeclaration: TypeSpec 	FunctionDeclarator 	SEMI {$$=register_ast("FuncDeclaration",0,"null",false,0,0);add_son($$,$1);add_son($$,$2);$2->type=$1->type;}
;

FunctionDeclarator: ID LPAR ParameterList RPAR {$$=id($1);$$->type="null";add_brother($$,$3);}
						;

ParameterList : ParameterDeclaration CommaParam_rep {$$=register_ast("ParamList",0,"null",false,0,0);add_son($$,$1);if($2!=NULL){add_son($$,$2);}}
						;

CommaParam_rep : 	/*empty*/		{$$=NULL;}
						| CommaParam_rep COMMA ParameterDeclaration {if($1!=NULL){$$=$1;add_brother($$,$3);}else{$$=$3;}}
						;

ParameterDeclaration :  TypeSpec {$$=register_ast("ParamDeclaration",0,"null",false,0,0);add_son($$,$1);}
						| TypeSpec ID {$$=register_ast("ParamDeclaration",0,"null",false,0,0);add_son($$,$1);add_son($$,id($2));$$->son->brother->type=$1->type;}
						;


                   		;
Declaration 	: TypeSpec	Declarator 	CommaDec_rep SEMI {$$=register_ast("Declaration",0,"null",false,0,0);aux = $1 ;add_son($$,$1);add_son($$,$2);$2->type="Null";if($3!=NULL){add_brother($$,$3);}}
						| error SEMI	{$$ = NULL; print = 1;}
						;

CommaDec_rep: 		/*empty*/		{$$=NULL;}
						| CommaDec_rep COMMA Declarator	{if ($1 != NULL){
											$$ = $1;
											ast_node* aux1=register_ast("Declaration",0,"null",false,0,0); 												add_brother($1,aux1);
											add_son(aux1,register_ast(tipo,0,"null",false,0,0));
											add_son(aux1,$3);$3->type="Null";
										} else {
											$$=register_ast("Declaration",0,"null",false,0,0);
											add_son($$,register_ast(tipo,0,"null",false,0,0));add_son($$,$3);$3->type="Null";}
										}
						;

TypeSpec: CHAR 				{$$=register_ast("Char",0,"Null",false,$1->lin,$1->col);strncpy(tipo,"Char",sizeof(tipo));}
						| INT  {$$=register_ast("Int",0,"Null",false,$1->lin,$1->col);strncpy(tipo,"Int",sizeof(tipo));}
						| VOID  {$$=register_ast("Void",0,"Null",false,$1->lin,$1->col);strncpy(tipo,"Void",sizeof(tipo));}
						| SHORT  {$$=register_ast("Short",0,"Null",false,$1->lin,$1->col);strncpy(tipo,"Short",sizeof(tipo));}
						| DOUBLE {$$=register_ast("Double",0,"Null",false,$1->lin,$1->col);strncpy(tipo,"Double",sizeof(tipo));}
						;
Declarator: ID {$$=id($1);}
						|ID ASSIGN Expr	{$$=id($1);add_brother($$,$3);}

						;
/*vvvvvvvvvvvvvvvvvvvv   ITS HEREEE reduce reduce  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
						;


Statement : 			 Expr SEMI {$$=$1;}

						| LBRACE RepeatStatError RBRACE {if(statList_childs($2)>=2){$$=register_ast("StatList",0,"null",false,0,0);add_son($$,$2);}else{if($2==NULL){$$=register_ast("Null",1,"null",false,0,0);}else{$$=$2;}}}
						| LBRACE RBRACE	{$$=NULL;}
						| SEMI {$$=register_ast("Null",1,"null",false,0,0);}
						| IF LPAR Expr RPAR StatementIncludingError {$$=register_ast("If",0,"null",false,0,0);add_son($$,$3);if($5==NULL){add_son($$,register_ast("Null",0,"null",false,0,0));}else{add_son($$,$5);}add_son($$,register_ast("Null",0,"null",false,0,0));}
						| IF LPAR Expr RPAR StatementIncludingError  ELSE StatementIncludingError {$$=register_ast("If",0,"null",false,0,0);add_son($$,$3);if($5==NULL){add_son($$,register_ast("Null",0,"null",false,0,0));}else{add_son($$,$5);}if($7==NULL){add_son($$,register_ast("Null",0,"null",false,0,0));}else{add_son($$,$7);}}
						| WHILE LPAR Expr RPAR StatementIncludingError {$$=register_ast("While",0,"null",false,0,0);add_son($$,$3);if($5==NULL){add_son($$,register_ast("Null",0,"null",false,0,0));}else{add_son($$,$5);}}
						| RETURN Expr_rep SEMI {$$=register_ast("Return",0,"null",false,0,0);if($2==NULL){add_son($$,register_ast("Null",0,"null",false,0,0));}else{add_son($$,$2);}}
						| LBRACE error RBRACE	{$$ = NULL; print = 1;}
						;


StatementIncludingError: Expr_rep SEMI {$$=$1;}

						| LBRACE RepeatStatError RBRACE {if(statList_childs($2)>=2){$$=register_ast("StatList",0,"null",false,0,0);add_son($$,$2);}else{if(is_null($2)==1){$$=register_ast("Null",0,"null",false,0,0);}else{$$=$2;}}}
						| LBRACE RBRACE	{$$=NULL;}
						| IF LPAR Expr RPAR StatementIncludingError {$$=register_ast("If",0,"null",false,0,0);add_son($$,$3);if($5==NULL){add_son($$,register_ast("Null",0,"null",false,0,0));}else{add_son($$,$5);}add_son($$,register_ast("Null",0,"null",false,0,0));}
						| IF LPAR Expr RPAR StatementIncludingError  ELSE StatementIncludingError {$$=register_ast("If",0,"null",false,0,0);add_son($$,$3);if($5==NULL){add_son($$,register_ast("Null",0,"null",false,0,0));}else{add_son($$,$5);}if($7==NULL){add_son($$,register_ast("Null",0,"null",false,0,0));}else{add_son($$,$7);}}
						| WHILE LPAR Expr RPAR StatementIncludingError {$$=register_ast("While",0,"null",false,0,0);add_son($$,$3);if($5==NULL){add_son($$,register_ast("Null",0,"null",false,0,0));}else{add_son($$,$5);}}
						| RETURN Expr_rep SEMI {$$=register_ast("Return",0,"null",false,0,0);if($2==NULL){add_son($$,register_ast("Null",0,"null",false,0,0));}else{add_son($$,$2);}}
						| error SEMI {$$ = NULL; print = 1;}
						| LBRACE error RBRACE	{$$ = NULL; print = 1;}
						;


Expr_rep: /*empty*/ 	{$$=NULL;}
						| Expr {$$=$1;}

                  ;

RepeatStatError: RepeatStatError StatementIncludingError {if($1 !=NULL){$$=$1;add_brother($$, $2);}else{$$=$2;}}
					  	| StatementIncludingError	{$$=$1;}
						;
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


Expr: Expr ASSIGN Expr {$$=register_ast("Store",0,"tipo",false,$1->lin,$1->col);add_son($$,$1);add_son($$,$3);}
						| Expr COMMA Expr {$$=register_ast("Comma",0,"tipo2",false,0,0);add_son($$,$1);add_son($$,$3);}

						| Expr PLUS Expr {$$=register_ast("Add",0,"tipo1",false,0,0);add_son($$,$1);add_son($$,$3);}
						| Expr MINUS Expr {$$=register_ast("Sub",0,"tipo1",false,0,0);add_son($$,$1);add_son($$,$3);}
						| Expr MUL Expr {$$=register_ast("Mul",0,"tipo1",false,0,0);add_son($$,$1);add_son($$,$3);}
						| Expr DIV Expr {$$=register_ast("Div",0,"tipo1",false,0,0);add_son($$,$1);add_son($$,$3);}
						| Expr MOD Expr {$$=register_ast("Mod",0,"int",false,0,0);add_son($$,$1);add_son($$,$3);}

						| Expr OR Expr  {$$=register_ast("Or",0,"int",false,0,0);add_son($$,$1);add_son($$,$3);}
						| Expr AND Expr  {$$=register_ast("And",0,"int",false,0,0);add_son($$,$1);add_son($$,$3);}
						| Expr BITWISEAND Expr  {$$=register_ast("BitWiseAnd",0,"int",false,0,0);add_son($$,$1);add_son($$,$3);}
						| Expr BITWISEOR  Expr  {$$=register_ast("BitWiseOr",0,"int",false,0,0);add_son($$,$1);add_son($$,$3);}
						| Expr BITWISEXOR Expr {$$=register_ast("BitWiseXor",0,"int",false,0,0);add_son($$,$1);add_son($$,$3);}

						| Expr EQ Expr  {$$=register_ast("Eq",0,"int",false,0,0);add_son($$,$1);add_son($$,$3);}
						| Expr NE Expr  {$$=register_ast("Ne",0,"int",false,0,0);add_son($$,$1);add_son($$,$3);}
						| Expr LE Expr  {$$=register_ast("Le",0,"int",false,0,0);add_son($$,$1);add_son($$,$3);}
						| Expr GE Expr  {$$=register_ast("Ge",0,"int",false,0,0);add_son($$,$1);add_son($$,$3);}
						| Expr LT Expr  {$$=register_ast("Lt",0,"int",false,0,0);add_son($$,$1);add_son($$,$3);}
						| Expr GT Expr   {$$=register_ast("Gt",0,"int",false,0,0);add_son($$,$1);add_son($$,$3);}

						| PLUS Expr   %prec NOT {$$=register_ast("Plus",0,"tipo",false,0,0);add_son($$,$2);}
						| MINUS Expr   %prec NOT {$$=register_ast("Minus",0,"tipo",false,0,0);add_son($$,$2);}
						| NOT Expr     %prec NOT{$$=register_ast("Not",0,"int",false,0,0);add_son($$,$2);}

						| RepeatExp RPAR	{$$ = $1;}
						| ID LPAR RPAR	{$$=register_ast("Call",0,"tipo",false,$1->lin,$1->col);add_son($$,id($1));}

						| ID     {$$=id($1);}
						| INTLIT  {$$=intlit($1);}
						| CHRLIT   {$$=chrlit($1);}
						| REALLIT   {$$=realit($1);}
						| LPAR Expr RPAR  {$$=$2;}

						| ID LPAR error RPAR	{$$ = NULL; print = 1;}
						| LPAR error RPAR	{$$ = NULL; print = 1;}
						;

/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/* shift reduce warn resolved*/
RepeatExp: RepeatExp COMMA Expr {$$=$1;add_son($1,$3);}
	  |ID LPAR Expr {$$=register_ast("Call",0,"tipo",false,$1->lin,$1->col);add_son($$,id($1));add_son($$,$3);}
	  ;
%%
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
