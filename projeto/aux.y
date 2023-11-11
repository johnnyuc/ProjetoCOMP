%{
#include <stdio.h>
#include "ast.h"
extern int yylex(void);
void yyerror(char *);
extern char *yytext;
extern int linha;
extern int coluna;

struct node *program;
struct node *noaux;
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




%%
Program:FunctionsAndDeclarations                      { $$ = program= newnode(Program, NULL);
                                                        addchild($$, $1); }
;

FunctionsAndDeclarations:TypeFuncDec                { $$ = $1;}      
    |FunctionsAndDeclarations TypeFuncDec	        { $$ = $1;
                                                      addbrother($1, $2);}
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
                                                      addchild($$, $2);} 
;

FunctionBody2:DeclarationsAndStatements             { $$ = $1;} 
    |                                               { $$=NULL;} 
;

DeclarationsAndStatements:Statement DeclarationsAndStatements   {$$=$1; addbrother($1,$2);}
    |Declaration DeclarationsAndStatements            {$$=$1; addbrother($1,$2);}
    |Statement                                        {$$=$1;}
    |Declaration                                      {$$=$1;}
;

FunctionDeclaration:TypeSpec FunctionDeclarator SEMI { $$= newnode(FuncDeclaration, NULL);
                                                      addchild($$, $1);
                                                      addchild($$, $2);
                                                      }
;

FunctionDeclarator:IDENTIFIER LPAR ParameterList RPAR { $$=newnode(Identifier,$1); addbrother($$,$3);} 
;

ParameterList:ParameterDeclaration ParameterListAux { $$= newnode(ParamList, NULL);
                                                      addchild($$, $1);
                                                      if($2!=NULL){addchild($$, $2);}else{addchild($$,newnode(Null,NULL));}}
;

ParameterListAux:ParameterListAux COMMA ParameterDeclaration { if($1!=NULL){$$=$1;addbrother($$,$3);}else{$$=$3;} } //?
                |                                            { $$=NULL;} //!?
;


ParameterDeclaration:TypeSpec ParameterDeclaration2 { $$ = newnode(ParamDeclaration, NULL);
                                                      addchild($$, $1);
                                                      if($2!=NULL){addchild($$, $2);}else{addchild($$,newnode(Null,NULL));}}
;

ParameterDeclaration2:IDENTIFIER                    { $$ = newnode(Identifier, $1); } //?
    |                                               { $$=NULL;} //!?
;


Declaration:TypeSpec Declarator DeclaratorList SEMI { $$=newnode(Declaration,NULL); addchild($$,$1);addchild($$,$2);if($3!=NULL){addbrother($$,$3);}else{addbrother($$,newnode(Null,NULL));}}
    |error SEMI                                     { $$=NULL;}
;

DeclaratorList:DeclaratorList COMMA Declarator      {if ($1 != NULL){
											$$ = $1;
                                             struct node *no1=newnode(Declaration,NULL);
                                            addchild(no1,newnode(Null,NULL));
                                            addbrother($1,noaux);
											addchild(no1,$3);
										} else {
											$$=newnode(Declaration,NULL);
                                            addchild($$,noaux);}
                                            addchild($$,$3);
										}
    |                                               { $$=NULL;}
;

TypeSpec:CHAR                                       { $$=noaux=newnode(Char, NULL);}                        
    |INT                                            { $$=noaux=newnode(Int, NULL);} 
    |VOID                                           { $$=noaux=newnode(Void, NULL);} 
    |SHORT                                          { $$=noaux=newnode(Short, NULL);} 
    |DOUBLE                                         { $$=noaux=newnode(Double, NULL);} 
;

Declarator:IDENTIFIER                               { $$=newnode(Identifier, $1);} //?
    |IDENTIFIER ASSIGN Expr2                        { $$=newnode(Identifier,$1); addbrother($$,$3);} //?
;

StatementEsp:error SEMI             {$$=NULL;}
    |Statement                      {$$=$1;}
;

Statement
    :SEMI                                          {$$=NULL;}
    |Expr2 SEMI                                         {$$ = $1;}
    |LBRACE RBRACE                                      {$$=NULL;}
    |LBRACE Statements RBRACE                           {if ($2 != NULL){if(countchildren($2)>=2){$$ = newnode(StatList, NULL);addchild($$,$2);}else{addchild($$,$2);}}else{$$=newnode(Null,NULL);}} //Problema com {;}
    |IF LPAR Expr2 RPAR StatementEsp ELSE StatementEsp  {$$=newnode(If,NULL);if ($3 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$3);}if ($5 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$5);};if ($7 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$7);}}
    |IF LPAR Expr2 RPAR StatementEsp                    {$$=newnode(If,NULL);if ($3 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$3);}if ($5 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$5);};}
    |WHILE LPAR Expr2 RPAR StatementEsp                 {$$=newnode(While,NULL);if ($3 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$3);}if ($5 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$5);};}
    |RETURN SEMI                                        {$$=newnode(Return,NULL);}      
    |RETURN Expr2 SEMI                                  {$$=newnode(Return,NULL);if ($2 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$2);}}
    |LBRACE error RBRACE                                {$$=NULL;}
;

Statements:StatementEsp             { $$ = $1;} //?
    |Statements StatementEsp        { if($1 !=NULL){$$=$1;addbrother($$, $2);}else{$$=$2;}} //?
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
    |PLUS Expr  %prec NOT                    { $$ = newnode(Add, NULL);
                                      addchild($$, $2); } //?
    |MINUS Expr %prec NOT                    { $$ = newnode(Minus, NULL);
                                      addchild($$, $2); } //?
    |NOT Expr   %prec NOT                    { $$ = newnode(Not, NULL);
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
    

Expr2:Expr2 COMMA Expr              {$$ = $1; addbrother($1,$3);}
     |Expr                          {$$ = $1;}
;

%%

