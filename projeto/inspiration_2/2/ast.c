#include "ast.h"

ast_node* register_ast(char* str,int tip){
	ast_node* node = (ast_node *) malloc(sizeof(ast_node));
	node->str = strdup(str);
	node->tip=tip;
	return node;
}


void add_brother(ast_node* node1, ast_node* node_brother){
	if (node1 == NULL || node_brother == NULL){
		return;
	}
	ast_node* aux = node1;
	if (aux->brother == NULL){
		aux->brother = node_brother;
	} else {
		while (aux->brother != NULL){
			aux = aux->brother;
		}
		aux->brother = node_brother;
	}
}

void add_son(ast_node* node, ast_node* node_son){
	if (node == NULL || node_son == NULL){
		return;
	}
	if (node->son == NULL){
		node->son = node_son;
	} else {
		add_brother(node->son, node_son);
	}
}

ast_node* id(char* str){
	char* arr = (char *) malloc(strlen(str)+5);
	sprintf(arr, "Id(%s)", str);
	return register_ast(arr,0);
}

ast_node* chrlit(char* str){
	char* arr = (char *) malloc(strlen(str)+5);
	sprintf(arr, "ChrLit(%s)", str);
	return register_ast(arr,0);
}

ast_node* intlit(char* str){
	char* arr = (char *) malloc(strlen(str)+5);
	sprintf(arr, "IntLit(%s)", str);
	return register_ast(arr,0);
}

ast_node* realit(char* str){
	char* arr = (char *) malloc(strlen(str)+5);
	sprintf(arr, "RealLit(%s)", str);
	return register_ast(arr,0);
}

void print_ast(ast_node* node,int level){
	if (node == NULL) return;
	if(node->tip!=1){
		for (int i = 0; i < level; i++) printf("..");
		printf("%s\n", node->str);
	}
	print_ast( node->son,level + 1);
	print_ast(node->brother,level);
}
int statList_childs(ast_node* node){
  if(node==NULL)
    return 0;

  int count = 1;
  ast_node* temp = node;
  while(temp->brother!=NULL){
    if(temp->tip==0){ count++;}
	
    temp = temp->brother;
	
  }
  return count;

}

int is_null(ast_node* node){
	if (node == NULL || node->brother == NULL ) return 0;
	return 1;
}
void free_ast(ast_node* node){
	if (node == NULL){
		return;
	}
	if (node->son != NULL){
		free(node->son);
	}
	if (node->brother != NULL){
		free(node->brother);
	}
	free(node->str);
	free(node);
}
