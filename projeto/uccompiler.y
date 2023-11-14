/* PROJETO META 2 */
/* Johnny Fernandes 2021190668 */
/* Stefanos Lucena  2021115090 */

/* -------------------------------------------- DEFINITIONS SECTION -------------------------------------------- */
%{
    #include <stdio.h>
    #include "ast.h"

    extern int yylex(void);
    void yyerror();
    extern char *yytext;

    extern int line;
    extern int col;

    struct node *program;
    struct node *noaux;

%}

/* Tokens */
%token CHAR
%token INT
%token VOID
%token SHORT
%token DOUBLE
%token IF
%token ELSE
%token WHILE
%token RETURN
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
%token RESERVED

%token <lexeme> IDENTIFIER NATURAL DECIMAL CHRLIT 

%left COMMA
%right ASSIGN
%left OR
%left AND
%left BITWISEOR
%left BITWISEAND
%left BITWISEXOR
%left EQ NE
%left GT LT GE LE
%left PLUS MINUS
%left MUL DIV MOD
%right NOT
%left LPAR RPAR
%right ELSE
%nonassoc RESERVED

%type <node> Program FunctionsAndDeclarations TypeFuncDec FunctionDefinition FunctionBody FunctionBodyOpt Expr3
%type <node> DeclarationsAndStatements FunctionDeclaration FunctionDeclarator ParameterList 
%type <node> ParameterListAux ParameterDeclaration ParameterDeclarationOpt Declaration DeclaratorList TypeSpec Declarator StatementGlobal Statement Statements ExprOpt Expr Expr2

%union{
    char *lexeme;
    struct node *node;
}

/* -------------------------------------------- RULES SECTION -------------------------------------------- */
%%
Program
    : 
    FunctionsAndDeclarations { $$ = program = newnode(Program, NULL); addchild($$, $1); }

;

FunctionsAndDeclarations
    : TypeFuncDec { $$ = $1; }
    | FunctionsAndDeclarations TypeFuncDec { $$ = $1; addbrother($1, $2); }
;

TypeFuncDec
    : FunctionDefinition  { $$ = $1; }
    | FunctionDeclaration { $$ = $1; }
    | Declaration         { $$ = $1; }
;

FunctionDefinition
    : TypeSpec FunctionDeclarator FunctionBody { $$= newnode(FuncDefinition, NULL); addchild($$, $1); addchild($$, $2); addchild($$, $3); }
    | TypeSpec FunctionDeclarator error { $$ = newnode(Null, NULL); }
;

FunctionBody
    : LBRACE FunctionBodyOpt RBRACE { $$ = newnode(FuncBody, NULL); addchild($$, $2); }

FunctionBodyOpt
    : DeclarationsAndStatements { $$ = $1; }
    | { $$ = NULL; }
;

DeclarationsAndStatements
    : Statement DeclarationsAndStatements   {if( $1 != NULL){$$=$1; if($2 != NULL){addbrother($1,$2);}} else{$$=$2;}}
    | Declaration DeclarationsAndStatements { $$ = $1; addbrother($1, $2); }
    | Statement { $$ = $1; }
    | Declaration { $$ = $1; }
;

FunctionDeclaration
    : TypeSpec FunctionDeclarator SEMI { $$ = newnode(FuncDeclaration, NULL); addchild($$, $1); addchild($$, $2); }  //?
;

FunctionDeclarator
    : IDENTIFIER LPAR ParameterList RPAR { $$ = newnode(Identifier, $1); addbrother($$, $3); }
;

ParameterList
    : ParameterDeclaration ParameterListAux { $$ = newnode(ParamList, NULL); addchild($$, $1); if($2 != NULL) {addchild($$, $2);} }
;

ParameterListAux: ParameterListAux COMMA ParameterDeclaration { if($1 != NULL) {$$ = $1; addbrother($$, $3);} else {$$ = $3;} }
    | { $$ = NULL; }
;


ParameterDeclaration
    : TypeSpec ParameterDeclarationOpt { $$ = newnode(ParamDeclaration, NULL); addchild($$, $1); if($2 != NULL) {addchild($$, $2);} }
;

ParameterDeclarationOpt
    : IDENTIFIER { $$ = newnode(Identifier, $1); }
    | { $$ = NULL; }
;

Declaration
    : TypeSpec Declarator DeclaratorList SEMI { $$ = newnode(Declaration, NULL); addchild($$, $1); addchild($$, $2); if($3 != NULL) {addbrother($$, $3);} }
    | error SEMI /* 1st error */ { $$ = newnode(Null, NULL); }
;

DeclaratorList
    : DeclaratorList COMMA Declarator {if ($1 != NULL){
											$$ = $1;
                                             struct node *no1=newnode(Declaration,NULL);
                                            addchild(no1,noaux);
                                            addbrother($1,no1);
											addchild(no1,$3);
										} else {
											$$=newnode(Declaration,NULL);
                                            addchild($$,noaux);
                                            addchild($$,$3);
                                        }
										}
    | { $$ = NULL; }
;

TypeSpec
    : CHAR { $$ = noaux = newnode(Char, NULL); }                        
    | INT { $$ = noaux = newnode(Int, NULL); } 
    | VOID { $$ = noaux = newnode(Void, NULL); } 
    | SHORT { $$ = noaux = newnode(Short, NULL); } 
    | DOUBLE { $$ = noaux = newnode(Double, NULL); } 
;

Declarator
    : IDENTIFIER { $$ = newnode(Identifier,$1);}
    | IDENTIFIER ASSIGN Expr2 { $$ = newnode(Identifier, $1); addbrother($$, $3); } //?
;

StatementGlobal
    : error SEMI /* 2nd error */ { $$ = newnode(Null, NULL); }
    | Statement { $$ = $1; }
;

Statement
    : SEMI { $$ = NULL; }
    | Expr3 SEMI { $$ = $1; }
    | LBRACE RBRACE { $$ = NULL; }
    | LBRACE Statements RBRACE { $$ = $2;}
    | IF LPAR Expr3 RPAR StatementGlobal ELSE StatementGlobal { $$ = newnode(If, NULL); if($3 == NULL) {addchild($$, newnode(Null, NULL));} else {addchild($$, $3);} if($5 == NULL) {addchild($$, newnode(Null, NULL));} else {addchild($$, $5);}; if($7 == NULL) {addchild($$, newnode(Null, NULL));} else {addchild($$, $7);} }
    | IF LPAR Expr3 RPAR StatementGlobal { $$ = newnode(If, NULL); if($3 == NULL) {addchild($$, newnode(Null, NULL));} else {addchild($$, $3);} if($5 == NULL) {addchild($$, newnode(Null, NULL));} else {addchild($$, $5);}; addchild($$, newnode(Null, NULL)); }
    | WHILE LPAR Expr3 RPAR StatementGlobal { $$ = newnode(While, NULL); if($3 == NULL) {addchild($$, newnode(Null, NULL));} else {addchild($$, $3);} if($5 == NULL) {addchild($$, newnode(Null, NULL));} else {addchild($$, $5);} }
    | RETURN SEMI { $$ = newnode(Return, NULL); addchild($$, newnode(Null, NULL)); }
    | RETURN Expr3 SEMI { $$ = newnode(Return, NULL); addchild($$, $2);}
    | LBRACE error RBRACE /* 3rd error */ { $$ = newnode(Null, NULL); }
;

Statements
    : StatementGlobal { $$ = $1; }
    | Statements StatementGlobal {
        if($1!=NULL){
            if ($1->category != StatList && countbrother($1) == 0) {
                // Criar um StatList se for o primeiro irmão a ser adicionado
                struct node *new_statlist = newnode(StatList, NULL);
                addchild(new_statlist, $1);
                addbrother($1, $2);
                $$ = new_statlist;
            } else {
                // Adicionar como irmão diretamente
                addchild($1, $2);
                $$ = $1;
            }
        }
        else{$$=$2;}
    }
;

ExprOpt
    : Expr2 { $$ = $1; }
    | { $$ = NULL; }
;

Expr
    : 
    Expr ASSIGN Expr { $$ = newnode(Store, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr PLUS Expr { $$ = newnode(Add, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr MINUS Expr { $$ = newnode(Sub, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr MUL Expr { $$ = newnode(Mul, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr DIV Expr { $$ = newnode(Div, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr MOD Expr { $$ = newnode(Mod, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr OR Expr { $$ = newnode(Or, NULL);  addchild($$, $1); addchild($$, $3); }
    |Expr AND Expr { $$ = newnode(And, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr BITWISEAND Expr { $$ = newnode(BitWiseAnd, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr BITWISEOR  Expr { $$ = newnode(BitWiseOr, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr BITWISEXOR Expr { $$ = newnode(BitWiseXor, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr EQ Expr { $$ = newnode(Eq, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr NE Expr { $$ = newnode(Ne, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr GE Expr { $$ = newnode(Ge, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr LT Expr { $$ = newnode(Lt, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr LE Expr { $$ = newnode(Le, NULL); addchild($$, $1); addchild($$, $3); }
    |Expr GT Expr { $$ = newnode(Gt, NULL); addchild($$, $1); addchild($$, $3); }
    |PLUS Expr %prec NOT { $$ = newnode(Plus, NULL); addchild($$, $2); }
    |MINUS Expr %prec NOT { $$ = newnode(Minus, NULL); addchild($$, $2); }
    |NOT Expr  %prec NOT { $$ = newnode(Not, NULL); addchild($$, $2); }
    |IDENTIFIER LPAR ExprOpt RPAR { $$ = newnode(Call, NULL); addchild($$, newnode(Identifier, $1)); addchild($$, $3); }
    |IDENTIFIER { $$ = newnode(Identifier,$1); }
    |NATURAL { $$ = newnode(Natural, $1); }
    |CHRLIT { $$ = newnode(ChrLit, $1); }
    |DECIMAL { $$ = newnode(Decimal, $1); }
    |LPAR Expr3 RPAR { $$ = $2; }
    |IDENTIFIER LPAR error RPAR { $$ = newnode(Null, NULL); }
    |LPAR error RPAR { $$ = newnode(Null, NULL); }
;

Expr2
    : Expr2 COMMA Expr { $$ = $1; addbrother($1, $3); }
    | Expr { $$ = $1; }
;

Expr3 
    : Expr3 COMMA Expr { $$ = newnode(Comma, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr { $$ = $1; }


%%

/* -------------------------------------------- SUBROUTINES SECTION -------------------------------------------- */