#include <stdlib.h>
#include "ast.h"

//typedef enum {int,char,void,short,double} basic_type;

typedef struct tables{
	char* str;
  int func;
	int type;
  struct variable* first_var;
	struct variable* next_var;
	struct tables* next_table;
} tables;

typedef struct variable {
    char* str;
    int lin, col;
    char* type;
    int tipo;
		int lido;
		struct para_list* next_para;
    struct variable* next;
} variable;

typedef struct para_list {
    char* type;
    struct para_list* first;
    struct para_list* next;
} para_list;


tables* register_table(char* str,int function,int type);
variable* register_var(char* str,int lin,int col,int function,char* type, para_list* v_type,int lido);
para_list* register_para(char* type);
void add_var(tables* tab, variable* new_var);
int find_var(tables* tab, char* new_var);
void print_tables(tables* tab);
char* var_return(char* var);
void print_m3(tables* global,ast_node* node,int level,int error,tables* t);
void ler(tables* tab, char* c);
int find_tab(tables* tab, char* new_var);
char* find(tables* tab, char* new_var,int t);
char* son_type(tables* tab,ast_node* node,int tipo,tables* t);
char* find_type(tables* tab, char* new_var);
int n_var_types(para_list* v_types);
