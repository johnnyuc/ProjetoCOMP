%{
#include <stdio.h>
#include "shared.h"

extern int yylex(void);
void yyerror(char *s);
extern char *yytext;
%}

%left COMMA
%token LBRACE
%token RBRACE
%token SEMI
%token IDENTIFIER
%token LPAR
%token RPAR
%token CHAR
%token INT
%token VOID
%token SHORT
%token DOUBLE
%token ASSIGN
%token IF
%token ELSE
%token WHILE
%token RETURN

%%

FunctionsAndDeclarations:
        FunctionDefinition FunctionsAndDeclarations
    |FunctionDeclaration FunctionsAndDeclarations
    |Declaration FunctionsAndDeclarations
    |FunctionDefinition
    |FunctionDeclaration
    |Declaration
;

FunctionDefinition:
    TypeSpec FunctionDeclarator FunctionBody
;

FunctionBody:
        LBRACE DeclarationsAndStatements RBRACE
    |LBRACE RBRACE
;

DeclarationsAndStatements:
        Statement DeclarationsAndStatements
    |Declaration DeclarationsAndStatements
    |Statement
    |Declaration
;

FunctionDeclaration:
    TypeSpec FunctionDeclarator SEMI
;

FunctionDeclarator:
    IDENTIFIER LPAR ParameterList RPAR
;

ParameterList:
    ParameterDeclaration
    |ParameterList COMMA ParameterDeclaration
;

ParameterDeclaration:
    TypeSpec IDENTIFIER
    |TypeSpec
;

Declaration:
    TypeSpec DeclaratorList SEMI
    |error SEMI { yyerror("syntax error"); }
;

DeclaratorList:
    Declarator
  | DeclaratorList COMMA Declarator
;

TypeSpec:
    CHAR
  |INT
  |VOID
  |SHORT
  |DOUBLE
;

Declarator:
    IDENTIFIER
    |IDENTIFIER ASSIGN Expr
;

Statement:
    Expr SEMI
    | LBRACE Statements RBRACE
    | LBRACE RBRACE
    | IF LPAR Expr RPAR Statement
    | IF LPAR Expr RPAR Statement ELSE Statement
    | WHILE LPAR Expr RPAR Statement
    | RETURN SEMI
    | RETURN Expr SEMI
    | SEMI
    | error SEMI { yyerror("syntax error"); }
    | LBRACE error RBRACE { yyerror("syntax error"); }
;


Statements:
    Statement
    | Statements Statement
;

Expr:
    IDENTIFIER LPAR error RPAR { yyerror("syntax error"); }
    |LPAR error RPAR { yyerror("syntax error"); }
;

%%

void yyerror (char *s) {
    printf ("Line %d, column %d :%s :%s\n",line,col,s,yytext);
}
