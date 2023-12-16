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
                    if (para!= no_type){
                        params=add_parameter(params,para,get_identifier(child->node));
                        if(para==void_type && countchildren(paramlist)>1){
                            struct node *sitio=getchild(child->node,0);
                            printf("Line %d, column %d: Invalid use of void type in declaration\n",sitio->token_line,sitio->token_column);
                            semantic_errors++;
                            params=NULL;
                            break;
                        }
                        else{
                            if(get_identifier(child->node)!=NULL)
                            if(is_parameter_name_used(params,get_identifier(child->node))==1){
                                struct node *sitio=getchild(child->node,1);
                                printf("Line %d, column %d: Symbol %s already defined\n",sitio->token_line,sitio->token_column,get_identifier(child->node));
                                
                            }
                        }
                        
                    }
                    else{
                        params=NULL; 
                        break;
                    }    
                }

                
                if(params!=NULL){
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
            


        } else if((symbol != NULL) && (search_table(tables,funcdeclarator->token) == NULL)){
                //Não sei se é preciso
            }
        
        else if((symbol != NULL) && (search_table(tables,funcdeclarator->token) != NULL)){ 
                struct node_list *child = paramlist->children;
                struct parameter_list *params=NULL;
                struct parameter_list *params2=symbol->parameters;
                struct parameter_list *params2_start = params2;

                    struct symbol_list *symbol_tableFunc=search_table(tables,funcdeclarator->token)->list;
                    int paramiguais=0;

                if(symbol->node->category==FuncDefinition){
                    printf("Line %d, column %d: Symbol %s already defined\n",funcdeclarator->token_line,funcdeclarator->token_column,funcdeclarator->token);
                    semantic_errors++;
                    return;
                }

                    while((child = child->next) != NULL){
                    enum type para=check_parameters(child->node);
                    if (para!= no_type){
                        params=add_parameter(params,para,get_identifier(child->node));
                        if(para==void_type && countchildren(paramlist)>1){
                            struct node *sitio=getchild(child->node,0);
                            printf("Line %d, column %d: Invalid use of void type in declaration\n",sitio->token_line,sitio->token_column);
                            semantic_errors++;
                            params=NULL;
                            break;
                        }
                        else{
                            if(get_identifier(child->node)!=NULL)
                            if(is_parameter_name_used(params,get_identifier(child->node))==1){
                                struct node *sitio=getchild(child->node,1);
                                printf("Line %d, column %d: Symbol %s already defined\n",sitio->token_line,sitio->token_column,get_identifier(child->node));
                                break;
                            }
                        }
                        
                    }
                    else{
                        params=NULL; 
                        break;
                    }    
                }

                    if(type!=search_symbol(symbol_tableGlobal,funcdeclarator->token)->type){
                    struct parameter_list *params3=symbol->parameters;
                    struct parameter_list *paramscopy2=params;
                    printf("Line %d, column %d: Conflicting types (got %s(", funcdeclarator->token_line, funcdeclarator->token_column, type_name(type));

                    // Imprime os tipos da lista mais longa
                    while (paramscopy2 != NULL) {
                        printf("%s", type_name(paramscopy2->parameter));
                        paramscopy2 = paramscopy2->next;

                        // Adiciona vírgula se houver mais tipos a serem impressos
                        if (paramscopy2 != NULL) {
                            printf(",");
                        }
                    }

                    // Imprime a mensagem indicando que há tipos esperados
                    printf("), expected %s(", type_name(symbol->type));

                    while (params3 != NULL) {
                        printf("%s", type_name(params3->parameter));
                        params3 = params3->next;

                        // Adiciona vírgula se houver mais tipos a serem impressos
                        if (params3 != NULL) {
                            printf(",");
                        }
                    }

                    printf("))\n");

                    semantic_errors++;
                    return;
                }

                    if(params!=NULL){
                        struct parameter_list *paramscopy=params;
                        struct parameter_list *paramscopy_start = paramscopy;
                        while (paramscopy != NULL && params2 != NULL) {
                            // Move para o próximo par de parâmetros
                            if(paramscopy->parameter!=params2->parameter){
                                paramscopy = paramscopy_start;
                                params2 = params2_start;
                                printf("Line %d, column %d: Conflicting types (got %s(", funcdeclarator->token_line, funcdeclarator->token_column, type_name(type));

                            // Imprime os tipos da lista mais longa
                            while (paramscopy != NULL) {
                                printf("%s", type_name(paramscopy->parameter));
                                paramscopy = paramscopy->next;

                                // Adiciona vírgula se houver mais tipos a serem impressos
                                if (paramscopy != NULL) {
                                    printf(",");
                                }
                            }

                            // Imprime a mensagem indicando que há tipos esperados
                            printf("), expected %s(", type_name(symbol->type));

                            while (params2 != NULL) {
                                printf("%s", type_name(params2->parameter));
                                params2 = params2->next;

                                // Adiciona vírgula se houver mais tipos a serem impressos
                                if (params2 != NULL) {
                                    printf(",");
                                }
                            }

                            printf("))\n");

                            semantic_errors++;
                            return;
                            }
                            paramscopy = paramscopy->next;
                            params2 = params2->next;
                        }

// Verifica se uma lista é mais longa que a outra
        if (paramscopy != NULL || params2 != NULL) {
            paramscopy = paramscopy_start;
            params2 = params2_start;
                    printf("Line %d, column %d: Conflicting types (got %s(", funcdeclarator->token_line, funcdeclarator->token_column, type_name(type));

                    // Imprime os tipos da lista mais longa
                    while (paramscopy != NULL) {
                        printf("%s", type_name(paramscopy->parameter));
                        paramscopy = paramscopy->next;

                        // Adiciona vírgula se houver mais tipos a serem impressos
                        if (paramscopy != NULL) {
                            printf(",");
                        }
                    }

                    // Imprime a mensagem indicando que há tipos esperados
                    printf("), expected %s(", type_name(symbol->type));

                    while (params2 != NULL) {
                        printf("%s", type_name(params2->parameter));
                        params2 = params2->next;

                        // Adiciona vírgula se houver mais tipos a serem impressos
                        if (params2 != NULL) {
                            printf(",");
                        }
                    }

                    printf("))\n");

                    semantic_errors++;
                    return;
        }

                        if ((paramscopy == NULL || params2 == NULL) && symbol_tableFunc->body==0) {
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
                    if (para!= no_type){
                        params=add_parameter(params,para,get_identifier(child->node));
                        if(para==void_type && countchildren(paramlist)>1){
                            struct node *sitio=getchild(child->node,0);
                            printf("Line %d, column %d: Invalid use of void type in declaration\n",sitio->token_line,sitio->token_column);
                            semantic_errors++;
                            params=NULL;
                            break;
                        }
                        else{
                            if(get_identifier(child->node)!=NULL)
                            if(is_parameter_name_used(params,get_identifier(child->node))==1){
                                struct node *sitio=getchild(child->node,1);
                                printf("Line %d, column %d: Symbol %s already defined\n",sitio->token_line,sitio->token_column,get_identifier(child->node));
                                break;
                            }
                        }
                        
                    }
                    else{
                        params=NULL; 
                        break;
                    }    
                }
                
                
               if(params!=NULL){
                    insert_symbol(symbol_tableGlobal,funcdeclarator->token,type,params,newnode(FuncDeclaration,NULL),0);

                    struct symbol_list *symbol_tableFunc = (struct symbol_list *) malloc(sizeof(struct symbol_list));
                    symbol_tableFunc->next=NULL;
                    symbol_tableFunc->identifier=funcdeclarator->token;

                    insert_table(tables,symbol_tableFunc,funcdeclarator->token);
               }
                
        } 

        else if(symbol != NULL){
                
                struct node_list *child = paramlist->children;
                struct parameter_list *params=NULL;
                struct parameter_list *params2=symbol->parameters;
                struct parameter_list *params2_start = params2;
                int paramiguais=0;
                
                
                    while((child = child->next) != NULL){
                    enum type para=check_parameters(child->node);
                    if (para!= no_type){
                        params=add_parameter(params,para,get_identifier(child->node));
                        if(para==void_type && countchildren(paramlist)>1){
                            struct node *sitio=getchild(child->node,0);
                            printf("Line %d, column %d: Invalid use of void type in declaration\n",sitio->token_line,sitio->token_column);
                            semantic_errors++;
                            params=NULL;
                            break;
                        }
                        else{
                            if(get_identifier(child->node)!=NULL)
                            if(is_parameter_name_used(params,get_identifier(child->node))==1){
                                struct node *sitio=getchild(child->node,1);
                                printf("Line %d, column %d: Symbol %s already defined\n",sitio->token_line,sitio->token_column,get_identifier(child->node));
                                break;
                            }
                        }
                        
                    }
                    else{
                        params=NULL; 
                        break;
                    }    
                }

                if(type!=search_symbol(symbol_tableGlobal,funcdeclarator->token)->type){
                    struct parameter_list *params3=symbol->parameters;
                    struct parameter_list *paramscopy2=params;
                    printf("Line %d, column %d: Conflicting types (got %s(", funcdeclarator->token_line, funcdeclarator->token_column, type_name(type));

                    // Imprime os tipos da lista mais longa
                    while (paramscopy2 != NULL) {
                        printf("%s", type_name(paramscopy2->parameter));
                        paramscopy2 = paramscopy2->next;

                        // Adiciona vírgula se houver mais tipos a serem impressos
                        if (paramscopy2 != NULL) {
                            printf(",");
                        }
                    }

                    // Imprime a mensagem indicando que há tipos esperados
                    printf("), expected %s(", type_name(symbol->type));

                    while (params3 != NULL) {
                        printf("%s", type_name(params3->parameter));
                        params3 = params3->next;

                        // Adiciona vírgula se houver mais tipos a serem impressos
                        if (params3 != NULL) {
                            printf(",");
                        }
                    }

                    printf("))\n");

                    semantic_errors++;
                    return;
                }

                if(params!=NULL){
                struct parameter_list *paramscopy=params;
                struct parameter_list *paramscopy_start = paramscopy;
                while (paramscopy != NULL && params2 != NULL) {
                    // Move para o próximo par de parâmetros
                    if(paramscopy->parameter!=params2->parameter){
                        paramscopy = paramscopy_start;
                        params2 = params2_start;
                        printf("Line %d, column %d: Conflicting types (got %s(", funcdeclarator->token_line, funcdeclarator->token_column, type_name(type));

                    // Imprime os tipos da lista mais longa
                    while (paramscopy != NULL) {
                        printf("%s", type_name(paramscopy->parameter));
                        paramscopy = paramscopy->next;

                        // Adiciona vírgula se houver mais tipos a serem impressos
                        if (paramscopy != NULL) {
                            printf(",");
                        }
                    }

                    // Imprime a mensagem indicando que há tipos esperados
                    printf("), expected %s(", type_name(symbol->type));

                    while (params2 != NULL) {
                        printf("%s", type_name(params2->parameter));
                        params2 = params2->next;

                        // Adiciona vírgula se houver mais tipos a serem impressos
                        if (params2 != NULL) {
                            printf(",");
                        }
                    }

                    printf("))\n");

                    semantic_errors++;
                    return;
                    }
                    paramscopy = paramscopy->next;
                    params2 = params2->next;
                }

// Verifica se uma lista é mais longa que a outra
        if (paramscopy != NULL || params2 != NULL) {
                    paramscopy = paramscopy_start;
                    params2 = params2_start;
                    printf("Line %d, column %d: Conflicting types (got %s(", funcdeclarator->token_line, funcdeclarator->token_column, type_name(type));

                    // Imprime os tipos da lista mais longa
                    while (paramscopy != NULL) {
                        printf("%s", type_name(paramscopy->parameter));
                        paramscopy = paramscopy->next;

                        // Adiciona vírgula se houver mais tipos a serem impressos
                        if (paramscopy != NULL) {
                            printf(",");
                        }
                    }

                    // Imprime a mensagem indicando que há tipos esperados
                    printf("), expected %s(", type_name(symbol->type));

                    while (params2 != NULL) {
                        printf("%s", type_name(params2->parameter));
                        params2 = params2->next;

                        // Adiciona vírgula se houver mais tipos a serem impressos
                        if (params2 != NULL) {
                            printf(",");
                        }
                    }

                    printf("))\n");

                    semantic_errors++;
                    return;
        }
        }
        
        }
    }



    if(node->category==Declaration){

        struct node *tspec = getchild(node,0);
        struct node *declarator = getchild(node,1);
        struct node *expr= getchild(node,2);
        struct symbol_list *symbol=search_symbol(symbol_tableGlobal,declarator->token);
        enum type type =category_type(tspec->category);

        if(check_types(tspec)==1){ //Se for void
            if(expr!=NULL){
                    check_expression(expr,symbol_tableGlobal);
            }
            printf("Line %d, column %d: Invalid use of void type in declaration\n",declarator->token_line, declarator->token_column);
            semantic_errors++;
            return;
        }
        
        else{
            if(symbol == NULL) { //Declarator
                if(expr!=NULL){
                    enum type typeexpr=check_expression(expr,symbol_tableGlobal);
                    if(typeexpr!=type && typeexpr!=undef_type){
                        if(((type==double_type && typeexpr==undef_type)||((type==integer_type && typeexpr==double_type))||(type==short_type && typeexpr==double_type)||((type==char_type && typeexpr!=integer_type)&&(char_type && typeexpr!=char_type)) || (type==void_type && typeexpr!=void_type))){
                            printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n",declarator->token_line,declarator->token_column,type_name(typeexpr),type_name(type));
                            semantic_errors++;
                        }
                    }
                }
                //Acho que vai ser preciso fazer um check das expressions aqui
                insert_symbol(symbol_tableGlobal,declarator->token,type,NULL,newnode(Declaration,NULL),0);

            } else if(symbol!=NULL && type!=symbol->type) { //Declarator
                printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n",declarator->token_line,declarator->token_column,type_name(type),type_name(symbol->type));
                semantic_errors++;
                if(expr!=NULL){
                    check_expression(expr,symbol_tableGlobal);
                }
            }
        }
    }
}

void check_funcbody(struct node *node, struct symbol_list *func_list){
    func_list->body=1;
    if(node->category==Declaration){

        struct node *tspec = getchild(node,0);
        struct node *declarator = getchild(node,1);
        struct node *expr= getchild(node,2);
        enum type type =category_type(tspec->category);

        if(check_types(tspec)==1){ //Se for void
            if(expr!=NULL){
                    check_expression(expr,func_list);
            }
            printf("Line %d, column %d: Invalid use of void type in declaration\n",declarator->token_line, declarator->token_column);
            semantic_errors++;
        }
        
        else{
            if(search_symbol(func_list,declarator->token) == NULL) { //Declarator
                if(expr!=NULL){
                    enum type typeexpr=check_expression(expr,func_list);
                    if(typeexpr!=type && typeexpr!=undef_type){
                        if(((type==double_type && typeexpr==undef_type)||((type==integer_type && typeexpr==double_type))||(type==short_type && typeexpr==double_type)||((type==char_type && typeexpr!=integer_type)&&(char_type && typeexpr!=char_type)) || (type==void_type && typeexpr!=void_type))){
                            printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n",declarator->token_line,declarator->token_column,type_name(typeexpr),type_name(type));
                            semantic_errors++;
                        }
                    }
                }
                insert_symbol(func_list,declarator->token,type,NULL,newnode(Declaration,NULL),0);

            } else if (search_symbol(func_list,declarator->token)!=NULL && type!=search_symbol(func_list,declarator->token)->type){ //Declarator
                printf("Line %d, column %d: Symbol %s already defined\n",declarator->token_line,declarator->token_column,declarator->token);
                semantic_errors++;
                if(expr!=NULL){
                    check_expression(expr,func_list);
                }
            }
             else if (search_symbol(func_list,declarator->token)!=NULL && type==search_symbol(func_list,declarator->token)->type){ //Declarator
                printf("Line %d, column %d: Symbol %s already defined\n",declarator->token_line,declarator->token_column,declarator->token);
                semantic_errors++;
                if(expr!=NULL){
                    check_expression(expr,func_list);
                }
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
        enum type type=check_expression(expression,func_list);

        if(expression->token!=NULL && search_symbol(symbol_tableGlobal, expression->token)!=NULL){
            if((search_symbol(symbol_tableGlobal, expression->token)->node->category==FuncDefinition) || (search_symbol(symbol_tableGlobal, expression->token)->node->category==FuncDeclaration)){
                            expression->type=search_symbol(symbol_tableGlobal, expression->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, expression->token)->parameters;
                            printf("Line %d, column %d: Conflicting types (got %s(",expression->token_line,expression->token_column,type_name(expression->type));
                            semantic_errors++;
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                param=param->next;
                                if(param!= NULL){
                                    printf(",");
                                }
                            }
                            printf("), expected int)\n");

                            check_statements(statementesp,func_list);
                            if (statementespelse->category != Null){
                                check_statements(statementespelse, func_list);
                            }
                            return;
            }
        }

        if(type!=integer_type && type!=char_type && type!=short_type){
            printf("Line %d, column %d: Conflicting types (got %s, expected int)\n",expression->token_line,expression->token_column,type_name(type));
            semantic_errors++;
        }
        check_statements(statementesp,func_list);
        if (statementespelse->category != Null){
            check_statements(statementespelse, func_list);
        }
    }

    else if(node->category==While){
        struct node *expression = getchild(node,0);
        struct node *statementesp = getchild(node,1);
        enum type type=check_expression(expression,func_list);

        if(expression->token!=NULL && search_symbol(symbol_tableGlobal, expression->token)!=NULL){
            if((search_symbol(symbol_tableGlobal, expression->token)->node->category==FuncDefinition) || (search_symbol(symbol_tableGlobal, expression->token)->node->category==FuncDeclaration)){
                            expression->type=search_symbol(symbol_tableGlobal, expression->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, expression->token)->parameters;
                            printf("Line %d, column %d: Conflicting types (got %s(",expression->token_line,expression->token_column,type_name(expression->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                param=param->next;
                                if(param!= NULL){
                                    printf(",");
                                }
                            }
                            printf("), expected int)\n");
                            if (statementesp->category != Null){
                                check_statements(statementesp, func_list);
                            }
                            return;   
            }
        }

        if(type!=integer_type && type!=char_type && type!=short_type){
            printf("Line %d, column %d: Conflicting types (got %s, expected int)\n",expression->token_line,expression->token_column,type_name(type));
        }
        if (statementesp->category != Null){
            check_statements(statementesp, func_list);
        }
    }

    else if(node->category==Return){
        //Lidar com o erro Line %d, column %d: Conflicting types (got %s, expected %s)\n Tem de ser nas expressions
        struct node *expression = getchild(node,0);
        enum type typeexpr =check_expression(expression,func_list);
        //Quando for void
        if(expression->category==Null){
            typeexpr=void_type;
            enum type type=search_symbol(func_list,"return")->type;
            if((type!=typeexpr && type_name(typeexpr)!=type_name(no_type)) && (type!=char_type || typeexpr!=integer_type) && (type!=double_type || typeexpr!=integer_type) && (type!=short_type || typeexpr!=integer_type)){
                printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n",node->token_line,node->token_column,type_name(typeexpr),type_name(type));
                semantic_errors++;
                return;
            }
        }
        else{
            enum type type=search_symbol(func_list,"return")->type;
            
            if(expression->token!=NULL && search_symbol(symbol_tableGlobal, expression->token)!=NULL){
            if((search_symbol(symbol_tableGlobal, expression->token)->node->category==FuncDefinition) || (search_symbol(symbol_tableGlobal, expression->token)->node->category==FuncDeclaration)){
                            expression->type=search_symbol(symbol_tableGlobal, expression->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, expression->token)->parameters;
                            printf("Line %d, column %d: Conflicting types (got %s(",expression->token_line,expression->token_column,type_name(expression->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                param=param->next;
                                if(param!= NULL){
                                    printf(",");
                                }
                            }
                            printf("), expected %s)\n",type_name(type));
                            return;
            }
            }
            if(typeexpr==undef_type){
                printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n",expression->token_line,expression->token_column,type_name(typeexpr),type_name(type));
                semantic_errors++;
            }

            if((((type==integer_type && typeexpr==double_type))||(type==short_type && typeexpr==double_type)||(type==char_type && typeexpr==double_type) || (type==void_type && typeexpr!=void_type))){
                printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n",expression->token_line,expression->token_column,type_name(typeexpr),type_name(type));
                semantic_errors++;
            }
        }
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
enum type check_expression(struct node *expression, struct symbol_list *func_list) {
    char *sinal;
    switch(expression->category) {
        case Identifier:
            if(search_symbol(func_list, expression->token) == NULL && search_symbol(symbol_tableGlobal, expression->token) == NULL) {
                expression->type=undef_type;
                printf("Line %d, column %d: Unknown symbol %s\n",expression->token_line, expression->token_column,expression->token);
                semantic_errors++;
            }
            else if (search_symbol(func_list, expression->token) != NULL) {
                expression->type = search_symbol(func_list, expression->token)->type;
            }
            else if(search_symbol(symbol_tableGlobal, expression->token) != NULL){
                expression->type = search_symbol(symbol_tableGlobal, expression->token)->type;
            }
            else if(search_symbol(symbol_tableGlobal, expression->token) != NULL && search_symbol(func_list, expression->token) != NULL){
                expression->type = search_symbol(symbol_tableGlobal, expression->token)->type;
            }
            return expression->type;
            break;

        case Natural:
            expression->type = integer_type;
            return expression->type;
            break;
        case ChrLit:
            expression->type = integer_type;
            return expression->type;
            break;
        case Decimal:
            expression->type = double_type;
            return expression->type;
            break;
        
        case Store:
            check_expression(getchild(expression, 0), func_list);
            check_expression(getchild(expression, 1), func_list);
            expression->type=getchild(expression, 0)->type;

            if(expression->category==Store){
                sinal="=";
            }

            if((getchild(expression, 0)->category==Identifier)||(getchild(expression, 1)->category==Identifier)){
                if(getchild(expression, 0)->token!=NULL && getchild(expression, 1)->token!=NULL){
                    if((search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)!=NULL) && (search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)!=NULL)){
                        if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration){
                            expression->type=undef_type;
                            getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                                struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                                printf("Line %d, column %d: Operator %s cannot be applied to types %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                                while(param!=NULL){
                                    printf("%s",type_name(param->parameter));
                                    param=param->next;
                                    if(param!= NULL){
                                        printf(",");
                                    }
                                }
                                printf("), %s\n",type_name(getchild(expression, 1)->type));
                                break;
                        }
                        if(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDeclaration){
                                getchild(expression, 1)->type=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->type;
                                struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->parameters;
                                printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                                while(param!=NULL){
                                    printf("%s",type_name(param->parameter));
                                    param=param->next;
                                    if(param!= NULL){
                                        printf(",");
                                    }
                                }
                                printf(")\n");
                                break;
                        }

                        if((search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration)&&(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDeclaration)){
                                expression->type=undef_type;
                                getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                                struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                                printf("Line %d, column %d: Operator %s cannot be applied to types %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                                while(param!=NULL){
                                    printf("%s",type_name(param->parameter));
                                    param=param->next;
                                    if(param != NULL){
                                        printf(",");
                                    }
                                }
                                getchild(expression, 1)->type=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->type;
                                struct parameter_list *param2=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->parameters;
                                printf("), %s(",type_name(getchild(expression, 1)->type));
                                while(param2!=NULL){
                                    printf("%s",type_name(param2->parameter));
                                    param2=param2->next;
                                    if(param2!= NULL){
                                        printf(",");
                                    }
                                }
                                printf(")\n");
                                break;
                        }
                    }
                }
                
                if(getchild(expression, 0)->token!=NULL){
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)!=NULL){
                        if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration){
                                expression->type=undef_type;
                                getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                                struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                                printf("Line %d, column %d: Operator %s cannot be applied to types %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                                while(param!=NULL){
                                    printf("%s",type_name(param->parameter));
                                    param=param->next;
                                    if(param!= NULL){
                                        printf(",");
                                    }
                                }
                                printf("), %s\n",type_name(getchild(expression, 1)->type));
                                break;
                        }
                    }
                }

                if(getchild(expression, 1)->token!=NULL){
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)!=NULL){
                        if(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDeclaration){
                                getchild(expression, 1)->type=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->type;
                                struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->parameters;
                                printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                                while(param!=NULL){
                                    printf("%s",type_name(param->parameter));
                                    param=param->next;
                                    if(param!= NULL){
                                        printf(",");
                                    }
                                }
                                printf(")\n");
                                break;
                        }
                    }
                }
            }

            if(getchild(expression, 0)->type==undef_type || getchild(expression, 1)->type==undef_type){
                if(expression->category==Store){
                    printf("Line %d, column %d: Operator = cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                    break;
                }
            }
            else if((((check_expression(getchild(expression, 0), func_list)==integer_type && check_expression(getchild(expression, 1), func_list)==double_type))||(check_expression(getchild(expression, 0), func_list)==short_type && check_expression(getchild(expression, 1), func_list)==double_type)||(check_expression(getchild(expression, 0), func_list)==char_type && check_expression(getchild(expression, 1), func_list)==double_type)|| (check_expression(getchild(expression, 0), func_list)==void_type && check_expression(getchild(expression, 1), func_list)!=void_type))){
                if(expression->category==Store){
                    printf("Line %d, column %d: Operator = cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
            }
            else if(getchild(expression, 0)->category!=Identifier){
                if(expression->category==Store){
                    printf("Line %d, column %d: Lvalue required\n",getchild(expression, 0)->token_line,getchild(expression, 0)->token_column);
                }
            }
            return expression->type;
            break;
        case Add:
        case Sub:
        case Mul:
        case Div:
            
            if(expression->category==Add){
                sinal="+";
            }
            if(expression->category==Sub){
                sinal="-";
            }
            if(expression->category==Mul){
                sinal="*";
            }
            if(expression->category==Div){
                sinal="/";
            }

            check_expression(getchild(expression, 0), func_list);
            check_expression(getchild(expression, 1), func_list);

            if((getchild(expression, 0)->category==Identifier)||(getchild(expression, 1)->category==Identifier)){
                expression->type=undef_type;
                if(getchild(expression, 0)->token!=NULL && getchild(expression, 1)->token!=NULL){
                    if((search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)!=NULL) && (search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)!=NULL)){
                        if((search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration)&&(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDeclaration)){
                                getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                                struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                                printf("Line %d, column %d: Operator %s cannot be applied to types %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                                while(param!=NULL){
                                    printf("%s",type_name(param->parameter));
                                    param=param->next;
                                    if(param != NULL){
                                        printf(",");
                                    }
                                }
                                getchild(expression, 1)->type=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->type;
                                struct parameter_list *param2=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->parameters;
                                printf("), %s(",type_name(getchild(expression, 1)->type));
                                while(param2!=NULL){
                                    printf("%s",type_name(param2->parameter));
                                    param2=param2->next;
                                    if(param2!= NULL){
                                        printf(",");
                                    }
                                }
                                printf(")\n");
                                break;
                        }

                        
                        
                        if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration){
                            getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                                struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                                printf("Line %d, column %d: Operator %s cannot be applied to types %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                                while(param!=NULL){
                                    printf("%s",type_name(param->parameter));
                                    param=param->next;
                                    if(param!= NULL){
                                        printf(",");
                                    }
                                }
                                printf("), %s\n",type_name(getchild(expression, 1)->type));
                                break;
                        }
                        if(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDeclaration){
                                getchild(expression, 1)->type=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->type;
                                struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->parameters;
                                printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                                while(param!=NULL){
                                    printf("%s",type_name(param->parameter));
                                    param=param->next;
                                    if(param!= NULL){
                                        printf(",");
                                    }
                                }
                                printf(")\n");
                                break;
                        }
                    }
                }

                if(getchild(expression, 0)->token!=NULL){
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)!=NULL){
                        if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration){
                                getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                                struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                                printf("Line %d, column %d: Operator %s cannot be applied to types %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                                while(param!=NULL){
                                    printf("%s",type_name(param->parameter));
                                    param=param->next;
                                    if(param!= NULL){
                                        printf(",");
                                    }
                                }
                                printf("), %s\n",type_name(getchild(expression, 1)->type));
                                break;
                        }
                    }
                }   

                if(getchild(expression, 1)->token!=NULL){
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)!=NULL){
                        if(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDeclaration){
                                getchild(expression, 1)->type=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->type;
                                struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->parameters;
                                printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                                while(param!=NULL){
                                    printf("%s",type_name(param->parameter));
                                    param=param->next;
                                    if(param!= NULL){
                                        printf(",");
                                    }
                                }
                                printf(")\n");
                                break;
                        }
                    }
                }
            }
            

            if(getchild(expression, 0)->type==undef_type || getchild(expression, 1)->type==undef_type){
                expression->type=undef_type;
                if(expression->category==Add){
                    printf("Line %d, column %d: Operator + cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Sub){
                    printf("Line %d, column %d: Operator - cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Mul){
                    printf("Line %d, column %d: Operator * cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Div){
                    printf("Line %d, column %d: Operator / cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
            }
            else if(getchild(expression, 0)->type==void_type || getchild(expression, 1)->type==void_type){
                expression->type=undef_type;
                if(expression->category==Add){
                    printf("Line %d, column %d: Operator + cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Sub){
                    printf("Line %d, column %d: Operator - cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Mul){
                    printf("Line %d, column %d: Operator * cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Div){
                    printf("Line %d, column %d: Operator / cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
            }
            else if(getchild(expression, 0)->type==double_type || getchild(expression, 1)->type==double_type){
                expression->type=double_type;
            }
            else if(getchild(expression, 0)->type==integer_type || getchild(expression, 1)->type==integer_type){
                expression->type=integer_type;
            }
            else if(getchild(expression, 0)->type==short_type|| getchild(expression, 1)->type==short_type){
                expression->type=short_type;
            }
            else if(getchild(expression, 0)->type==char_type|| getchild(expression, 1)->type==char_type){
                expression->type=char_type;
            }
            return expression->type;
            break;
        case Or:
        case And:
        case BitWiseAnd:
        case BitWiseOr:
        case BitWiseXor:
        case Mod:
        case Eq:
        case Ne:
        case Ge:
        case Lt:
        case Le:
        case Gt:

            if(expression->category==Or){
                sinal="||";
            }
            if(expression->category==And){
                sinal="&&";
            }
            if(expression->category==BitWiseAnd){
                sinal="&";
            }
            if(expression->category==BitWiseOr){
                sinal="|";
            }
            if(expression->category==BitWiseXor){
                sinal="^";
            }
            if(expression->category==Mod){
                sinal="%%";
            }
            if(expression->category==Eq){
                sinal="==";
            }
            if(expression->category==Ne){
                sinal="!=";
            }
            if(expression->category==Ge){
                sinal=">=";
            }
            if(expression->category==Lt){
                sinal="<";
            }
            if(expression->category==Le){
                sinal="<=";
            }
            if(expression->category==Gt){
                sinal=">";
            }

            check_expression(getchild(expression, 0), func_list);
            check_expression(getchild(expression, 1), func_list);
            expression->type=integer_type;
            
            if((getchild(expression, 0)->category==Identifier)||(getchild(expression, 1)->category==Identifier)){
                if(getchild(expression, 0)->token!=NULL && getchild(expression, 1)->token!=NULL){
                if((search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)!=NULL) && (search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)!=NULL)){
                    if((search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration)&&(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDeclaration)){
                            getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                            printf("Line %d, column %d: Operator %s cannot be applied to types %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                param=param->next;
                                if(param != NULL){
                                    printf(",");
                                }
                            }
                            getchild(expression, 1)->type=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->type;
                            struct parameter_list *param2=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->parameters;
                            printf("), %s(",type_name(getchild(expression, 1)->type));
                            while(param2!=NULL){
                                printf("%s",type_name(param2->parameter));
                                param2=param2->next;
                                if(param2!= NULL){
                                    printf(",");
                                }
                            }
                            printf(")\n");
                            break;
                    }
                    
                    
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration){
                        getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                            printf("Line %d, column %d: Operator %s cannot be applied to types %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                param=param->next;
                                if(param!= NULL){
                                    printf(",");
                                }
                            }
                            printf("), %s\n",type_name(getchild(expression, 1)->type));
                            break;
                    }
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDeclaration){
                            getchild(expression, 1)->type=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->parameters;
                            printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                param=param->next;
                                if(param!= NULL){
                                    printf(",");
                                }
                            }
                            printf(")\n");
                            break;
                    }
                } 
                }
                if(getchild(expression, 0)->token!=NULL){
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)!=NULL){
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration){
                            getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                            printf("Line %d, column %d: Operator %s cannot be applied to types %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                param=param->next;
                                if(param!= NULL){
                                    printf(",");
                                }
                            }
                            printf("), %s\n",type_name(getchild(expression, 1)->type));
                            break;
                    }
                }
                }

                if(getchild(expression, 1)->token!=NULL){
                 if(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)!=NULL){
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDeclaration){
                            getchild(expression, 1)->type=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->parameters;
                            printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                param=param->next;
                                if(param!= NULL){
                                    printf(",");
                                }
                            }
                            printf(")\n");
                            break;
                    }
                }
                }
            }
            
            if(getchild(expression, 0)->type==undef_type || getchild(expression, 1)->type==undef_type){
                if(expression->category==Or){
                    printf("Line %d, column %d: Operator || cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==And){
                    printf("Line %d, column %d: Operator && cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==BitWiseAnd){
                    printf("Line %d, column %d: Operator & cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==BitWiseOr){
                    printf("Line %d, column %d: Operator | cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==BitWiseXor){
                    printf("Line %d, column %d: Operator ^ cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Mod){
                    printf("Line %d, column %d: Operator %% cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Eq){
                    if(getchild(expression, 0)->type != undef_type || getchild(expression, 1)->type != undef_type)
                        printf("Line %d, column %d: Operator == cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Ne){
                    printf("Line %d, column %d: Operator != cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Ge){
                    printf("Line %d, column %d: Operator >= cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Lt){
                    printf("Line %d, column %d: Operator < cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Le){
                    printf("Line %d, column %d: Operator <= cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Gt){
                    printf("Line %d, column %d: Operator > cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
            }

                else if(getchild(expression, 0)->type==double_type || getchild(expression, 1)->type==double_type){
                if(expression->category==BitWiseAnd){
                    printf("Line %d, column %d: Operator & cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Or){
                    printf("Line %d, column %d: Operator || cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==And){
                    printf("Line %d, column %d: Operator && cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==BitWiseAnd){
                    printf("Line %d, column %d: Operator & cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==BitWiseOr){
                    printf("Line %d, column %d: Operator | cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==BitWiseXor){
                    printf("Line %d, column %d: Operator ^ cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
                else if(expression->category==Mod){
                    printf("Line %d, column %d: Operator %% cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                }
            }
        
            return expression->type;
            break;
            
        case Plus:
        case Minus:
            if(expression->category==Plus){
                sinal="+";
            }
            if(expression->category==Minus){
                sinal="-";
            }
            
            check_expression(getchild(expression, 0), func_list);
            expression->type=getchild(expression, 0)->type;

            if(getchild(expression, 0)->category==Identifier){
                if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)!=NULL){
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration){
                            getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                            printf("Line %d, column %d: Operator %s cannot be applied to type %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters->next;
                                param=param->next;
                                if(param!= NULL){
                                    printf(",");
                                }
                            }
                            printf(")\n");
                            break;
                    }
                }
            }
            
            if(getchild(expression, 0)->type==undef_type || getchild(expression, 0)->type==void_type){
                if(expression->category==Plus){
                    printf("Line %d, column %d: Operator + cannot be applied to type %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type));
                }
                else if(expression->category==Minus){
                    printf("Line %d, column %d: Operator - cannot be applied to type %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type));
                }
            }
            return expression->type;
            break;
            
        case Not:
            if(expression->category==Not){
                sinal="!";
            }

            check_expression(getchild(expression, 0), func_list);
            expression->type=integer_type;

            if(getchild(expression, 0)->category==Identifier){
                if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)!=NULL){
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration){
                            getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                            printf("Line %d, column %d: Operator %s cannot be applied to type %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters->next;
                                param=param->next;
                                if(param!= NULL){
                                    printf(",");
                                }
                            }
                            printf(")\n");
                            break;
                    }
                }
            }

            if(getchild(expression, 0)->type==double_type){
                printf("Line %d, column %d: Operator %s cannot be applied to type %s\n",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
            }
            
            return expression->type;
            break;

        case Comma:
            if(expression->category==Comma){
                sinal=",";
            }
            check_expression(getchild(expression, 0), func_list);
            check_expression(getchild(expression, 1), func_list);
            expression->type=getchild(expression, 1)->type;

            if((getchild(expression, 0)->category==Identifier)||(getchild(expression, 1)->category==Identifier)){
                if(getchild(expression, 0)->token!=NULL && getchild(expression, 1)->token!=NULL)
                if((search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)!=NULL) && (search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)!=NULL)){
                    if((search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration)&&(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDeclaration)){
                            expression->type=undef_type;
                            getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                            printf("Line %d, column %d: Operator %s cannot be applied to types %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                param=param->next;
                                if(param != NULL){
                                    printf(",");
                                }
                            }
                            getchild(expression, 1)->type=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->type;
                            struct parameter_list *param2=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->parameters;
                            printf("), %s(",type_name(getchild(expression, 1)->type));
                            while(param2!=NULL){
                                printf("%s",type_name(param2->parameter));
                                param2=param2->next;
                                if(param2!= NULL){
                                    printf(",");
                                }
                            }
                            printf(")\n");
                            break;
                    }
                    
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration){
                            expression->type=undef_type;
                            getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                            printf("Line %d, column %d: Operator %s cannot be applied to types %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                param=param->next;
                                if(param!= NULL){
                                    printf(",");
                                }
                            }
                            printf("), %s\n",type_name(getchild(expression, 1)->type));
                            break;
                    }
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDeclaration){
                            expression->type=undef_type;
                            getchild(expression, 1)->type=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->parameters;
                            printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                param=param->next;
                                if(param!= NULL){
                                    printf(",");
                                }
                            }
                            printf(")\n");
                            break;
                    }
                }
                else if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)!=NULL){
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==FuncDeclaration){
                            getchild(expression, 0)->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                            printf("Line %d, column %d: Operator %s cannot be applied to types %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                param=param->next;
                                if(param!= NULL){
                                    printf(",");
                                }
                            }
                            printf("), %s\n",type_name(getchild(expression, 1)->type));
                            break;
                    }
                }

                else if(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)!=NULL){
                    if(search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDefinition || search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->node->category==FuncDeclaration){
                            expression->type=undef_type;
                            getchild(expression, 1)->type=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->type;
                            struct parameter_list *param=search_symbol(symbol_tableGlobal, getchild(expression, 1)->token)->parameters;
                            printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s(",expression->token_line,expression->token_column,sinal,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                            while(param!=NULL){
                                printf("%s",type_name(param->parameter));
                                param=param->next;
                                if(param!= NULL){
                                    printf(",");
                                }
                            }
                            printf(")\n");
                            break;
                    }
                }
            }

            if(getchild(expression, 0)->type==undef_type || getchild(expression, 1)->type==undef_type){
                    printf("Line %d, column %d: Operator , cannot be applied to types %s, %s\n",expression->token_line,expression->token_column,type_name(getchild(expression, 0)->type),type_name(getchild(expression, 1)->type));
                    semantic_errors++;
                    break;
            }

            return expression->type;
            break;
            
        case Call:
            if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token) == NULL && search_symbol(func_list, getchild(expression, 0)->token) == NULL) {
                //Da erro aqui
                expression->type=undef_type;
                struct node_list *arguments = expression->children;
                int countargs=countchildren(expression)-1;
                int countparams=0;
                if (arguments!=NULL){
                        while((arguments = arguments->next) != NULL){
                            check_expression(arguments->node, func_list);
                        }
                }
                if(countargs!=countparams){
                        printf("Line %d, column %d: Wrong number of arguments to function %s (got %d, required %d)\n",getchild(expression, 0)->token_line,getchild(expression, 0)->token_column,getchild(expression, 0)->token,countargs,countparams);
                        semantic_errors++;
                }

            } else{
                struct node_list *arguments = expression->children;

                if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token) != NULL && search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category!=Declaration){
                    expression->type=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;
                    int countargs=countchildren(expression)-1;

                    if(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters->parameter!=void_type){
                        int countparams=count_parameters(search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters);
                        struct parameter_list *params=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->parameters;
                        enum type typefunc=search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->type;

                        //Se tiver argumentos
                        if (arguments!=NULL){
                            while((arguments = arguments->next) != NULL){
                                check_expression(arguments->node, func_list);
                            }
                        }
                        //Se o numero de argumentos nao corresponder
                        if(countargs!=countparams){
                            printf("Line %d, column %d: Wrong number of arguments to function %s (got %d, required %d)\n",getchild(expression, 0)->token_line,getchild(expression, 0)->token_column,getchild(expression, 0)->token,countargs,countparams);
                            semantic_errors++;
                        }
                        //Se o numero de argumentos corresponder mas forem diferentes
                        
                        else{
                            int n=1;
                            if(getchild(expression, n) != NULL)
                            while ((getchild(expression, n)) != NULL && params!= NULL) {
                                // Verifica se os tipos dos parâmetros são diferentes
                                //Perguntar quando é que chamar uma funcao com parametros de tipos diferentes dá erro
                                if ((params->parameter==integer_type && getchild(expression, n)->type ==double_type) || (params->parameter==short_type && getchild(expression, n)->type ==double_type) || (params->parameter==char_type && getchild(expression, n)->type ==double_type) || getchild(expression, n)->type ==void_type || getchild(expression, n)->type ==undef_type) {
                                    
                                    if((getchild(expression, n))->category==Call){
                                        check_expression(getchild(expression, n),func_list);
                                        //Se for um call com outro call dentro, o erro tem de dar no primeiro parametro do segundo call
                                        printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n",getchild(getchild(expression, n),0)->token_line,getchild(getchild(expression, n),0)->token_column,type_name(getchild(expression, n)->type),type_name(params->parameter));
                                        semantic_errors++;
                                        break;
                                    }
                                    printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n",getchild(expression, n)->token_line,getchild(expression, n)->token_column,type_name(getchild(expression, n)->type),type_name(params->parameter));
                                    semantic_errors++;
                                    break;
                                }
                                n++;
                                params = params->next;  
                                    
                         }
                        }

                        
                    }

                    //Se o parametro de dentro for void tipo main(void)
                    else{
                        int countparams=0;
                        if (arguments!=NULL){
                            while((arguments = arguments->next) != NULL){
                                check_expression(arguments->node, func_list);
                            }
                        }
                        if(countargs!=countparams){
                            printf("Line %d, column %d: Wrong number of arguments to function %s (got %d, required %d)\n",getchild(expression, 0)->token_line,getchild(expression, 0)->token_column,getchild(expression, 0)->token,countargs,countparams);
                            semantic_errors++;
                        }
                    }
                }

                //Se estivermos a chamar
                else if(search_symbol(func_list, getchild(expression, 0)->token) != NULL || search_symbol(symbol_tableGlobal, getchild(expression, 0)->token)->node->category==Declaration){
                    expression->type=search_symbol(func_list, getchild(expression, 0)->token)->type;
                    int countargs=countchildren(expression)-1;
                    int countparams=0;

                    if (arguments!=NULL){
                        while((arguments = arguments->next) != NULL){
                            check_expression(arguments->node, func_list);
                        }
                    }
                    if(countargs!=countparams){
                        printf("Line %d, column %d: Wrong number of arguments to function %s (got %d, required %d)\n",getchild(expression, 0)->token_line,getchild(expression, 0)->token_column,getchild(expression, 0)->token,countargs,countparams);
                        semantic_errors++;
                    }
                }
            }
            return expression->type;
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

// Função para comparar um nome com todos os nomes na lista de parâmetros
int isNameInParameterList(const char *name, const struct parameter_list *paramList) {
    const struct parameter_list *current = paramList;

    // Percorre a lista de parâmetros
    while (current != NULL) {
        // Compara o nome atual com o nome fornecido
        if (strcmp(current->name, name) == 0) {
            return 1; // O nome já existe na lista
        }
        current = current->next;
    }

    return 0; // O nome não existe na lista
}


struct parameter_list *add_parameter(struct parameter_list *list, enum type parameter, char *name) {
    // Verifica se já existe um parâmetro com o mesmo nome
    if (name != NULL && is_parameter_name_used(list, name)) {
        // Pode lidar com o erro de outra forma, se necessário
        return NULL;
    }

    // Aloca memória para o novo nó de parâmetro
    struct parameter_list *new_param = malloc(sizeof(struct parameter_list));
    if (new_param == NULL) {
        // Lida com falha na alocação de memória, por exemplo, retornando ou saindo
        return list;
    }

    // Inicializa o campo de nome
    if (name != NULL) {
        new_param->name = strdup(name);
    } else if(name == NULL && parameter != void_type) {
        // printf("Unexpected error: Parameter symbol required\n");
        new_param->name = NULL;
    } else if(name != NULL && parameter == void_type){
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

int is_parameter_name_used(struct parameter_list *list, const char *param_name) {
    struct parameter_list *current_param = list;
    int count = 0;

    while (current_param != NULL) {
        if (current_param->name != NULL && strcmp(param_name, current_param->name) == 0) {
            count++;
            if (count > 1) {
                // O nome do parâmetro foi usado mais de duas vezes
                return 1;
            }
        }
        current_param = current_param->next;
    }

    // O nome do parâmetro foi usado duas vezes ou menos
    return 0;
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

    // Set to keep track of printed identifiers
    // This will help in avoiding duplicates
    char printed_identifiers[100][100];
    int num_printed = 0;

    // Print the return symbol first
    struct symbol_list *return_symbol = search_symbol(tables->list, "return");
    if (return_symbol != NULL) {
        printf("%s\t%s", return_symbol->identifier, type_name(return_symbol->type));
        printf("\n");

        // Add the identifier to the printed list
        strncpy(printed_identifiers[num_printed++], return_symbol->identifier, 100);
    }

    // Print the rest of the symbols
    for (symbol = tables->list->next; symbol != NULL; symbol = symbol->next) {
        // Check if the identifier has been printed already
        int already_printed = 0;
        for (int i = 0; i < num_printed; ++i) {
            if (strcmp(symbol->identifier, printed_identifiers[i]) == 0) {
                already_printed = 1;
                break;
            }
        }

        if (!already_printed) {
            printf("%s\t%s", symbol->identifier, type_name(symbol->type));

            if (symbol->is_parameter == 1) {
                printf("\t%s", "param");
            }

            printf("\n");

            // Add the identifier to the printed list
            strncpy(printed_identifiers[num_printed++], symbol->identifier, 100);
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
        printf("%s", category_name2[node->category]);
        if(node->type!=no_type  && node->category!=Int && node->category!=Char && node->category!=Void && node->category!=Double && node->category!=Short){
            printf(" - %s", type_name(node->type));
        }
        printf("\n");
    }
    else{
        struct symbol_list *symbol=search_symbol(symbol_tableGlobal,node->token);
        printf("%s(%s)", category_name2[node->category], node->token);
        
        if(node->category != Null && type_name(node->type)!="none"){
            printf(" - %s",type_name(node->type));
        
            if(symbol!=NULL){ //Ver isto
                // Verificar se a lista de parâmetros não é nula
                if (symbol->parameters != NULL && type_name(node->type)!="undef") {
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
            }
        }
        printf("\n");
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