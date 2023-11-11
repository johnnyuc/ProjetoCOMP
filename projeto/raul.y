%{
#include <stdio.h>
#include "ast.h"
extern int yylex(void);
void yyerror(char *);
extern char *yytext;
extern int linha;
extern int coluna;

struct node *program;
%}

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
%left OR AND BITWISEOR BITWISEAND BITWISEXOR
%left EQ NE LE GE LT GT
%right PLUS MINUS
%left MUL DIV MOD
%right NOT
%left LPAR RPAR
%right ELSE
%nonassoc RESERVED


%type <node> Program FunctionsAndDeclarations TypeFuncDec FunctionDefinition FunctionBody FunctionBody2 DeclarationsAndStatements
%type <node> FunctionDeclaration FunctionDeclarator ParameterList ParameterListAux ParameterDeclaration ParameterDeclaration2 Declaration DeclaratorList
%type <node> TypeSpec Declarator StatementEsp Statement Statements ExprOp Expr Expr2

%union{
    char *lexeme;
    struct node *node;
}

%locations
%{
#define LOCATE(node, line, column) { node->token_line = line; node->token_column = column; }
%}



%%
Program:FunctionsAndDeclarations                      { $$ = program= newnode(Program, NULL);
                                                        addchild($$, $1); }
;

FunctionsAndDeclarations:TypeFuncDec                { $$ = $1;}      
    |FunctionsAndDeclarations TypeFuncDec	        { $$ = $1;
                                                      addchild($$, $2);}
;

TypeFuncDec:FunctionDefinition                        {$$=$1;}
    |FunctionDeclaration                              {$$=$1;}
    |Declaration                                      {$$=$1;}
;

FunctionDefinition:TypeSpec FunctionDeclarator FunctionBody{ $$= newnode(FuncDefinition, NULL);
                                                            addchild($$, $1);
                                                            addchild($$, $2);
                                                            addchild($$, $3);}

    |TypeSpec FunctionDeclarator error                      {$$=NULL;}             
;

FunctionBody:LBRACE FunctionBody2 RBRACE            { $$= newnode(FuncBody, NULL);
                                                      addchild($$, $2);} //Nao sei 
;

FunctionBody2:DeclarationsAndStatements             { $$ = $1;} 
    |                                               { $$=NULL;} //!?
;

DeclarationsAndStatements:Statement DeclarationsAndStatements   {$$=$1;}
    |Declaration DeclarationsAndStatements            {$$=$1;}
    |Statement                                        {$$=$1;}
    |Declaration                                      {$$=$1;}
;

FunctionDeclaration:TypeSpec FunctionDeclarator SEMI { $$= newnode(FuncDeclaration, NULL);
                                                      addchild($$, $1);
                                                      addchild($$, $2);}
;

FunctionDeclarator:IDENTIFIER LPAR ParameterList RPAR { $$ = newnode(Call, NULL);
                                                        addchild($$, newnode(Identifier, $1));
                                                        addchild($$, $3); } //?
;

ParameterList:ParameterDeclaration ParameterListAux { $$= newnode(ParamList, NULL);
                                                      addchild($$, $1);
                                                      addchild($$, $2);}
;

ParameterListAux:ParameterListAux COMMA ParameterDeclaration { $$ = newnode(Comma, NULL);
                                                               addchild($$, $1);
                                                               addchild($$, $3); } //?
                |                                            { $$=NULL;} //!?
;


ParameterDeclaration:TypeSpec ParameterDeclaration2 { $$ = newnode(ParamDeclaration, NULL);
                                                      addchild($$, $1);
                                                      addchild($$, $2);}
;

ParameterDeclaration2:IDENTIFIER                    { $$ = newnode(Identifier, $1); } //?
    |                                               { $$=NULL;} //!?
;


Declaration:TypeSpec Declarator DeclaratorList SEMI { $$ = newnode(Declaration, NULL);
                                                      addchild($$, $1);
                                                      addchild($$, $2);
                                                      addchild($$, $3);}
    |error SEMI                                     { $$=NULL;}
;

DeclaratorList:DeclaratorList COMMA Declarator      { $$ = newnode(Comma, NULL);
                                                      addchild($$, $1);
                                                      addchild($$, $3); } //?
    |                                               { $$=NULL;}
;

TypeSpec:CHAR                                       { $$=newnode(Char, NULL);}                        
    |INT                                            { $$=newnode(Int, NULL);} 
    |VOID                                           { $$=newnode(Void, NULL);} 
    |SHORT                                          { $$=newnode(Short, NULL);} 
    |DOUBLE                                         { $$=newnode(Double, NULL);} 
;

Declarator:IDENTIFIER                               { $$=newnode(Identifier, $1);} //?
    |IDENTIFIER ASSIGN Expr2                        { $$=newnode(Store, NULL);
                                                      addchild($$, newnode(Identifier, $1));
                                                      addchild($$, $3);} //?
;

StatementEsp:error SEMI             {$$=NULL;}
    |Statement                      {$$=$1;}
;

Statement:SEMI                                          {}
    |Expr2 SEMI                                         { $$ = $1;}
    |LBRACE RBRACE                                      {}
    |LBRACE Statements RBRACE                           { $$ = $2;}
    |IF LPAR Expr2 RPAR StatementEsp ELSE StatementEsp  {}
    |IF LPAR Expr2 RPAR StatementEsp                    {}
    |WHILE LPAR Expr2 RPAR StatementEsp                 {}
    |RETURN SEMI                                        {}      
    |RETURN Expr2 SEMI                                  {}
    |LBRACE error RBRACE                                { $$=NULL;}
;

Statements:StatementEsp             { $$ = newnode(StatList, NULL);
                                      addchild($$, $1); } //?
    |Statements StatementEsp        { $$ = $1;
                                      addchild($$, $2); } //?
;

ExprOp:Expr2                        { $$ = $1; }
    |                               {$$=NULL;} //!?
;


Expr:Expr ASSIGN Expr               { $$ = newnode(Store, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr PLUS Expr                 { $$ = newnode(Add, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr MINUS Expr                { $$ = newnode(Sub, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr MUL Expr                  { $$ = newnode(Mul, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr DIV Expr                  { $$ = newnode(Div, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr MOD Expr                  { $$ = newnode(Mod, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }                  
    |Expr OR Expr                   { $$ = newnode(Or, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr AND Expr                  { $$ = newnode(And, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr BITWISEAND Expr           { $$ = newnode(BitWiseAnd, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr BITWISEOR Expr            { $$ = newnode(BitWiseOr, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr BITWISEXOR Expr           { $$ = newnode(BitWiseXor, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr EQ Expr                   { $$ = newnode(Eq, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr NE Expr                   { $$ = newnode(Ne, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr GE Expr                   { $$ = newnode(Ge, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr LT Expr                   { $$ = newnode(Lt, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr LE Expr                   { $$ = newnode(Le, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |Expr GT Expr                   { $$ = newnode(Gt, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    |PLUS Expr                      { $$ = newnode(Add, NULL);
                                      addchild($$, $2); } //?
    |MINUS Expr                     { $$ = newnode(Sub, NULL);
                                      addchild($$, $2); } //?
    |NOT Expr                       { $$ = newnode(Not, NULL);
                                      addchild($$, $2); } //?
    |IDENTIFIER LPAR ExprOp RPAR    { $$ = newnode(Call, NULL);
                                      addchild($$, newnode(Identifier, $1));
                                      addchild($$, $3); } //?
    |IDENTIFIER                     { $$ = newnode(Identifier, $1);}
    |NATURAL                        { $$ = newnode(Natural, $1);}
    |CHRLIT                         { $$ = newnode(ChrLit, $1);}
    |DECIMAL                        { $$ = newnode(Decimal, $1);}
    |LPAR Expr2 RPAR                { $$ = $2;}
    |IDENTIFIER LPAR error RPAR     { $$=NULL;}
    |LPAR error RPAR                { $$=NULL;}
;
    

Expr2:Expr2 COMMA Expr              { $$ = newnode(Comma, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); } //?
     |Expr                          { $$ = $1;}
;

%%

