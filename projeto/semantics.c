#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantics.h"

int semantic_errors = 0;

struct symbol_list *symbol_table;


param_list* register_param(char* type) {
    param_list* param = (param_list *) malloc(sizeof(param_list));
    param->type = type;
    param->next_param = NULL;
    return param;
}

void check_expression(struct node *expression, struct symbol_list *scope) {
    switch(expression->category) {
        case Identifier:
            if(search_symbol(scope, expression->token) == NULL) {
                printf("Variable %s (%d:%d) undeclared\n", expression->token, expression->token_line, expression->token_column);
                semantic_errors++;
            } else {
                expression->type = search_symbol(scope, expression->token)->type;
            }
            break;
        case Natural:
            expression->type = integer_type;
            break;
        case Decimal:
            expression->type = double_type;
            break;
        case Call:
            if(search_symbol(symbol_table, getchild(expression, 0)->token) == NULL || search_symbol(symbol_table, getchild(expression, 0)->token)->node->category != FuncDeclaration) {
                printf("Function %s (%d:%d) undeclared\n", getchild(expression, 0)->token, getchild(expression, 0)->token_line, getchild(expression, 0)->token_column);
                semantic_errors++;
            } else {
                struct node *arguments = getchild(expression, 1);
                struct node *parameters = getchild(search_symbol(symbol_table, getchild(expression, 0)->token)->node, 1);
                if(parameters != NULL && countchildren(arguments) != countchildren(parameters)) {
                    printf("Calling %s (%d:%d) with incorrect arguments\n", getchild(expression, 0)->token, getchild(expression, 0)->token_line, getchild(expression, 0)->token_column);
                    semantic_errors++;
                } else {
                    struct node_list *argument = arguments->children;
                    while((argument = argument->next) != NULL)
                        check_expression(argument->node, scope);
                }
            }
            break;
        case If:
            check_expression(getchild(expression, 0), scope);
            check_expression(getchild(expression, 1), scope);
            check_expression(getchild(expression, 2), scope);
            break;
        case Add:
        case Sub:
        case Mul:
        case Div:
            check_expression(getchild(expression, 0), scope);
            check_expression(getchild(expression, 1), scope);
            break;
        default:
            break;
    }
}

void check_parameters(struct node *parameters, struct symbol_list *scope) {
    struct node_list *parameter = parameters->children;
    while((parameter = parameter->next) != NULL) {
        struct node *id = getchild(parameter->node, 1);
        enum type type = category_type(getchild(parameter->node, 0)->category);
        if(search_symbol(symbol_table, id->token) == NULL) {
            insert_symbol(symbol_table, id->token, type, NULL, parameter->node);
            insert_symbol(scope, id->token, type, NULL, parameter->node);
        } else {
            printf("Identifier %s (%d:%d) already declared\n", id->token, id->token_line, id->token_column);
            semantic_errors++;
        }
    }
}

void check_function(struct node *function) {
    struct node *id = getchild(function, 1);
    if(search_symbol(symbol_table, id->token) == NULL) {
        insert_symbol(symbol_table, id->token, no_type, NULL, function);
    } else {
        printf("Identifier %s (%d:%d) already declared\n", id->token, id->token_line, id->token_column);
        semantic_errors++;
    }
    struct symbol_list *scope = (struct symbol_list *) malloc(sizeof(struct symbol_list));
    scope->next_symbol = NULL;
    //check_parameters(getchild(function, 1), scope);
    //check_expression(getchild(function, 0), scope);
    /* ToDo: scope should be free'd */
}

void check_nodes(struct node *node) {
    if (node == NULL) return;

    if (node->category == FuncDefinition || node->category == Declaration) {
        check_function(node);
    }
    // Percorre os filhos
    if(node->children!=NULL){
        struct node_list *child = node->children;
        while((child = child->next) != NULL){
            check_nodes(child->node);
        }
    }

    if(node->brothers!= NULL){
        struct node_list *brother = node->brothers;
        while ((brother = brother->next) != NULL) {
            check_nodes(brother->node);
        }
    }
}

// semantic analysis begins here, with the AST root node
int check_program(struct node *program) {
    symbol_table = (struct symbol_list *) malloc(sizeof(struct symbol_list));
    symbol_table->next_symbol = NULL;

    insert_symbol(symbol_table, "putchar", integer_type, register_param("int"), newnode(FuncDeclaration, NULL)); /* predeclared functions (no children) */
    insert_symbol(symbol_table, "getchar", integer_type, register_param("void"), newnode(FuncDeclaration, NULL));

    check_nodes(program);

    return semantic_errors;
}

// insert a new symbol in the list, unless it is already there
struct symbol_list *insert_symbol(struct symbol_list *table, char *identifier, enum type type, param_list *param, struct node *node) {
    struct symbol_list *new = (struct symbol_list *) malloc(sizeof(struct symbol_list));
    new->identifier = strdup(identifier);
    new->type = type;
    new->node = node;
    new->next_symbol = NULL;
    new->next_param = param;
    struct symbol_list *symbol = table;
    while(symbol != NULL) {
        if(symbol->next_symbol == NULL) {
            symbol->next_symbol = new;    /* insert new symbol at the tail of the list */
            break;
        } else if(strcmp(symbol->next_symbol->identifier, identifier) == 0) {
            free(new);
            return NULL;           /* return NULL if symbol is already inserted */
        }
        symbol = symbol->next_symbol;
    }
    return new;
}

// look up a symbol by its identifier
struct symbol_list *search_symbol(struct symbol_list *table, char *identifier) {
    struct symbol_list *symbol;
    for(symbol = table->next_symbol; symbol != NULL; symbol = symbol->next_symbol)
        if(strcmp(symbol->identifier, identifier) == 0)
            return symbol;
    return NULL;
}

void show_symbol_table() {
    struct symbol_list *symbol;
    printf("===== Global Symbol Table =====\n");
    for(symbol = symbol_table->next_symbol; symbol != NULL; symbol = symbol->next_symbol) { 
         if(symbol->next_param != NULL) {
             printf("%s %s(%s)\n", symbol->identifier, type_name(symbol->type), symbol->next_param->type);
        }
        else{
            printf("%s %s\n", symbol->identifier, type_name(category_type(getchild(symbol->node, 0)->category)));
        }
    }
}