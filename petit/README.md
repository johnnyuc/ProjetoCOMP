# Petit

Petit programming language and compiler

## Example

Petit is an educational programming language for learning compilers. Here's an example:

    factorial(integer n) =
        if n then n * factorial(n-1) else 1

## Grammar

    program: IDENTIFIER '(' parameters ')' '=' expression
           | program IDENTIFIER '(' parameters ')' '=' expression

    parameters: parameter
              | parameters ',' parameter

    parameter: INTEGER IDENTIFIER
             | DOUBLE IDENTIFIER

    arguments: expression
             | arguments ',' expression

    expression: IDENTIFIER
              | NATURAL
              | DECIMAL
              | IDENTIFIER '(' arguments ')'
              | IF expression THEN expression ELSE expression
              | expression '+' expression
              | expression '-' expression
              | expression '*' expression
              | expression '/' expression
              | '(' expression ')'

## Tutorials and exercises

1. [Compilers tutorial I: Lexical analysis](/tutorial/p1_lexical_analysis.md)
2. [Compilers tutorial II: Advanced lex features](/tutorial/p2_advanced_lex.md)
3. [Compilers tutorial III: Syntactic analysis](/tutorial/p3_syntactic_analysis.md)
3. [Compilers tutorial IV: Abstract syntax](/tutorial/p4_abstract_syntax.md)
7. [Solutions for most exercises](/source/)
