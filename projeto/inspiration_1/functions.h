#include "structures.h"

// META 2
_program* new_program(void* next);
_program* new_declaration(_terminal* id, void* expression);
_program* set_typeSpec(_typeSpec* typespec, _declaration* header);
_program* new_functionDeclaration(_typeSpec* typespec, _terminal* id, _parameterList* parameter_list);
_program* new_functionDefinition(_typeSpec* typespec, _terminal* id, _parameterList* parameter_list, _functionBody* function_body);
_program* new_parameterList(_parameterDeclaration* parameter);
_program* new_parameterDeclaration(_typeSpec* typespec, _terminal* id);
_program* new_functionBody(void* declaration_or_statement);
void* append_list(void* header, void* new);
_program* new_statementList(void* statement);
_program* new_if(void* condition, void* if_branch, void* else_branch);
_program* new_while(void* condition, void* branch);
_program* new_return(void* expression);
_program* new_binaryOp(node op, void* left_expression, void* right_expression);
_program* new_unaryOp(node op, void* expression);
_program* new_call(_terminal* id, void* parameter);
_program* new_terminal(node type, char* value);
_program* new_typeSpec(node type);
void printTree(_program* astree, int depth);
void cleanTree(_program* astree);


// META 3
//void iniciar_tabela(void);																		// Inicia a Tabela (getchar + putchar) [GLOBAL]
//elemento_tabela * inserir_elem(char * id, t_elemento tipo_elem, t_basico tipo_basico);			// Inserir ID (var ou func) na tabela [GLOBAL]
//elemento_tabela * inserir_param_func(char * id_funcao, char * id, t_basico tipo_basico);		// Inserir ID-Param a uma Funcao [AMBIENTE PARTICULAR]
//elemento_tabela * inserir_var_func(char * id_funcao, char * id, t_basico tipo_basico);			// Inserir ID-Var Local a uma Funcao [AMBIENTE PARTICULAR]
void tabela_print(void);																		// Imprimir Tabela
t_basico check_program(_program* astree, char* ambiente);
void printTreeAnotada(_program* astree, int depth, int isAnotada);
//elemento_tabela * pesquisa_elem_by_id(char * id);												// Pesquisa ID (Func ou Var) na Tabela GLOBAL
//t_basico pesquisa_var_by_func(char * id, char * id_func);										// Pesquisa Var-ID (Ambiente Func [Param + Local] depois global) | Return o tipo da var em registo


