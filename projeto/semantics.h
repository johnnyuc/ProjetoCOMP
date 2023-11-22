#ifndef _SEMANTICS_H
#define _SEMANTICS_H

#include "ast.h"

int check_program(struct node *program);

struct symbol_list {
	char *identifier;
	enum type type;
	struct node *node;
	struct para_list* next_para;
	struct symbol_list *next;
};

typedef struct para_list {
    char* type;
    struct para_list* first;
    struct para_list* next;
} para_list;

para_list* register_para(char* type);
struct symbol_list *insert_symbol(struct symbol_list *table, char *identifier,enum type type, para_list* v_type, struct node *node);
struct symbol_list *search_symbol(struct symbol_list *symbol_table, char *identifier);
void show_symbol_table();

#endif