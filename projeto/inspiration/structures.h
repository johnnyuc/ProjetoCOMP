#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef enum {
	null_node, binary_op, unary_op, type_node,
	if_node, while_node, return_node, call_node, call_parameter,
	stat_list, declarator_list, parameter_list,
	declaration, parameter_declaration,
	declarations_statements, // declarations_and_statements
	function_body, function_definition, function_declaration,
	charlit_node, intlit_node, reallit_node, id_node,
	declarator, function_declarator,
	functions_declarations, program,
} ast_node_type;

typedef enum {
	op_or, op_and, op_bitwiseand, op_bitwiseor, op_bitwisexor, // operadores lógicos
	op_eq, op_ne, // operadores de igualdade
	op_lt, op_le, op_ge, op_gt, // operadores relacionais
	op_add, op_sub, op_mul, op_div, op_mod, // operadores aritméticos
	op_store, op_comma
} binary_operator;

typedef enum {
	 op_minus, op_plus,op_not
} unary_operator;

typedef enum {
	sym_char, sym_int, sym_short, sym_double, sym_void,	// tipos de dados
} type;

typedef enum {op_call} miscellaneous;

typedef struct _s0 {
	ast_node_type node_type;
} ast_node;

typedef struct _s1 {
	ast_node_type node_type;
	binary_operator operator;
	ast_node* left;
	ast_node* right;
} ast_binary;

typedef struct _s2 {
	ast_node_type node_type;
	unary_operator operator;
	ast_node* right;
} ast_unary;

typedef struct _s3 {
	ast_node_type node_type;
	type typespec;
} ast_type;

typedef struct _s4 {
	ast_node_type node_type;
	char* value;
} ast_chrlit;

typedef struct _s5 {
	ast_node_type node_type;
	char* value;
} ast_intlit;

typedef struct _s6 {
	ast_node_type node_type;
	char* value;
} ast_reallit;

typedef struct _s7 {
	ast_node_type node_type;
	ast_node* condition;
	ast_node* if_branch;
	ast_node* else_branch;
} ast_if;

typedef struct _s8 {
	ast_node_type node_type;
	ast_node* condition;
	ast_node* while_branch;
} ast_while;

typedef struct _s9 {
	ast_node_type node_type;
	ast_node* function_name;
	ast_node* parameters;
} ast_call;

typedef struct _s10 {
	ast_node_type node_type;
	ast_node* return_value;
} ast_return;

typedef struct _s11 {
	ast_node_type node_type;
	ast_node* statement;
	ast_node* next;
} ast_statlist;
/*
typedef struct _s12 {
	ast_node_type node_type;
	ast_node* id;
	ast_node* expression;
	ast_node* next;
} ast_declarator_list;
*/
typedef struct _s13 {
	ast_node_type node_type;
	ast_node* typespec;
	//ast_node* declarator_list;
	ast_node* id;
	ast_node* expression;
	ast_node* next;
} ast_declaration;

typedef struct _s14 {
	ast_node_type node_type;
	ast_node* typespec;
	ast_node* id;
} ast_parameter_declaration;

typedef struct _s15 {
	ast_node_type node_type;
	ast_node* parameter_declaration;
	ast_node* next;
} ast_parameter_list;

typedef struct _s16 {
	ast_node_type node_type;
	ast_node* declaration_or_statement;
	ast_node* next;
} ast_declarations_and_statements;

typedef struct _s17 {
	ast_node_type node_type;
	ast_node* declarations_and_statements;
} ast_function_body;

typedef struct _s18 {
	ast_node_type node_type;
	ast_node* typespec;
	ast_node* id;
	ast_node* parameter_list;
	ast_node* function_body;
} ast_function_definition;

typedef struct _s19 {
	ast_node_type node_type;
	ast_node* typespec;
	ast_node* id;
	ast_node* parameter_list;
} ast_function_declaration;

typedef struct _s20 {
	ast_node_type node_type;
	ast_node* function_or_declaration;
	ast_node* next;
} ast_functions_and_declarations;

typedef struct _s21 {
	ast_node_type node_type;
	ast_node* functions_and_declarations;
} ast_program;

typedef struct _s22 {
	ast_node_type node_type;
	ast_node* id;
	ast_node* expression;
} ast_declarator;

typedef struct _s23 {
	ast_node_type node_type;
	ast_node* id;
	ast_node* parameter_list;
} ast_function_declarator;

typedef struct _s24{
	ast_node_type node_type;
	ast_node* expression;
	ast_node* next;
} ast_call_parameter;

#endif