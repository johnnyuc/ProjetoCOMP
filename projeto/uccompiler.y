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

/* Precedences */

%left COMMA
%right RESERVED
%right ASSIGN
%left OR AND BITWISEOR BITWISEAND BITWISEXOR
%left EQ NE LE GE LT GT
%left PLUS MINUS
%left MUL DIV MOD
%left LPAR RPAR
%right NOT

/* Non associatives */
%right ELSE

%type <node> Program FunctionsAndDeclarations TypeFuncDec FunctionDefinition FunctionBody FunctionBodyOpt 
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
    FunctionsAndDeclarations  
    {

        $$ = newnode(Program,NULL);
        addchild($$, $1);
    }

;

FunctionsAndDeclarations
    : TypeFuncDec
    {
        $$=$1;
    }
    | FunctionsAndDeclarations TypeFuncDec
    {
        // Anexa o novo TypeFuncDec à lista de nós da esquerda para a direita.
        addchild($1, $2);
        $$ = $1; // Passa a lista modificada para cima na árvore de análise.
    }
;

TypeFuncDec
    : FunctionDefinition
    | FunctionDeclaration
    | Declaration
;

FunctionDefinition
    : TypeSpec FunctionDeclarator FunctionBody
    | TypeSpec FunctionDeclarator error {$$ = NULL;}
;

FunctionBody
    : LBRACE FunctionBodyOpt RBRACE

FunctionBodyOpt
    : DeclarationsAndStatements
    |                           {$$ = NULL;}
;

DeclarationsAndStatements
    : Statement DeclarationsAndStatements
    | Declaration DeclarationsAndStatements
    | Statement
    | Declaration
;

FunctionDeclaration
    : TypeSpec FunctionDeclarator SEMI
;

FunctionDeclarator
    : IDENTIFIER LPAR ParameterList RPAR
;

ParameterList
    : ParameterDeclaration ParameterListAux
;

ParameterListAux: ParameterListAux COMMA ParameterDeclaration
                |                                  {$$ = NULL;}
;


ParameterDeclaration
    : TypeSpec ParameterDeclarationOpt
;

ParameterDeclarationOpt
    :
    IDENTIFIER
    |                   {$$ = NULL;}
;


Declaration
    : TypeSpec Declarator DeclaratorList SEMI
    {

    }

    | error SEMI /* 1st error */                
    {
        $$ = NULL;
    }
;

DeclaratorList
    : DeclaratorList COMMA Declarator
    |
    {
        $$ = NULL;
    }
;

TypeSpec
    : CHAR
    | INT
    | VOID
    | SHORT
    | DOUBLE
;

Declarator
    : IDENTIFIER
    | IDENTIFIER ASSIGN Expr2
;

StatementGlobal
    : error SEMI /* 2nd error */                    {$$ = NULL;}
    | Statement
;

Statement
    : SEMI
    | Expr2 SEMI
    | LBRACE RBRACE
    | LBRACE Statements RBRACE
    | IF LPAR Expr2 RPAR StatementGlobal ELSE StatementGlobal
    | IF LPAR Expr2 RPAR StatementGlobal
    | WHILE LPAR Expr2 RPAR StatementGlobal
    | RETURN SEMI
    | RETURN Expr2 SEMI
    | LBRACE error RBRACE /* 3rd error */           {$$ = NULL;}
;

Statements
    : StatementGlobal
    | Statements StatementGlobal
;

ExprOpt
    :
    Expr2
    |               {$$ = NULL;}
;


Expr
    : 
    Expr ASSIGN Expr
    |Expr PLUS Expr
    |Expr MINUS Expr
    |Expr MUL Expr
    |Expr DIV Expr
    |Expr MOD Expr
    |Expr OR Expr
    |Expr AND Expr
    |Expr BITWISEAND Expr
    |Expr BITWISEOR Expr
    |Expr BITWISEXOR Expr
    |Expr EQ Expr
    |Expr NE Expr
    |Expr GE Expr
    |Expr LT Expr
    |Expr LE Expr
    |Expr GT Expr
    |PLUS Expr
    |MINUS Expr
    |NOT Expr
    |IDENTIFIER LPAR ExprOpt RPAR
    |IDENTIFIER
    |NATURAL
    |CHRLIT
    |DECIMAL
    |LPAR Expr2 RPAR
    |IDENTIFIER LPAR error RPAR            {$$ = NULL;}
    |LPAR error RPAR                       {$$ = NULL;}
;
    

Expr2: Expr2 COMMA Expr 
     | Expr
;


%%

/* -------------------------------------------- SUBROUTINES SECTION -------------------------------------------- */