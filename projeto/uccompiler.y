/* PROJETO META 2 */
/* Johnny Fernandes 2021190668 */
/* Stefanos Lucena  2021115090 */

/* -------------------------------------------- DEFINITIONS SECTION -------------------------------------------- */
%{
    #include <stdio.h>
    #include "ast.h"

    int yylex(void);
    void yyerror(char *s);
    extern char *yytext;
    struct node *program;
%}

/* Tokens */
%token LBRACE RBRACE SEMI IDENTIFIER LPAR RPAR CHAR INT VOID SHORT DOUBLE ASSIGN WHILE RETURN BITWISEAND BITWISEOR BITWISEXOR AND MUL COMMA DIV EQ GE GT LE LT MINUS MOD NE NOT OR PLUS DECIMAL NATURAL CHRLIT

/* Precedences */
%left COMMA
%right ASSIGN
%left OR AND BITWISEOR BITWISEAND BITWISEXOR
%left EQ NE LE GE LT GT
%left PLUS MINUS
%left MUL DIV MOD
%right NOT
%left LPAR RPAR

/* Non associatives */
%nonassoc ELSE
%nonassoc IF

/* -------------------------------------------- RULES SECTION -------------------------------------------- */
%%
Stm
    : FunctionsAndDeclarations
;

FunctionsAndDeclarations
    : TypeFuncDec
    | FunctionsAndDeclarations TypeFuncDec
;

TypeFuncDec
    : FunctionDefinition
    | FunctionDeclaration
    | Declaration
;

FunctionDefinition
    : TypeSpec FunctionDeclarator FunctionBody
;

FunctionBody
    : LBRACE DeclarationsAndStatements RBRACE
    | LBRACE RBRACE


DeclarationsAndStatements
    : Statement DeclarationsAndStatements
    | Declaration DeclarationsAndStatements
    | Statement
    | Declaration
    | error SEMI
;

FunctionDeclaration
    : TypeSpec FunctionDeclarator SEMI
;

FunctionDeclarator
    : IDENTIFIER LPAR ParameterList RPAR
;

ParameterList
    : ParameterDeclaration
    | ParameterList COMMA ParameterDeclaration
;

ParameterDeclaration
    : TypeSpec IDENTIFIER
    | TypeSpec
;

Declaration
    : TypeSpec Declarator SEMI
    | TypeSpec Declarator COMMA DeclaratorList SEMI
;

TypeSpec
    : CHAR
    | INT
    | VOID
    | SHORT
    | DOUBLE
;

DeclaratorList
    : Declarator
    | DeclaratorList COMMA Declarator
;

Declarator
    : IDENTIFIER
    | IDENTIFIER ASSIGN Expr
;

Statement
    : SEMI
    | Expr SEMI
    | LBRACE RBRACE
    | LBRACE Statements RBRACE
    | IF LPAR Expr RPAR Statement ELSE Statement
    | WHILE LPAR Expr RPAR Statement
    | RETURN SEMI
    | RETURN Expr SEMI
    | LBRACE error RBRACE /* 3rd error */
;

Statements
    : Statement
    | Statements Statement
;

Expr
    : Expr ASSIGN Expr
    | Expr PLUS Expr
    | Expr MINUS Expr
    | Expr MUL Expr
    | Expr DIV Expr
    | Expr MOD Expr
    | Expr OR Expr
    | Expr AND Expr
    | Expr BITWISEAND Expr
    | Expr BITWISEOR Expr
    | Expr BITWISEXOR Expr
    | Expr EQ Expr
    | Expr NE Expr
    | Expr LE Expr
    | Expr GE Expr
    | Expr LT Expr
    | Expr GT Expr
    | PLUS Expr
    | MINUS Expr
    | NOT Expr
    | IDENTIFIER LPAR RPAR
    | MultiExpr RPAR
    | IDENTIFIER
    | NATURAL
    | CHRLIT
    | DECIMAL
    | LPAR Expr RPAR
    | IDENTIFIER LPAR error RPAR /* 4th error */
    | LPAR error RPAR /* 5th error */
;

MultiExpr
    : MultiExpr COMMA Expr /* Can generate Expr COMMA Expr */
    | IDENTIFIER LPAR Expr
;
%%

/* -------------------------------------------- SUBROUTINES SECTION -------------------------------------------- */
