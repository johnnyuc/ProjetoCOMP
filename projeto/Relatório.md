# Relatório de Projeto [Compiladores 22/23]

O presente relatório foi elaborado por Johnny Fernandes [2021190668] e Stefanos Lucena [2021115090] no âmbito da cadeira e projeto de Compiladores, leccionado na Licenciatura em Engenharia Informática. O relatório subdivide-se em três secções que dizem respeito ao desenvolvimento técnico do projeto, nomeadamente: i) a gramática reescrita, ii) algoritmos e estruturas de dados da AST e da tabela de símbolos e iii) geração de código. Por não termos concluído com sucesso a meta 4 do relatório, o mesmo irá explicar a hipotética abordagem de desenvolvimento.

## i) a gramática reescrita

**Introdução e Contexto**
Dado o âmbito do projeto para o desenvolvimento de um compilador para um subconjunto da linguagem C, usamos, de acordo com os conhecimentos adquiridos em aula, a gramática LALR(1) que combina a eficiência da análise LR(1) com a simplicidade da LALR. Além disso, a gramática segue a notação EBNF fornecida no documento.

**Desafios e Soluções**
Um desafio foi manter a gramática livre de ambiguidades, especialmente nas expressões. A gramática inicial não era totalmente livre de ambiguidades e por isso foi necessário fazer correções para que não surgissem os warnings SR/RR. Em concreto as dificuldades começaram a surgir com as FunctionsAndDeclarations e Expr (Expressions) e conseguimos a correção desses problemas usando a precedência e associatividade e seguindo o padrão da linguagem C.

**Gramática:** Essencial à estruturação das regras de aceitação da linguagem.

- **Program:** Define a estrutura geral do programa. Cria um nó raiz para a árvore de sintaxe abstrata (AST) e adiciona FunctionsAndDeclarations como filho desse nó.
- **FunctionsAndDeclarations:** Permite uma sequência de definições e declarações de funções ou declarações de variáveis. Adiciona cada TypeFuncDec como irmão do anterior.
- **TypeFuncDec:** Agrupa FunctionDefinition, FunctionDeclaration e Declaration sob uma regra comum. A escolha entre essas regras é feita com base no contexto do código.
- **FunctionDefinition:** Representa a definição de uma função, composta por um tipo (TypeSpec), declarador de função (FunctionDeclarator) e corpo da função (FunctionBody). Em caso de erro na definição da função, um nó Null é criado.
- **FunctionBody e FunctionBodyOpt:** Juntos, definem o corpo de uma função, permitindo que este seja opcionalmente vazio ou que contenha as declarações e instruções, sempre delimitado por chavetas (curly brackets).
- **DeclarationsAndStatements:** Combina declarações e instruções em qualquer ordem dentro do corpo da função. Adiciona cada Statement ou Declaration como irmão do anterior.
- **FunctionDeclaration e FunctionDeclarator:** Juntos, estes elementos representam a criação de uma função, começando com a definição do tipo, seguido pelo identificador da função e sua lista de parâmetros, e finalizando com um ponto-e-vírgula.
- **ParameterList, ParameterListAux, ParameterDeclaration, ParameterDeclarationOpt:** Facilitam a definição de parâmetros em funções, permitindo listar vários parâmetros, adicionar novos, e incluir identificadores opcionais para cada parâmetro.
- **Declaration, DeclaratorList, Declarator:** Permitem declarar variáveis, seja individualmente ou várias do mesmo tipo numa linha, incluindo nomes e valores iniciais opcionais.
- **StatementGlobal, Statement, Statements:** Abrangem a definição e o manuseio das instruções, desde a gestão dos erros até a elaboração de listas de comandos e estruturas de controlo.
- **ExprOpt, Expr, ExprAux, ExprComma:** Permitindo expressões opcionais, operações aritméticas, lógicas e relacionais, além da capacidade de construir listas de expressões separadas por vírgulas.
- **TypeSpec:** Especifica o tipo de uma variável ou função: CHAR, INT, VOID, SHORT e DOUBLE.

## ii) algoritmos e estruturas de dados da AST e tabela de símbolos

**Introdução e Contexto:** No desenvolvimento de um compilador. a criação eficiente de uma Árvore de Sintaxe Abstrata (AST) e uma tabela de símbolos adequada são cruciais para a boa análise sintática e semântica e por isso abaixo mostramos as técnicas usadas.

**Desafios e Soluções:** Enfrentámos desafios na representação exata da estrutura do código e na gestão de variáveis em diferentes scopes.

**Árvore de Sintaxe Abstrata (AST):** A estrutura `node` na AST é o núcleo para representar as construções sintáticas do código. Cada nó armazena informações cruciais que incluem:

-   **Categoria Sintática:** Classifica a natureza do nó (por exemplo, `Program`, `Declaration`, `FuncDeclaration`), crucial para determinar o papel do nó na estrutura do código.
-   **Token e Localização:** Armazena o token associado ao nó e sua posição no código (linha e coluna), o que é vital para a análise semântica e para reportar erros de maneira precisa.
-   **Tipo do Nó:** Identifica o tipo de dados ou a natureza da construção (como tipo de operação ou estrutura de controlo), essencial para verificar a consistência do código.
-   **Ponteiros para Filhos e Irmãos:** Permite a construção de uma estrutura de árvore hierárquica, onde cada nó pode ter múltiplos filhos (representando, por exemplo, os componentes de uma expressão) e irmãos (sequências de declarações ou instruções).

**Manipulação da AST:** Funções específicas facilitam a construção e análise da AST:

-   **`addchild` e `addbrother`:** Permitem adicionar nós como filhos ou irmãos, construindo a árvore de acordo com a estrutura sintática do código.
-   **`countchildren` e `countbrother`:** Auxiliam na contagem de filhos e irmãos de um nó, o que é útil para entender a complexidade das expressões e instruções.
-   **`show`:** Fornece uma representação visual da AST, crucial para depuração e análise do processo de compilação.
-   **`deallocate`:** Libera a memória alocada pela AST, importante para evitar vazamentos de memória.

**Tabela de Símbolos:** A tabela de símbolos, representada por `table`, é essencial para o rastreamento de variáveis e funções. Ela armazena:

-   **Identificador e Tipo:** Cada entrada na tabela tem um identificador (nome da variável ou função) e seu tipo, permitindo a verificação de tipos e a resolução de referências.
-   **Nó Associado:** Cada entrada pode ter um nó associado, provavelmente vinculando a variável ou função à sua representação na AST.
-   **Lista de Parâmetros (para funções):** Armazena os tipos e identificadores dos parâmetros das funções, vital para a validação de chamadas de função.
-   **Flag de Erro:** Indica a presença de erros, como variáveis não declaradas ou uso incorreto de tipos.

## iii) geração de código

A geração de código não foi desenvolvida no âmbito deste projeto por um insucesso no desenvolvimento da componente de análise semântica, contudo não deixamos de fora a sua explicação e abordagem pretendida pelo grupo.

Para tal iriamos basear o nosso desenvolvimento no arquivo `codegen.c` disponibilizado no repositório do professor Raul [Petit], que utiliza uma abordagem específica para transformar cada nó da Árvore de Sintaxe Abstrata (AST) em instruções de código. O processo envolve a tradução de operações aritméticas (como adição, subtração, multiplicação e divisão) em instruções de máquina e também são gerados códigos para literais naturais, decimais e identificadores. Seria necessário para o funcionamento completo do compilador, a adição de vários outros elementos que não estão presentes no ficheiro original.

Além disso, a estrutura do código no ficheiro aborda a geração de instruções para estruturas condicionais e chamadas de funções. Este método permite converter eficientemente a estrutura abstrata da AST num código executável específico para a plataforma alvo, formando a base para a geração de código no compilador.