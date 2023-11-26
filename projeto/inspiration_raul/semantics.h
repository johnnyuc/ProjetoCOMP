#ifndef _SEMANTICS_H
#define _SEMANTICS_H

#include "ast.h"

int check_program(struct node *program);


struct symbol_list {
	char *identifier;
	enum type type;
	struct parameter_list *parameters;
	struct node *node;
	struct symbol_list *next;
};

struct symbol_tables {
	struct symbol_list *list;
	char *func_name;
	struct symbol_tables *next;
};

struct parameter_list{
	enum type parameter;
	struct parameter_list *next;
};

struct parameter_list *create_parameter(enum type parameter);
void check_node(struct node *node);
void check_funcbody(struct node *node, struct symbol_list *func_list);
enum type check_parameters(struct node *paramdeclaration);
int count_parameters(struct parameter_list *list);
struct parameter_list *add_parameter(struct parameter_list *list, enum type parameter);
void free_parameters(struct parameter_list *list);
struct symbol_list *insert_symbol(struct symbol_list *table, char *identifier, enum type type, struct parameter_list *parameters, struct node *node);
struct symbol_list *search_symbol(struct symbol_list *symbol_table, char *identifier);
int count_elements(struct node_list *list);
void show_symbol_table();
void show_symbol_table_func();

struct symbol_tables *insert_table(struct symbol_tables *table_list, struct symbol_list *list, char *token);

#endif