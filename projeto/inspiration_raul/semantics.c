#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantics.h"

int semantic_errors = 0;

struct symbol_tables *tables;
struct symbol_list *symbol_tableGlobal;

int check_program(struct node *program){
    tables = (struct symbol_tables *) malloc(sizeof(struct symbol_tables));
    //Perguntar se é preciso uma lista ligada para os parametros
    symbol_tableGlobal = (struct symbol_list *) malloc(sizeof(struct symbol_list));

    struct parameter_list *params_putchar = create_parameter(integer_type);
    insert_symbol(symbol_tableGlobal,"putchar",integer_type,params_putchar,NULL);
    
    struct parameter_list *params_getchar = create_parameter(void_type);
    insert_symbol(symbol_tableGlobal,"getchar",integer_type,params_getchar,NULL);
    
    
    if (program == NULL || program->children == NULL) {
        return 0;
    }


    struct node_list *child = program->children;
    while((child = child->next) != NULL){
        check_node(child->node);
    }
    return semantic_errors;
}


void check_node(struct node *node){
    if(node->category==FuncDefinition) {
        struct node *tspec = getchild(node,0);
        struct node *funcdeclarator = getchild(node,1);
        struct node *paramlist = getchild(node,2);
        struct node *funcbody = getchild(node,3);

        if(search_symbol(symbol_tableGlobal,funcdeclarator->token) == NULL) {
                enum type type =category_type(tspec->category);
                struct node_list *child = paramlist->children;
                struct parameter_list *params=NULL;
                while((child = child->next) != NULL){
                    params=add_parameter(params,check_parameters(child->node));
                }
                
                insert_symbol(symbol_tableGlobal,funcdeclarator->token,type,params,NULL);

                struct symbol_list *symbol_tableFunc = (struct symbol_list *) malloc(sizeof(struct symbol_list));

                insert_table(tables,symbol_tableFunc,funcdeclarator->token);

                struct node_list *body = funcbody->children;
                while((body = body->next) != NULL){
                    check_funcbody(body->node, symbol_tableFunc);
                }
                
        } else { 
                printf("Symbol %s already defined\n",funcdeclarator->token);
                semantic_errors++;
        }
    }


    if(node->category==FuncDeclaration){
        //FuncDeclaration tem 3 Filhos (TypeSpec FuncDeclarator e Param List)
        //ParamList tem os filhos ParamaDeclaration
        struct node *tspec = getchild(node,0);
        struct node *funcdeclarator = getchild(node,1);
        struct node *paramlist = getchild(node,2);

        if(search_symbol(symbol_tableGlobal,funcdeclarator->token) == NULL) {
                enum type type =category_type(tspec->category);
                struct node_list *child = paramlist->children;
                struct parameter_list *params=NULL;
                while((child = child->next) != NULL){
                    params=add_parameter(params,check_parameters(child->node));
                }
                
                insert_symbol(symbol_tableGlobal,funcdeclarator->token,type,params,NULL);
        } else { 
                printf("Symbol %s already defined\n",funcdeclarator->token);
                semantic_errors++;
        }
    }



    if(node->category==Declaration){

        struct node *tspec = getchild(node,0);
        struct node *declarator = getchild(node,1);

        if(check_types(tspec)==1){ //Se for void
            printf("Line %d, col %d: Invalid use of void type in declaration\n",tspec->token_line, tspec->token_column);
            semantic_errors++;
        }
        
        else{
            if(search_symbol(symbol_tableGlobal,declarator->token) == NULL) { //Declarator
                enum type type =category_type(tspec->category);
                insert_symbol(symbol_tableGlobal,declarator->token,type,NULL,NULL);
            } else { //Declarator
                printf("Symbol %s already defined\n",declarator->token);
                semantic_errors++;
            }
        }
    }
}

void check_funcbody(struct node *node, struct symbol_list *func_list){
    if(node->category==Declaration){

        struct node *tspec = getchild(node,0);
        struct node *declarator = getchild(node,1);

        if(check_types(tspec)==1){ //Se for void
            printf("Line %d, col %d: Invalid use of void type in declaration\n",tspec->token_line, tspec->token_column);
            semantic_errors++;
        }
        
        else{
            if(search_symbol(func_list,declarator->token) == NULL) { //Declarator
                enum type type =category_type(tspec->category);
                insert_symbol(func_list,declarator->token,type,NULL,NULL);
            } else { //Declarator
                printf("Symbol %s already defined\n",declarator->token);
                semantic_errors++;
            }
        }
    }
}

enum type check_parameters(struct node *paramdeclaration) {
    struct node *tspec = getchild(paramdeclaration,0);
    enum type type =category_type(tspec->category);
    if(type==void_type && getchild(paramdeclaration,1)!=NULL){
        printf("Line %d, col %d: Invalid use of void type in declaration\n",tspec->token_line, tspec->token_column);
    }   
    return type;
}


// Cria um novo nó de parâmetro e o adiciona à lista
struct parameter_list *add_parameter(struct parameter_list *list, enum type parameter) {
    // Aloca memória para o novo nó de parâmetro
    struct parameter_list *new_param = malloc(sizeof(struct parameter_list));
    if (new_param == NULL) {
        // Lida com falha na alocação de memória, por exemplo, retornando ou saindo
        return list;
    }

    // Preenche os dados do novo nó
    new_param->parameter = parameter;
    new_param->next = NULL;

    // Adiciona o novo nó à lista existente
    if (list == NULL) {
        // Se a lista estiver vazia, o novo nó se torna o primeiro nó
        list = new_param;
    } else {
        // Caso contrário, encontra o último nó na lista e adiciona o novo nó como próximo
        struct parameter_list *last_param = list;
        while (last_param->next != NULL) {
            last_param = last_param->next;
        }
        last_param->next = new_param;
    }

    return list;
}

int check_types(struct node *node){
  struct node* aux = node;
  //printf("%s\n",node->str);
  if(aux->category==Void){
    return 1;
  }
  return 0;
}

// Função para contar o número de elementos em uma lista ligada
int count_elements(struct node_list *list) {
    int count = 0;
    while (list != NULL) {
        count++;
        list = list->next;
    }
    return count;
}

// Função para contar o número de elementos em uma lista ligada
int count_parameters(struct parameter_list *list) {
    int count = 0;
    while (list != NULL) {
        count++;
        list = list->next;
    }
    return count;
}


struct parameter_list *create_parameter(enum type parameter) {
    struct parameter_list *new_parameter = (struct parameter_list *)malloc(sizeof(struct parameter_list));
    new_parameter->parameter = parameter;
    new_parameter->next = NULL;
    return new_parameter;
}

// insert a new symbol in the list, unless it is already there
struct symbol_list *insert_symbol(struct symbol_list *table, char *identifier, enum type type, struct parameter_list *parameters, struct node *node) {
    struct symbol_list *new = (struct symbol_list *) malloc(sizeof(struct symbol_list));
    new->identifier = strdup(identifier);
    new->type = type;
    new->parameters= parameters;
    new->node = node;
    new->next = NULL;
    struct symbol_list *symbol = table;
    while(symbol != NULL) {
        if(symbol->next == NULL) {
            symbol->next = new;    /* insert new symbol at the tail of the list */
            break;
        } else if(strcmp(symbol->next->identifier, identifier) == 0) {
            free(new);
            return NULL;           /* return NULL if symbol is already inserted */
        }
        symbol = symbol->next;
    }
    return new;
}

struct symbol_tables *insert_table(struct symbol_tables *table_list, struct symbol_list *list, char *token){
    struct symbol_tables *new = (struct symbol_tables *) malloc(sizeof(struct symbol_tables));
    new->list = list;
    new->func_name = token;
    new->next = NULL;
    struct symbol_tables *symbol = table_list;
    while(symbol != NULL) {
        if(symbol->next == NULL) {
            symbol->next = new;    /* insert new symbol at the tail of the list */
            break;
        } else if(strcmp(symbol->next->func_name, token) == 0) {
            free(new);
            return NULL;           /* return NULL if symbol is already inserted */
        }
        symbol = symbol->next;
    }
    return new;
}

// look up a symbol by its identifier
struct symbol_list *search_symbol(struct symbol_list *table, char *identifier) {
    struct symbol_list *symbol;
    for(symbol = table->next; symbol != NULL; symbol = symbol->next)
        if(strcmp(symbol->identifier, identifier) == 0)
            return symbol;
    return NULL;
}

void show_symbol_table() {
    struct symbol_list *symbol;
    printf("===== Global Symbol Table =====\n");
    for (symbol = symbol_tableGlobal->next; symbol != NULL; symbol = symbol->next) {
        printf("%s  %s", symbol->identifier, type_name(symbol->type));

        // Verificar se a lista de parâmetros não é nula
        if (symbol->parameters != NULL) {
            struct parameter_list *param = symbol->parameters;

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
    while((tables = tables->next) != NULL){
        show_symbol_table_func();
    }
}

void show_symbol_table_func() {
    struct symbol_list *symbol;
    printf("\n===== Function %s Symbol Table =====\n", tables->func_name);
        for (symbol = tables->list->next; symbol != NULL; symbol = symbol->next) {
            printf("%s  %s", symbol->identifier, type_name(symbol->type));

            // Verificar se a lista de parâmetros não é nula
            if (symbol->parameters != NULL) {
                struct parameter_list *param = symbol->parameters;

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
