#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct ast_node{
	int tip;
	int lin,col;
	char* str;
	char* type;
	bool id;
	struct ast_node* son;
	struct ast_node* brother;
}ast_node;

typedef struct Inf{
  char* str;
	int lin,col;
}Inf;


ast_node* register_ast(char* str,int tip, char* type,bool id,int lin,int col);
void add_brother(ast_node* node1, ast_node* node_brother);
void add_son(ast_node* node, ast_node* node_son);
ast_node* id(Inf* inf);
ast_node* chrlit(Inf* inf);
ast_node* intlit(Inf* inf);
ast_node* realit(Inf* inf);
void print_ast(ast_node* node,int level);
int statList_childs(ast_node* node);
int is_null(ast_node* node);
void free_ast(ast_node* node);
