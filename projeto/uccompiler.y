/* PROJETO META 2 */
/* Johnny Fernandes 2021190668 */
/* Stefanos Lucena  2021115090 TESTE COMMIT*/

/* -------------------------------------------- DEFINITIONS SECTION -------------------------------------------- */
%{
    #include <stdio.h> 
    #include "ast.h"

    int yylex(void);
    void yyerror();

    struct node *program; // Root node
    struct node *no_aux; // Auxiliary node
%}

/* Tokens */
%token CHAR INT VOID SHORT DOUBLE IF ELSE WHILE RETURN BITWISEAND BITWISEOR BITWISEXOR AND ASSIGN MUL COMMA DIV EQ GE GT LBRACE LE LPAR LT MINUS MOD NE NOT OR PLUS RBRACE RPAR SEMI RESERVED


/* Associativity and precedence */
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

/* Retrieve terminal values */
%token <lexeme> IDENTIFIER
%token <lexeme> NATURAL
%token <lexeme> DECIMAL
%token <lexeme> CHRLIT

/* Nodes */
%type <node> Program
%type <node> FunctionsAndDeclarations
%type <node> TypeFuncDec
%type <node> FunctionDefinition
%type <node> FunctionBody
%type <node> FunctionBodyOpt
%type <node> ExprComma
%type <node> DeclarationsAndStatements
%type <node> FunctionDeclaration
%type <node> FunctionDeclarator
%type <node> ParameterList
%type <node> ParameterListAux
%type <node> ParameterDeclaration
%type <node> ParameterDeclarationOpt
%type <node> Declaration
%type <node> DeclaratorList
%type <node> TypeSpec
%type <node> Declarator
%type <node> StatementGlobal
%type <node> Statement
%type <node> Statements
%type <node> ExprOpt
%type <node> Expr
%type <node> ExprAux

/* Union */
%union{
    char *lexeme;
    struct node *node;
}

/* -------------------------------------------- RULES SECTION -------------------------------------------- */
%%
Program
    : FunctionsAndDeclarations {
            $$ = program = newnode(Program, NULL);
            addchild($$, $1);
        }
;

FunctionsAndDeclarations
    : TypeFuncDec {
            $$ = $1;
        }
    | FunctionsAndDeclarations TypeFuncDec {
            $$ = $1;
            addbrother($1, $2);
        }
;

TypeFuncDec
    : FunctionDefinition {
            $$ = $1;
        }
    | FunctionDeclaration {
            $$ = $1;
        }
    | Declaration {
            $$ = $1;
        }
;

FunctionDefinition
    : TypeSpec FunctionDeclarator FunctionBody {
            $$= newnode(FuncDefinition, NULL);
            addchild($$, $1);
            addchild($$, $2);
            addchild($$, $3);
        }
    | TypeSpec FunctionDeclarator error {
            $$ = newnode(Null, NULL);
        }
;

FunctionBody
    : LBRACE FunctionBodyOpt RBRACE {
            $$ = newnode(FuncBody, NULL);
            addchild($$, $2);
        }

FunctionBodyOpt
    : DeclarationsAndStatements {
            $$ = $1;
        }
    | {
            $$ = NULL; // Empty function body
        }
;

DeclarationsAndStatements
    : Statement DeclarationsAndStatements {
            if($1 != NULL) {
                $$ = $1;
                if($2 != NULL) {
                    addbrother($1, $2);
                }
            } else {
                $$ = $2;
            }
        }
    | Declaration DeclarationsAndStatements {
            $$ = $1; 
            addbrother($1, $2);
        }
    | Statement {
            $$ = $1;
        }
    | Declaration {
            $$ = $1;
        }
;

FunctionDeclaration
    : TypeSpec FunctionDeclarator SEMI {
            $$ = newnode(FuncDeclaration, NULL);
            addchild($$, $1);
            addchild($$, $2);
        }
;

FunctionDeclarator
    : IDENTIFIER LPAR ParameterList RPAR {
            $$ = newnode(Identifier, $1);
            addbrother($$, $3);
        }
;

ParameterList
    : ParameterDeclaration ParameterListAux {
            $$ = newnode(ParamList, NULL);
            addchild($$, $1);
            if($2 != NULL) {
                addchild($$, $2);
            }
        }
;

ParameterListAux
    : ParameterListAux COMMA ParameterDeclaration {
            if($1 != NULL) {
                $$ = $1;
                addbrother($$, $3);
            } else {
                $$ = $3;
            }
        }
    | {
            $$ = NULL;
        }
;

ParameterDeclaration
    : TypeSpec ParameterDeclarationOpt {
            $$ = newnode(ParamDeclaration, NULL);
            addchild($$, $1);
            if($2 != NULL) {
                addchild($$, $2);
            }
        }
;

ParameterDeclarationOpt
    : IDENTIFIER {
            $$ = newnode(Identifier, $1);
        }
    | {
            $$ = NULL;
        }
;

Declaration
    : TypeSpec Declarator DeclaratorList SEMI {
            $$ = newnode(Declaration, NULL);
            addchild($$, $1);
            addchild($$, $2);
            if($3 != NULL) {
                addbrother($$, $3);
            }
        }
    | error SEMI {
            $$ = newnode(Null, NULL);  // 1st error
        }
;

DeclaratorList
    : DeclaratorList COMMA Declarator {
        if ($1 == NULL) {
            $$ = newnode(Declaration, NULL);
            addchild($$, no_aux);
            addchild($$, $3);
        } else {
            $$ = $1;
            struct node *newDeclarationNode = newnode(Declaration, NULL);
            addchild(newDeclarationNode, no_aux);
            addbrother($1, newDeclarationNode);
            addchild(newDeclarationNode, $3);
            }
        }
    | {
            $$ = NULL;
        }
;

TypeSpec
    : CHAR {
            $$ = no_aux = newnode(Char, NULL);
        }
    | INT {
            $$ = no_aux = newnode(Int, NULL);
        }
    | VOID {
            $$ = no_aux = newnode(Void, NULL);
        }
    | SHORT {
            $$ = no_aux = newnode(Short, NULL);
        }
    | DOUBLE {
            $$ = no_aux = newnode(Double, NULL);
        }
;

Declarator
    : IDENTIFIER {
            $$ = newnode(Identifier, $1);
        }
    | IDENTIFIER ASSIGN ExprAux {
            $$ = newnode(Identifier, $1);
            addbrother($$, $3);
        }
;

StatementGlobal
    : error SEMI {
            $$ = newnode(Null, NULL); // 2nd error
        }
    | Statement {
            $$ = $1;
        }
;

Statement
    : SEMI {
            $$ = NULL;
        }
    | ExprComma SEMI {
            $$ = $1;
        }
    | LBRACE RBRACE {
            $$ = NULL;
        }
    | LBRACE Statements RBRACE {
            $$ = $2;
        }
    | IF LPAR ExprComma RPAR StatementGlobal ELSE StatementGlobal {
            $$ = newnode(If, NULL);

            if($3 == NULL) {
                addchild($$, newnode(Null, NULL));
            } else {
                addchild($$, $3);
            }
            
            if($5 == NULL) {
                addchild($$, newnode(Null, NULL));
            } else {
                addchild($$, $5);
            }

            if($7 == NULL) {
                addchild($$, newnode(Null, NULL));
            } else {
                addchild($$, $7);
            }
        }
    | IF LPAR ExprComma RPAR StatementGlobal {
            $$ = newnode(If, NULL);

            if($3 == NULL) {
                addchild($$, newnode(Null, NULL));
            } else {
                addchild($$, $3);
            }

            if($5 == NULL) {
                addchild($$, newnode(Null, NULL));
            } else {
                addchild($$, $5);
            }

            addchild($$, newnode(Null, NULL)); // Else is null
        }
    | WHILE LPAR ExprComma RPAR StatementGlobal {
            $$ = newnode(While, NULL); 
            
            if($3 == NULL) {
                addchild($$, newnode(Null, NULL));
            } else {
                addchild($$, $3);
            } 
            
            if($5 == NULL) {
                addchild($$, newnode(Null, NULL));
            } else {
                addchild($$, $5);
            } 
        }
    | RETURN SEMI {
            $$ = newnode(Return, NULL); 
            addchild($$, newnode(Null, NULL)); 
        }
    | RETURN ExprComma SEMI {
            $$ = newnode(Return, NULL); 
            addchild($$, $2);
        }
    | LBRACE error RBRACE {
            $$ = newnode(Null, NULL); // 3rd error
        }
;

Statements
    : StatementGlobal { 
            $$ = $1; 
        }
    | Statements StatementGlobal {
        if ($1 == NULL) {
            $$ = $2;
        } else {
            if ($1->category == StatList || countbrother($1) != 0) {
                addchild($1, $2);
                $$ = $1;
            } else {
                struct node *newStatementList = newnode(StatList, NULL);
                addchild(newStatementList, $1);
                addbrother($1, $2);
                $$ = newStatementList;
            }
        }
    }   
;

ExprOpt
    : ExprAux {
            $$ = $1; 
        }
    | { 
            $$ = NULL; 
        }
;

Expr
    : Expr ASSIGN Expr {
        $$ = newnode(Store, NULL);
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr PLUS Expr {
        $$ = newnode(Add, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr MINUS Expr {
        $$ = newnode(Sub, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr MUL Expr {
        $$ = newnode(Mul, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr DIV Expr {
        $$ = newnode(Div, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr MOD Expr {
        $$ = newnode(Mod, NULL);
        addchild($$, $1);
        addchild($$, $3); 
        }
    | Expr OR Expr {
        $$ = newnode(Or, NULL);
        addchild($$, $1);
        addchild($$, $3); 
        }
    | Expr AND Expr {
        $$ = newnode(And, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr BITWISEAND Expr {
        $$ = newnode(BitWiseAnd, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr BITWISEOR  Expr {
        $$ = newnode(BitWiseOr, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr BITWISEXOR Expr {
        $$ = newnode(BitWiseXor, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr EQ Expr {
        $$ = newnode(Eq, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr NE Expr {
        $$ = newnode(Ne, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr GE Expr {
        $$ = newnode(Ge, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr LT Expr {
        $$ = newnode(Lt, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr LE Expr {
        $$ = newnode(Le, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | Expr GT Expr {
        $$ = newnode(Gt, NULL); 
        addchild($$, $1); 
        addchild($$, $3); 
        }
    | PLUS Expr %prec NOT  {
        $$ = newnode(Plus, NULL); 
        addchild($$, $2); 
        }
    | MINUS Expr %prec NOT {
        $$ = newnode(Minus, NULL); 
        addchild($$, $2); 
        }
    | NOT Expr {
        $$ = newnode(Not, NULL); 
        addchild($$, $2); 
        }
    | IDENTIFIER LPAR ExprOpt RPAR {
        $$ = newnode(Call, NULL); 
        addchild($$, newnode(Identifier, $1)); 
        addchild($$, $3); 
        }
    | IDENTIFIER {
        $$ = newnode(Identifier,$1); 
        }
    | NATURAL {
        $$ = newnode(Natural, $1); 
        }
    | CHRLIT {
        $$ = newnode(ChrLit, $1);
        }
    | DECIMAL {
        $$ = newnode(Decimal, $1);
        }
    | LPAR ExprComma RPAR {
        $$ = $2;
        }
    | IDENTIFIER LPAR error RPAR {
        $$ = newnode(Null, NULL);
        }
    | LPAR error RPAR {
        $$ = newnode(Null, NULL);
        }
;

ExprAux
    : ExprAux COMMA Expr {
            $$ = $1; 
            addbrother($1, $3); 
        }
    | Expr {
            $$ = $1;
        }
;

ExprComma 
    : ExprComma COMMA Expr {
            $$ = newnode(Comma, NULL); 
            addchild($$, $1); 
            addchild($$, $3);
        }
    | Expr {
            $$ = $1;
        }

%%

/* -------------------------------------------- SUBROUTINES SECTION -------------------------------------------- */
