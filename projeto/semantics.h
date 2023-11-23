#ifndef _SEMANTICS_H
#define _SEMANTICS_H

#include "ast.h"

int check_program(struct node *program);

struct symbol_list {
	char *identifier;
	enum type type;
	struct node *node;
	struct param_list* next_param;
	struct symbol_list *next_symbol;
};

typedef struct param_list {
    char* type;
    struct param_list* next_param;
} param_list;

struct symbol_list *insert_symbol(struct symbol_list *table, char *identifier, enum type type, param_list *param, struct node *node);
struct symbol_list *search_symbol(struct symbol_list *symbol_table, char *identifier);
void show_symbol_table();

#endif