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
%left OR
%left AND
%left BITWISEOR
%left BITWISEAND
%left BITWISEXOR
%left EQ NE
%left GT LT GE LE //Mudei isto para cima até ao OR
%left PLUS MINUS
%left MUL DIV MOD
%right NOT
%left LPAR RPAR
%right ELSE
%nonassoc RESERVED


%type <node> Program FunctionsAndDeclarations TypeFuncDec FunctionDefinition FunctionBody FunctionBody2 DeclarationsAndStatements
%type <node> FunctionDeclaration FunctionDeclarator ParameterList ParameterListAux ParameterDeclaration ParameterDeclaration2 Declaration DeclaratorList
%type <node> TypeSpec Declarator StatementEsp Statement Statements ExprOp Expr Expr2 Expr3

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
                                                        if($1!=NULL)addchild($$, $1); }
;

FunctionsAndDeclarations:TypeFuncDec                { $$ = $1;}      
    |FunctionsAndDeclarations TypeFuncDec	        { $$ = $1;
                                                      if($1!=NULL)addbrother($1, $2);}
;

TypeFuncDec:FunctionDefinition                        {$$=$1;}
    |FunctionDeclaration                              {$$=$1;}
    |Declaration                                      {$$=$1;}
;

FunctionDefinition:TypeSpec FunctionDeclarator FunctionBody{ $$= newnode(FuncDefinition, NULL);
                                                            addchild($$, $1);
                                                            if($2!=NULL)addchild($$, $2);
                                                            if($3!=NULL)addchild($$, $3);}

    |TypeSpec FunctionDeclarator error                      {$$=NULL;}             
;

FunctionBody:LBRACE FunctionBody2 RBRACE            { $$= newnode(FuncBody, NULL);
                                                      if($2!=NULL)addchild($$, $2);} 
;

FunctionBody2:DeclarationsAndStatements             { $$ = $1;} 
    |                                               { $$=NULL;} 
;

DeclarationsAndStatements:Statement DeclarationsAndStatements   {if ($1 != NULL){
                                                                    $$ = $1;
                                                                    if ($2 != NULL){
                                                                        addbrother($1, $2);
                                                                        }
                                                                    }
                                                                    else {
                                                                        $$ = $2;  // Se $1 for NULL, então $$ recebe $2 diretamente
                                                                    }
                                                                }
    |Declaration DeclarationsAndStatements                      {if ($1 != NULL){
                                                                    $$ = $1;
                                                                        if ($2 != NULL){
                                                                        addbrother($1, $2);
                                                                        }
                                                                    }
                                                                    else {
                                                                        $$ = $2;  // Se $1 for NULL, então $$ recebe $2 diretamente
                                                                    }
                                                                }
    |Statement                                        {if($1!=NULL)$$=$1;}
    |Declaration                                      {if($1!=NULL)$$=$1;}
;

FunctionDeclaration:TypeSpec FunctionDeclarator SEMI { $$= newnode(FuncDeclaration, NULL);
                                                      addchild($$, $1);
                                                      if($2!=NULL)addchild($$, $2);
                                                      }
;

FunctionDeclarator:IDENTIFIER LPAR ParameterList RPAR { $$=newnode(Identifier,$1); addbrother($$,$3); LOCATE($$, @1.first_line, @1.first_column);} 
;

ParameterList:ParameterDeclaration ParameterListAux { $$= newnode(ParamList, NULL);
                                                      addchild($$, $1);
                                                      if($2!=NULL){addchild($$, $2);}}
;

ParameterListAux:ParameterListAux COMMA ParameterDeclaration { if($1!=NULL){$$=$1;addbrother($$,$3);}else{$$=$3;} } //?
                |                                            { $$=NULL;} //!?
;


ParameterDeclaration:TypeSpec ParameterDeclaration2 { $$ = newnode(ParamDeclaration, NULL);
                                                      addchild($$, $1);
                                                      if($2!=NULL){addchild($$, $2);}}
;

ParameterDeclaration2:IDENTIFIER                    { $$ = newnode(Identifier, $1); LOCATE($$, @1.first_line, @1.first_column);} //?
    |                                               { $$=NULL;} //!?
;


Declaration:TypeSpec Declarator DeclaratorList SEMI { $$=newnode(Declaration,NULL); if($1!=NULL)addchild($$,$1); if($2!=NULL)addchild($$,$2);if($3!=NULL){addbrother($$,$3);}}
    |error SEMI                                     { $$=NULL;}
;

DeclaratorList:DeclaratorList COMMA Declarator      {if ($1 != NULL) {
                                                            $$ = $1;
                                                            struct node *new_declaration = newnode(Declaration, NULL);
                                                            addchild(new_declaration, noaux); // Adiciona o tipo (TypeSpec)
                                                            addbrother($1, new_declaration); // Adiciona a nova Declaração como irmão do último Declarator
                                                            addchild(new_declaration, $3); // Adiciona o Declarator atual à nova Declaração
                                                        } else {
                                                            $$ = newnode(Declaration, NULL);
                                                            addchild($$, noaux); // Adiciona o tipo (TypeSpec)
                                                            if($3!=NULL)addchild($$, $3); // Adiciona o Declarator
                                                        }
										            }
    |                                               { $$=NULL;}
;

TypeSpec:CHAR                                       { $$=noaux=newnode(Char, NULL); LOCATE($$, @1.first_line, @1.first_column);}                        
    |INT                                            { $$=noaux=newnode(Int, NULL); LOCATE($$, @1.first_line, @1.first_column);} 
    |VOID                                           { $$=noaux=newnode(Void, NULL); LOCATE($$, @1.first_line, @1.first_column);} 
    |SHORT                                          { $$=noaux=newnode(Short, NULL); LOCATE($$, @1.first_line, @1.first_column);} 
    |DOUBLE                                         { $$=noaux=newnode(Double, NULL); LOCATE($$, @1.first_line, @1.first_column);} 
;

Declarator:IDENTIFIER                               { $$=newnode(Identifier, $1); LOCATE($$, @1.first_line, @1.first_column);} //?
    |IDENTIFIER ASSIGN Expr2                        { $$=newnode(Identifier,$1); addbrother($$,$3); LOCATE($$, @1.first_line, @1.first_column);} //?
;

StatementEsp:error SEMI             {$$=NULL;}
    |Statement                      {$$=$1;}
;

Statement:SEMI                                          {$$ = NULL;}
    |Expr3 SEMI                                         {$$ = $1;}
    |LBRACE RBRACE                                      {$$ = NULL;}
    |LBRACE Statements RBRACE                           {$$ = $2;}
    |IF LPAR Expr3 RPAR StatementEsp ELSE StatementEsp  {$$=newnode(If,NULL); LOCATE($$, @1.first_line, @1.first_column); if ($3 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$3);}if ($5 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$5);};if ($7 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$7);}}
    |IF LPAR Expr3 RPAR StatementEsp                    {$$=newnode(If,NULL); LOCATE($$, @1.first_line, @1.first_column); if ($3 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$3);}if ($5 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$5);};addchild($$, newnode(Null,NULL));}
    |WHILE LPAR Expr3 RPAR StatementEsp                 {$$=newnode(While,NULL); LOCATE($$, @1.first_line, @1.first_column); if ($3 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$3);}if ($5 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$5);};}
    |RETURN SEMI                                        {$$=newnode(Return,NULL);LOCATE($$, @1.first_line, @1.first_column); addchild($$, newnode(Null, NULL));}      
    |RETURN Expr3 SEMI                                  {$$=newnode(Return,NULL); LOCATE($$, @1.first_line, @1.first_column); if ($2 == NULL) {addchild($$, newnode(Null,NULL));}else{addchild($$,$2);}}
    |LBRACE error RBRACE                                {$$ = NULL;}
;

Statements:StatementEsp                   {$$ = $1;}
        |Statements StatementEsp        {if($1!=NULL){
                                                if ($1->category != StatList && countbrothers($1) == 0) {
                                                    struct node *new_statlist = newnode(StatList, NULL);
                                                    addchild(new_statlist, $1);
                                                    if($2!=NULL)addchild(new_statlist, $2);
                                                    $$ = new_statlist;
                                                } else {
                                                    // Adicionar como filho diretamente
                                                    if($2!=NULL)addchild($1, $2);
                                                    $$ = $1;
                                                }
                                            }
                                            else{
                                                $$=$2;
                                            }
                                        }

;

ExprOp:Expr2                        { $$ = $1; }
    |                               {$$=NULL;} 
;


Expr:Expr ASSIGN Expr               { $$ = newnode(Store, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr PLUS Expr                 { $$ = newnode(Add, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr MINUS Expr                { $$ = newnode(Sub, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr MUL Expr                  { $$ = newnode(Mul, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr DIV Expr                  { $$ = newnode(Div, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr MOD Expr                  { $$ = newnode(Mod, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}                  
    |Expr OR Expr                   { $$ = newnode(Or, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr AND Expr                  { $$ = newnode(And, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr BITWISEAND Expr           { $$ = newnode(BitWiseAnd, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr BITWISEOR Expr            { $$ = newnode(BitWiseOr, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr BITWISEXOR Expr           { $$ = newnode(BitWiseXor, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr EQ Expr                   { $$ = newnode(Eq, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr NE Expr                   { $$ = newnode(Ne, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr GE Expr                   { $$ = newnode(Ge, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr LT Expr                   { $$ = newnode(Lt, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr LE Expr                   { $$ = newnode(Le, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |Expr GT Expr                   { $$ = newnode(Gt, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);}
    |PLUS Expr  %prec NOT           { $$ = newnode(Plus, NULL);
                                      addchild($$, $2); LOCATE(getchild($$,0), @1.first_line, @1.first_column);} //?
    |MINUS Expr %prec NOT           { $$ = newnode(Minus, NULL);
                                      addchild($$, $2); LOCATE(getchild($$,0), @1.first_line, @1.first_column);} //?
    |NOT Expr   %prec NOT           { $$ = newnode(Not, NULL);
                                      addchild($$, $2); LOCATE(getchild($$,0), @1.first_line, @1.first_column);} //?
    |IDENTIFIER LPAR ExprOp RPAR    { $$ = newnode(Call, NULL);
                                      addchild($$, newnode(Identifier, $1));
                                      if($3!=NULL)addchild($$, $3); LOCATE(getchild($$,0), @1.first_line, @1.first_column);} //?
    |IDENTIFIER                     { $$ = newnode(Identifier, $1);LOCATE($$, @1.first_line, @1.first_column);}
    |NATURAL                        { $$ = newnode(Natural, $1);LOCATE($$, @1.first_line, @1.first_column);}
    |CHRLIT                         { $$ = newnode(ChrLit, $1);LOCATE($$, @1.first_line, @1.first_column);}
    |DECIMAL                        { $$ = newnode(Decimal, $1);LOCATE($$, @1.first_line, @1.first_column);}
    |LPAR Expr3 RPAR                { $$ = $2;}
    |IDENTIFIER LPAR error RPAR     { $$=NULL;}
    |LPAR error RPAR                { $$=NULL;}
;
    

Expr2: Expr2 COMMA Expr {$$ = $1; addbrother($1,$3);}
    | Expr {$$=$1;}
;

Expr3: Expr3 COMMA Expr {$$=newnode(Comma,NULL);addchild($$,$1);if($3!=NULL)addchild($$,$3);LOCATE($$, @1.first_line, @1.first_column);}
    | Expr  {$$=$1;}
;


%%



