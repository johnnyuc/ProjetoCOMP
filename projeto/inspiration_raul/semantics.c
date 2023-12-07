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
    tables->next = NULL;
    //Perguntar se é preciso uma lista ligada para os parametros
    symbol_tableGlobal = (struct symbol_list *) malloc(sizeof(struct symbol_list));
    symbol_tableGlobal->next = NULL;

    struct parameter_list *params_putchar = create_parameter(integer_type);
    insert_symbol(symbol_tableGlobal,"putchar",integer_type,params_putchar,newnode(FuncDeclaration,NULL),0);
    
    struct parameter_list *params_getchar = create_parameter(void_type);
    insert_symbol(symbol_tableGlobal,"getchar",integer_type,params_getchar,newnode(FuncDeclaration,NULL),0);
    
    
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
        struct symbol_list *symbol=search_symbol(symbol_tableGlobal,funcdeclarator->token);
        enum type type =category_type(tspec->category);

        if(symbol == NULL) {
                struct node_list *child = paramlist->children;
                struct parameter_list *params=NULL;
                while((child = child->next) != NULL){
                    enum type para=check_parameters(child->node);
                    if (para!= no_type)
                        params=add_parameter(params,para,get_identifier(child->node));
                    else{
                        params=NULL; 
                        break;
                    }    
                }
                
                
                insert_symbol(symbol_tableGlobal,funcdeclarator->token,type,params,newnode(FuncDefinition,NULL),0);

                struct symbol_list *symbol_tableFunc = (struct symbol_list *) malloc(sizeof(struct symbol_list));
                symbol_tableFunc->next=NULL;
                symbol_tableFunc->identifier=funcdeclarator->token;

                insert_table(tables,symbol_tableFunc,funcdeclarator->token);

                insert_symbol(symbol_tableFunc,"return",type,NULL,newnode(Return,NULL),0); //Ver isto

                insert_params_to_symbol_table(symbol_tableFunc, params);

                struct node_list *body = funcbody->children;
                while((body = body->next) != NULL){
                    check_funcbody(body->node, symbol_tableFunc);
                }
            


        } 
        
        else if((symbol != NULL) && (search_table(tables,funcdeclarator->token) != NULL)){ 

                struct node_list *child = paramlist->children;
                struct parameter_list *params=NULL;
                struct parameter_list *params2=symbol->parameters;
                if(type!=search_symbol(symbol_tableGlobal,funcdeclarator->token)->type){
                    printf("Line %d, column %d: Symbol %s already defined\n",funcdeclarator->token_line,funcdeclarator->token_column,funcdeclarator->token);
                    semantic_errors++;
                }

                else{
                    struct symbol_list *symbol_tableFunc=search_table(tables,funcdeclarator->token)->list;
                    int paramiguais=0;

                    while((child = child->next) != NULL){
                        enum type para=check_parameters(child->node);
                        if (para!= no_type)
                            params=add_parameter(params,para,get_identifier(child->node));
                        else{
                            params=NULL; 
                            break;
                        }    
                    }

                    struct parameter_list *paramscopy=params;
                    while (paramscopy != NULL && params2 != NULL) {
                        // Verifica se os tipos dos parâmetros são diferentes
                        if (paramscopy->parameter != params2->parameter) {
                            //printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n",funcdeclarator->token_line,funcdeclarator->token_column,type_name(paramscopy->parameter),type_name(params2->parameter)); //trocar?
                            // Pode incrementar uma contagem de erros ou lidar de outra forma
                            semantic_errors++;
                            return;
                        }

                        // Move para o próximo par de parâmetros
                        paramscopy = paramscopy->next;
                        params2 = params2->next;
                    }

                    // Verifica se uma lista é mais longa que a outra
                    if (paramscopy != NULL || params2 != NULL) {
                        printf("Line %d, column %d: Wrong number of arguments to function %s (got %d, required %d)\n", funcdeclarator->token_line,funcdeclarator->token_column,funcdeclarator->token,count_parameters(paramscopy),count_parameters(params2));
                        semantic_errors++;
                    }
                    else{
                        symbol->node=newnode(FuncDefinition,NULL);
                        insert_table(tables,symbol_tableFunc,funcdeclarator->token);

                        insert_symbol(symbol_tableFunc,"return",type,NULL,newnode(Return,NULL),0); //Ver isto

                        insert_params_to_symbol_table(symbol_tableFunc, params);

                        struct node_list *body = funcbody->children;
                        while((body = body->next) != NULL){
                            check_funcbody(body->node, symbol_tableFunc);
                        }
                    }
            }
        }
    }


    if(node->category==FuncDeclaration){
        //FuncDeclaration tem 3 Filhos (TypeSpec FuncDeclarator e Param List)
        //ParamList tem os filhos ParamaDeclaration
        struct node *tspec = getchild(node,0);
        struct node *funcdeclarator = getchild(node,1);
        struct node *paramlist = getchild(node,2);
        struct symbol_list *symbol=search_symbol(symbol_tableGlobal,funcdeclarator->token);
        enum type type =category_type(tspec->category);

        if(symbol==NULL) {
                struct node_list *child = paramlist->children;
                struct parameter_list *params=NULL;

                while((child = child->next) != NULL){
                    enum type para=check_parameters(child->node);
                    if (para!= no_type)
                        params=add_parameter(params,para,get_identifier(child->node));
                    else{
                        params=NULL; 
                        break;
                    }    
                }
                
                insert_symbol(symbol_tableGlobal,funcdeclarator->token,type,params,newnode(FuncDeclaration,NULL),0);

                struct symbol_list *symbol_tableFunc = (struct symbol_list *) malloc(sizeof(struct symbol_list));
                symbol_tableFunc->next=NULL;
                symbol_tableFunc->identifier=funcdeclarator->token;
                
                insert_table(tables,symbol_tableFunc,funcdeclarator->token);
                
        } 

        else if(symbol != NULL){
                struct node_list *child = paramlist->children;
                struct parameter_list *params=NULL;
                struct parameter_list *params2=symbol->parameters;
                int paramiguais=0;
                
                while((child = child->next) != NULL){
                    enum type para=check_parameters(child->node);
                    if (para!= no_type)
                        params=add_parameter(params,para,get_identifier(child->node));
                    else{
                        params=NULL; 
                        break;
                    }    
                }

                struct parameter_list *paramscopy=params;
                while (paramscopy != NULL && params2 != NULL) {
                    // Verifica se os tipos dos parâmetros são diferentes
                    if (paramscopy->parameter != params2->parameter) {
                        //printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n",funcdeclarator->token_line,funcdeclarator->token_column,type_name(paramscopy->parameter),type_name(params2->parameter)); //trocar?
                        // Pode incrementar uma contagem de erros ou lidar de outra forma
                        semantic_errors++;
                        return;
                    }

                    // Move para o próximo par de parâmetros
                    paramscopy = paramscopy->next;
                    params2 = params2->next;
                }

                // Verifica se uma lista é mais longa que a outra
                if (paramscopy != NULL || params2 != NULL) {
                    printf("Line %d, column %d: Wrong number of arguments to function %s (got %d, required %d)\n", funcdeclarator->token_line,funcdeclarator->token_column,funcdeclarator->token,count_parameters(paramscopy),count_parameters(params2));
                    semantic_errors++;

                }else{
                    insert_symbol(symbol_tableGlobal,funcdeclarator->token,type,params,newnode(FuncDeclaration,NULL),0);
            }
        
        }else { 
            printf("Line %d, column %d: Symbol %s already defined\n",funcdeclarator->token_line,funcdeclarator->token_column,funcdeclarator->token);
            semantic_errors++;
        }
    }



    if(node->category==Declaration){

        struct node *tspec = getchild(node,0);
        struct node *declarator = getchild(node,1);
        struct node *expr= getchild(node,2);
        struct symbol_list *symbol=search_symbol(symbol_tableGlobal,declarator->token);
        enum type type =category_type(tspec->category);

        if(check_types(tspec)==1){ //Se for void
            printf("Line %d, column %d: Invalid use of void type in declaration\n",declarator->token_line, declarator->token_column);
            semantic_errors++;
        }
        
        else{
            if(symbol == NULL) { //Declarator
                
                //Acho que vai ser preciso fazer um check das expressions aqui
                insert_symbol(symbol_tableGlobal,declarator->token,type,NULL,newnode(Declaration,NULL),0);
                
            } else if(symbol!=NULL && type!=symbol->type) { //Declarator
                printf("Line %d, column %d: Symbol %s already defined\n",declarator->token_line,declarator->token_column,declarator->token);
                semantic_errors++;
            }
        }
    }
}

void check_funcbody(struct node *node, struct symbol_list *func_list){
    if(node->category==Declaration){

        struct node *tspec = getchild(node,0);
        struct node *declarator = getchild(node,1);
        struct node *expr= getchild(node,2);

        if(check_types(tspec)==1){ //Se for void
            printf("Line %d, column %d: Invalid use of void type in declaration\n",tspec->token_line, tspec->token_column);
            semantic_errors++;
        }
        
        else{
            if(search_symbol(func_list,declarator->token) == NULL) { //Declarator
                enum type type =category_type(tspec->category);
                declarator->type=tspec->type;
                insert_symbol(func_list,declarator->token,type,NULL,newnode(Declaration,NULL),0);

            } else { //Declarator
                printf("Line %d, column %d: Symbol %s already defined\n",declarator->token_line,declarator->token_column,declarator->token);
                semantic_errors++;
            }
        }
    }
    else{
        check_statements(node,func_list);
    }
}

void check_statements(struct node *node, struct symbol_list *func_list){
    if(node->category==If){
        struct node *expression = getchild(node,0);
        struct node *statementesp = getchild(node,1);
        struct node *statementespelse = getchild(node,2);
        enum type type =category_type(expression->category);
        check_expression(expression,func_list);
        check_statements(statementesp,func_list);
        if (statementespelse->category != Null){
            check_statements(statementespelse, func_list);
        }
    }

    else if(node->category==While){
        struct node *expression = getchild(node,0);
        struct node *statementesp = getchild(node,1);
        enum type type =category_type(expression->category);
        check_expression(expression,func_list);
        if (statementesp->category != Null){
            check_statements(statementesp, func_list);
        }
    }

    else if(node->category==Return){
        //Lidar com o erro Line %d, column %d: Conflicting types (got %s, expected %s)\n Tem de ser nas expressions
        struct node *expression = getchild(node,0);
        enum type type =category_type(expression->category);
        check_expression(expression,func_list);   
    }

    else if(node->category==StatList){
        struct node_list *statements=node->children;
        while((statements = statements->next) != NULL){
            check_statements(statements->node,func_list); 
        }
    }
    else{
        check_expression(node,func_list);
    }

}

//Acho que a função vai ter de ser enum type para se poder fazer o erro do return
void check_expression(struct node *expression, struct symbol_list *func_list) {
    switch(expression->category) {
        case Identifier:
            if(search_symbol(func_list, expression->token) == NULL && search_symbol(symbol_tableGlobal, expression->token) == NULL) {
                printf("Line %d, column %d: Unknown symbol %s\n",expression->token_line, expression->token_column,expression->token);
                semantic_errors++;
            } 
            else if (search_symbol(func_list, expression->token) != NULL) {
                expression->type = search_symbol(func_list, expression->token)->type;
            }
            else if(search_symbol(symbol_tableGlobal, expression->token) != NULL){
                expression->type = search_symbol(symbol_tableGlobal, expression->token)->type;
            }

            break;

        case Natural:
            expression->type = integer_type;
            break;
        case ChrLit:
            expression->type = integer_type;
            break;
        case Decimal:
            expression->type = double_type;
            break;
        
        case Store:
        case Add:
        case Sub:
        case Mul:
        case Div:
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
            check_expression(getchild(expression, 0), func_list);
            check_expression(getchild(expression, 1), func_list);
            break;
        
        case Plus:
        case Minus:
        case Not:
            check_expression(getchild(expression, 0), func_list);
            break;

        case Comma:
            check_expression(getchild(expression, 0), func_list);
            check_expression(getchild(expression, 1), func_list);
            break;
            
        case Call:
            if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token) == NULL) {
                printf("Line %d, column %d: Unknown symbol %s\n",expression->token_line, expression->token_column,expression->token);
                semantic_errors++;
            } else {
                struct node *parameters = getchild(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node, 1);
                struct node_list *arguments = expression->children;

                    if (arguments!=NULL){
                        while((arguments = arguments->next) != NULL){
                            check_expression(arguments->node, func_list);
                        }
                    }
                }
            break;
        default:
            break;
    }
}

enum type check_parameters(struct node *paramdeclaration) {
    struct node *tspec = getchild(paramdeclaration,0);
    enum type type =category_type(tspec->category);
    if(type==void_type && getchild(paramdeclaration,1)!=NULL){
        printf("Line %d, column %d: Invalid use of void type in declaration\n",tspec->token_line, tspec->token_column);
        semantic_errors++;
        return no_type;
    }

    return type;
}

char *get_identifier(struct node *paramdeclaration){
    struct node *tspec = getchild(paramdeclaration,0);
    enum type type =category_type(tspec->category);
    if(type!=void_type && getchild(paramdeclaration,1)!=NULL){
        struct node *id = getchild(paramdeclaration,1);
        return id->token;  
    }
    return NULL; 
}


// Cria um novo nó de parâmetro e o adiciona à lista
struct parameter_list *add_parameter(struct parameter_list *list, enum type parameter, char *name) {
    // Aloca memória para o novo nó de parâmetro
    struct parameter_list *new_param = malloc(sizeof(struct parameter_list));
    if (new_param == NULL) {
        // Lida com falha na alocação de memória, por exemplo, retornando ou saindo
        return list;
    }

    // Inicializa o campo de nome
    if (name != NULL) {
        new_param->name = strdup(name);
    } else if(name == NULL && parameter!=void_type) {
        //printf("Unexpected error: Parameter symbol required\n");
        new_param->name = NULL;
    } else if(name != NULL && parameter==void_type){
        new_param->name = NULL;
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

void insert_params_to_symbol_table(struct symbol_list *symbol_tableFunc, struct parameter_list *params) {
    while (params != NULL) {
        // Assuming you have some default node for parameters, modify as needed

        // Check if the parameter type is not void before inserting
        if (params->parameter != void_type && params->name!=NULL) {
            // Insert the parameter into the symbol table
            symbol_tableFunc = insert_symbol(symbol_tableFunc,params->name, params->parameter, NULL, NULL,1);
        }
        params = params->next; // Move to the next parameter
    }
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
struct symbol_list *insert_symbol(struct symbol_list *table, char *identifier, enum type type, struct parameter_list *parameters, struct node *node, int is_parameter) {
    struct symbol_list *new = (struct symbol_list *) malloc(sizeof(struct symbol_list));
    if (identifier!=NULL)new->identifier = strdup(identifier);
    else new->identifier=NULL;
    new->type = type;
    new->parameters = parameters;
    new->node = node;
    new->is_parameter = is_parameter;  // Adiciona o novo campo is_parameter
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
        } else if((token !=NULL) && (symbol->next->func_name != NULL) && (strcmp(symbol->next->func_name, token) == 0)) {
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

struct symbol_tables *search_table(struct symbol_tables *table, char *token) {
    struct symbol_tables *symbol;
    for(symbol = table->next; symbol != NULL; symbol = symbol->next)
        if(strcmp(symbol->func_name, token) == 0)
            return symbol;
    return NULL;
}

void show_symbol_table() {
    struct symbol_list *symbol;
    printf("===== Global Symbol Table =====\n");
    for (symbol = symbol_tableGlobal->next; symbol != NULL; symbol = symbol->next) {
        printf("%s\t%s", symbol->identifier, type_name(symbol->type));

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
        if(search_symbol(symbol_tableGlobal,tables->func_name)->node->category==FuncDefinition)
        show_symbol_table_func();
    }
printf("\n");
}

void show_symbol_table_func() {
    struct symbol_list *symbol;
    printf("\n===== Function %s Symbol Table =====\n", tables->func_name);

    // Print the return symbol first
    struct symbol_list *return_symbol = search_symbol(tables->list, "return");
    if (return_symbol != NULL) {
        printf("%s\t%s", return_symbol->identifier, type_name(return_symbol->type));
        printf("\n");
    }

    // Print the rest of the symbols
    for (symbol = tables->list->next; symbol != NULL; symbol = symbol->next) {
        if (strcmp(symbol->identifier, "return") != 0) {
            printf("%s\t%s", symbol->identifier, type_name(symbol->type));
            
            if(symbol->is_parameter==1){
                printf("\t%s","param");
            }

            printf("\n");
        }
    }
}

char *category_name2[] = names;

// traverse the AST and print its content
void show_new(struct node *node, int depth) {
    if (node == NULL) return; // if tree is empty
    int i;
    for (i = 0; i < depth; i++)
        printf("..");
    if (node->token == NULL){
        printf("%s\n", category_name2[node->category]);
        /*
        if(node->category >= 11){
            printf(" - %s",type_name(node->type));
        }
        printf("\n");
        */
    }
    else{
        printf("%s(%s) - %s\n", category_name2[node->category], node->token, type_name(node->type));
        /*
        if(node->type != no_type){
            printf(" - %s",type_name(node->type));
        }
        printf("\n");
        */
    }
    // Iterate over the children of the current node
    struct node_list *child = node->children;
    while (child != NULL) {
        show_new(child->node, depth + 1);
        child = child->next;
    }

    // Iterate over the brothers of the current node
    struct node_list *brother = node->brothers;
    while (brother != NULL) {
        show_new(brother->node, depth);
        brother = brother->next;
    }
}


