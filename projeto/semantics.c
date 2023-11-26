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
    check_node(program);

    return semantic_errors;
}

void check_node(struct node *node) {
    
    if (node == NULL) return;

    //node = Declaration
    if(node->category == Declaration) {
        check_Declaration(node,global_table);
    }
    
    //node = FuncDefinition
    if (node->category == FuncDefinition){
        check_FuncDefinition(node);
    }

    //node = FuncDeclaration
    if (node->category == FuncDeclaration){
        //COMPLETAR
    }

    // Percorre a árvore
    if(node->children!=NULL){
        struct node_list *child = node->children;
        while((child = child->next) != NULL){
            check_node(child->node);
        }
    }
}

void check_Declaration(struct node *node, struct table *table){

    struct node *tspec = getchild(node,0);
    struct node *declarator = getchild(node, 1);

    //trata de um caso de erro específico
    if(tspec->category==Void){
        printf("Line %d, col %d: Invalid use of void type in declaration\n",tspec->token_line, tspec->token_column);
        semantic_errors++;
    }

    else{

        //Verifica a existencia do novo simbolo na tabela

        //se não tiver ainda, adiciona
        if( (declarator != NULL) && (search_symbol(table, declarator->token) == NULL) ) {
            enum type type =category_type(tspec->category);
            insert_symbol(table, declarator->token,type,NULL, NULL);

        } 

        //se já estiver, printa a mensagem de erro e itera sobre semantic errors
        else if( ( declarator != NULL) && (search_symbol(table, declarator->token) != NULL) ) {
            printf("Line %d, column %d: Symbol %s already defined\n", declarator->token_line, declarator->token_column,declarator->token);
            semantic_errors++;
        }

    }
    
}

void check_FuncDefinition(struct node *node){

    //declaração dos filhos do node FuncDefinition
    struct node *tspec = getchild(node,0);
    struct node *func_declarator = getchild(node,1);
    struct node *param_list = getchild(node,2);
    struct node *func_body = getchild(node,3);

    //Declaração "global" do type, pois usamos em func_declarator e func_body
    enum type type = category_type(tspec->category);
    
    //Verifica se o novo simbolo já se encontra na Table

    //Caso não esteja adiciona o novo símbolo
    if(search_symbol(global_table, func_declarator->token) == NULL) {

        //agupa os parametros do novo simbolo em uma lista de parametros auxiliar
        struct node_list *parameter_node = param_list->children;
        struct parameter_list *parameter_list_aux = NULL;

        while((parameter_node = parameter_node->next) != NULL){
            parameter_list_aux= add_parameter(parameter_list_aux, check_parameter(parameter_node->node));
        }

        //insere o novo simbolo na global table
        insert_symbol(global_table, func_declarator->token ,type, parameter_list_aux, NULL);

        //criação das tables para cada nova função
        struct table *new_symble_table = (struct table *) malloc(sizeof(struct table));
        new_symble_table->next = NULL;

        //inserçao da nova table na lista de tables
        insert_table(tables,new_symble_table,func_declarator->token);

        //itera sobre o corpo da funcao para adicionar novos simbolos na table da funcao 
        struct node_list *body = func_body->children;
        while((body = body->next) != NULL){

            check_Declaration(body->node,new_symble_table);

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
            symbol = search_symbol(global_table, Identifier->token);
            
            printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n",Identifier->token_line,Identifier->token_column,type_name(symbol->type),type_name(void_type));
            semantic_errors++;
        }

    }

}

//insere um novo simbolo na table
struct table *insert_symbol(struct table *table, char *identifier,enum type type, struct parameter_list *v_type, struct node *node) {
    struct table *new = (struct table *) malloc(sizeof(struct table));
    if(identifier!=NULL){
        new->identifier = strdup(identifier);
    }
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
struct table *search_symbol(struct table *table, char *identifier) {
    struct table *symbol;
    for(symbol = table->next; symbol != NULL; symbol = symbol->next)

        if( (identifier!=NULL) && (symbol->identifier!= NULL) && (strcmp(symbol->identifier, identifier) == 0) ){
            return symbol;
        }
    return NULL;
}

//registra um novo parametro
struct parameter_list *register_parameter(enum type parameter){

    struct parameter_list *new_parameter = (struct parameter_list *)malloc(sizeof(struct parameter_list));
    new_parameter->parameter = parameter;
    new_parameter->next=NULL;
    return new_parameter;
}

//adiciona um novo paramatero a lista de parametros
struct parameter_list *add_parameter(struct parameter_list *add_parametermeter_list,enum type parameter){

    //cria o novo parâmetro
    struct parameter_list *new_parameter = (struct parameter_list *)malloc(sizeof(struct parameter_list));
    new_parameter = register_parameter(parameter);

    // Se a lista de parametros estiver vazia, o novo parametro se torna o primeiro da lista
    if (add_parametermeter_list == NULL) {
        add_parametermeter_list = new_parameter;
    } 

    //Se não, percorremos a lista e adicionamos o novo parametro na útlima posição
    else{
        while (add_parametermeter_list->next != NULL) {
            add_parametermeter_list = add_parametermeter_list->next;
        }
        add_parametermeter_list->next = new_parameter;
    }

    return add_parametermeter_list;    
}

enum type check_parameter(struct node *param_declaration) {
    struct node *tspec = getchild(param_declaration,0);
    enum type type =category_type(tspec->category);

    if(type==void_type && getchild(param_declaration,1)!=NULL){
        printf("Line %d, col %d: Invalid use of void type in declaration\n",tspec->token_line, tspec->token_column);
    }   

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
        } else if( /*(token!=NULL)&& */ (symbol->next->func_name!=NULL) && (strcmp(symbol->next->func_name, token) == 0) ) {
            free(new);
            return NULL;           /* return NULL if symbol is already inserted */
        }
        symbol = symbol->next;
    }
    return new;
}

//printa as tables
void show_tables() {
    struct table *symbol;

   while((tables = tables->next) != NULL){

    if(tables->func_name == NULL){
         printf("===== Global Symbol Table =====\n");
    }

    else{
        printf("\n===== Function %s Symbol Table =====\n", tables->func_name);
    }
        
        for (symbol = tables->table->next; symbol != NULL; symbol = symbol->next) {

            printf("%s  %s", symbol->identifier, type_name(symbol->type));

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
}
