#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantics.h"

int semantic_errors = 0; //contador de erros semanticos

struct table_list *tables; //lista de tables

struct table *global_table; //table global


// semantic analysis begins here, with the AST root node
int check_program(struct node *program) {

    //alocação de memória
    tables = (struct table_list *) malloc(sizeof(struct table_list));
    tables->next = NULL;

    global_table = (struct table *) malloc(sizeof(struct table));
    global_table->next = NULL;


    //insere os síbolos pré-definidos na global table
    struct parameter_list *parameter1 = register_parameter(integer_type);

    insert_symbol(global_table, "putchar", integer_type, parameter1, NULL);

    struct parameter_list *parameter2 = register_parameter(void_type);
    insert_symbol(global_table, "getchar", integer_type, parameter2, NULL);

    //insere a global table na lista de tables, como a primeira table na lista
    insert_table(tables,global_table,NULL);

    // Inicia a verificação recursiva dos nodes
    check_node(program,global_table);

    return semantic_errors;
}


void check_node(struct node *node, struct table *current_table) {
    
    if (node == NULL) return;

    // Se for uma declaração, verifique usando a tabela corrente
    if(node->category == Declaration) {
        check_Declaration(node, current_table);
    }
    
    // Se for uma definição de função, crie uma nova tabela para a função
    else if (node->category == FuncDefinition) {
        check_FuncDefinition(node, current_table);
    }

    // Se for uma declaração de função, verifique usando a tabela global
    else if (node->category == FuncDeclaration) {
        check_FuncDeclaration(node, current_table);
    }

    // Percorre a árvore
    if (node->children != NULL) {
        struct node_list *child = node->children;
        while ((child = child->next) != NULL) {
            // Para declarações e definições dentro de funções, use a tabela da função
            if (node->category == FuncBody && child->node->category == Declaration) {
                // Obtenha a tabela da função atual
                struct table *func_table = get_function_table(tables, node->token);
                check_node(child->node, func_table);
            } else {
                // Caso contrário, continue usando a tabela atual
                check_node(child->node, current_table);
            }
        }
    }
}

void check_Declaration(struct node *node, struct table *table){

    struct node *tspec = getchild(node,0);
    struct node *declarator = getchild(node, 1);

    /*
    //trata de um caso de erro específico
    if(tspec!=NULL && tspec->category==Void){
        printf("Line %d, column %d: Invalid use of void type in declaration\n",tspec->token_line, tspec->token_column);
        semantic_errors++;
    }
    */

    //else{

        //Verifica a existencia do novo simbolo na tabela

        //se não tiver ainda, adiciona
        if( ( (declarator != NULL) && (search_symbol(table, declarator) == NULL) ) || ( (declarator != NULL) && (search_symbol(table, declarator) != NULL) && ( search_symbol(table, declarator)->node->type==declarator->type) ) ) {
            enum type type =category_type(tspec->category);

            if(type!=void_type && declarator->token!=NULL){
                 insert_symbol(table, declarator->token,type,NULL, declarator);
            }

        } 

        //se já estiver, printa a mensagem de erro e itera sobre semantic errors
        else if( ( declarator != NULL) && (search_symbol(table, declarator) != NULL) ) {
            printf("Line %d, column %d: Symbol %s already defined\n", declarator->token_line, declarator->token_column,declarator->token);
            semantic_errors++;
        }

    //}
    
}

void check_FuncDefinition(struct node *node,struct table *table){

    //Variael auxiliar para verificar se tivemos um irregularidade encontrada na verificação dos parâmetros
    int parameter_error = 1;

    //declaração dos filhos do node FuncDefinition
    struct node *tspec = getchild(node,0);
    struct node *func_declarator = getchild(node,1);
    struct node *param_list = getchild(node,2);
    struct node *func_body = getchild(node,3);

    //Declaração "global" do type, pois usamos em func_declarator e func_body
    enum type type = category_type(tspec->category);
    
    //Verifica se o novo simbolo já se encontra na Table

    //Caso não esteja adiciona o novo símbolo
    if( (search_symbol(table, func_declarator) == NULL) || ( (search_symbol(table, func_declarator) != NULL) && (search_symbol(table, func_declarator)->node!=NULL) && (search_symbol(table, func_declarator)->node->category == 35) ) ) {

        //declaração do nó filho de ParamList
        struct node *ParamDeclaration;

        //guarda os parametros da funcao
        struct parameter_list *parameter_list_aux = NULL;

        //variavel auxiliar para percorrer os filhos de ParamList
        int i = 0;

        //variavel auxiliar para percorrer os filhos de FuncBody
        int j = 0;

        while ((ParamDeclaration = getchild(param_list, i)) != NULL) {
            
            //se o parametro for do tipo void e ParamList tiver mais que um filho, temos um erro
            if(check_parameter(ParamDeclaration)==void_type && i>0){

                //zera a lista de parametros
                parameter_list_aux = NULL;

                //sinaliza erro nos parametros da funcao
                parameter_error = 0;
                break;
            }

            //se nao, adicionar o novo parametro na lista de parametros
            else{
                parameter_list_aux = add_parameter(parameter_list_aux, check_parameter(ParamDeclaration),getchild(ParamDeclaration,1));
                i++;
            }
            
        }

        //se nao tivermos tido erros , insere o novo simbolo na tabela
        if(parameter_error==1) insert_symbol(table, func_declarator->token ,type, parameter_list_aux, func_declarator);


        //criação das table para a nova função
        struct table *new_symble_table = (struct table *) malloc(sizeof(struct table));
        new_symble_table->next = NULL;

        //insert do return default na table da nova função
        insert_symbol(new_symble_table,"return",type,NULL,NULL);


        //percorre a lista de parametros e insere os simbolos da nova funcao em sua table
        while(parameter_list_aux != NULL){

            if(parameter_list_aux->Identifier != NULL){
                insert_symbol(new_symble_table,parameter_list_aux->Identifier,parameter_list_aux->parameter,parameter_list_aux,node);
            }
            parameter_list_aux = parameter_list_aux->next;

        }



        //inserçao da nova table na lista de tables
        insert_table(tables,new_symble_table,func_declarator->token);

        //declaracao do no que vai receber os nos filhos de func_body
        struct node *func_body_child;

        while((func_body_child = getchild(func_body,j))!=NULL){
            
            //verificacao das filhos declarationsdo func_body
            if(func_body_child->category==Declaration){
                
                //se as declarations tiverem bem adiciona na table da nova funcao
                check_Declaration(func_body_child,new_symble_table);

            }

            j++;
        }

    } 

    //Caso já esteja, imprime a mensagem de erro e incrementa semantic_errors
    else {
        printf("Line %d, column %d: Symbol %s already declared\n", func_declarator->token_line, func_declarator->token_column,func_declarator->token);
        semantic_errors++;
    }

    //Trata do caso especial de termos uma funcao a retornar um valor de tipo diferente do indicado em sua assinatura
    if(type==void_type && getchild(func_body,0)!=NULL){

        struct node *rturn = getchild(func_body,0);
        
        if(rturn->children!=NULL){

            struct node *Identifier = getchild(rturn,0);

            struct table *symbol;
            symbol = search_symbol(global_table, Identifier);
            
            printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n",Identifier->token_line,Identifier->token_column,type_name(symbol->type),type_name(void_type));
            semantic_errors++;
        }

    }

}

void check_FuncDeclaration(struct node *node,struct table *table){

    //Variael auxiliar para verificar se tivemos um irregularidade encontrada na verificação dos parâmetros
    int parameter_error = 1;

    //Declaracao dos filhos de FuncDeclaration
    struct node *tspec = getchild(node,0);
    struct node *func_declarator = getchild(node,1);
    struct node *param_list = getchild(node,2);

    enum type type = category_type(tspec->category);
    
    //Verifica se o novo simbolo já se encontra na Table

    //Caso não esteja adiciona o novo símbolo
    if( search_symbol(table, func_declarator) == NULL ){

        //declaração do nó filho de ParamList
        struct node *ParamDeclaration;

        //guarda os parametros da funcao
        struct parameter_list *parameter_list_aux = NULL;

        //variavel auxiliar para percorrer os filhos de ParamList
        int i = 0;
        
        while ((ParamDeclaration = getchild(param_list, i)) != NULL) {
            
            //se o parametro for do tipo void e ParamList tiver mais que um filho, temos um erro
            if(check_parameter(ParamDeclaration)==void_type && i>0){

                //zera a lista de parametros
                parameter_list_aux = NULL;

                //sinaliza erro nos parametros da funcao
                parameter_error = 0;
                break;
            }

            //se nao, adicionar o novo parametro na lista de parametros
            else{
                parameter_list_aux = add_parameter(parameter_list_aux, check_parameter(ParamDeclaration),getchild(ParamDeclaration,1));
                i++;
            }
        }

        //se nao tivermos tido erros , insere o novo simbolo na tabela
        if(parameter_error==1) insert_symbol(table, func_declarator->token ,type, parameter_list_aux, func_declarator);

    } 

    //Caso já esteja, imprime a mensagem de erro e incrementa semantic_errors
    else {
        printf("Line %d, column %d: Symbol %s already declared\n", func_declarator->token_line, func_declarator->token_column,func_declarator->token);
        semantic_errors++;
    }

}

//insere um novo simbolo na table
struct table *insert_symbol(struct table *table, char *identifier,enum type type, struct parameter_list *v_type, struct node *node) {
    struct table *new = (struct table *) malloc(sizeof(struct table));
    if(identifier!=NULL) new->identifier = strdup(identifier);
    else new->identifier = NULL;
    new->type = type;
    new->parameter = v_type;
    new->node = node;
    new->next = NULL;
    struct table *symbol = table;
    
    while(symbol != NULL) {
        if(symbol->next == NULL) {
            symbol->next = new;    /* insert new symbol at the tail of the list */
            break;
        } else if( (identifier != NULL) && (symbol->next->identifier!= NULL) ){
                if ( strcmp(symbol->next->identifier, identifier) == 0 ){
                free(new);
                return NULL;           /* return NULL if symbol is already inserted */
            }
        }
        symbol = symbol->next;
    }
    return new;
}

//verifica a existencia de um simbolo na table
struct table *search_symbol(struct table *table, struct node *node) {
    struct table *symbol;
    if (table!=NULL){
        for(symbol = table->next; symbol != NULL; symbol = symbol->next)

            if( (node->token!=NULL) && (symbol->identifier!= NULL) && (strcmp(symbol->identifier, node->token) == 0) ){
                symbol->node = node;
                return symbol;
            }
        return NULL;
    }
    return NULL;
}

struct table *search_symbol2(struct table *table, char *identifier){
    struct table *symbol;
    for(symbol = table->next; symbol != NULL; symbol = symbol->next)
        if( (symbol->identifier!=NULL) && (identifier!=NULL) && (strcmp(symbol->identifier, identifier) == 0) )
            return symbol;
    return NULL;
}



//registra um novo parametro
struct parameter_list *register_parameter(enum type parameter){

    struct parameter_list *new_parameter = (struct parameter_list *)malloc(sizeof(struct parameter_list));
    new_parameter->parameter = parameter;
    new_parameter->next=NULL;
    return new_parameter;
}

//Adiciona um novo paramatero a lista de parametros
struct parameter_list *add_parameter(struct parameter_list *parameter_list, enum type parameter,struct node *Identifier) {
    // Cria o novo parâmetro
    struct parameter_list *new_parameter = (struct parameter_list *)malloc(sizeof(struct parameter_list));
    if (new_parameter == NULL) {
        // Falha ao alocar memória, retorne a lista original
        return parameter_list;
    }
    if (Identifier!=NULL) new_parameter->Identifier = Identifier->token;
    else new_parameter->Identifier = NULL;
    new_parameter->parameter = parameter;
    new_parameter->next = NULL;

    // Se a lista de parâmetros estiver vazia, o novo parâmetro se torna o primeiro da lista
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

    /*
    if(type==void_type && getchild(param_declaration,0)!=NULL){
        printf("Line %d, column %d: Invalid use of void type in declaration\n",tspec->token_line, tspec->token_column);
    }
    */

    return type;
}

//insere uma table na lista de tables
struct table_list *insert_table(struct table_list *table_list, struct table *new_table, char *token){
    struct table_list *new = (struct table_list *) malloc(sizeof(struct table_list));
    new->table = new_table;
    new->func_name = token;
    new->next = NULL;
    struct table_list *symbol = table_list;
    while(symbol != NULL) {
        if(symbol->next == NULL) {
            symbol->next = new;    /* insert new symbol at the tail of the list */
            break;
        } else if( (token!=NULL) && (symbol->next->func_name!=NULL) && (strcmp(symbol->next->func_name, token) == 0) ) {
            free(new);
            return NULL;           /* return NULL if symbol is already inserted */
        }
        symbol = symbol->next;
    }
    return new;
}

struct table *get_function_table(struct table_list *table_list, char *function_name) {
    // Itera através da lista de tabelas
    while (table_list != NULL) {
        // Verifica se o nome da função corresponde
        if ( function_name!= NULL && table_list->func_name != NULL && strcmp(table_list->func_name, function_name) == 0) {
            // Retorna a tabela de símbolos da função encontrada
            return table_list->table;
        }
        table_list = table_list->next;
    }
    // Retorna NULL se a função não for encontrada
    return NULL;
}
/*

//printa as tables
void show_tables() {

    struct table *table_functions;
    struct table *table_global;

    struct table *table_FuncDeclaration_aux;

    struct table_list *head = tables; // salva o inico das tableas de fucntion la lista

    while((tables = tables->next) != NULL){

        if(tables->func_name == NULL){
            printf("===== Global Symbol Table =====\n");        

            for (table_global = tables->table->next; table_global != NULL; table_global = table_global->next) {

                insert_symbol(table_global_store,table_global->identifier,table_global->type,global_table->parameter,tables->table->node);

                printf("%s\t%s", table_global->identifier, type_name(table_global->type));

                // Verificar se a lista de parâmetros não é nula
                if (table_global->parameter != NULL) {
                    struct parameter_list *param = table_global->parameter;

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
        printf("\n");
        }
    }

    tables = head;
}
*/


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
            printf("\n===== Function %s Symbol Table =====\n", tables->func_name);

            for (symbol = tables->table->next; symbol != NULL; symbol = symbol->next) {

                printf("%s\t%s", symbol->identifier, type_name(symbol->type));

                // Verificar se a lista de parâmetros não é nula
                if (symbol->parameter != NULL) {

                    printf("\tparam");
                }
                printf("\n");
            }
        }
    }
    printf("\n");   
}




/*
CHECKPOINT:
1 - fazer uma segunda versão da função search_symbol para ter uma igual a do Raul
2 - implementar solucao do Raul para ultima correção para o 70
3 - Construir a tree anotada



DÚVIDAS FRED, PQ O 35 NO IF DO FUNCTIONDEFINITION??

*/
