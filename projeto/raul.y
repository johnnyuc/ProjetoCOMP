%{
#include <stdio.h>
extern int yylex(void);
void yyerror(char *);
extern char *yytext;
extern int linha;
extern int coluna;
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
%token CHRLIT
%token IDENTIFIER
%token NATURAL
%token DECIMAL
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


%left COMMA
%right ASSIGN
%left OR AND BITWISEOR BITWISEAND BITWISEXOR
%left EQ NE LE GE LT GT
%left PLUS MINUS
%left MUL DIV MOD
%right NOT
%left LPAR RPAR
%nonassoc ELSE
%nonassoc IF


%%
Program:FunctionsAndDeclarations
    |
    ;

FunctionsAndDeclarations: FunctionDefiniton
    |FunctionDeclaration
    |Declaration
    |FunctionsAndDeclarations FunctionDefiniton
    |FunctionsAndDeclarations FunctionDeclaration
    |FunctionsAndDeclarations Declaration
    ;

FunctionDefiniton: TypeSpec FunctionDeclarator FunctionBody
    ;

FunctionBody: LBRACE FunctionBody2 RBRACE
    ;
FunctionBody2: DeclarationsAndStatements
    |
    ;

DeclarationsAndStatements: Statement DeclarationsAndStatements
    |Declaration DeclarationsAndStatements
    |Statement
    |Declaration
    ;

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI
    ;

FunctionDeclarator: IDENTIFIER LPAR ParameterList RPAR
    ;

ParameterList: ParameterDeclaration ParameterList2
    ;

ParameterList2: ParameterList2 COMMA ParameterDeclaration
              |
              ;

ParameterDeclaration: TypeSpec ParameterDeclaration2
                    ;

ParameterDeclaration2: IDENTIFIER
                     |
                     ;

Declaration:TypeSpec Declarator Declaration2 SEMI
    |error SEMI
    ;

Declaration2: Declaration2 COMMA Declarator
    |
    ;

TypeSpec: CHAR
    |INT
    |VOID
    |SHORT 
    |DOUBLE 
    ;

Declarator:IDENTIFIER 
    |IDENTIFIER ASSIGN Expr
    ;

StatementEsp: error SEMI
    | Statement
    ;

Statement:SEMI
    | Expr SEMI
    | LBRACE RBRACE
    | LBRACE Statements RBRACE
    | IF LPAR Expr RPAR StatementEsp StatementElse
    | WHILE LPAR Expr RPAR StatementEsp
    | RETURN SEMI
    | RETURN Expr SEMI
    | LBRACE error RBRACE
;

StatementElse:ELSE StatementEsp
    |
    ;
    
Statements: StatementEsp
    | Statements StatementEsp
;


Expr: Expr ASSIGN Expr
    |Expr COMMA Expr
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
    |IDENTIFIER LPAR Expr RPAR
    |IDENTIFIER LPAR Expr2 RPAR
    |IDENTIFIER
    |NATURAL
    |CHRLIT
    |DECIMAL
    |LPAR Expr RPAR
    |IDENTIFIER LPAR error RPAR
    |LPAR error RPAR
    |RESERVED error
    ;
    

Expr2: Expr2 COMMA Expr 
     |
     ;

%%

