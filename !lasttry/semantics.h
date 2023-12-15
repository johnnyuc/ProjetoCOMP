#ifndef _SEMANTICS_H
#define _SEMANTICS_H

#include "ast.h"

int check_program(struct node *program);

//tabela
struct table {
	char *identifier;
	enum type type;
	struct node *node;
	struct parameter_list* parameter;
	struct table *next;
	int error_flag;
};

//lista de parametros
struct parameter_list {
    enum type parameter;
	char *Identifier;
    struct parameter_list* next;
};

//lista de tabelas
struct table_list {
	char *func_name;
	struct table *table;
	struct table_list *next;
};

//parameters
struct parameter_list *register_parameter(enum type parameter);
struct parameter_list *add_parameter(struct parameter_list *parameter_list, enum type parameter,struct node *Identifier);
enum type check_parameter(struct node *param_declaration);

//table
struct table *insert_symbol(struct table *table, char *identifier,enum type type, struct parameter_list *v_type, struct node *node,int error_flag);
struct table *search_symbol(struct table *table, struct node *node);

//tables
struct table_list *insert_table(struct table_list *table_list, struct table *new_table, char *token);
void show_tables();

//checks
void check_node(struct node *node, struct table *current_table);
struct table *get_function_table(struct table_list *table_list, char *function_name);
void check_FuncDefinition(struct node *node,struct table *table);
void check_Declaration(struct node *node, struct table *table,int error_flag);
void check_FuncDeclaration(struct node *node,struct table *table);
void check_Statement(struct node *node, struct table *table);
void check_Expression(struct node *node, struct table *table);

//tree
void show_annotated(struct node *node, int depth);

//Raul
struct table *search_symbol2(struct table *table, char *identifier);
struct table_list *search_table(struct table_list *table, char *token);
struct parameter_list *add_parameter2(struct parameter_list *list, enum type parameter, char *name);
char *get_identifier(struct node *paramdeclaration);
int count_parameters(struct parameter_list *list);
void insert_params_to_symbol_table(struct table *symbol_tableFunc, struct parameter_list *params);

#endif
