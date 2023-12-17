#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantics.h"

// Variáveis globais
int semantic_errors = 0; // Contador de erros semânticos
struct table_list *tables; // Lista de tabelas
struct table *global_table; // Tabela global

// A análise semântica começa aqui, na raiz da AST
int check_program(struct node *program) {

    // Alocação de memória para a lista de tabelas
    tables = (struct table_list *) malloc(sizeof(struct table_list));
    tables->next = NULL;

    // Alocação de memória para a tabela global
    global_table = (struct table *) malloc(sizeof(struct table));
    global_table->next = NULL;

    // Insere os símbolos padrão na tabela global
    struct parameter_list *parameter1 = register_parameter(integer_type);
    insert_symbol(global_table, "putchar", integer_type, parameter1, NULL,1);
    struct parameter_list *parameter2 = register_parameter(void_type);
    insert_symbol(global_table, "getchar", integer_type, parameter2, NULL,1);

    // Insere a tabela global na lista de tabelas [primeira]
    insert_table(tables, global_table, NULL);

    // Inicia a verificação recursiva dos nodes
    check_node(program, global_table);

    return semantic_errors;
}

// Função para verificação dos nodes [função recursiva]
void check_node(struct node *node, struct table *current_table) {
    
    if (node == NULL) return;

    // Dependendo da categoria do node, chama a função de verificação correspondente
    if(node->category == Declaration) check_Declaration(node, current_table);
    else if (node->category == FuncDefinition) check_FuncDefinition(node, current_table);
    else if (node->category == FuncDeclaration) check_FuncDeclaration(node, current_table);

    // Percorre a árvore
    if (node->children != NULL) {
        struct node_list *child = node->children;
        while ((child = child->next) != NULL) {
            // Para declarações e definições de funções [se o filho for uma destas categorias, cria uma tabela própria]
            if (node->category == FuncBody && child->node->category == Declaration) {
                // Obter a tabela da função atual
                struct table *func_table = get_function_table(tables, node->token);
                check_node(child->node, func_table);
            } else {
                // Caso contrário, continuamos a usar a tabela atual
                check_node(child->node, current_table);
            }
        }
    }
}

// Função para verificação de declarações
void check_Declaration(struct node *node, struct table *table) {

    struct node *tspec = getchild(node,0);
    struct node *declarator = getchild(node, 1);
    struct node *expression = getchild(node,2);

    // Verifica a existência do símbolo na tabela [se não estiver presente, adiciona]
    if((declarator != NULL) && (search_symbol(table, declarator) == NULL))  {
        enum type type =category_type(tspec->category);
        if(type!=void_type && declarator->token!=NULL){
            struct node *expression_son1 = getchild(expression,0);
            struct node *expression_son2 = getchild(expression,1);

            //Error case: quanto fazemos int i = i+i por exemplo. Não sei se precisava
            if(expression != NULL && expression_son1 != NULL && expression_son2 != NULL){
                if( (expression_son1->token != NULL) && (expression_son2->token != NULL) && 
                ((strcmp(declarator->token, expression_son1->token) == 0) || (strcmp(declarator->token, expression_son2->token) == 0))) {
                    insert_symbol(table, declarator->token, type, NULL, newnode(Declaration, NULL),0);
                    if(expression != NULL && table != NULL) check_Expression(expression, table);
                }

                else {
                    insert_symbol(table, declarator->token,type,NULL, newnode(Declaration,NULL),1);
                    if(expression!=NULL && table!=NULL) check_Expression(expression,table);
                }
            } else {
                insert_symbol(table, declarator->token,type,NULL, newnode(Declaration,NULL),1);
                    if(expression != NULL && table != NULL) check_Expression(expression,table);

            }

        }

        else if(type == void_type && declarator->token != NULL && expression != NULL) check_Expression(expression,table);
    }

    else if((declarator != NULL) && (declarator->token != NULL) && (search_symbol(table, declarator) != NULL) && 
    (search_symbol(table, declarator)->node->type == declarator->type) && (expression != NULL) && (table != NULL)) 
        check_Expression(expression,table);
        
    // Se já estiver, printa a mensagem de erro e acrescenta um erro
    else if( ( declarator != NULL) && (search_symbol(table, declarator) != NULL) ) {
        printf("Line %d, column %d: Symbol %s already defined\n", declarator->token_line, declarator->token_column, declarator->token);
        semantic_errors++;
    }

}

// Função para verificação de definições de funções
void check_FuncDefinition(struct node *node,struct table *table){

    //Variael auxiliar para verificar se temos alguma irregularidade encontrada na verificação dos parâmetros
    int parameter_error = 1;

    struct node *tspec = getchild(node,0);
    struct node *func_declarator = getchild(node,1);
    struct node *param_list = getchild(node,2);
    struct node *func_body = getchild(node,3);

    // Declaração "global" do type, usada em func_declarator e func_body
    enum type type = category_type(tspec->category);
    
    // Verifica a existência do símbolo na tabela [se não estiver presente, adiciona]
    if(search_symbol(table, func_declarator) == NULL) { 
        struct node *ParamDeclaration;
        struct parameter_list *parameter_list_aux = NULL;

        // Variável auxiliar para percorrer os filhos de ParamList
        int i = 0, j = 0;
        // Variável auxiliar para verificar se temos um parâmetro void
        int void_check = 1, table_error = 1;

        while ((ParamDeclaration = getchild(param_list, i)) != NULL) {
            if(check_parameter(ParamDeclaration)==void_type) void_check = 1;
        
            // Se o parâmetro for do tipo void e ParamList tiver mais do que um filho, temos um erro
            if((check_parameter(ParamDeclaration) == void_type && i > 0) || (check_parameter(ParamDeclaration) == void_type && getchild(ParamDeclaration, 1) != NULL)) {
                parameter_list_aux = NULL;
                parameter_error = 0;
                table_error = 0;
                break;
            }

            // Se não, adicionar o parâmetro na lista de parâmetros
            else{
                parameter_list_aux = add_parameter(parameter_list_aux, check_parameter(ParamDeclaration),getchild(ParamDeclaration,1));
                i++;
            }
        }

        if(void_check == 0 && i > 1) parameter_error=0;

        // Se não tivermos tido erros, insere o novo símbolo na tabela
        if(parameter_error == 1) insert_symbol(table, func_declarator->token ,type, parameter_list_aux, func_declarator,1);

        if(table_error == 1) { 

            // Criação de uma tabela para a nova função
            struct table *new_symbol_table = (struct table *) malloc(sizeof(struct table));
            new_symbol_table->next = NULL;

            // Insert do return default na table da nova função
            insert_symbol(new_symbol_table, "return", type, NULL, NULL, 1);

            // Percorre a lista de parâmetros e insere os símbolos da nova função na tabela
            while(parameter_list_aux != NULL){
                if(parameter_list_aux->Identifier != NULL) insert_symbol(new_symbol_table, parameter_list_aux->Identifier, parameter_list_aux->parameter, parameter_list_aux, node, 1);
                parameter_list_aux = parameter_list_aux->next;
            }

            // Inserção da nova tabela na lista de tabelas
            insert_table(tables,new_symbol_table,func_declarator->token);

            struct node *func_body_child;
            while((func_body_child = getchild(func_body, j)) != NULL) {
                if(func_body_child->category==Declaration) check_Declaration(func_body_child,new_symbol_table);
                else check_Statement(func_body_child,new_symbol_table);
                j++;
            }
        }
    }

    else if ( (search_symbol(table, func_declarator) != NULL) && (search_symbol(table, func_declarator)->node!=NULL) && ( search_symbol(table, func_declarator)->node->category == 35 ) ){
        
        int i = 0;
        int verify_parameters = 0;
        struct node *ParamDeclaration;
        struct parameter_list *global_aux = search_symbol2(global_table,func_declarator->token)->parameter;

        while ((ParamDeclaration = getchild(param_list, i)) != NULL) {
            if( (global_aux != NULL)  && (check_parameter(ParamDeclaration)!=global_aux->parameter)) verify_parameters=1;
            if(global_aux != NULL) global_aux = global_aux->next;
            i++;
        }

        if(type != search_symbol2(global_table, func_declarator->token)->type || verify_parameters == 1) {
            printf("Line %d, column %d: Symbol %s already defined\n", func_declarator->token_line, func_declarator->token_column, func_declarator->token);
            semantic_errors++;
        }

        else {
            struct table *symbol_tableFunc;

            if(search_table(tables,func_declarator->token) != NULL) {
                symbol_tableFunc = search_table(tables,func_declarator->token)->table;
                struct node *ParamDeclaration;
                struct parameter_list *parameter_list_aux = NULL;
                int i = 0, j = 0;

                while ((ParamDeclaration = getchild(param_list, i)) != NULL) {
                    if(check_parameter(ParamDeclaration) == void_type && i > 0) {
                        parameter_list_aux = NULL;
                        parameter_error = 0;
                        break;
                    }

                    else {
                        parameter_list_aux = add_parameter(parameter_list_aux, check_parameter(ParamDeclaration), getchild(ParamDeclaration, 1));
                        i++;
                    }
            
                }

                // Só irá imprimir a tabela se o número de argumentos forem iguais e os tipos iguais, mas os identifiers podem ser diferentes
                int param_erros = 0;
                int k = 0;
                int n_param = count_parameters(parameter_list_aux);
                struct parameter_list *head = parameter_list_aux;

                if (count_parameters(symbol_tableFunc->parameter) == count_parameters(parameter_list_aux)) {
                    while(k!=n_param) {
                        if(symbol_tableFunc->parameter->Identifier != NULL && parameter_list_aux->Identifier != NULL && 
                        type_name(parameter_list_aux->parameter) != type_name(symbol_tableFunc->parameter->parameter))
                                param_erros++;
                
                        symbol_tableFunc->parameter = symbol_tableFunc->parameter->next;
                        parameter_list_aux = parameter_list_aux->next;
                        k++;
                    }

                }

                else if (count_parameters(symbol_tableFunc->parameter) != count_parameters(parameter_list_aux)) param_erros++;

                if(param_erros == 0) {

                    // Insert do return default na tabela da nova função
                    insert_symbol(symbol_tableFunc,"return",type,NULL,NULL,1);

                    // Se nao tiver erros, insere o novo símbolo na tabela
                    parameter_list_aux = head;

                    while(parameter_list_aux != NULL){
                        if(parameter_list_aux->Identifier != NULL)
                            insert_symbol(symbol_tableFunc, parameter_list_aux->Identifier, parameter_list_aux->parameter, parameter_list_aux,node, 1);
                        parameter_list_aux = parameter_list_aux->next;
                    }

                    // Inserção da nova tabela na lista de tabelas
                    insert_table(tables, symbol_tableFunc, func_declarator->token);
                    struct node *func_body_child;

                    while((func_body_child = getchild(func_body,j))!=NULL){
                        if(func_body_child->category == Declaration){
                            // Se as declarations estiverem bem, adiciona na table da nova função
                            check_Declaration(func_body_child,symbol_tableFunc);
                        }
                        
                        else check_Statement(func_body_child,symbol_tableFunc);
                        j++;
                    }
                }
            }
        } 
    }

    // Caso já esteja, imprime a mensagem de erro e incrementa semantic_errors
    else {
        printf("Line %d, column %d: Symbol %s already declared\n", func_declarator->token_line, func_declarator->token_column, func_declarator->token);
        semantic_errors++;
    }

    // Trata do caso especial de termos uma função a retornar um valor de tipo diferente do indicado na assinatura
    if(type == void_type && getchild(func_body, 0) != NULL) {
        struct node *rturn = getchild(func_body, 0);
        if(rturn->children!=NULL){
            struct node *Identifier = getchild(rturn,0);
            struct table *symbol;
            symbol = search_symbol(global_table, Identifier);
            if(symbol != NULL) printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n", 
            Identifier->token_line, Identifier->token_column, type_name(symbol->type), type_name(void_type));
            semantic_errors++;
        }

    }
}

// Função para verificação de declarações de funções
void check_FuncDeclaration(struct node *node,struct table *table){

    // Variavel auxiliar para verificar se temos uma irregularidade encontrada na verificação dos parâmetros
    int parameter_error = 1;

    // Declaração dos filhos de FuncDeclaration
    struct node *tspec = getchild(node,0);
    struct node *func_declarator = getchild(node,1);
    struct node *param_list = getchild(node,2);

    enum type type = category_type(tspec->category);
    
    // Verifica se o novo símbolo já se encontra na tabela [se não estiver, adiciona]
    if(search_symbol(table, func_declarator) == NULL){

        struct node *ParamDeclaration;
        struct parameter_list *parameter_list_aux = NULL;
        int i = 0;
        int void_check = 0;
        
        while ((ParamDeclaration = getchild(param_list, i)) != NULL) {

            if(check_parameter(ParamDeclaration) == void_type) void_check=1;

            // Se o parâmetro for do tipo void e ParamList tiver mais que um filho, temos um erro
            if( (check_parameter(ParamDeclaration)==void_type && i>0) || (check_parameter(ParamDeclaration)==void_type && getchild(ParamDeclaration,1)!=NULL) ){
                parameter_list_aux = NULL;
                parameter_error = 0;
                break;
            }

            // Se não, adiciona o novo parâmetro na lista de parâmetros
            else {
                parameter_list_aux = add_parameter(parameter_list_aux, check_parameter(ParamDeclaration), getchild(ParamDeclaration, 1));
                i++;
            }
        }

        // Cobre o caso de erro de termos mais de 1 parâmetro sendo que temos void
        if(void_check == 1 && i > 1) parameter_error = 0;

        // Se não tivermos tido erros, insere o novo símbolo na tabela
        if(parameter_error == 1) {
            insert_symbol(table, func_declarator->token, type, parameter_list_aux, newnode(FuncDeclaration, NULL), 1);
            struct table *new_symble_table = (struct table *) malloc(sizeof(struct table));
            new_symble_table->next = NULL;
            new_symble_table->identifier = func_declarator->token;
            new_symble_table->parameter = parameter_list_aux;

            //insert_symbol(new_symble_table,func_declarator->token ,type, parameter_list_aux, newnode(FuncDeclaration,NULL));
            insert_table(tables,new_symble_table,func_declarator->token);
        }
    } 

    else if( (search_symbol(table, func_declarator) != NULL) && (category_type(search_symbol(table, func_declarator)->node->category)==5) ){
        
        struct node *ParamDeclaration;
        struct parameter_list *parameter_list_aux = NULL;
        int i = 0;
        
        while ((ParamDeclaration = getchild(param_list, i)) != NULL) {
            
            // Se o parâmetro for do tipo void e ParamList tiver mais que um filho, temos um erro
            if(check_parameter(ParamDeclaration) == void_type && i > 0) {
                parameter_list_aux = NULL;
                parameter_error = 0;
                break;
            }

            // Se não, adicionar o novo parâmetro na lista de parâmetros
            else {
                parameter_list_aux = add_parameter(parameter_list_aux, check_parameter(ParamDeclaration), getchild(ParamDeclaration, 1));
                i++;
            }
        }

        // Se não tivermos tido erros, insere o novo símbolo na tabela

        if(parameter_error == 1) {
            if(table!=global_table) insert_symbol(table, func_declarator->token, type, parameter_list_aux, newnode(FuncDeclaration, NULL), 1);
            struct table *new_symble_table = (struct table *) malloc(sizeof(struct table));
            new_symble_table->next = NULL;
            new_symble_table->identifier = func_declarator->token;
            
            insert_table(tables,new_symble_table,func_declarator->token);
        }
    }

    // Caso já esteja, imprime a mensagem de erro e incrementa semantic_errors
    else {
        printf("Line %d, column %d: Symbol %s already declared\n", func_declarator->token_line, func_declarator->token_column, func_declarator->token);
        semantic_errors++;
    }
}

// Função para verificação de statements
void check_Statement(struct node *node, struct table *table){

    if(node != NULL){

        if(node->category == If){
            struct node *expression = getchild(node, 0);
            struct node *statementesp = getchild(node, 1);
            struct node *statementespelse = getchild(node, 2);

            check_Expression(expression, table);

            check_Statement(statementesp, table);
            if ((statementespelse != NULL) /*&& (statementespelse->category != Null)*/) check_Statement(statementespelse, table);
        }

        else if(node->category == While){
            struct node *expression = getchild(node, 0);
            struct node *statementesp = getchild(node, 1);
            check_Expression(expression, table);
            if (statementesp->category != Null) check_Statement(statementesp, table);
        }

        else if(node->category == Return){
            //Lidar com o erro Line %d, column %d: Conflicting types (got %s, expected %s)\n tem de ser nas expressions
            struct node *expression = getchild(node,0);
            check_Expression(expression,table);   
        }

        else if(node->category == StatList){
            struct node_list *statements = node->children;
            while((statements = statements->next) != NULL){
                check_Statement(statements->node,table); 
            }
        }

        else check_Expression(node,table);
    }
}

// Função para verificação de expressões
void check_Expression(struct node *node, struct table *table){
        switch(node->category) {

        case Identifier:
            if(search_symbol2(table, node->token) == NULL && search_symbol2(global_table, node->token) == NULL) {
                printf("Line %d, column %d: Unknown symbol %s\n",node->token_line, node->token_column,node->token);
                semantic_errors++;
                node->type = undef_type;
            }
            
            else if (search_symbol2(table, node->token) != NULL)
                node->type = search_symbol2(table, node->token)->type;


            else if(search_symbol2(global_table, node->token) != NULL) {
                node->type = search_symbol2(global_table, node->token)->type;
                if(search_symbol2(global_table, node->token)->parameter!=NULL && node->parent->category != Call) node->error = 0;
            }

            if(search_symbol2(table, node->token)!=NULL && search_symbol2(table, node->token)->error_flag==0){
                node->type = undef_type;
            }

            break;

        case Natural:
        case ChrLit:
            node->type = integer_type;
            break;
        case Decimal:
            node->type = double_type;
            break;
        
        case Store:

            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);

            if((getchild(node, 0)->error == 0 || getchild(node, 1)->error == 0) || (getchild(node, 0)->type == undef_type || getchild(node, 1)->type == undef_type) || (getchild(node, 0)->type == double_type || getchild(node, 1)->type == double_type)) {
                node->type=undef_type;
                printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", 
                    node->token_line, node->token_column, operand_name(node->category), type_name(getchild(node, 0)->type), type_name(getchild(node, 1)->type));
            }
            else if(getchild(node, 0)->type == void_type || getchild(node, 1)->type == void_type) node->type = undef_type;
            else if(getchild(node, 0)->type == double_type || getchild(node, 1)->type == double_type) node->type = double_type;
            else if(getchild(node, 0)->type == integer_type || getchild(node, 1)->type == integer_type) node->type = integer_type;
            else if(getchild(node, 0)->type == short_type|| getchild(node, 1)->type == short_type) node->type = short_type;
            else if(getchild(node, 0)->type == char_type|| getchild(node, 1)->type == char_type) node->type = char_type;

            break;

        case Add:
        case Sub:
        case Mul:
        case Div:

            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);

            if((getchild(node, 0)->error == 0 || getchild(node, 1)->error == 0) || (getchild(node, 0)->type == undef_type || getchild(node, 1)->type == undef_type) || (getchild(node, 0)->type == undef_type || getchild(node, 1)->type == undef_type)) {
                node->type=undef_type;
                printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", 
                    node->token_line, node->token_column, operand_name(node->category), type_name(getchild(node, 0)->type), type_name(getchild(node, 1)->type));
            }
            else if(getchild(node, 0)->type == void_type || getchild(node, 1)->type == void_type) node->type = undef_type;
            else if(getchild(node, 0)->type == double_type || getchild(node, 1)->type == double_type) node->type = double_type;
            else if(getchild(node, 0)->type == integer_type || getchild(node, 1)->type == integer_type) node->type = integer_type;
            else if(getchild(node, 0)->type == short_type|| getchild(node, 1)->type == short_type) node->type = short_type;
            else if(getchild(node, 0)->type == char_type|| getchild(node, 1)->type == char_type) node->type = char_type;

            break;

        case Mod:
        case Or:
        case And:
        case BitWiseAnd:
        case BitWiseOr:
        case BitWiseXor:
        case Eq:
        case Ne:
        case Ge:
        case Lt:
        case Le:
        case Gt:
        
                check_Expression(getchild(node, 0), table);
                check_Expression(getchild(node, 1), table);

                if(getchild(node, 0)->type == undef_type || getchild(node, 1)->type == undef_type) 
                    printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", 
                        node->token_line, node->token_column, operand_name(node->category), type_name(getchild(node, 0)->type), type_name(getchild(node, 1)->type));

                node->type=integer_type;

                break;
        
        case Plus:
        case Minus:
            check_Expression(getchild(node, 0), table);
            enum type operandType = getchild(node, 0)->type;
            node->type = operandType;
            break;

        case Not:
            check_Expression(getchild(node, 0), table);
            node->type = integer_type;
            break;

        case Comma:
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);

            if(getchild(node,1)->error == 0) node->type = undef_type;
            
            else if(getchild(node, 0)->type == undef_type) node->type=undef_type;
            else if(getchild(node, 1)->type == undef_type) node->type=undef_type;
            else if(getchild(node, 1)->type == void_type) node->type=void_type;
            else if(getchild(node, 1)->type==double_type) node->type=double_type;
            else if(getchild(node, 1)->type==integer_type) node->type=integer_type;
            else if(getchild(node, 1)->type==short_type) node->type=short_type;
            else if(getchild(node, 1)->type==char_type) node->type=char_type;

            break;
            
        case Call:
            if(search_symbol2(global_table, getchild(node, 0)->token) == NULL) {
                struct table *table_aux = search_symbol2(table,getchild(node, 0)->token);
                if(table_aux != NULL){
                    node->type = table_aux->type;
                    //Identifier
                    check_Expression(getchild(node, 0), table);

                    //Expressions
                    int i = 1;
                    while(getchild(node,i)!=NULL){
                        check_Expression(getchild(node, i), table);
                        i++;
                    }

                    if(count_parameters(table_aux->parameter) != i-1)
                        printf("Line %d, column %d: Wrong number of arguments to function %s (got %d, required %d)\n", 
                        getchild(node,0)->token_line, getchild(node,0)->token_column, table_aux->identifier, i-1, count_parameters(table_aux->parameter));
                }

                else{
                    node->type = undef_type;
                    //Identifier
                    check_Expression(getchild(node, 0),table);

                    //Expressions
                    int i = 1;
                    while(getchild(node,i)!=NULL){
                        check_Expression(getchild(node,i),table);
                        i++;
                    }

                    printf("Line %d, column %d: Wrong number of arguments to function %s (got %d, required %d)\n", 
                    getchild(node,0)->token_line, getchild(node,0)->token_column, getchild(node,0)->token, i-1, 0);
                }

            }

            else if(search_symbol2(global_table, getchild(node, 0)->token) != NULL){

                struct table *table_aux = search_symbol2(global_table, getchild(node, 0)->token);
                if(table_aux != NULL) node->type = table_aux->type;
                //Identifier
                check_Expression(getchild(node, 0),table);
                //Expressions

                int i = 1;
                while(getchild(node, i) != NULL){
                    check_Expression(getchild(node, i), table);
                    i++;
                }
            }
            break;
        default:
            break;
    }
}

// Insere um novo símbolo na tabela
struct table *insert_symbol(struct table *table, char *identifier,enum type type, struct parameter_list *v_type, struct node *node, int error_flag) {
    struct table *new = (struct table *) malloc(sizeof(struct table));
    if(identifier != NULL) new->identifier = strdup(identifier);
    else new->identifier = NULL;
    new->type = type;
    new->parameter = v_type;
    new->node = node;
    new->error_flag = error_flag;
    new->next = NULL;
    struct table *symbol = table;
    
    while(symbol != NULL) {
        if(symbol->next == NULL) {
            symbol->next = new;
            break;
        } else if( (identifier != NULL) && (symbol->next->identifier!= NULL) ){
                if ( strcmp(symbol->next->identifier, identifier) == 0 ){
                free(new);
                return NULL;
            }
        }
        symbol = symbol->next;
    }
    return new;
}

// Verifica a existência de um simbolo na tabele
struct table *search_symbol(struct table *table, struct node *node) {
    struct table *symbol;
    if (table!=NULL){
        for(symbol = table->next; symbol != NULL; symbol = symbol->next)
            if( (node!=NULL) && (node->token!=NULL) && (symbol->identifier!= NULL) && (strcmp(symbol->identifier, node->token) == 0) ){
                symbol->node = node;
                return symbol;
            }
        return NULL;
    }
    return NULL;
}

// Regista um novo parâmetro
struct parameter_list *register_parameter(enum type parameter){

    struct parameter_list *new_parameter = (struct parameter_list *)malloc(sizeof(struct parameter_list));
    new_parameter->parameter = parameter;
    new_parameter->next=NULL;
    return new_parameter;
}

// Adiciona um novo parâmetro na lista de parâmetros
struct parameter_list *add_parameter(struct parameter_list *parameter_list, enum type parameter,struct node *Identifier) {
    struct parameter_list *new_parameter = (struct parameter_list *)malloc(sizeof(struct parameter_list));
    if (new_parameter == NULL) {
        return parameter_list;
    }

    if (Identifier!=NULL) new_parameter->Identifier = Identifier->token;
    else new_parameter->Identifier = NULL;
    new_parameter->parameter = parameter;
    new_parameter->next = NULL;

    // Se a lista de parâmetros estiver vazia, o novo parâmetro torna-se o primeiro da lista
    if (parameter_list == NULL) {
        return new_parameter;
    } 

    // Caso contrário, percorremos a lista e adicionamos o novo parâmetro na última posição
    else {
        struct parameter_list *current = parameter_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_parameter;
    }

    return parameter_list;    
}

enum type check_parameter(struct node *param_declaration) {
    struct node *tspec = getchild(param_declaration,0);
    enum type type = category_type(tspec->category);

    return type;
}

// Insere uma tabela na lista de tabelas
struct table_list *insert_table(struct table_list *table_list, struct table *new_table, char *token){
    struct table_list *new = (struct table_list *) malloc(sizeof(struct table_list));
    new->table = new_table;
    new->func_name = token;
    new->next = NULL;
    struct table_list *symbol = table_list;
    while(symbol != NULL) {
        if(symbol->next == NULL) {
            symbol->next = new;
            break;
        } else if( (token!=NULL) && (symbol->next->func_name!=NULL) && (strcmp(symbol->next->func_name, token) == 0) ) {
            free(new);
            return NULL;
        }
        symbol = symbol->next;
    }
    return new;
}

// Função para obter a tabela de símbolos de uma função
struct table *get_function_table(struct table_list *table_list, char *function_name) {
    while (table_list != NULL) {
        if ( function_name!= NULL && table_list->func_name != NULL && strcmp(table_list->func_name, function_name) == 0) {
            return table_list->table;
        }
        table_list = table_list->next;
    }
    return NULL;
}

// Função para imprimir as tabelas
void show_tables() {
    struct table *symbol;

    while((tables = tables->next) != NULL){
        if(tables->func_name == NULL){
            printf("===== Global Symbol Table =====\n");
            for (symbol = tables->table->next; symbol != NULL; symbol = symbol->next) {

                printf("%s\t%s", symbol->identifier, type_name(symbol->type));

                // Verificar se a lista de parâmetros não é nula
                if (symbol->parameter != NULL) {
                    struct parameter_list *param = symbol->parameter;

                    // Verificar se o tipo de parâmetro não é no_type
                    if (param->parameter != no_type) {
                        printf("(");
                        while (param != NULL) {
                            printf("%s", type_name(param->parameter));
                            param = param->next;
                            if (param != NULL) {
                                printf(",");
                            }
                        }
                        printf(")");
                    }
                }
                printf("\n");
            }
        }

        else{
            int i = 0;
            for (symbol = tables->table->next; symbol != NULL; symbol = symbol->next) {
                if(i==0 && tables!=NULL){
                    printf("\n===== Function %s Symbol Table =====\n", tables->func_name);
                    i++;
                }

                printf("%s\t%s", symbol->identifier, type_name(symbol->type));

                // Verificar se a lista de parâmetros não é nula
                if (symbol->parameter != NULL) printf("\tparam");
                printf("\n");
            }
        }
    }

    printf("\n");

}

char *category_name_note[] = names;
// Faz a impressão da árvore anotada
void show_annotated(struct node *node, int depth) {
    if (node == NULL) return; // if tree is empty
    int i;
    for (i = 0; i < depth; i++)
        printf("..");
        
    if (node->token == NULL) {


        if(strcmp(type_name(node->type),"none")!=0 && (node->category!=Int) && (node->category!=Short) && (node->category!= Natural) && (node->category!=Double) && (node->category!= Decimal) && (node->category!=Void) && (node->category!=Char))
            printf("%s - %s\n", category_name_note[node->category], type_name(node->type));

        else  printf("%s\n", category_name_note[node->category]);
    }

    else {

        if(strcmp(type_name(node->type),"none")!=0){

            // Trata do caso de imprimir na tree x(x) - x(x)
            struct table *table_local = search_symbol(global_table,node);

            if(table_local!=NULL && table_local->parameter!=NULL && node->type != undef_type) {
                struct parameter_list* head = table_local->parameter;
                printf("%s(%s) - %s(%s", category_name_note[node->category], node->token, type_name(node->type),type_name(table_local->parameter->parameter));
                while((table_local->parameter = table_local->parameter->next) !=NULL) {
                    printf(",%s",type_name(table_local->parameter->parameter));
                }
                printf(")\n");

                table_local->parameter = head;
            }

            else printf("%s(%s) - %s\n", category_name_note[node->category], node->token, type_name(node->type));
        }

        else printf("%s(%s)\n", category_name_note[node->category], node->token);
    }

    // Itera sobre os filhos do nó atual
    struct node_list *child = node->children;
    while (child != NULL) {
        show_annotated(child->node, depth + 1);
        child = child->next;
    }

    // Iterata sobre os irmãos do nó atual
    struct node_list *brother = node->brothers;
    while (brother != NULL) {
        show_annotated(brother->node, depth);
        brother = brother->next;
    }
}

// Função para obter um nó [baseado no token]
struct table_list *search_table(struct table_list *table, char *token) {
    struct table_list *symbol;
    for(symbol = table->next; symbol != NULL; symbol = symbol->next){
        if( (symbol->func_name != NULL) && (token != NULL) && (strcmp(symbol->func_name, token) == 0) ){
            return symbol;
        }
    }
    return NULL;
}

// Função para obter um nó [baseado no identifier]
struct table *search_symbol2(struct table *table, char *identifier){
    struct table *symbol;
    for(symbol = table->next; symbol != NULL; symbol = symbol->next){
        if( (symbol->identifier != NULL) && (identifier != NULL) && (strcmp(symbol->identifier, identifier) == 0) ){
            return symbol;
        }
    }
    return NULL;
}

// Cria um novo nó de parâmetro e o adiciona à lista
struct parameter_list *add_parameter2(struct parameter_list *list, enum type parameter, char *name) {

    struct parameter_list *new_param = malloc(sizeof(struct parameter_list));
    if (new_param == NULL) return list;

    if (name != NULL) new_param->Identifier = strdup(name);
    else if(name == NULL && parameter != void_type) new_param->Identifier = NULL;
    else if(name != NULL && parameter == void_type) new_param->Identifier = NULL;

    // Preenche os dados do novo nó
    new_param->parameter = parameter;
    new_param->next = NULL;
    
    // Adiciona o novo nó à lista existente
    if (list == NULL) list = new_param;
    else {
        // Caso contrário, encontra o último nó na lista e adiciona o novo nó como próximo
        struct parameter_list *last_param = list;
        while (last_param->next != NULL) {
            last_param = last_param->next;
        }
        last_param->next = new_param;
    }
    return list;
}

// Função para obter o tipo de um parâmetro
char *get_identifier(struct node *paramdeclaration){
    struct node *tspec = getchild(paramdeclaration, 0);
    enum type type = category_type(tspec->category);
    if(type != void_type && getchild(paramdeclaration, 1) != NULL){
        struct node *id = getchild(paramdeclaration, 1);
        return id->token;  
    }
    return NULL; 
}

// Função para contar o número de elementos da lista
int count_parameters(struct parameter_list *list) {
    int count = 0;
    while (list != NULL) {
        count++;
        list = list->next;
    }
    return count;
}

// Função para obter o tipo de um parâmetro
void insert_params_to_symbol_table(struct table *symbol_tableFunc, struct parameter_list *params) {
    while (params != NULL) {
        if (params->parameter != void_type && params->Identifier != NULL)
            symbol_tableFunc = insert_symbol(symbol_tableFunc, params->Identifier, params->parameter, NULL, NULL, 1);
        params = params->next;
    }
}
