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
	int is_parameter;
};

struct symbol_tables {
	struct symbol_list *list;
	char *func_name;
	struct symbol_tables *next;
};

struct parameter_list {
    char *name;
    enum type parameter;
    struct parameter_list *next;
	int param;
};

struct parameter_list *create_parameter(enum type parameter);
void check_node(struct node *node);
void check_funcbody(struct node *node, struct symbol_list *func_list);
void check_statements(struct node *node, struct symbol_list *func_list);
void check_expression(struct node *expression, struct symbol_list *func_list);
struct symbol_tables *search_table(struct symbol_tables *table, char *token);
void insert_params_to_symbol_table(struct symbol_list *symbol_tableFunc, struct parameter_list *params);
enum type check_parameters(struct node *paramdeclaration);
char *get_identifier(struct node *paramdeclaration);
int count_parameters(struct parameter_list *list);
struct parameter_list *add_parameter(struct parameter_list *list, enum type parameter, char *name);
void free_parameters(struct parameter_list *list);
struct symbol_list *insert_symbol(struct symbol_list *table, char *identifier, enum type type, struct parameter_list *parameters, struct node *node, int is_parameter);
struct symbol_list *search_symbol(struct symbol_list *symbol_table, char *identifier);
int count_elements(struct node_list *list);
void show_symbol_table();
void show_symbol_table_func();

struct symbol_tables *insert_table(struct symbol_tables *table_list, struct symbol_list *list, char *token);

void show_new(struct node *root, int depth);

#endif