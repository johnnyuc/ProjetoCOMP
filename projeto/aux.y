Statement
    : MatchedStmt
    | UnmatchedStmt
    ;

MatchedStmt
    : IF LPAR Expr RPAR MatchedStmt ELSE MatchedStmt
    | OtherStmt
    ;

UnmatchedStmt
    : IF LPAR Expr RPAR StatementGlobal ELSE UnmatchedStmt
    | IF LPAR Expr RPAR MatchedStmt     // Eliminar a opção de ter 'ELSE MatchedStmt' aqui
;

OtherStmt
    : ExprOpt SEMI
    | LBRACE RBRACE
    | LBRACE Statements RBRACE
    | WHILE LPAR Expr RPAR StatementGlobal
    | RETURN ExprOpt SEMI
    | LBRACE error RBRACE /* 3rd error */
;

Statements
    : StatementGlobal
    | Statements StatementGlobal
;

ExprOpt
    :
    Expr
    |;
;