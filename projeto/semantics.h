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
};

//lista de parametros
struct parameter_list {
    enum type parameter;
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
struct parameter_list *add_parameter(struct parameter_list *add_parameter_list,enum type parameter);
enum type check_parameter(struct node *param_declaration);

//table
struct table *insert_symbol(struct table *table, char *identifier,enum type type, struct parameter_list *v_type, struct node *node);
struct table *search_symbol(struct table *symbol_table, char *identifier);

//tables
struct table_list *insert_table(struct table_list *table_list, struct table *new_table, char *token);
void show_tables();

//checks
void check_node(struct node *node);
void check_FuncDefinition(struct node *node);
void check_Declaration(struct node *node, struct table *table);
void check_funcbody(struct node *node, struct table *func_list);

#endif
