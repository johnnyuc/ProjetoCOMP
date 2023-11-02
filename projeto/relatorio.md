## Relatório

# FunctionsandDeclarations:

# Statement:
0. Foram feita correções para tratar do "if-else pendente" (dangling else)
1. Introdução de StatementNoIf para tratar instruções que não são seguidas por um ELSE.
2. Criação de IfStatement para lidar especificamente com instruções IF e resolver o problema do "if-else pendente".
3. Modificação das produções IF e WHILE em Statement para usar as novas produções e evitar ambiguidades.
4. Obs: O operador "%prec IF" foi usado para garantir que o ELSE se associe ao IF mais interno em situações ambíguas, resolvendo o problema do "if-else pendente". Em resumo a regra passou a ter a mesma precedência do IF.
6. Statement inicial com conflitos:
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

# Dúvidas:
0. Falar com o Fred sobre o %prec IF


Expr:
    Expr ASSIGN Expr
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
    | IDENTIFIER LPAR ExprList RPAR
    | IDENTIFIER
    | NATURAL
    | CHRLIT
    | DECIMAL
    | LPAR Expr RPAR
;

ExprList:
    Expr
    | ExprList COMMA Expr
;









