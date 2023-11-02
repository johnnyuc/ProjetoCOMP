#include "structures.h"
#include <stdlib.h>
#include <stdio.h>

#define PRINTDOTS for(unsigned int i = 0; i < dots; i++){printf(".");} 

void* emalloc(size_t size){
	void* pointer = malloc(size);

	if(pointer == NULL)
		printf("Error: malloc(%zu) failed!\n", size);

	return pointer;
}

ast_node* new_ast_null(void){
	ast_node* node = emalloc(sizeof(ast_node));

	node->node_type = null_node;

	return node;
}
ast_node* new_ast_binary(binary_operator operator, ast_node* left, ast_node* right){
	ast_binary* node = emalloc(sizeof(ast_binary));

	node->node_type = binary_op;
	node->operator = operator;
	node->left = left;
	node->right = right;

	return (ast_node*) node;
}

ast_node* new_ast_unary(unary_operator operator, ast_node* right){
	ast_unary* node = emalloc(sizeof(ast_unary));

	node->node_type = unary_op;
	node->operator = operator;
	node->right = right;

	return (ast_node*) node;
}

ast_node* new_ast_type(type typespec){
	ast_type* node = emalloc(sizeof(ast_type));

	node->node_type = type_node;
	node->typespec = typespec;

	return (ast_node*) node;
}

ast_node* new_ast_id(char* value){
	ast_chrlit* node = emalloc(sizeof(ast_chrlit));

	node->node_type = id_node;
	node->value = value;

	return (ast_node*) node;
}

ast_node* new_ast_chrlit(char* value){
	ast_chrlit* node = emalloc(sizeof(ast_chrlit));

	node->node_type = charlit_node;
	node->value = value;

	return (ast_node*) node;
}

ast_node* new_ast_intlit(char* value){
	ast_intlit* node = emalloc(sizeof(ast_intlit));

	node->node_type = intlit_node;
	node->value = value;

	return (ast_node*) node;
}

ast_node* new_ast_reallit(char* value){
	ast_reallit* node = emalloc(sizeof(ast_reallit));

	node->node_type = reallit_node;
	node->value = value;

	return (ast_node*) node;
}

ast_node* new_ast_if(ast_node* condition, ast_node* if_branch, ast_node* else_branch){
	ast_if* node = emalloc(sizeof(ast_if));

	node->node_type = if_node;
	node->condition = condition;
	node->if_branch = if_branch;
	node->else_branch = else_branch;
	return (ast_node*) node;
}

ast_node* new_ast_while(ast_node* condition, ast_node* while_branch){
	ast_while* node = emalloc(sizeof(ast_while));

	node->node_type = while_node;
	node->condition = condition;
	node->while_branch = while_branch;

	return (ast_node*) node;
}

ast_node* new_ast_call(ast_node* function_name, ast_node* expression, ast_node* head){
	ast_call* node = emalloc(sizeof(ast_call));
	ast_call_parameter* cp = emalloc(sizeof(ast_call_parameter));

	node->node_type = call_node;
	node->function_name = function_name;
	cp->node_type = call_parameter;
	cp->expression = expression;
	cp->next = head;
	node->parameters = (ast_node*) cp;

	return (ast_node*) node;
}


ast_node* new_ast_call_parameter(ast_node* head, ast_node* expression){
	ast_call_parameter* node = emalloc(sizeof(ast_call_parameter));
	ast_call_parameter* h = (ast_call_parameter*) head;

	node->node_type = call_parameter;
	node->expression = expression;
	node->next = NULL;
	for(; h->next; h=(ast_call_parameter*)h->next)
		;
	h->next = (ast_node*) node;

	return (ast_node*) head;
}

ast_node* new_ast_return(ast_node* return_value){
	ast_return* node = emalloc(sizeof(ast_return));

	node->node_type = return_node;
	node->return_value = return_value;

	return (ast_node*) node;
}

ast_node* new_ast_statlist(ast_node* statement, ast_node* head){
	if(statement){
		ast_statlist* node = emalloc(sizeof(ast_statlist));

		node->node_type = stat_list;
		node->statement = statement;
		node->next = head;

		return (ast_node*) node;
	}
	else
		return head;
}

ast_node* new_ast_declarator(ast_node* id, ast_node* expression){
	ast_declaration* node = emalloc(sizeof(ast_declaration));

	node->node_type = declaration;
	node->id = id;
	node->expression = expression;

	return (ast_node*) node;
}

ast_node* new_ast_declarator_list(ast_node* declarator, ast_node* head){
	ast_declaration* new = (ast_declaration*) declarator;

	new->next = head;

	return (ast_node*) declarator;
}

ast_node* new_ast_declaration(ast_node* typespec, ast_node* declarator_list){
	ast_declaration* node = (ast_declaration*) declarator_list;

	while(node){
		node->typespec = typespec;
		node = (ast_declaration*) node->next;
	}
	return (ast_node*) declarator_list;
}

ast_node* new_ast_parameter_declaration(ast_node* typespec, ast_node* id){
	ast_parameter_declaration* node = emalloc(sizeof(ast_parameter_declaration));

	node->node_type = parameter_declaration;
	node->typespec = typespec;
	node->id = id;

	return (ast_node*) node;
}

ast_node* new_ast_parameter_list(ast_node* parameter_declaration, ast_node* head){
	ast_parameter_list* node = emalloc(sizeof(ast_parameter_list));

	node->node_type = parameter_list;
	node->parameter_declaration = parameter_declaration;
	node->next = head;

	return (ast_node*) node;
}

ast_node* new_ast_declarations_and_statements(ast_node* declaration_or_statement, ast_node* head){
	ast_declarations_and_statements* node = emalloc(sizeof(ast_declarations_and_statements));

	node->node_type = declarations_statements;
	node->declaration_or_statement = declaration_or_statement;
	node->next = head;

	return (ast_node*) node;
}

ast_node* new_ast_function_body(ast_node* declarations_and_statements){
	ast_function_body* node = emalloc(sizeof(ast_function_body));

	node->node_type = function_body;
	node->declarations_and_statements = declarations_and_statements;

	return (ast_node*) node;
}

ast_node* new_ast_function_declarator(ast_node* id, ast_node* parameter_list){
	ast_function_declarator* node = emalloc(sizeof(ast_function_declarator));

	node->node_type = function_declarator;
	node->id = id;
	node->parameter_list = parameter_list;

	return (ast_node*) node;
}

ast_node* new_ast_function_definition(ast_node* typespec, ast_node* function_declarator, ast_node* function_body){
	ast_function_definition* node = emalloc(sizeof(ast_function_definition));

	node->node_type = function_definition;
	node->typespec = typespec;
	node->id = ((ast_function_declarator*) function_declarator)->id;
	node->parameter_list = ((ast_function_declarator*) function_declarator)->parameter_list;
	node->function_body = function_body;

	free(function_declarator);

	return (ast_node*) node;
}

ast_node* new_ast_function_declaration(ast_node* typespec, ast_node* function_declarator){
	ast_function_declaration* node = emalloc(sizeof(ast_function_declaration));

	node->node_type = function_declaration;
	node->typespec = typespec;
	node->id = ((ast_function_declarator*) function_declarator)->id;
	node->parameter_list = ((ast_function_declarator*) function_declarator)->parameter_list;

	free(function_declarator);

	return (ast_node*) node;
}

ast_node* new_ast_functions_and_declarations(ast_node* function_or_declaration, ast_node* head){
	ast_functions_and_declarations* node = emalloc(sizeof(ast_functions_and_declarations));

	node->node_type = functions_declarations;
	node->function_or_declaration = function_or_declaration;
	node->next = head;

	return (ast_node*) node;
}

ast_node* new_ast_program(ast_node* functions_and_declarations){
	ast_program* node = emalloc(sizeof(ast_program));

	node->node_type = program;
	node->functions_and_declarations = functions_and_declarations;

	return (ast_node*) node;
}


void print_ast(ast_node* as_tree, unsigned int dots){
	if(!as_tree)
		return;

	switch(as_tree->node_type){
		case program:
			printf("Program\n");
			ast_program* p = (ast_program*) as_tree;
			print_ast(p->functions_and_declarations, dots+2);
			break;
		case functions_declarations:
			;
			ast_functions_and_declarations* func_and_dec = ((ast_functions_and_declarations*) as_tree);
			for(; func_and_dec; func_and_dec = (ast_functions_and_declarations*) func_and_dec->next){
				print_ast(func_and_dec->function_or_declaration, dots);
			}
			break;
		case function_definition:
			PRINTDOTS;
			printf("FuncDefinition\n");
			ast_function_definition* function_def = (ast_function_definition*) as_tree;
			print_ast(function_def->typespec, dots + 2);
			print_ast(function_def->id, dots + 2);
			print_ast(function_def->parameter_list, dots + 2);
			print_ast(function_def->function_body, dots + 2);
			break;
		case function_body:
			PRINTDOTS;
			ast_function_body* fb = (ast_function_body*) as_tree;
			printf("FuncBody\n");
			print_ast(fb->declarations_and_statements, dots + 2);
			break;
		case declarations_statements:
			;
			ast_declarations_and_statements* ds = (ast_declarations_and_statements*) as_tree;
			for(; ds; ds = (ast_declarations_and_statements*) ds->next){
				print_ast(ds->declaration_or_statement, dots);
			}
			break;
		case return_node:
			PRINTDOTS;
			ast_return* ret = (ast_return*) as_tree;
			printf("Return\n");
			if(ret->return_value)
				print_ast(ret->return_value, dots + 2);
			else{
				PRINTDOTS;
				printf("..Null\n");
			}
			break;
		case function_declaration:
			PRINTDOTS;
			printf("FuncDeclaration\n");
			ast_function_declaration* function_dec = (ast_function_declaration*) as_tree;
			print_ast(function_dec->typespec, dots + 2);
			print_ast(function_dec->id, dots + 2);
			print_ast(function_dec->parameter_list, dots + 2);
			break;
		case declaration:
			;
			ast_declaration* d = (ast_declaration*) as_tree;
			for(; d; d = (ast_declaration*) d->next){
				PRINTDOTS;
				printf("Declaration\n");
				print_ast(d->typespec, dots + 2);
				print_ast(d->id, dots+2);
				print_ast(d->expression, dots + 2);
			}
			break;
		case if_node:
			PRINTDOTS;
			ast_if* iif = (ast_if*) as_tree;
			printf("If\n");
			print_ast(iif->condition, dots + 2);
			if(iif->if_branch)
				print_ast(iif->if_branch, dots + 2);
			else{
				PRINTDOTS;
				printf("..Null\n");
			}
			if(iif->else_branch){
				//printf("Dots: %d\n", dots);
				print_ast(iif->else_branch, dots + 2);
			}
			else{
				PRINTDOTS;
				printf("..Null\n");
			}
			break;
		case while_node:
			PRINTDOTS;
			ast_while* wh = (ast_while*) as_tree;
			printf("While\n");
			print_ast(wh->condition, dots + 2);
			if(wh->while_branch)
				print_ast(wh->while_branch, dots + 2);
			else{
				PRINTDOTS;
				printf("..Null\n");
			}
			break;
		case stat_list:
			;
			ast_statlist* sl = (ast_statlist*) as_tree;
			if(sl->next){
				PRINTDOTS;
				printf("StatList\n");
				for(; sl; sl = (ast_statlist*) sl->next){
					print_ast(sl->statement, dots + 2);
				}
			}
			else
				print_ast(sl->statement, dots);
			break;
		case call_node:
			PRINTDOTS;
			ast_call* call = (ast_call*) as_tree;
			printf("Call\n");
			print_ast(call->function_name, dots + 2);
			print_ast(call->parameters, dots + 2);
			break;
		case call_parameter:
			;
			ast_call_parameter* cp = (ast_call_parameter*) as_tree;
			for(; cp; cp = (ast_call_parameter*) cp->next){
				print_ast(cp->expression, dots);
			}
			break;
		case binary_op:
			PRINTDOTS;
			ast_binary* bin = (ast_binary*) as_tree;
			switch (bin->operator){
				case op_or: printf("Or\n"); break;
				case op_and: printf("And\n"); break;
				case op_bitwiseand: printf("BitWiseAnd\n"); break;
				case op_bitwiseor: printf("BitWiseOr\n"); break;
				case op_bitwisexor: printf("BitWiseXor\n"); break;
				case op_eq: printf("Eq\n"); break;
				case op_ne: printf("Ne\n"); break;
				case op_lt: printf("Lt\n"); break;
				case op_le: printf("Le\n"); break;
				case op_ge: printf("Ge\n"); break;
				case op_gt: printf("Gt\n"); break;
				case op_add: printf("Add\n"); break;
				case op_sub: printf("Sub\n"); break;
				case op_mul: printf("Mul\n"); break;
				case op_div: printf("Div\n"); break;
				case op_mod: printf("Mod\n"); break;
				case op_store: printf("Store\n"); break;
				case op_comma: printf("Comma\n"); break;
				default: printf("O operador não é válido!\n"); break;
			}
			print_ast(bin->left, dots + 2);
			print_ast(bin->right, dots + 2);
			break;
		case unary_op:
			PRINTDOTS;
			ast_unary* un = (ast_unary*) as_tree;
			switch (un->operator){
				case op_minus: printf("Minus\n"); break;
				case op_plus: printf("Plus\n"); break;
				case op_not: printf("Not\n"); break;
				default: printf("O operador unário não é válido!\n"); break;
			}
			print_ast(un->right, dots + 2);
			break;
		case type_node:
			PRINTDOTS;
			ast_type* t = (ast_type*) as_tree;
			switch(t->typespec){
				case sym_char:
					printf("Char\n");
					break;
				case sym_short:
					printf("Short\n");
					break;
				case sym_int:
					printf("Int\n");
					break;
				case sym_double:
					printf("Double\n");
					break;
				case sym_void:
					printf("Void\n");
					break;
				default:
					printf("Tipo inválido!\n");
					break;
			}
			break;
		case id_node:
			PRINTDOTS;
			ast_chrlit* id = (ast_chrlit*) as_tree;
			printf("Id(%s)\n",id->value);
			break;
		case parameter_list:
			PRINTDOTS;
			printf("ParamList\n");
			ast_parameter_list* param_list = (ast_parameter_list*) as_tree;
			for(; param_list; param_list = (ast_parameter_list*) param_list->next){
				print_ast(param_list->parameter_declaration, dots + 2);
			}
			break;
		case parameter_declaration:
			PRINTDOTS;
			printf("ParamDeclaration\n");
			ast_parameter_declaration* param_declaration = (ast_parameter_declaration*) as_tree;
			print_ast(param_declaration->typespec, dots+2);
			print_ast(param_declaration->id, dots+2);
			break;
		case charlit_node:
			;
			ast_chrlit* chr = (ast_chrlit*) as_tree;
			PRINTDOTS;
			printf("ChrLit('%s)\n", chr->value);
			break;
		case intlit_node:
			PRINTDOTS;
			ast_intlit* intl = (ast_intlit*) as_tree;
			printf("IntLit(%s)\n", intl->value);
			break;
		case reallit_node:
			PRINTDOTS;
			ast_reallit* real = (ast_reallit*) as_tree;
			printf("RealLit(%s)\n", real->value);
			break;
		default:
			break;
	}
}

void clean_ast(ast_node* as_tree) {
	if(!as_tree)
		return;

	ast_node* temp;
	
	switch(as_tree->node_type){
		case program:
			;//printf("free program\n");
			ast_program* p = (ast_program*) as_tree;
			clean_ast(p->functions_and_declarations);
			free(as_tree);
			break;
		case functions_declarations:
			;//printf("free func and dec\n");
			ast_functions_and_declarations* func_and_dec = ((ast_functions_and_declarations*) as_tree);
			for(; func_and_dec; func_and_dec = (ast_functions_and_declarations*) func_and_dec->next, free(temp)){
				temp = (ast_node*) func_and_dec;
				clean_ast(func_and_dec->function_or_declaration);
			}
			break;
		case function_definition:
			;//printf("free function definition\n");
			ast_function_definition* function_def = (ast_function_definition*) as_tree;
			clean_ast(function_def->typespec);
			clean_ast(function_def->id);
			clean_ast(function_def->parameter_list);
			clean_ast(function_def->function_body);
			free(as_tree);
			break;
		case function_body:
			;//printf("free function body\n");
			ast_function_body* fb = (ast_function_body*) as_tree;
			clean_ast(fb->declarations_and_statements);
			free(as_tree);
			break;
		case declarations_statements:
			;//printf("free declarations and statements\n");
			ast_declarations_and_statements* ds = (ast_declarations_and_statements*) as_tree;
			for(; ds; ds = (ast_declarations_and_statements*) ds->next, free(temp) ){
				temp = (ast_node*) ds;
				clean_ast(ds->declaration_or_statement);
			}
			break;
		case return_node:
			;//printf("free return\n");
			ast_return* ret = (ast_return*) as_tree;
			clean_ast(ret->return_value);
			free(as_tree);
			break;
		case function_declaration:
			;//printf("free function declaration\n");
			ast_function_declaration* function_dec = (ast_function_declaration*) as_tree;
			clean_ast(function_dec->typespec);
			clean_ast(function_dec->id);
			clean_ast(function_dec->parameter_list);
			free(as_tree);
			break;
		case declaration:
			;//printf("free declaration\n");
			ast_declaration* d = (ast_declaration*) as_tree;
			for(; d; d = (ast_declaration*) d->next){
				clean_ast(d->typespec);
				clean_ast(d->id);
				clean_ast(d->expression);
			}
			free(as_tree);
			break;
		case if_node:
			;//printf("free if\n");
			ast_if* iif = (ast_if*) as_tree;
			clean_ast(iif->condition);
			clean_ast(iif->if_branch);
			clean_ast(iif->else_branch);
			free(as_tree);
			break;
		case while_node:
			;//printf("free while\n");
			ast_while* wh = (ast_while*) as_tree;
			clean_ast(wh->condition);
			clean_ast(wh->while_branch);
			free(as_tree);
			break;
		case stat_list:
			;//printf("free stat list\n");
			ast_statlist* sl = (ast_statlist*) as_tree;
			for(; sl; sl = (ast_statlist*) sl->next, free(temp)){
				temp = (ast_node*) sl;
				clean_ast(sl->statement);
			}
			break;
		case call_node:
			;//printf("free call\n");
			ast_call* call = (ast_call*) as_tree;
			clean_ast(call->function_name);
			clean_ast(call->parameters);
			free(as_tree);
			break;
		case call_parameter:
			;//printf(" free call param\n");
			ast_call_parameter* cp = (ast_call_parameter*) as_tree;
			for(; cp; cp = (ast_call_parameter*) cp->next, free(temp)){
				temp = (ast_node*) cp;
				clean_ast(cp->expression);
			}
			break;
		case binary_op:
			;//printf("free binary\n");
			ast_binary* bin = (ast_binary*) as_tree;
			clean_ast(bin->left);
			clean_ast(bin->right);
			free(as_tree);
			break;
		case unary_op:
			;//printf("free unary\n");
			ast_unary* un = (ast_unary*) as_tree;
			clean_ast(un->right);
			free(as_tree);
			break;
		case type_node:
			;//printf("free type\n");
			free(as_tree);
			break;
		case id_node:
			;//printf("free id\n");
			free(as_tree);
			break;
		case parameter_list:
			;//printf("free parameter list\n");
			ast_parameter_list* param_list = (ast_parameter_list*) as_tree;
			for(; param_list; param_list = (ast_parameter_list*) param_list->next, free(temp)){
				temp = (ast_node*) param_list;
				clean_ast(param_list->parameter_declaration);
			}
			break;
		case parameter_declaration:
			;//printf("free param declaration\n");
			ast_parameter_declaration* param_declaration = (ast_parameter_declaration*) as_tree;
			clean_ast(param_declaration->typespec);
			clean_ast(param_declaration->id);
			free(as_tree);
			break;
		case charlit_node:
			;//printf("free charlit\n");
			free(as_tree);
			break;
		case intlit_node:
			;//printf("free intlit\n");
			free(as_tree);
			break;
		case reallit_node:
			;//printf("free reallit\n");
			free(as_tree);
			break;
		default:
			;//printf("free default?\n");
			free(as_tree);	// ponteiro valido esquecido no case?
			break;
	}
}
