## Relatório

# Program: 
Define a estrutura geral do programa. Cria um nó raiz para a árvore de sintaxe abstrata (AST) e adiciona FunctionsAndDeclarations como filho desse nó.

# FunctionsAndDeclarations: 
Permite uma sequência de definições e declarações de funções ou declarações de variáveis. Adiciona cada TypeFuncDec como irmão do anterior.

# TypeFuncDec: 
Agrupa FunctionDefinition, FunctionDeclaration e Declaration sob uma regra comum. A escolha entre essas regras é feita com base no contexto do código.

# FunctionDefinition:
 Representa a definição de uma função, composta por um tipo (TypeSpec), declarador de função (FunctionDeclarator) e corpo da função (FunctionBody). Em caso de erro na definição da função, um nó Null é criado.

# FunctionBody: 
Define o corpo de uma função, que é opcional (FunctionBodyOpt), delimitado por chaves.

# FunctionBodyOpt: 
Permite ter um corpo de função vazio ou com DeclarationsAndStatements.

# DeclarationsAndStatements: 
Combina declarações e instruções em qualquer ordem dentro do corpo da função. Adiciona cada Statement ou Declaration como irmão do anterior.

# FunctionDeclaration: 
Representa a declaração de uma função, iniciando com o tipo e terminado com o SEMI.

# FunctionDeclarator: 
Define o identificador de uma função e sua lista de parâmetros.

# ParameterList: 
Lista de parâmetros de uma função, permitindo múltiplos ParameterDeclaration.

# ParameterListAux: 
Auxilia na construção da lista de parâmetros, adicionando cada ParameterDeclaration como irmão do anterior. Pode ser vazio.

# ParameterDeclaration: 
Declaração de um único parâmetro de função, incluindo seu tipo e opcionalmente um identificador.

# ParameterDeclarationOpt: 
Opção para incluir um identificador em ParameterDeclaration.

# Declaration: 
Define uma declaração de variável, que pode incluir múltiplas variáveis do mesmo tipo (DeclaratorList).

# DeclaratorList: 
Permite a declaração de múltiplas variáveis na mesma linha, separadas por vírgulas.

# TypeSpec: 
Especifica o tipo de uma variável ou função: CHAR, INT, VOID, SHORT e DOUBLE.

# Declarator:
 Define um identificador de variável e, opcionalmente, uma atribuição.

# StatementGlobal: 
Auxilia na gestão de erros e na definição de Statement.

# Statement: 
Define as várias formas de uma instrução (como expressões, blocos, estruturas de controle if, while, etc.).

# Statements: 
Permite múltiplas instruções, criando uma lista de instruções ou adicionando instruções a uma lista existente.

# ExprOpt: 
Fornece uma expressão opcional.

# Expr: 
Define a estrutura de uma expressão, incluindo operações aritméticas, lógicas e relacionais.

# ExprAux:
 Auxilia na construção de expressões, permitindo a combinação de múltiplas expressões separadas por vírgulas.

# ExprComma: 
Define uma expressão que pode ser uma lista separada por vírgulas.
