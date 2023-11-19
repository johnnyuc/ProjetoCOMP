#include "structures.h"
#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BURRY(_depth_, _string_)  for(int i = 0; i < _depth_; i++){printf("..");} printf("%s", _string_);
#define DEBUG 0
#define PRINT_ERROS 0
#define ANTI_SEGM_FAULT_TEMP 1


// META 2
	char* names[] = {
		"Char", "Short", "Int", "Double", "Void", "Id",    
		"Program", "Declaration", "FuncDeclaration", "ParamDeclaration", "ParamList", "FuncBody", "StatList", "FuncDefinition",
		"If", "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Add", "Sub", "Mul", "Div", "Mod", "Store",
		"Comma", "BitWiseAnd", "BitWiseXor", "BitWiseOr", "While", "Call", "Not", "Minus", "Plus", "Return",
		"ChrLit", "IntLit", "RealLit", "Null", "Undef"
	};

	// META 3 *
	char* t_basico_str[] = {"char", "short", "int", "double", "void", "undef", "NULO Sem Significado"};
	// * * *

	void* emalloc(size_t size){
		void* pointer = malloc(size);

		if(pointer == NULL)
			printf("Error: malloc(%zu) failed!\n", size);

		return pointer;
	}

	_program* new_program(void* next){
		_program* new = emalloc(sizeof(_program));

		new->type = is_program;
		new->next = next;

		return new;
	}

	_program* new_declaration(_terminal* id, void* expression){
		_declaration* new = emalloc(sizeof(_declaration));

		new->type = is_declaration;
		new->typespec = NULL;
		new->id = id;
		new->expression = expression;
		new->next = NULL;

		return (_program*) new;
	}

	_program* set_typeSpec(_typeSpec* typespec, _declaration* header){
		_declaration* head = header;
		_typeSpec* new_typespec;

		head->typespec = typespec;

		while(head->next && ((_declaration*)head->next)->type == is_declaration){
			new_typespec = emalloc(sizeof(_typeSpec));
			memcpy((void*) new_typespec, (void*) typespec, sizeof(_typeSpec));

			((_declaration*)head->next)->typespec = new_typespec;

			head = head->next;
		}

		return (_program*) header;
	}

	_program* new_functionDeclaration(_typeSpec* typespec, _terminal* id, _parameterList* parameter_list){
		_functionDeclaration* new = emalloc(sizeof(_functionDeclaration));

		new->type = is_functionDeclaration;
		new->typespec = typespec;
		new->id = id;
		new->parameter_list = parameter_list;
		new->next = NULL;

		return (_program*) new;
	}

	_program* new_functionDefinition(_typeSpec* typespec, _terminal* id, _parameterList* parameter_list, _functionBody* function_body){
		_functionDefinition* new = emalloc(sizeof(_functionDefinition));

		new->type = is_functionDefinition;
		new->typespec = typespec;
		new->id = id;
		new->parameter_list = parameter_list;
		new->function_body = function_body;
		new->next = NULL;

		return (_program*) new;
	}

	_program* new_parameterList(_parameterDeclaration* parameter){
		_parameterList* new = emalloc(sizeof(_parameterList));

		new->type = is_parameterList;
		new->first_parameter = parameter;

		return (_program*) new;
	}

	_program* new_parameterDeclaration(_typeSpec* typespec, _terminal* id){
		_parameterDeclaration* new = emalloc(sizeof(_parameterDeclaration));

		new->type = is_parameterDeclaration;
		new->typespec = typespec;
		new->id = id;
		new->next = NULL;

		return (_program*) new;
	}

	_program* new_functionBody(void* declaration_or_statement){
		_functionBody* new = emalloc(sizeof(_functionBody));

		new->type = is_functionBody;
		new->declaration_or_statement = declaration_or_statement;

		return (_program*) new;
	}

	void* append_list(void* header, void* new){
		_if* head = (_if*) header;
		if(!head)
			return new;

		while(head->next)
			head = (_if*) head->next;

		head->next = new;
		return header;
	}

	_program* new_statementList(void* statement){
		_statementList* new = emalloc(sizeof(_statementList));

		new->type = is_statementList;
		new->first_statement = statement;
		new->next = NULL;

		return (_program*) new;
	}

	_program* new_if(void* condition, void* if_branch, void* else_branch){
		_if* new = emalloc(sizeof(_if));

		new->type = is_if;
		new->condition = condition;
		new->if_branch = if_branch;
		new->else_branch = else_branch;
		new->next = NULL;

		return (_program*) new;
	}

	_program* new_while(void* condition, void* branch){
		_while* new = emalloc(sizeof(_while));

		new->type = is_while;
		new->condition = condition;
		new->branch = branch;
		new->next = NULL;

		return (_program*) new;
	}

	_program* new_return(void* expression){
		_return* new = emalloc(sizeof(_return));

		new->type = is_return;
		new->expression = expression;
		new->next = NULL;

		return (_program*) new;
	}

	_program* new_binaryOp(node op, void* left_expression, void* right_expression){
		_binaryOp* new = emalloc(sizeof(_binaryOp));

		new->type = op;
		new->left_expression = left_expression;
		new->right_expression = right_expression;
		new->next = NULL;

		return (_program*) new;
	}

	_program* new_unaryOp(node op, void* expression){
		_unaryOp* new = emalloc(sizeof(_unaryOp));

		new->type = op;
		new->expression = expression;
		new->next = NULL;

		return (_program*) new;
	}

	_program* new_call(_terminal* id, void* parameter){
		_call* new = emalloc(sizeof(_call));

		new->type = is_call;
		new->id = id;
		new->first_parameter = parameter;
		new->next = NULL;

		return (_program*) new;
	}

	_program* new_terminal(node type, char* value){
		_terminal* new = emalloc(sizeof(_terminal));

		new->type = type;
		new->value = value;
		new->next = NULL;

		return (_program*) new;
	}

	_program* new_typeSpec(node type){
		_typeSpec* new = emalloc(sizeof(_typeSpec));

		new->type = type;

		return (_program*) new;
	}

	void printTree(_program* astree, int depth){
		if(!astree)
			return;
		BURRY(depth, names[astree->type]);
		switch(astree->type){
			case Char:
			case Int:
			case Short:
			case Double:
			case Void:
			case is_null:;
				printf("\n");
				break;
			case Id:
			case is_chrLit:
			case is_intLit:
			case is_realLit:;
				_terminal* terminal = (_terminal*) astree;
				printf("(%s)\n", terminal->value);
				printTree((_program*)terminal->next, depth);
				break;
			case is_or:
			case is_and:
			case is_eq:
			case is_ne:
			case is_lt:
			case is_gt:
			case is_le:
			case is_ge:
			case is_add:
			case is_sub:
			case is_mul:
			case is_div:
			case is_mod:
			case is_store:
			case is_comma:
			case is_bitwiseand:
			case is_bitwisexor:
			case is_bitwiseor:;
				printf("\n");
				_binaryOp* bin = (_binaryOp*) astree;
				printTree((_program*)bin->left_expression, depth + 1);
				printTree((_program*)bin->right_expression, depth + 1);
				printTree((_program*)bin->next, depth);
				break;
			case is_return:
			case is_not:
			case is_minus:
			case is_plus:;
				printf("\n");
				_unaryOp* un = (_unaryOp*) astree;
				printTree((_program*)un->expression, depth + 1);
				printTree((_program*)un->next, depth);
				break;
			case is_while:;
				printf("\n");
				_while* wh = (_while*) astree;
				printTree((_program*)wh->condition, depth + 1);
				printTree((_program*)wh->branch, depth + 1);
				printTree((_program*)wh->next, depth);
				break;
			case is_call:;
				printf("\n");
				_call* call = (_call*) astree;
				printTree((_program*)call->id, depth + 1);
				printTree((_program*)call->first_parameter, depth + 1);
				printTree((_program*)call->next, depth);
				break;
			case is_if:;
				printf("\n");
				_if* iif = (_if*) astree;
				printTree((_program*)iif->condition, depth + 1);
				printTree((_program*)iif->if_branch, depth + 1);
				printTree((_program*)iif->else_branch, depth + 1);
				printTree((_program*)iif->next, depth);
				break;
			case is_functionDefinition:;
				printf("\n");
				_functionDefinition* fdef = (_functionDefinition*) astree;
				printTree((_program*)fdef->typespec, depth + 1);
				printTree((_program*)fdef->id, depth + 1);
				printTree((_program*)fdef->parameter_list, depth + 1);
				printTree((_program*)fdef->function_body, depth + 1);
				printTree((_program*)fdef->next, depth);
				return;
			case is_parameterList:;
				printf("\n");
				_parameterList* plist = (_parameterList*) astree;
				printTree((_program*)plist->first_parameter, depth + 1);
				break;
			case is_functionBody:;
				printf("\n");
				_functionBody* fbody = (_functionBody*) astree;
				printTree((_program*)fbody->declaration_or_statement, depth + 1);
				break;
			case is_statementList:
				printf("\n");
				_statementList* slist = (_statementList*) astree;
				printTree((_program*)slist->first_statement, depth + 1);
				printTree((_program*)slist->next, depth);
				break;
			case is_declaration:
				printf("\n");
				_declaration* dec = (_declaration*) astree;
				printTree((_program*)dec->typespec, depth + 1);
				printTree((_program*)dec->id, depth + 1);
				printTree((_program*)(_program*)dec->expression, depth + 1);
				printTree((_program*)dec->next, depth);
				break;
			case is_functionDeclaration:
				printf("\n");
				_functionDeclaration* fdec = (_functionDeclaration*) astree;
				printTree((_program*)fdec->typespec, depth + 1);
				printTree((_program*)fdec->id, depth + 1);
				printTree((_program*)fdec->parameter_list, depth + 1);
				printTree((_program*)fdec->next, depth);
				break;
			case is_parameterDeclaration:
				printf("\n");
				_parameterDeclaration* pdec = (_parameterDeclaration*) astree;
				printTree((_program*)pdec->typespec, depth + 1);
				printTree((_program*)pdec->id, depth + 1);
				printTree((_program*)pdec->next, depth);
				break;
			case is_program:
				printf("\n");
				printTree((_program*)astree->next, depth + 1);
				break;
		}
	}


	void cleanTree(_program* astree){
		if(!astree)
			return;

		switch(astree->type){
			case Char:
			case Int:
			case Short:
			case Double:
			case Void:
			case is_null:;
				free(astree);
				break;
			case Id:
			case is_chrLit:
			case is_intLit:
			case is_realLit:;
				_terminal* terminal = (_terminal*) astree;
				free(terminal->value);
				cleanTree((_program*)terminal->next);
				free(terminal);
				break;
			case is_or:
			case is_and:
			case is_eq:
			case is_ne:
			case is_lt:
			case is_gt:
			case is_le:
			case is_ge:
			case is_add:
			case is_sub:
			case is_mul:
			case is_div:
			case is_mod:
			case is_store:
			case is_comma:
			case is_bitwiseand:
			case is_bitwisexor:
			case is_bitwiseor:;
				_binaryOp* bin = (_binaryOp*) astree;
				cleanTree((_program*)bin->left_expression);
				cleanTree((_program*)bin->right_expression);
				cleanTree((_program*)bin->next);
				free(bin);
				break;
			case is_return:
			case is_not:
			case is_minus:
			case is_plus:;
				_unaryOp* un = (_unaryOp*) astree;
				cleanTree((_program*)un->expression);
				cleanTree((_program*)un->next);
				free(un);
				break;
			case is_while:;
				_while* wh = (_while*) astree;
				cleanTree((_program*)wh->condition);
				cleanTree((_program*)wh->branch);
				cleanTree((_program*)wh->next);
				free(wh);
				break;
			case is_call:;
				_call* call = (_call*) astree;
				cleanTree((_program*)call->id);
				cleanTree((_program*)call->first_parameter);
				cleanTree((_program*)call->next);
				free(call);
				break;
			case is_if:;
				_if* iif = (_if*) astree;
				cleanTree((_program*)iif->condition);
				cleanTree((_program*)iif->if_branch);
				cleanTree((_program*)iif->else_branch);
				cleanTree((_program*)iif->next);
				free(iif);
				break;
			case is_functionDefinition:;
				_functionDefinition* fdef = (_functionDefinition*) astree;
				cleanTree((_program*)fdef->typespec);
				cleanTree((_program*)fdef->id);
				cleanTree((_program*)fdef->parameter_list);
				cleanTree((_program*)fdef->function_body);
				cleanTree((_program*)fdef->next);
				free(fdef);
				return;
			case is_parameterList:;
				_parameterList* plist = (_parameterList*) astree;
				cleanTree((_program*)plist->first_parameter);
				free(plist);
				break;
			case is_functionBody:;
				_functionBody* fbody = (_functionBody*) astree;
				cleanTree((_program*)fbody->declaration_or_statement);
				free(fbody);
				break;
			case is_statementList:;
				_statementList* slist = (_statementList*) astree;
				cleanTree((_program*)slist->first_statement);
				cleanTree((_program*)slist->next);
				free(slist);
				break;
			case is_declaration:;
				_declaration* dec = (_declaration*) astree;
				cleanTree((_program*)dec->typespec);
				cleanTree((_program*)dec->id);
				cleanTree((_program*)(_program*)dec->expression);
				cleanTree((_program*)dec->next);
				free(dec);
				break;
			case is_functionDeclaration:;
				_functionDeclaration* fdec = (_functionDeclaration*) astree;
				cleanTree((_program*)fdec->typespec);
				cleanTree((_program*)fdec->id);
				cleanTree((_program*)fdec->parameter_list);
				cleanTree((_program*)fdec->next);
				free(fdec);
				break;
			case is_parameterDeclaration:;
				_parameterDeclaration* pdec = (_parameterDeclaration*) astree;
				cleanTree((_program*)pdec->typespec);
				cleanTree((_program*)pdec->id);
				cleanTree((_program*)pdec->next);
				free(pdec);
				break;
			case is_program:;
				cleanTree((_program*)astree->next);
				free(astree);
				break;
		}
	}



// META 3
#define DEC 0
#define DEF 1

extern elemento_tabela * raiz_tabela;


// (INICIA) colocar na tabela os IDs das funcoes pre-definidas getchar, putchar
void iniciar_tabela(void) {
	elemento_tabela * novo = (elemento_tabela *) malloc(sizeof(elemento_tabela));
	elemento_tabela * aux;
	elemento_tabela * pre;

	strcpy(novo->id, "putchar");
	novo->tipo_elemento=tipo_funcao;
	novo->flag_def = 0;
	
	novo->tipo_basico = tipo_int;													// no caso das funcoes, e o valor de return
	novo->lista_param_func = (elemento_funcao *) malloc(sizeof(elemento_funcao));
	novo->lista_param_func->id[0] = '\0';											// sem var de entrada, apenas tipo [DECLARACAO]
	novo->lista_param_func->tipo_basico = tipo_int;
	novo->lista_param_func->next = NULL;
	novo->lista_vars_func = NULL;													// nao esta [ainda] definido no ficheiro, nem vai estra...
	novo->next=NULL;
	
	raiz_tabela = novo;
	
	
	novo = (elemento_tabela *) malloc(sizeof(elemento_tabela));

	strcpy(novo->id, "getchar");
	novo->tipo_elemento=tipo_funcao;
	novo->flag_def = 0;
	
	novo->tipo_basico = tipo_int;													// no caso das funcoes, e o valor de return
	novo->lista_param_func = (elemento_funcao *) malloc(sizeof(elemento_funcao));
	novo->lista_param_func->id[0] = '\0';											// sem var de entrada, apenas tipo [DECLARACAO]
	novo->lista_param_func->tipo_basico = tipo_void;
	novo->lista_param_func->next = NULL;
	novo->lista_vars_func = NULL;													// nao esta [ainda] definido no ficheiro, nem vai estar...
	novo->next=NULL;
	
	for(aux=raiz_tabela; aux; pre=aux, aux=aux->next);
	pre->next=novo;
}

// Procura um ID (Var ou Funcao) na tabela Global, devolve NULL caso nao exista
elemento_tabela * pesquisa_elem_by_id(char * id) {
	elemento_tabela * aux;
	
	for(aux=raiz_tabela; aux; aux=aux->next)
		if(strcmp(aux->id, id) == 0)
			return aux;																		// Encontrado
			
	return NULL;																			// Nao Encontrado
}


// Procura uma ID-var (Var-param -> var-local -> var-global) na tabela para func (primeiro no ambiente da funcao, depois no ambiente global), devolve undef caso nao exista
t_basico pesquisa_var_by_func(char * id, char * id_func) {
	elemento_tabela * aux;
	elemento_funcao * aux_func;
	
	if (id_func) {
		for(aux=raiz_tabela; aux && (strcmp(aux->id, id_func) != 0); aux=aux->next);
		if (!aux || (aux->tipo_elemento != tipo_funcao) || !(aux->flag_def))
			return tipo_undef;																	// Erro, Funcao Nao Existe ou nao definida
		
		if(aux->lista_param_func && aux->lista_param_func->tipo_basico != tipo_void) {			// se tem parametros [AMBIENTE Func]
			for(aux_func=aux->lista_param_func; aux_func; aux_func=aux_func->next) {
				if(strcmp(aux_func->id, id) == 0)
					return aux_func->tipo_basico;												// Encontrado PARAM
			}
		}
				
		if(aux->lista_vars_func) {																// se tem vars locais [AMBIENTE Func]
			for(aux_func=aux->lista_vars_func; aux_func; aux_func=aux_func->next) {
				if(strcmp(aux_func->id, id) == 0)
					return aux_func->tipo_basico;												// Encontrado LOCAL
			}
		}
	}
	
	for(aux=raiz_tabela; aux ; aux=aux->next) {												// Por ultimo verificar na tabela global [AMBIENTE GLOBAL]
		if((aux->tipo_elemento == tipo_var) && (strcmp(aux->id, id) == 0))
			return aux->tipo_basico;														// Encontrado GLOBAL
	}
	
	return tipo_undef;																		// Nao Encontrado, var nao declarada
}

// Insere um novo ID no fim da lista parametros da funcao (ambiente da funcao)
elemento_tabela * inserir_param_func(char * id_funcao, char * id, t_basico tipo_basico) {
	elemento_funcao * novo = (elemento_funcao *) malloc(sizeof(elemento_funcao));
	elemento_funcao * aux;
	elemento_funcao * pre;
	
	elemento_tabela * aux_tab;
	
	char str_nulo[] = "\0";

	strcpy(novo->id, id);
	novo->tipo_basico = tipo_basico;
	novo->next=NULL;
	
	if (!id_funcao) {																			// ATENCAO: temporario, ate id_funcao funcionar direito
		id_funcao = str_nulo;
	}
	
	if(raiz_tabela) {
		for(aux_tab=raiz_tabela; (aux_tab) && (strcmp(aux_tab->id, id_funcao) != 0); aux_tab=aux_tab->next);
		
		if(aux_tab == NULL) {
			free(novo);
			return NULL;																		// erro , nao existe funcao (id_funcao)
		}
		
		if(aux_tab->lista_param_func) {
			for(aux=pre=aux_tab->lista_param_func; aux; pre=aux, aux=aux->next)
				if(!(strcmp(id, "") == 0) && strcmp(aux->id, id) == 0) {
					free(novo);
					printf("parametros com id iguais\n");
					return NULL;																// erro, parametros com IDs iguais
				}
				
			pre->next=novo;
		} else
			aux_tab->lista_param_func=novo;	
	} else {
		free(novo);
		return NULL; 																			// Erro, tabela nao tem elementos
	}
	
	return aux_tab;
}


// Insere um novo ID no fim da lista global (func/var - ambiente global/principal)
elemento_tabela * inserir_elem(char def_ou_dec, char * id, t_elemento tipo_elem, t_basico tipo_basico) {
	elemento_tabela * novo = (elemento_tabela *) malloc(sizeof(elemento_tabela));
	elemento_tabela * aux;
	elemento_tabela * pre;

	strcpy(novo->id, id);
	novo->tipo_elemento = tipo_elem;
	novo->tipo_basico = tipo_basico;
	novo->flag_def = def_ou_dec;
	novo->lista_param_func = NULL;
	novo->lista_vars_func = NULL;
	novo->next=NULL;	
	
	if(raiz_tabela) {
		for(aux=raiz_tabela; aux; pre=aux, aux=aux->next)
			if(strcmp(aux->id, id) == 0) {
				if ( ((aux->tipo_elemento == tipo_elem) == tipo_funcao) && (aux->flag_def == DEC) && (def_ou_dec == DEF) ) {	// se ja existia uma dec e agora vem uma def, retorna aux
					free(novo);
					return aux;
				}
				else {
					free(novo);
					return NULL;																	// Erro, ja exite este ID na tabela
				}
			}
			
		pre->next=novo;																			// adiciona ao final da lista
	}
	else																				
		raiz_tabela=novo;		
	return novo;
}

// Insere um novo ID no fim da lista de Vars da funcao (ambiente da funcao)
elemento_tabela * inserir_var_func(char * id_funcao, char * id, t_basico tipo_basico) {
	elemento_funcao * novo = (elemento_funcao *) malloc(sizeof(elemento_funcao));
	elemento_funcao * aux;
	elemento_funcao * pre;
	
	elemento_tabela * aux_tab;

	strcpy(novo->id, id);
	novo->tipo_basico = tipo_basico;
	novo->next=NULL;	

	if(raiz_tabela) {
		for(aux_tab=raiz_tabela; aux_tab && (strcmp(aux_tab->id, id_funcao) != 0); aux_tab=aux_tab->next);
		
		if(aux_tab == NULL) {
			free(novo);
			return NULL;																		// erro , nao existe funcao (id_funcao)
		}
			
		if(aux_tab->lista_param_func) {
			for(aux=aux_tab->lista_param_func; aux; aux=aux->next)
				if(strcmp(aux->id, id) == 0) {
					free(novo);
					return NULL;																// erro, parametro-ID com ID igual ao ao Var-ID a inserir
				}
		}
			
		if(aux_tab->lista_vars_func) {
			for(aux=aux_tab->lista_vars_func; aux; pre=aux, aux=aux->next)
				if(strcmp(aux->id, id) == 0) {
					free(novo);
					return NULL;																// erro, Vars Locais com IDs iguais
				}
			pre->next=novo;
		} else
			aux_tab->lista_vars_func=novo;	
	} else {
		free(novo);
		return NULL; 																			// Erro, tabela nao tem elementos
	}
	
	return aux_tab;
}



void printTreeAnotada(_program* astree, int depth, int isAnotada){
		if(!astree)
			return;

		BURRY(depth, names[astree->type]);

		//if (DEBUG) printf("DEBUG: print\n");

		switch(astree->type){
			case Char:
			case Int:
			case Short:
			case Double:
			case Void:
			case is_null:;
				printf("\n");
				break;

			case Id:;

				//if (DEBUG) printf("DEBUG: print id 1\n");

				_terminal* terminal_id = (_terminal*) astree;

				//if ((terminal_id->anotation > nulo || terminal_id->anotation < (t_basico) 0) && ANTI_SEGM_FAULT_TEMP) terminal_id->anotation = nulo;

				printf("(%s)", terminal_id->value);

				//if (DEBUG) printf("DEBUG: print id 1\n");

				if (isAnotada == 2) {
					
					//if (DEBUG) printf("DEBUG: print id 1\n");

					elemento_tabela * aux = pesquisa_elem_by_id(terminal_id->value);

					//if (DEBUG) printf("DEBUG: print id 2\n");

					if (!aux)
						printf(" - %s\n", t_basico_str[tipo_undef]);
					else {
						elemento_funcao * aux_func = aux->lista_param_func;
						printf(" - %s", t_basico_str[aux->tipo_basico]);

						//if (DEBUG) printf("DEBUG: print id 3\n");
		
						if (aux->tipo_elemento == tipo_var) {
							printf("\n");
						} else {
							printf("(");
							for ( ; aux_func; aux_func=aux_func->next) {
								if (aux->lista_param_func != aux_func)
									printf(",");
								printf("%s", t_basico_str[aux_func->tipo_basico]);
							}
							printf(")\n");
						}
						//if (DEBUG) printf("DEBUG: print id 4\n");
					}
					
				} else {

					if (terminal_id->anotation == nulo || !isAnotada)
						printf("\n");
					else
						printf(" - %s\n", t_basico_str[terminal_id->anotation]);

				}
				
				printTreeAnotada((_program*)terminal_id->next, depth,1);
				break;

			case is_chrLit:
			case is_intLit:
			case is_realLit:;

				_terminal* terminal = (_terminal*) astree;

				//if ((terminal->anotation > nulo || terminal->anotation < (t_basico) 0) && ANTI_SEGM_FAULT_TEMP) terminal->anotation = nulo;

				printf("(%s)", terminal->value);

				printf(" - %s\n", t_basico_str[terminal->anotation]);

				printTreeAnotada((_program*)terminal->next, depth,1);
				break;

			case is_or:
			case is_and:
			case is_eq:
			case is_ne:
			case is_lt:
			case is_gt:
			case is_le:
			case is_ge:
			case is_add:
			case is_sub:
			case is_mul:
			case is_div:
			case is_mod:
			case is_store:
			case is_comma:
			case is_bitwiseand:
			case is_bitwisexor:
			case is_bitwiseor:;

				_binaryOp* bin = (_binaryOp*) astree;

				//if ((bin->anotation > nulo || bin->anotation < (t_basico) 0) && ANTI_SEGM_FAULT_TEMP) bin->anotation = nulo;

				if (bin->anotation == nulo || !isAnotada)
					printf("\n");
				else
					printf(" - %s\n", t_basico_str[bin->anotation]);

				printTreeAnotada((_program*)bin->left_expression, depth + 1, 1);
				printTreeAnotada((_program*)bin->right_expression, depth + 1, 1);
				printTreeAnotada((_program*)bin->next, depth, 1);

				break;
			case is_return:

				printf("\n");
				_unaryOp* un2 = (_unaryOp*) astree;

				printTreeAnotada((_program*)un2->expression, depth + 1, 1);
				printTreeAnotada((_program*)un2->next, depth, 1);
				break;

			case is_not:
			case is_minus:
			case is_plus:;

				_unaryOp* un = (_unaryOp*) astree;

				//if ((un->anotation > nulo || un->anotation < (t_basico) 0) && ANTI_SEGM_FAULT_TEMP) un->anotation = nulo;

				if (un->anotation == nulo || !isAnotada)
					printf("\n");
				else
					printf(" - %s\n", t_basico_str[un->anotation]);

				printTreeAnotada((_program*)un->expression, depth + 1, 1);
				printTreeAnotada((_program*)un->next, depth, 1);
				break;
			case is_while:;
				printf("\n");
				_while* wh = (_while*) astree;
				printTreeAnotada((_program*)wh->condition, depth + 1, 1);
				printTreeAnotada((_program*)wh->branch, depth + 1, 1);
				printTreeAnotada((_program*)wh->next, depth, 1);
				break;

			case is_call:;

				_call* call = (_call*) astree;

				if (DEBUG) printf("DEBUG: print call 1\n");
				//if ((call->anotation > nulo || call->anotation < (t_basico) 0) && ANTI_SEGM_FAULT_TEMP) call->anotation = nulo;
				if (DEBUG) printf("DEBUG: print call 2\n");

				if (call->anotation == nulo || !isAnotada)
					printf("\n");
				else
					printf(" - %s\n", t_basico_str[call->anotation]);

				if (DEBUG) printf("DEBUG: print call 3\n");

				printTreeAnotada((_program*)call->id, depth + 1, 2);
				if (DEBUG) printf("DEBUG: print call 4\n");
				printTreeAnotada((_program*)call->first_parameter, depth + 1, 1);
				printTreeAnotada((_program*)call->next, depth, 1);
				break;

			case is_if:;
				printf("\n");
				_if* iif = (_if*) astree;
				printTreeAnotada((_program*)iif->condition, depth + 1, 1);
				printTreeAnotada((_program*)iif->if_branch, depth + 1, 1);
				printTreeAnotada((_program*)iif->else_branch, depth + 1, 1);
				printTreeAnotada((_program*)iif->next, depth, 1);
				break;
			case is_functionDefinition:;
				printf("\n");
				_functionDefinition* fdef = (_functionDefinition*) astree;
				printTreeAnotada((_program*)fdef->typespec, depth + 1, 1);
				printTreeAnotada((_program*)fdef->id, depth + 1, 0);
				printTreeAnotada((_program*)fdef->parameter_list, depth + 1, 0);
				printTreeAnotada((_program*)fdef->function_body, depth + 1, 1);
				printTreeAnotada((_program*)fdef->next, depth, 1);
				return;
			case is_parameterList:;
				printf("\n");
				_parameterList* plist = (_parameterList*) astree;
				printTreeAnotada((_program*)plist->first_parameter, depth + 1, 0);
				break;
			case is_functionBody:;
				printf("\n");
				_functionBody* fbody = (_functionBody*) astree;
				printTreeAnotada((_program*)fbody->declaration_or_statement, depth + 1, 1);
				break;
			case is_statementList:
				printf("\n");
				_statementList* slist = (_statementList*) astree;
				printTreeAnotada((_program*)slist->first_statement, depth + 1, 1);
				printTreeAnotada((_program*)slist->next, depth, 1);
				break;
			case is_declaration:
				printf("\n");
				_declaration* dec = (_declaration*) astree;
				printTreeAnotada((_program*)dec->typespec, depth + 1, 0);
				printTreeAnotada((_program*)dec->id, depth + 1, 0);
				printTreeAnotada((_program*)(_program*)dec->expression, depth + 1, 0);
				printTreeAnotada((_program*)dec->next, depth, 1);
				break;
			case is_functionDeclaration:
				printf("\n");
				_functionDeclaration* fdec = (_functionDeclaration*) astree;
				printTreeAnotada((_program*)fdec->typespec, depth + 1, 0);
				printTreeAnotada((_program*)fdec->id, depth + 1, 0);
				printTreeAnotada((_program*)fdec->parameter_list, depth + 1, 0);
				printTreeAnotada((_program*)fdec->next, depth, 1);
				break;
			case is_parameterDeclaration:
				printf("\n");
				_parameterDeclaration* pdec = (_parameterDeclaration*) astree;
				printTreeAnotada((_program*)pdec->typespec, depth + 1, 0);
				printTreeAnotada((_program*)pdec->id, depth + 1, 0);
				printTreeAnotada((_program*)pdec->next, depth, 1);
				break;
			case is_program:
				printf("\n");
				printTreeAnotada((_program*)astree->next, depth + 1, 1);
				break;
		}
	}


void print_tipo_funcao_tabela(elemento_tabela* funcao){
	printf("%s(", t_basico_str[funcao->tipo_basico]);
	elemento_funcao* head = funcao->lista_param_func;
	while(head->next){
		printf("%s,", t_basico_str[head->tipo_basico]);
		head = head->next;
	}
	printf("%s)", t_basico_str[head->tipo_basico]);
}

void print_tipo_functionDeclaration(_functionDeclaration* fDeclaration){
	printf("%s(", t_basico_str[fDeclaration->typespec->type]);
	_parameterDeclaration* head = fDeclaration->parameter_list->first_parameter;
	while(head->next){
		printf("%s,", t_basico_str[head->typespec->type]);
		head = head->next;
	}
	printf("%s)", t_basico_str[head->typespec->type]);
}


// Print Tabela
void tabela_print(void) {
	elemento_tabela * aux;
	elemento_funcao * aux_func;
	
	printf("===== Global Symbol Table =====\n");
	for(aux=raiz_tabela; aux; aux=aux->next) {
		printf("%s\t%s", aux->id, t_basico_str[aux->tipo_basico]);
		if (aux->tipo_elemento == tipo_var) {
			printf("\n");
		} else {
			printf("(");
			for (aux_func=aux->lista_param_func; aux_func; aux_func=aux_func->next) {
				if (aux->lista_param_func != aux_func)
					printf(",");
				printf("%s", t_basico_str[aux_func->tipo_basico]);
			}
			printf(")\n");
		}
	}
	
	
	putchar('\n');
	for(aux=raiz_tabela; aux; aux=aux->next) {
		if (aux->tipo_elemento == tipo_funcao) {
			if(aux->flag_def == DEC)
				continue;																	// Func nao definida (ignorar, ex.: putchar, getchar)
			
			printf("===== Function %s Symbol Table =====\n", aux->id);
			printf("return\t%s\n", t_basico_str[aux->tipo_basico]);
		
			if(aux->lista_param_func && aux->lista_param_func->tipo_basico != tipo_void) {	// se tem parametros
				for(aux_func=aux->lista_param_func; aux_func; aux_func=aux_func->next) {
					printf("%s\t%s\tparam\n", aux_func->id, t_basico_str[aux_func->tipo_basico]);
				}
			}
			
			if(aux->lista_vars_func) {														// se tem vars locais
				for(aux_func=aux->lista_vars_func; aux_func; aux_func=aux_func->next) {
					printf("%s\t%s\n", aux_func->id, t_basico_str[aux_func->tipo_basico]);
				}
			}
			putchar('\n');
		}
	}
}


elemento_tabela* inserir_funcao(char def_ou_dec, _functionDeclaration* fdec) {

	elemento_tabela* novo = inserir_elem(def_ou_dec, fdec->id->value, tipo_funcao, (t_basico) fdec->typespec->type);

	fdec->id->anotation = (t_basico) fdec->typespec->type;

	_parameterDeclaration* new_head = fdec->parameter_list->first_parameter;
		
	while(new_head){
		if(new_head->id) {
			inserir_param_func(fdec->id->value, new_head->id->value, (t_basico) new_head->typespec->type);
			new_head->id->anotation = (t_basico) new_head->typespec->type;
		} else
			inserir_param_func(fdec->id->value, "", (t_basico) new_head->typespec->type);
		
		new_head = new_head->next;
	}
		
	return novo;
}



t_basico check_functionCall(_call* fCall, char* ambiente){
	// verificar existência da função, tipos de parâmetros...

	elemento_tabela* result = pesquisa_elem_by_id(fCall->id->value);
	t_basico tipo_basico_var;

	if (!result) { // se funcao nao existe
		return tipo_undef;
	}

	if (result->tipo_elemento != tipo_funcao) { // se existe mas e var e nao funtion
		return tipo_undef;
	}
	
	_terminal * new_head_id = fCall->first_parameter; // lista de parametros
	elemento_funcao* head = result->lista_param_func; // lista de parâmetros já inseridos

	while(new_head_id && head){ // enquanto ambos tiverem parâmetros
		
		// vai dar merda depois, exemplo: function(function());
		tipo_basico_var = pesquisa_var_by_func(new_head_id->value, ambiente);

		if (tipo_basico_var == tipo_undef) {
			// erro var nao existe
		}

		if((t_basico) tipo_basico_var != head->tipo_basico) //se os tipos forem diferentes
			if( !(((tipo_basico_var == tipo_char) || (tipo_basico_var == tipo_int)) && ((head->tipo_basico == tipo_int) || (head->tipo_basico == tipo_char))) ) //ex: putchar(int)
				if (PRINT_ERROS) printf("Conflicting types (got %s, expected %s)\n", t_basico_str[tipo_basico_var], t_basico_str[head->tipo_basico]);
				
		new_head_id->anotation = tipo_basico_var;
		new_head_id = new_head_id->next;
		head = head->next;
	}

	while(new_head_id) {
		if (PRINT_ERROS) printf("Redeclaração tem mais parametros\n");
		tipo_basico_var = pesquisa_var_by_func(new_head_id->value, ambiente);
		new_head_id->anotation = tipo_basico_var;

		new_head_id = new_head_id->next;
	}

	while(head){
		if (PRINT_ERROS) printf("Declaração tem mais parametros\n");

		head = head->next;
	}

	return result->tipo_basico;
}


t_basico check_functionDefinition(_functionDefinition* fDefinition){
	// verificar se existem declarações anteriores, se está de acordo com essas declarações, se é redefinição...
	elemento_tabela* result = pesquisa_elem_by_id(fDefinition->id->value);

	if(result){

		if(result->flag_def == DEC) {
			result->flag_def = DEF;

			if((t_basico) fDefinition->typespec->type != result->tipo_basico) { // se os tipos de retorno não forem iguais
				if (PRINT_ERROS) printf("Conflicting types (got %s, expected %s)\n", t_basico_str[fDefinition->typespec->type], t_basico_str[result->tipo_basico]);
				result->flag_def = DEC;
			}
			_parameterDeclaration* new_head = fDefinition->parameter_list->first_parameter; // lista de parametros da redeclaração/definição
			elemento_funcao* head = result->lista_param_func; // lista de parâmetros já inseridos

			if (DEBUG) printf("DEBUG: check_functionDefinition 1\n");

			while(new_head && head){ // enquanto ambos tiverem parâmetros

				if (DEBUG && !(new_head->typespec) ) printf("DEBUG: check_functionDefinition erro 1\n");
				else { if (DEBUG && !(new_head->typespec->type) ) printf("DEBUG: check_functionDefinition erro 2\n"); }
				
				if (DEBUG && !(new_head->id) ) printf("DEBUG: check_functionDefinition erro 3\n");
				else {
				if (DEBUG && !(new_head->id->value) ) printf("DEBUG: check_functionDefinition erro 4\n");
				if (DEBUG && !(new_head->id->anotation ) ) printf("DEBUG: check_functionDefinition erro 5\n");
				}

				if((t_basico) new_head->typespec->type != head->tipo_basico){ //se os tipos forem diferentes
					if (PRINT_ERROS) printf("Conflicting types (got %s, expected %s)\n", t_basico_str[new_head->typespec->type], t_basico_str[head->tipo_basico]);
					result->flag_def = DEC;
				}

				if( new_head->id && (strcmp( head->id, "") == 0) && (strcmp( new_head->id->value, head->id) != 0)) {
					strcpy(head->id, new_head->id->value);
				}

				else { 
					if( new_head->id && new_head->id->value && (strcmp( head->id, "") != 0) && (strcmp( new_head->id->value, head->id) != 0))
						if (PRINT_ERROS) printf("Erro\n"); // parametros com Ids diferentes (entre definicoes e declaracoes)
				}

				if(new_head->id)
					new_head->id->anotation = (t_basico) new_head->typespec->type;

				new_head = new_head->next;
				head = head->next;

			}
			if (DEBUG) printf("DEBUG: check_functionDefinition 2\n");
			while(new_head) {
				if (PRINT_ERROS) printf("Redeclaração tem mais parametros\n");
				break;
			}

			while(head){
				if (PRINT_ERROS) printf("Declaração tem mais parametros\n");
				break;
			}


		} else{
			if (PRINT_ERROS) printf("Symbol %s already defined\n", result->id);
			// tem de se alterar a tabela para esta definição?
		}

		if (DEBUG) printf("DEBUG: check_functionDefinition 3\n");

		return result->tipo_basico;

	} else{
		_parameterDeclaration* new_head2 = fDefinition->parameter_list->first_parameter;
		int count = 0;
		int firstvoid = 0;

		while(new_head2){
			if(count == 0 && new_head2->typespec->type == (node) tipo_void) {
				firstvoid = 1;
				if (new_head2->id) {
					return nulo;
				}
			}
				
			if(count > 0 && new_head2->typespec->type == (node) tipo_void)
				return nulo;
			if(count > 0 && firstvoid)
				return nulo;

			new_head2 = new_head2->next;
			count++;
		}

		inserir_funcao(DEF, (_functionDeclaration*) fDefinition);
		if (DEBUG) printf("DEBUG: check_functionDefinition 4\n");

		return (t_basico) fDefinition->typespec->type;
	}

	return -1;
}

t_basico check_functionDeclaration(_functionDeclaration* fDeclaration){
	// verificar se é redeclaração, se está de acordo com a declaração anterior, etc

	elemento_tabela* result = pesquisa_elem_by_id(fDeclaration->id->value);

	
	if(result){ // se existe

		if((t_basico) fDeclaration->typespec->type != result->tipo_basico) // se os tipos de retorno não forem iguais
			if (PRINT_ERROS) printf("Conflicting types (got %s, expected %s)\n", t_basico_str[fDeclaration->typespec->type], t_basico_str[result->tipo_basico]);

		_parameterDeclaration* new_head = fDeclaration->parameter_list->first_parameter; // lista de parametros da redeclaração/definição
		elemento_funcao* head = result->lista_param_func; // lista de parâmetros já inseridos
		while(new_head && head){ // enquanto ambos tiverem parâmetros
			if((t_basico) new_head->typespec->type != head->tipo_basico) //se os tipos forem diferentes
				if (PRINT_ERROS) printf("Conflicting types (got %s, expected %s)\n", t_basico_str[new_head->typespec->type], t_basico_str[head->tipo_basico]);
		
			if( new_head->id->value && (strcmp( new_head->id->value, head->id) != 0))
				if (PRINT_ERROS) printf("Erro\n"); // parametros com Ids diferentes (entre declaracoes)

			new_head = new_head->next;
			head = head->next;
		}

		while(new_head) {
			if (PRINT_ERROS) printf("Redeclaração tem mais parametros\n");
			break;
		}

		while(head){
			if (PRINT_ERROS) printf("Declaração tem mais parametros\n");
			break;
		}

		return result->tipo_basico;

	} else { // se nao existe
		_parameterDeclaration* new_head2 = fDeclaration->parameter_list->first_parameter;
		int count = 0;
		int firstvoid = 0;

		while(new_head2){
			if(count == 0 && new_head2->typespec->type == (node) tipo_void) {
				firstvoid = 1;
				if (new_head2->id) {
					return nulo;
				}
			}
			
			if(count > 0 && new_head2->typespec->type == (node) tipo_void)
				return nulo;
			if(count > 0 && firstvoid)
				return nulo;

			new_head2 = new_head2->next;
			count++;
		}

		inserir_funcao(DEC, fDeclaration);

		return (t_basico) fDeclaration->typespec->type;
	}

	return -1;
}



t_basico check_program(_program* astree, char* ambiente){
	//elemento_funcao * aux_func;
	//elemento_tabela * aux_tab;
	t_basico type_left;
	t_basico type_right;

	if (DEBUG) printf("DEBUG: %s\n", (ambiente == NULL ? "NULL" : ambiente) );

	if(!astree) {
		if (DEBUG) printf("DEBUG: 0\n");
		return nulo;
	}
		


	switch(astree->type){
		case is_program:
			// INICIA

			if (DEBUG) printf("DEBUG: 1\n");

			iniciar_tabela();
			check_program(astree->next, ambiente);
			return nulo;
			break;
			
		case is_chrLit:
		case is_intLit:;
			// ESTATICO - Int

			if (DEBUG) printf("DEBUG: 2\n");
			_terminal* terminal = (_terminal*) astree;
			terminal->anotation = tipo_int;
			check_program((_program*)terminal->next, ambiente);
			return terminal->anotation;
			break;
				
		case is_realLit:;
			// ESTATICO - Double

			if (DEBUG) printf("DEBUG: 3\n");

			_terminal* terminal_real = (_terminal*) astree;
			terminal_real->anotation = tipo_double;
			check_program((_program*)terminal_real->next, ambiente);
			return terminal_real->anotation;
			break;
				
		case is_not:;
			// ESTATICO - Int

			if (DEBUG) printf("DEBUG: 4\n");

			_unaryOp* not = (_unaryOp*) astree;
			not->anotation = tipo_int;

			check_program((_program*)not->expression, ambiente);
			check_program((_program*)not->next, ambiente);
			return tipo_int;
			break;
				
		case is_minus:
		case is_plus:;
			// DINAMICO - depende dos filhos

			if (DEBUG) printf("DEBUG: 5\n");

			_unaryOp* un = (_unaryOp*) astree;
			un->anotation = check_program((_program*)un->expression, ambiente);
			check_program((_program*)un->next, ambiente);
			return un->anotation;
			break;
				
		case is_call:;
			// DINAMICO - depende dos filhos

			if (DEBUG) printf("DEBUG: 6\n");

			_call* call = (_call*) astree;
			call->anotation = call->id->anotation = check_functionCall(call, ambiente);
			check_program((_program*)call->next, ambiente);
			return call->anotation;
			break;

		// não têm irmãos nem filhos nem têm anotação, simplesmente retornam um valor que não pode corresponder a nenhum tipo
		case Char:
		case Int:
		case Short:
		case Double:
		case Void:
		case is_null:

			if (DEBUG) printf("DEBUG: 7\n");

			return nulo;
			break;

		case Id:
			// ?

			if (DEBUG) printf("DEBUG: 8\n");

			_terminal* terminal_id = (_terminal*) astree;
			t_basico tipo_var_pesquisa = pesquisa_var_by_func(terminal_id->value, ambiente);

			terminal_id->anotation = tipo_var_pesquisa;
			
			check_program((_program*)astree->next, ambiente);
			return terminal_id->anotation ;
			break;

		case is_or:
		case is_and:
		case is_eq:
		case is_ne:
		case is_lt:
		case is_gt:
		case is_le:
		case is_ge:
		case is_mod:
		case is_bitwiseand:
		case is_bitwisexor:
		case is_bitwiseor:;
			// ESTATICO - Int

			if (DEBUG) printf("DEBUG: 9\n");

			_binaryOp* bin = (_binaryOp*) astree;
			bin->anotation = tipo_int;
			check_program((_program*)bin->left_expression, ambiente);
			check_program((_program*)bin->right_expression, ambiente);
			check_program((_program*)bin->next, ambiente);
			return bin->anotation;
			break;

		case is_add:;
			// DINAMICO - depende dos filhos

			if (DEBUG) printf("DEBUG: 10\n");

			_binaryOp* add = (_binaryOp*) astree;
			type_left = check_program((_program*)add->left_expression, ambiente);
			type_right = check_program((_program*)add->right_expression, ambiente);
			if(type_left == tipo_double || type_right == tipo_double)
				add->anotation = tipo_double;
			else if(type_left == tipo_int || type_right == tipo_int)
				add->anotation = tipo_int;
			else if(type_left == tipo_short || type_right == tipo_short)
				add->anotation = tipo_short;
			else if(type_left == tipo_char && type_right == tipo_char)
				add->anotation = tipo_char;
			else if(type_left == tipo_void || type_left == tipo_undef || type_right == tipo_void || type_right == tipo_undef){
				if (PRINT_ERROS) printf("Operator + cannot be applied to types %s, %s\n", t_basico_str[type_left], t_basico_str[type_right]);
				add->anotation = tipo_undef;
			}
			check_program((_program*)add->next, ambiente);
			return add->anotation;
			break;

		case is_sub:;
			// DINAMICO - depende dos filhos

			if (DEBUG) printf("DEBUG: 11\n");

			_binaryOp* sub = (_binaryOp*) astree;
			type_left = check_program((_program*)sub->left_expression, ambiente);
			type_right = check_program((_program*)sub->right_expression, ambiente);
			if(type_left == tipo_double || type_right == tipo_double)
				sub->anotation = tipo_double;
			else if(type_left == tipo_int || type_right == tipo_int)
				sub->anotation = tipo_int;
			else if(type_left == tipo_short || type_right == tipo_short)
				sub->anotation = tipo_short;
			else if(type_left == tipo_char && type_right == tipo_char)
				sub->anotation = tipo_char;
			else if(type_left == tipo_void || type_left == tipo_undef || type_right == tipo_void || type_right == tipo_undef){
				if (PRINT_ERROS) printf("Operator - cannot be applied to types %s, %s\n", t_basico_str[type_left], t_basico_str[type_right]);
				sub->anotation = tipo_undef;
			}
			check_program((_program*)sub->next, ambiente);
			return sub->anotation;
			break;

		case is_div:;
			// DINAMICO - depende dos filhos

			if (DEBUG) printf("DEBUG: 12\n");

			_binaryOp* div = (_binaryOp*) astree;
			type_left = check_program((_program*)div->left_expression, ambiente);
			type_right = check_program((_program*)div->right_expression, ambiente);
			if(type_left == tipo_double || type_right == tipo_double)
				div->anotation = tipo_double;
			else if(type_left == tipo_int || type_right == tipo_int)
				div->anotation = tipo_int;
			else if(type_left == tipo_short || type_right == tipo_short)
				div->anotation = tipo_short;
			else if(type_left == tipo_char && type_right == tipo_char)
				div->anotation = tipo_char;
			else if(type_left == tipo_void || type_left == tipo_undef || type_right == tipo_void || type_right == tipo_undef){
				if (PRINT_ERROS) printf("Operator / cannot be applied to types %s, %s\n", t_basico_str[type_left], t_basico_str[type_right]);
				div->anotation = tipo_undef;
			}
			check_program((_program*)div->next, ambiente);
			return div->anotation;
			break;
			
		case is_mul:;
			// DINAMICO - depende dos filhos

			if (DEBUG) printf("DEBUG: 13\n");

			_binaryOp* mul = (_binaryOp*) astree;
			type_left = check_program((_program*)mul->left_expression, ambiente);
			type_right = check_program((_program*)mul->right_expression, ambiente);
			if(type_left == tipo_double || type_right == tipo_double)
				mul->anotation = tipo_double;
			else if(type_left == tipo_int || type_right == tipo_int)
				mul->anotation = tipo_int;
			else if(type_left == tipo_short || type_right == tipo_short)
				mul->anotation = tipo_short;
			else if(type_left == tipo_char && type_right == tipo_char)
				mul->anotation = tipo_char;
			else if(type_left == tipo_void || type_left == tipo_undef || type_right == tipo_void || type_right == tipo_undef){
				if (PRINT_ERROS) printf("Operator * cannot be applied to types %s, %s\n", t_basico_str[type_left], t_basico_str[type_right]);
				mul->anotation = tipo_undef;
			}
			check_program((_program*)mul->next, ambiente);
			return mul->anotation;
			break;


		case is_store:;
			// DINAMICO

			if (DEBUG) printf("DEBUG: 14\n");

			_binaryOp* store = (_binaryOp*) astree;
			type_left = check_program((_program*)store->left_expression, ambiente);
			type_right = check_program((_program*)store->right_expression, ambiente);
			

			store->anotation = type_left;
			/* esta a dar erros em cenas certas
			if(type_left == tipo_undef || type_left == tipo_void || type_right == tipo_undef || type_right == tipo_void
				|| (type_left == tipo_char && type_right != tipo_char) || (type_left == tipo_int && type_right == tipo_double)
				|| (type_left == tipo_short && type_right != tipo_char && type_right != tipo_short))
				printf("Operator = cannot be applied to types %s, %s\n", t_basico_str[type_left], t_basico_str[type_right]);

			*/
			check_program((_program*)store->next, ambiente);
			return store->anotation;
			break;

		case is_comma:;

			if (DEBUG) printf("DEBUG: 15\n");

			_binaryOp* comma = (_binaryOp*) astree;

			type_left = check_program((_program*)comma->left_expression, ambiente);
			type_right = check_program((_program*)comma->right_expression, ambiente);

			if(type_left == tipo_double || type_right == tipo_double)
				comma->anotation = tipo_double;
			else if(type_left == tipo_int || type_right == tipo_int)
				comma->anotation = tipo_int;
			else if(type_left == tipo_short || type_right == tipo_short)
				comma->anotation = tipo_short;
			else if(type_left == tipo_char && type_right == tipo_char)
				comma->anotation = tipo_char;
			else if(type_left == tipo_void || type_left == tipo_undef || type_right == tipo_void || type_right == tipo_undef){
				if (PRINT_ERROS) printf("Operator , cannot be applied to types %s, %s\n", t_basico_str[type_left], t_basico_str[type_right]);
				comma->anotation = tipo_undef;
			}

			check_program((_program*)comma->next, ambiente);
			
			return comma->anotation;
			break;

		case is_return:;

			if (DEBUG) printf("DEBUG: 16\n");

			_return* ret = (_return*) astree;
			check_program((_program*)ret->expression, ambiente);
			check_program((_program*)ret->next, ambiente);

			return nulo;
			break;

		case is_while:;

			if (DEBUG) printf("DEBUG: 17\n");

			_while* wh = (_while*) astree;
			check_program((_program*)wh->condition, ambiente);
			check_program((_program*)wh->branch, ambiente);
			check_program((_program*)wh->next, ambiente);
			
			return nulo;
			break;

		case is_if:;

			if (DEBUG) printf("DEBUG: 18\n");

			_if* iif = (_if*) astree;
			check_program((_program*)iif->condition, ambiente);
			check_program((_program*)iif->if_branch, ambiente);
			check_program((_program*)iif->else_branch, ambiente);
			check_program((_program*)iif->next, ambiente);
			return nulo;
			break;

		case is_functionDeclaration:;
			// DINAMICO
			
			if (DEBUG) printf("DEBUG: 19\n");
			
			_functionDeclaration* fdec = (_functionDeclaration*) astree;
			fdec->anotation = check_functionDeclaration(fdec);
			
			check_program((_program*)fdec->next, ambiente);

			return fdec->anotation;
			break;

		case is_functionDefinition:; 
			// DINAMICO

			if (DEBUG) printf("DEBUG: 20\n");

			_functionDefinition* fdef = (_functionDefinition*) astree;
			fdef->anotation = check_functionDefinition(fdef);

			check_program((_program*)fdef->function_body, fdef->id->value);
			check_program((_program*)fdef->next, ambiente);

			return fdef->anotation;
			break;

		case is_functionBody:;
			// NAO FAZ NADA

			if (DEBUG) printf("DEBUG: 22\n");

			_functionBody* fbody = (_functionBody*) astree;
			check_program((_program*) fbody->declaration_or_statement, ambiente);
			return nulo;
			break;

		case is_statementList:;
			// ?

			if (DEBUG) printf("DEBUG: 23\n");

			_statementList* slist = (_statementList*) astree;
			check_program((_program*)slist->first_statement, ambiente);
			check_program((_program*)slist->next, ambiente);
			return nulo;
			break;

		case is_declaration:;
			// DINAMICO

			if (DEBUG) printf("DEBUG: 24\n");

			_declaration* dec = (_declaration*) astree;
			if (!((t_basico) dec->typespec->type == tipo_void)) {
				if(ambiente == NULL){ // ambiente global
					inserir_elem(DEC, dec->id->value, tipo_var, (t_basico) dec->typespec->type);
				}
				else{ // função <ambiente>
					inserir_var_func(ambiente, dec->id->value, (t_basico) dec->typespec->type);
				}

				dec->id->anotation = dec->anotation = (t_basico) dec->typespec->type;
			}


			check_program((_program*)(_program*)dec->expression, ambiente);
			check_program((_program*)dec->next, ambiente);
			
			return dec->id->anotation;
			break;

		default:
			if (DEBUG) printf("DEBUG: Default 25\n");
			// check_program(astree->next, ambiente);
			break;
	}
	
	return nulo;
}

/*
void anotate(_program* node, t_basico tipo){
	switch(node->type){
		case is_chrLit:
		case is_intlit:
		case is_realLit:
		case Id:
			_terminal* term = (_terminal*) node;
			term->anotation = tipo;
			break;
		case is_call:
			_call* c = (_call*) node;
			c->anotation = tipo;
			break;
		case is_not:
		case is_plus:
		case is_minus:
			_unaryOp* un = (_unaryOp*) node;
			un->anotation = tipo;
		case is_or:
		case is_and:
		case is_eq:
		case is_ne:
		case is_lt:
		case is_gt:
		case is_le:
		case is_ge:
		case is_add:
		case is_sub:
		case is_mul:
		case is_div:
		case is_mod:
		case is_store:
		case is_bitwiseand:
		case is_bitwisexor:
		case is_bitwiseor:
	}
}

void check_program(_program* astree){

}

void check_unaryOp(_unaryOp* op){
	op->anotation = op->
}

void check_terminal(_terminal* term){
	switch(term->type){
		case is_intLit:
		case is_chrLit:
			term->anotation = tipo_int;
			break;
		case is_realLit:
			term->anotation = tipo_double;
			break;
	}
}
*/
