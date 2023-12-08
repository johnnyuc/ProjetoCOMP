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

    insert_symbol(global_table, "putchar", integer_type, parameter1, NULL,1);

    struct parameter_list *parameter2 = register_parameter(void_type);
    insert_symbol(global_table, "getchar", integer_type, parameter2, NULL,1);

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
    struct node *expression = getchild(node,2);


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
        if( (declarator != NULL) && (search_symbol(table, declarator) == NULL) )  {
            enum type type =category_type(tspec->category);

            if(type!=void_type && declarator->token!=NULL){
                
                struct node *expression_son1 = getchild(expression,0);
                struct node *expression_son2 = getchild(expression,1);

                //Error case: quanto fazemos int i = i+i por exemplo. Não sei se precisava
                if(expression!=NULL && expression_son1!=NULL && expression_son2!=NULL){

                    if( (expression_son1->token!=NULL) && (expression_son2->token!=NULL) && ( (strcmp(declarator->token,expression_son1->token)==0) || (strcmp(declarator->token,expression_son2->token)==0) ) ){
                        
                        insert_symbol(table, declarator->token,type,NULL, newnode(Declaration,NULL),0);

                        if(expression!=NULL && table!=NULL){
                            check_Expression(expression,table);
                        }

                    }

                    else{

                        insert_symbol(table, declarator->token,type,NULL, newnode(Declaration,NULL),1);

                        if(expression!=NULL && table!=NULL){
                            check_Expression(expression,table);
                        }

                    }

                }
                else{

                    insert_symbol(table, declarator->token,type,NULL, newnode(Declaration,NULL),1);

                        //faz sentido ficar dentro do If
                        if(expression!=NULL && table!=NULL){
                            check_Expression(expression,table);
                        }

                }

            }

            //case 1: Expression
            else if(type==void_type && declarator->token!=NULL){

                if(expression!=NULL){
                    check_Expression(expression,table);
                }

            }

        }

        else if( (declarator != NULL) && (search_symbol(table, declarator) != NULL) && ( search_symbol(table, declarator)->node->type==declarator->type) ) {
              
            if(declarator->token!=NULL){

                if(expression!=NULL && table!=NULL){
                    check_Expression(expression,table);
                }

            }

        }
           
        //se já estiver, printa a mensagem de erro e itera sobre semantic errors
        else if( ( declarator != NULL) && (search_symbol(table, declarator) != NULL) ) {
            printf("Line %d, column %d: Symbol %s already defined\n", declarator->token_line, declarator->token_column,declarator->token);
            semantic_errors++;

        }

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
    if( search_symbol(table, func_declarator) == NULL){ 

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
        if(parameter_error==1) insert_symbol(table, func_declarator->token ,type, parameter_list_aux, func_declarator,1);


        //criação das table para a nova função
        struct table *new_symble_table = (struct table *) malloc(sizeof(struct table));
        new_symble_table->next = NULL;

        //insert do return default na table da nova função
        insert_symbol(new_symble_table,"return",type,NULL,NULL,1);


        //percorre a lista de parametros e insere os simbolos da nova funcao em sua table
        while(parameter_list_aux != NULL){

            if(parameter_list_aux->Identifier != NULL){
                insert_symbol(new_symble_table,parameter_list_aux->Identifier,parameter_list_aux->parameter,parameter_list_aux,node,1);
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

            else{

                check_Statement(func_body_child,new_symble_table);

            }

            j++;
        }

    }


    //Corrigir número magico
    else if ( (search_symbol(table, func_declarator) != NULL) && (search_symbol(table, func_declarator)->node!=NULL) && ( search_symbol(table, func_declarator)->node->category == 35 ) ){

        if(type!=search_symbol2(global_table,func_declarator->token)->type) {
            printf("Line %d, column %d: Symbol %s already defined\n",func_declarator->token_line,func_declarator->token_column,func_declarator->token);
            semantic_errors++;
        }

        else{
        
            struct table *symbol_tableFunc;

            if( search_table(tables,func_declarator->token) != NULL ) {
            
                symbol_tableFunc = search_table(tables,func_declarator->token)->table;

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

                //só irá imprimir a table se o numero de argumentos forem iguais e os tipos e identifiers iguais

                int param_erros = 0;
                int k = 0;
                int n_param = count_parameters(parameter_list_aux);

                struct parameter_list *head = parameter_list_aux;

                if (count_parameters(symbol_tableFunc->parameter)==count_parameters(parameter_list_aux)){
                    
                    while(k!=n_param){

                        if(symbol_tableFunc->parameter->Identifier!=NULL && parameter_list_aux->Identifier!=NULL){

                            if( (type_name(parameter_list_aux->parameter)!=type_name(symbol_tableFunc->parameter->parameter) ) || ( strcmp (parameter_list_aux->Identifier,symbol_tableFunc->parameter->Identifier)!=0 ) ){
                                
                                param_erros++;

                            }

                        }
                        
                        symbol_tableFunc->parameter = symbol_tableFunc->parameter->next;
                        parameter_list_aux = parameter_list_aux->next;
                        k++;

                    }

                }

                else if (count_parameters(symbol_tableFunc->parameter)!=count_parameters(parameter_list_aux)){
                    
                    param_erros++;

                }

                if(param_erros==0){

                    //insert do return default na table da nova função
                    insert_symbol(symbol_tableFunc,"return",type,NULL,NULL,1);

                    //se nao tivermos tido erros , insere o novo simbolo na tabela
                    //if(parameter_error==1) insert_symbol(symbol_tableFunc, func_declarator->token ,type, parameter_list_aux, func_declarator);

                    //volta a lista pro inicio pois percorremos ela na verificação
                    parameter_list_aux = head;

                    //percorre a lista de parametros e insere os simbolos da nova funcao em sua table
                    while(parameter_list_aux != NULL){

                        if(parameter_list_aux->Identifier != NULL){
                            insert_symbol(symbol_tableFunc,parameter_list_aux->Identifier,parameter_list_aux->parameter,parameter_list_aux,node,1);
                        }
                        parameter_list_aux = parameter_list_aux->next;

                    }

                    //inserçao da nova table na lista de tables
                    insert_table(tables,symbol_tableFunc,func_declarator->token);

                    //declaracao do no que vai receber os nos filhos de func_body
                    struct node *func_body_child;

                    while((func_body_child = getchild(func_body,j))!=NULL){
                
                        //verificacao das filhos declarationsdo func_body
                        if(func_body_child->category==Declaration){
                            
                            //se as declarations tiverem bem adiciona na table da nova funcao
                            check_Declaration(func_body_child,symbol_tableFunc);

                        }
                        
                        else{

                            check_Statement(func_body_child,symbol_tableFunc);
                        }

                        j++;
                    }
                }
            }
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
            if(symbol!=NULL) printf("Line %d, column %d: Conflicting types (got %s, expected %s)\n",Identifier->token_line,Identifier->token_column,type_name(symbol->type),type_name(void_type));
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

        if(parameter_error==1) {

            insert_symbol(table, func_declarator->token ,type, parameter_list_aux, newnode(FuncDeclaration,NULL),1);

            struct table *new_symble_table = (struct table *) malloc(sizeof(struct table));
            new_symble_table->next=NULL;
            new_symble_table->identifier = func_declarator->token;
            new_symble_table->parameter = parameter_list_aux;

            //insert_symbol(new_symble_table,func_declarator->token ,type, parameter_list_aux, newnode(FuncDeclaration,NULL));
            
            insert_table(tables,new_symble_table,func_declarator->token);

        }

    } 

    else if( (search_symbol(table, func_declarator) != NULL) && (category_type(search_symbol(table, func_declarator)->node->category)==5) ){
        
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

        if(parameter_error==1) {

            
            if(table!=global_table) insert_symbol(table, func_declarator->token ,type, parameter_list_aux, newnode(FuncDeclaration,NULL),1);

            struct table *new_symble_table = (struct table *) malloc(sizeof(struct table));
            new_symble_table->next=NULL;
            new_symble_table->identifier=func_declarator->token;
            
            insert_table(tables,new_symble_table,func_declarator->token);

        }
    }

    //Caso já esteja, imprime a mensagem de erro e incrementa semantic_errors
    else {
        printf("Line %d, column %d: Symbol %s already declared\n", func_declarator->token_line, func_declarator->token_column,func_declarator->token);
        semantic_errors++;
    }

}

void check_Statement(struct node *node, struct table *table){

    if(node!=NULL){

        if(node->category==If){
            struct node *expression = getchild(node,0);
            struct node *statementesp = getchild(node,1);
            struct node *statementespelse = getchild(node,2);

            check_Expression(expression,table);

            check_Statement(statementesp,table);
            if ( (statementespelse!=NULL) /*&& (statementespelse->category != Null)*/ ){
                check_Statement(statementespelse, table);
            }
        }

        else if(node->category==While){
            struct node *expression = getchild(node,0);
            struct node *statementesp = getchild(node,1);
            check_Expression(expression,table);
            if (statementesp->category != Null){
                
                check_Statement(statementesp, table);
            }
        }

        else if(node->category==Return){
            //Lidar com o erro Line %d, column %d: Conflicting types (got %s, expected %s)\n Tem de ser nas expressions
            struct node *expression = getchild(node,0);

            check_Expression(expression,table);   
        }

        else if(node->category==StatList){
            struct node_list *statements=node->children;
            while((statements = statements->next) != NULL){

                check_Statement(statements->node,table); 

            }
        }

        else{
            check_Expression(node,table);
        }

    }

}

void check_Expression(struct node *node, struct table *table){
        switch(node->category) {

        case Identifier:

            if(search_symbol2(table, node->token) == NULL && search_symbol2(global_table, node->token) == NULL) {
                printf("Line %d, column %d: Unknown symbol %s\n",node->token_line, node->token_column,node->token);
                semantic_errors++;
                node->type = undef_type;
            }
            
            else if ( search_symbol2(table, node->token) != NULL) {
                node->type = search_symbol2(table, node->token)->type;
            }


            else if(search_symbol2(global_table, node->token) != NULL){
                node->type = search_symbol2(global_table, node->token)->type;

            }

            if(search_symbol2(table, node->token)!=NULL && search_symbol2(table, node->token)->error_flag==0){
                node->type = undef_type;
            }

            break;

        case Natural:
            node->type = integer_type;
            break;
        case ChrLit:
            node->type = integer_type;
            break;
        case Decimal:
            node->type = double_type;
            break;
        
        case Store:
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);
            
            if(getchild(node, 0)->type==char_type){
                node->type=char_type;
            }
            else if(getchild(node, 0)->type == undef_type){
                node->type=undef_type;
            }
            else if(getchild(node, 0)->type==double_type){
                node->type=double_type;
            }
            else if(getchild(node, 0)->type==integer_type){
                node->type=integer_type;
            }
            else if(getchild(node, 0)->type==short_type){
                node->type=short_type;
            }

            break;

        case Add:

            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);
            
            
            if(getchild(node, 0)->type == undef_type || getchild(node, 1)->type == undef_type){
                node->type=undef_type;
            }
            /*
            else if(){
                node->type=undef_type;
            }
            */
            else if(getchild(node, 0)->type==double_type || getchild(node, 1)->type==double_type){
                node->type=double_type;
            }
            else if(getchild(node, 0)->type==integer_type || getchild(node, 1)->type==integer_type){
                node->type=integer_type;
            }
            else if(getchild(node, 0)->type==short_type|| getchild(node, 1)->type==short_type){
                node->type=short_type;
            }
            else if(getchild(node, 0)->type==char_type|| getchild(node, 1)->type==char_type){
                node->type=char_type;
            }

            break;

        case Sub:
        
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);

            if(getchild(node, 0)->type == undef_type|| getchild(node, 1)->type == undef_type){
                node->type=undef_type;
            }
            else if(getchild(node, 0)->type==double_type || getchild(node, 1)->type==double_type){
                node->type=double_type;
            }
            else if(getchild(node, 0)->type==integer_type || getchild(node, 1)->type==integer_type){
                node->type=integer_type;
            }
            else if(getchild(node, 0)->type==short_type|| getchild(node, 1)->type==short_type){
                node->type=short_type;
            }
            else if(getchild(node, 0)->type==char_type|| getchild(node, 1)->type==char_type){
                node->type=char_type;
            }

            break;

        case Mul:

            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);

            enum type type1_Mul = getchild(node, 0)->type;
            enum type type2_Mul = getchild(node, 1)->type;

            // Verifica se algum dos tipos é indefinido
            if (type1_Mul == undef_type || type2_Mul == undef_type) {
                node->type = undef_type;
            }
            // Verifica se algum dos tipos é double
            else if (type1_Mul == double_type || type2_Mul == double_type) {
                node->type = double_type;
            }
            // Verifica se algum dos tipos é integer
            else if (type1_Mul == integer_type || type2_Mul == integer_type) {
                node->type = integer_type;
            }
            // Verifica se algum dos tipos é short
            else if (type1_Mul == short_type || type2_Mul == short_type) {
                node->type = short_type;
            }

            // Se todos os tipos são char, então o resultado é char
            else if (type1_Mul == char_type && type2_Mul == char_type) {
                node->type = char_type;
            }

            break;

        case Div:
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);

            enum type type1_div = getchild(node, 0)->type;
            enum type type2_div = getchild(node, 1)->type;

            // Verifica se algum dos tipos é indefinido
            if (type1_div == undef_type || type2_div == undef_type) {
                node->type = undef_type;
            }
            // Verifica se algum dos tipos é double
            else if (type1_div == double_type || type2_div == double_type) {
                node->type = double_type;
            }
            // Verifica se algum dos tipos é integer
            else if (type1_div == integer_type || type2_div == integer_type) {
                node->type = integer_type;
            }
            // Verifica se algum dos tipos é short
            else if (type1_div == short_type || type2_div == short_type) {
                node->type = short_type;
            }

            // Se todos os tipos são char, então o resultado é char
            else if (type1_div == char_type && type2_div == char_type) {
                node->type = char_type;
            }


            break;
        case Mod:
        case Or:
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);
            node->type = integer_type;
            break;

        case And:
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);
            node->type = integer_type;
            break;
            
        case BitWiseAnd://?
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);
            node->type = integer_type;
            break;

        case BitWiseOr://?
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);
            node->type = integer_type;
            break;

        case BitWiseXor://?
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);
            node->type = integer_type;
            break;

        case Eq:
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);
            if(getchild(node, 0)->type==double_type || getchild(node, 1)->type==double_type){
                node->type=double_type;
            }
            else if(getchild(node, 0)->type==integer_type || getchild(node, 1)->type==integer_type){
                node->type=integer_type;
            }
            else if(getchild(node, 0)->type==short_type|| getchild(node, 1)->type==short_type){
                node->type=short_type;
            }

        case Ne:
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);
            node->type = integer_type;
            break;
        case Ge:
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);
            node->type = integer_type;
            break;
        case Lt:
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);
            node->type = integer_type;
            break;
        case Le:
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);
            node->type = integer_type;
            break;
        case Gt:
            check_Expression(getchild(node, 0), table);
            check_Expression(getchild(node, 1), table);
            node->type = integer_type;
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
            if(getchild(node, 0)->type == undef_type|| getchild(node, 1)->type == undef_type){
                node->type=undef_type;
            }
            else if(getchild(node, 0)->type==double_type || getchild(node, 1)->type==double_type){
                node->type=double_type;
            }
            else if(getchild(node, 0)->type==integer_type || getchild(node, 1)->type==integer_type){
                node->type=integer_type;
            }
            else if(getchild(node, 0)->type==short_type|| getchild(node, 1)->type==short_type){
                node->type=short_type;
            }
            else if(getchild(node, 0)->type==char_type|| getchild(node, 1)->type==char_type){
                node->type=char_type;
            }
            break;
            
        case Call:


            if(search_symbol2(global_table, getchild(node, 0)->token) == NULL) {
                
                //Call
                struct table *table_aux = search_symbol2(table,getchild(node, 0)->token);
                
                if(table_aux!=NULL){
                    node->type = table_aux->type;
                    
                    //Identifier
                    check_Expression(getchild(node, 0),table);
                    //Expressions
                    int i = 1;
                    while(getchild(node,i)!=NULL){
                        check_Expression(getchild(node,i),table);
                        i++;
                    }
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
                }

            }

            else if(search_symbol2(global_table, getchild(node, 0)->token) != NULL){

                struct table *table_aux = search_symbol2(global_table,getchild(node, 0)->token);
                if(table_aux!=NULL){
                    node->type = table_aux->type;
                }
                  
                //Identifier
                check_Expression(getchild(node, 0),table);
                //Expressions
                int i = 1;
                while(getchild(node,i)!=NULL){
                    check_Expression(getchild(node,i),table);
                    i++;
                }
            }

            break;
        default:
            break;
    }
}

//insere um novo simbolo na table
struct table *insert_symbol(struct table *table, char *identifier,enum type type, struct parameter_list *v_type, struct node *node,int error_flag) {
    struct table *new = (struct table *) malloc(sizeof(struct table));
    if(identifier!=NULL) new->identifier = strdup(identifier);
    else new->identifier = NULL;
    new->type = type;
    new->parameter = v_type;
    new->node = node;
    new->error_flag = error_flag;
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
                // print category name
                //printf("%d\n", node->category);
                return symbol;
            }
        return NULL;
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
                if (symbol->parameter != NULL) {

                    printf("\tparam");
                }
                printf("\n");

            }

        }
    }

    printf("\n");

}

char *category_name_note[] = names;


// traverse the AST and print its content
void show_annotated(struct node *node, int depth) {
    if (node == NULL) return; // if tree is empty
    int i;
    for (i = 0; i < depth; i++)
        printf("..");
        
    if (node->token == NULL){


        if(strcmp(type_name(node->type),"none")!=0 && (node->category!=Int) && (node->category!=Short) && (node->category!= Natural) && (node->category!=Double) && (node->category!= Decimal) && (node->category!=Void) && (node->category!=Char) ){

            printf("%s - %s\n", category_name_note[node->category], type_name(node->type));

        }

        else{

            printf("%s\n", category_name_note[node->category]);

        }


    }


    else{

        if(strcmp(type_name(node->type),"none")!=0){

            //trata do caso de imprimir na tree x(x) - x(x)
            struct table *table_local = search_symbol(global_table,node);

            if(table_local!=NULL && table_local->parameter!=NULL && node->type != undef_type){

                struct parameter_list* head = table_local->parameter;

                printf("%s(%s) - %s(%s", category_name_note[node->category], node->token, type_name(node->type),type_name(table_local->parameter->parameter));
                while((table_local->parameter = table_local->parameter->next) !=NULL){
                    printf(",%s",type_name(table_local->parameter->parameter));
                }
                printf(")\n");

                table_local->parameter = head;

            }

            else{

            printf("%s(%s) - %s\n", category_name_note[node->category], node->token, type_name(node->type));
            
            }

        }

        else{

            printf("%s(%s)\n", category_name_note[node->category], node->token);
        }


    }

    // Iterate over the children of the current node
    struct node_list *child = node->children;
    while (child != NULL) {
        show_annotated(child->node, depth + 1);
        child = child->next;
    }

    // Iterate over the brothers of the current node
    struct node_list *brother = node->brothers;
    while (brother != NULL) {
        show_annotated(brother->node, depth);
        brother = brother->next;
    }

}


//-----------------------------[Raul]-------------------------------------------------
struct table_list *search_table(struct table_list *table, char *token) {

    struct table_list *symbol;

    for(symbol = table->next; symbol != NULL; symbol = symbol->next){

        if( (symbol->func_name!=NULL) && (token!=NULL) && (strcmp(symbol->func_name, token) == 0) ){
            return symbol;
        }
        
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

// Cria um novo nó de parâmetro e o adiciona à lista
struct parameter_list *add_parameter2(struct parameter_list *list, enum type parameter, char *name) {

    // Aloca memória para o novo nó de parâmetro
    struct parameter_list *new_param = malloc(sizeof(struct parameter_list));
    if (new_param == NULL) {
        // Lida com falha na alocação de memória, por exemplo, retornando ou saindo
        return list;
    }

    // Inicializa o campo de nome
    if (name != NULL) {
        new_param->Identifier = strdup(name);
    } else if(name == NULL && parameter!=void_type) {
        //printf("Unexpected error: Parameter symbol required\n");
        new_param->Identifier = NULL;
    } else if(name != NULL && parameter==void_type){
        new_param->Identifier = NULL;
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

char *get_identifier(struct node *paramdeclaration){
    struct node *tspec = getchild(paramdeclaration,0);
    enum type type =category_type(tspec->category);
    if(type!=void_type && getchild(paramdeclaration,1)!=NULL){
        struct node *id = getchild(paramdeclaration,1);
        return id->token;  
    }
    return NULL; 
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

void insert_params_to_symbol_table(struct table *symbol_tableFunc, struct parameter_list *params) {
    while (params != NULL) {
        // Assuming you have some default node for parameters, modify as needed

        // Check if the parameter type is not void before inserting
        if (params->parameter != void_type && params->Identifier!=NULL) {
            // Insert the parameter into the symbol table
            symbol_tableFunc = insert_symbol(symbol_tableFunc,params->Identifier, params->parameter, NULL, NULL,1);
        }
        params = params->next; // Move to the next parameter
    }
}


