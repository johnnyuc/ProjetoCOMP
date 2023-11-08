#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ast_node{
	int tip; 
	char* str;
	struct ast_node* son;
	struct ast_node* brother;
}ast_node;


ast_node* register_ast(char* str,int tip);
void add_brother(ast_node* node1, ast_node* node_brother);
void add_son(ast_node* node, ast_node* node_son);
ast_node* id(char* str);
ast_node* chrlit(char* str);
ast_node* intlit(char* str);
ast_node* realit(char* str);
void print_ast(ast_node* node,int level );
int statList_childs(ast_node* node);
int is_null(ast_node* node);
void free_ast(ast_node* node);
