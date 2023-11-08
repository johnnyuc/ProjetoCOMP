#include "symbol_table.h"


extern struct tables* global;


void check_program(ast_node* node);
void check_para(variable* v,ast_node* a,char* nome);
void check_ParamDeclaration(tables* tab,tables* tab1, ast_node* node,char* type,char* nome);
void check_Declaration(tables* global,tables* tab, ast_node* node);
int check_types(ast_node* node);
int compare_Param_List(tables* global,ast_node* n2,char* nome);
void check_functionbody(tables* tab,tables* tab1, ast_node* node);
void check_node(tables* tab,tables* tab1, ast_node* node);
void check_number(tables* tab,tables* tab1, ast_node* node);
void check_param(para_list* para,ast_node* node,tables* tab,tables* tab1,int count);
void check_id(tables* global,tables* tab,ast_node* n);
char* ftype(tables* global,tables* tab,ast_node* n);
void check_statment(tables* global,tables* tab,ast_node* n);
void check_store(tables* global,tables* tab,ast_node* aux);
void check_return(tables* global,tables* tab,ast_node* aux);
void check_decla(tables* global,tables* tab,ast_node* aux);
void check_if(tables* global,tables* tab,ast_node* aux);
