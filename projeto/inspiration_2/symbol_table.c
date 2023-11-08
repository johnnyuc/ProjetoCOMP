#include<stdio.h>
#include<string.h>
#include <stdbool.h>
#include "symbol_table.h"



tables* register_table(char* str,int function,int type){
      tables* tab = (tables *) malloc(sizeof(tables));
      tab->str=strdup(str);
      tab->first_var=NULL;
      tab->next_table=NULL;
      tab->next_var=NULL;
      tab->func=function;
      tab->type=type;
      return tab;
}

variable* register_var(char* str,int lin,int col,int function,char* type, para_list* v_type,int lido){
	variable* var = (variable *) malloc(sizeof(variable));
	var->str = strdup(str);
	var->lin=lin;
  var->col=col;
  var->type=type;
  var->tipo=function;
  var->next_para=v_type;
  var->next=NULL;
  var->lido=lido;
	return var;
}
para_list* register_para(char* type){
  para_list* para = (para_list *) malloc(sizeof(para_list));
  para->type=type;
  para->next=NULL;
  return para;
}

void add_var(tables* tab, variable* new_var){
  //printf("%s--%s\n",tab->str,new_var->str);
  if(tab->first_var==NULL){
    //printf("errr12123423orr");
    tab->first_var=new_var;
    tab->next_var=new_var;
  }else{//printf("errrorr");
    if (find_var(tab,new_var->str)==0){
  		tab->next_var->next=new_var;
      tab->next_var=new_var;
  	} /*else {
  		printf("Line %d, column %d: Symbol %s already defined\n", new_var->lin, new_var->col,new_var->str);
  	}*/
  }

}

int find_var(tables* tab, char* new_var){
	variable* aux= tab->first_var;

	while (aux != NULL){
		if (strcmp(aux->str, new_var) == 0){
		    return 1;
		}
		aux=aux->next;
	}
	return 0;
}



void print_tables(tables* tab){
    if (tab == NULL) return;
    if(tab->type==0){
      if(tab->func==0){
        printf("===== %s Symbol Table =====\n", tab->str);
      }else{
        printf("===== Function %s Symbol Table =====\n", tab->str);
      }
      variable* aux = tab->first_var;
      while(aux!=NULL){
        if(aux->tipo==2){
            printf("%s\t%s\tparam\n",aux->str,aux->type);
        }else if(aux->tipo==0){
          printf("%s\t%s\n",aux->str,aux->type);
        }else{
          //printf("%d\n",n_var_types(aux->next_para));
          if(n_var_types(aux->next_para)==0){
            printf("%s\t%s()\n",aux->str,aux->type);
          }else{
            int cont = n_var_types(aux->next_para);
            printf("%s\t%s(",aux->str,aux->type);
            para_list* aux1 = aux->next_para;
            printf("%s",aux1->type);
            if(cont>1){
              aux1=aux1->next->next;
              while(aux1!=NULL){
                printf(",%s",aux1->type);
                aux1=aux1->next;
              }
            }
            printf(")\n");
          }
        }
        aux=aux->next;
      }
      printf("\n");
    }
    print_tables(tab->next_table);
}

char* var_return(char* var){
  if(strcmp(var,"Char")==0){
    return "char";
  }else if(strcmp(var,"Int")==0){
    return "int";
  }else if(strcmp(var,"Double")==0){
    return "double";
  }else if(strcmp(var,"Short")==0){
    return "short";
  }else if(strcmp(var,"Void")==0){
    return "void";
  }else{
    return NULL;
  }
}

void print_m3(tables* global,ast_node* node,int level,int error,tables* t){
  if (node == NULL) return;
  char* name;
  char* var1;
  int erro = error;

  if(strcmp("FuncDefinition",node->str)==0){
    name =  malloc(strlen(node->son->brother->str) + 1); ;
    strcpy(name,node->son->brother->str);
    strcpy(name, name + 3);
    name[strlen(name)-1] = '\0';
    ler(global,name);
    ler(t,name);
    if(find_tab(global,name)>0){
      tables* tab1=global;
      while(strcmp(tab1->str,name)!=0  ){
        tab1=tab1->next_table;
      }
      t=tab1;
      if(tab1->type == 1){
        erro= 1;
      }else{
        erro = 0;
      }
    }else{
      erro= 1;
    }
  }else if(strcmp("FuncDeclaration",node->str)==0){
    name =  malloc(strlen(node->son->brother->str) + 1); ;
    strcpy(name,node->son->brother->str);
    strcpy(name, name + 3);
    name[strlen(name)-1] = '\0';
    ler(global,name);
    ler(t,name);
  }else if(strcmp("Declaration",node->str)==0){
    name =  malloc(strlen(node->son->brother->str) + 1); ;
    strcpy(name,node->son->brother->str);
    strcpy(name, name + 3);
    name[strlen(name)-1] = '\0';
    ler(global,name);
    ler(t,name);
  }else if(strcmp("ParamDeclaration",node->str)==0){
    if(node->son->brother!=NULL){
      name =  malloc(strlen(node->son->brother->str) + 1); ;
      strcpy(name,node->son->brother->str);
      strcpy(name, name + 3);
      name[strlen(name)-1] = '\0';
      ler(global,name);
      ler(t,name);
    }
  }
  if(node->tip!=1){
    for (int i = 0; i < level; i++) printf("..");
    if(erro==1){
        printf("%s\n", node->str);
    }else if(node->id!=true && strcmp(node->type,"null")==0 ){
      printf("%s\n", node->str);
    }else if(node->id==true && strcmp(node->type,"null")==0){
      printf("%s - ", node->str);
      name =  malloc(strlen(node->str) + 1); ;
      strcpy(name,node->str);
      strcpy(name, name + 3);
      name[strlen(name)-1] = '\0';
      var1 = malloc(10);
      var1 = find(t,name,0);
      if(strcmp(  var1,"undef")==0){
        var1 = find(global,name,0);
        if(strcmp(  var1,"undef")==0)
          printf("%s\n",var1);
      }
    }else if(strcmp(node->type,"tipo")==0){
      printf("%s - ", node->str);
      var1 = malloc(10);
      var1 = son_type(global,node,0,t);
      printf("%s\n", var1);
    }else if(strcmp(node->type,"tipo1")==0){
      printf("%s - ", node->str);
      var1 = malloc(10);
      var1 = son_type(global,node,1,t);
      printf("%s\n", var1);
    }else if(strcmp(node->type,"tipo2")==0){
      printf("%s - ", node->str);
      var1 = malloc(10);
      var1 = son_type(global,node,2,t);
      printf("%s\n", var1);
    }else if(strcmp(node->type,"null")!=0 && strcmp(node->type,"Null")!=0){
      printf("%s - %s\n", node->str,node->type);
    }else{
      printf("%s\n", node->str);
    }
  }
  print_m3( global,node->son,level + 1,erro,t);
  print_m3(global,node->brother,level,erro,t);
}
void ler(tables* tab, char* c){
  variable* aux= tab->first_var;
	while (aux != NULL){
		if (strcmp(aux->str, c) == 0){
      aux->lido=1;
		}
		aux=aux->next;
	}
}
int find_tab(tables* tab, char* new_var){
	tables* aux= tab;
	while (aux != NULL){
		if (strcmp(aux->str, new_var) == 0){
      if(aux->type==1){
        return 1;
      }else{
        return 2;
      }
		}
		aux=aux->next_table;
	}
	return 0;
}
char* find(tables* tab, char* new_var,int t){
  variable* aux2=NULL;
  variable* aux= tab->first_var;
    while (aux != NULL){
      if (strcmp(aux->str, new_var) == 0){
        if(aux->tipo==1){
          if(aux->lido==1){
            aux2=aux;
          }
        }else{
          if(aux->lido==1)
            aux2=aux;
        }
      }
      aux=aux->next;
    }
  if(aux2!=NULL){
    if(t==1){
      printf("%s\n",aux2->type);
    }else{
      if(aux2->tipo==0){
        printf("%s\n",aux2->type);
      }else{
        if(n_var_types(aux2->next_para)==0){
          printf("%s\n",aux2->type);
        }else{
          int cont = n_var_types(aux2->next_para);
          printf("%s(",aux2->type);
          para_list* aux1 = aux2->next_para;
          printf("%s",aux1->type);
          if(cont>1){
            aux1=aux1->next->next;
            while(aux1!=NULL){
              printf(",%s",aux1->type);
              aux1=aux1->next;
            }
          }
          printf(")\n");
        }
      }
    }
  }else{
    return "undef";
  }
  return "null";
}

char* son_type(tables* global,ast_node* node,int tipo,tables* t){
  char* var1;
  char* var2;
  char* name1;
  char* name2;
  var1 = malloc(10);
  var2 = malloc(10);
  bool onlychild = false;
  if(node->son->brother==NULL){
    onlychild = true;
    if(strcmp(node->son->type,"tipo")==0){
      var1 = son_type(global,node->son,0,t);
    }else if(strcmp(node->son->type,"tipo1")==0){
      var1 = son_type(global,node->son,1,t);
    }else if(strcmp(node->son->type,"tipo2")==0){
      var1 = son_type(global,node->son,2,t);
    }else{
      if(strcmp(node->son->type,"char")==0 || strcmp(node->son->type,"int")==0 ||strcmp(node->son->type,"double")==0 ){
        var1 = node->son->type;
      }else{
        name1 =  malloc(strlen(node->son->str) + 1); ;
        strcpy(name1,node->son->str);
        strcpy(name1, name1 + 3);
        name1[strlen(name1)-1] = '\0';
        var1 = find_type(t,name1);
        //printf("  %s\n",var1);
        if(strcmp(  var1,"undef")==0){
          var1 = find_type(global,name1);
        }
      }
    }

  }else if(node->son->son!=NULL && node->son->brother->son!=NULL){

    if(strcmp(node->son->type,"tipo")==0){
      var1 = son_type(global,node->son,0,t);
      var2 = son_type(global,node->son->brother,0,t);
    }else if(strcmp(node->son->type,"tipo1")==0){
      var1 = son_type(global,node->son,1,t);
      var2 = son_type(global,node->son->brother,1,t);
    }else if(strcmp(node->son->type,"tipo2")==0){
      var1 = son_type(global,node->son,2,t);
      var2 = son_type(global,node->son->brother,2,t);
    }else if(strcmp(node->son->type,"int")==0){
      var1="int";
      var2="int";
    }
  }else if(node->son->brother->son!=NULL){
    if(strcmp(node->son->brother->type,"tipo")==0){
      var2 = son_type(global,node->son->brother,0,t);
    }else if(strcmp(node->son->brother->type,"tipo1")==0){
      var2 = son_type(global,node->son->brother,1,t);
    }else if(strcmp(node->son->brother->type,"tipo2")==0){
      var2 = son_type(global,node->son->brother,2,t);
    }else if(strcmp(node->son->brother->type,"int")==0){
      var2="int";
    }
    if(node->son!=NULL){
      if(strcmp(node->son->type,"char")==0 || strcmp(node->son->type,"int")==0 ||strcmp(node->son->type,"double")==0 ){
        var1 = node->son->type;
      }else{
        name1 =  malloc(strlen(node->son->str) + 1); ;
        strcpy(name1,node->son->str);
        strcpy(name1, name1 + 3);
        name1[strlen(name1)-1] = '\0';
        var1 = find_type(t,name1);
        if(strcmp(  var1,"undef")==0){
          var1 = find_type(global,name1);
        }
      }
    }
  }else if(node->son->son!=NULL){
    if(strcmp(node->son->type,"tipo")==0){
      var1 = son_type(global,node->son,0,t);
    }else if(strcmp(node->son->type,"tipo1")==0){
      var1 = son_type(global,node->son,1,t);
    }else if(strcmp(node->son->type,"tipo2")==0){
      var1 = son_type(global,node->son,2,t);
    }else if(strcmp(node->son->type,"int")==0){
      var1="int";
    }
    if(node->son->brother!=NULL){
      if(strcmp(node->son->brother->type,"char")==0 || strcmp(node->son->brother->type,"int")==0 ||strcmp(node->son->brother->type,"double")==0 ){
        var2 = node->son->brother->type;
      }else{
        name2 =  malloc(strlen(node->son->brother->str) + 1); ;
        strcpy(name2,node->son->brother->str);
        strcpy(name2, name2 + 3);
        name2[strlen(name2)-1] = '\0';
        var2 = find_type(t,name2);
        if(strcmp(var2,"undef")==0){
          var2 = find_type(global,name2);
        }
      }
    }
  }else{
    if(node->son!=NULL){
      if(strcmp(node->son->type,"char")==0 || strcmp(node->son->type,"int")==0 ||strcmp(node->son->type,"double")==0 ){
        var1 = node->son->type;
      }else{
        name1 =  malloc(strlen(node->son->str) + 1); ;
        strcpy(name1,node->son->str);
        strcpy(name1, name1 + 3);
        name1[strlen(name1)-1] = '\0';
        var1 = find_type(t,name1);
        if(strcmp(  var1,"undef")==0){
          var1 = find_type(global,name1);
        }
      }
    }
    if(node->son->brother!=NULL){
      if(strcmp(node->son->brother->type,"char")==0 || strcmp(node->son->brother->type,"int")==0 ||strcmp(node->son->brother->type,"double")==0 ){
        var2 = node->son->brother->type;
      }else{
        name2 =  malloc(strlen(node->son->brother->str) + 1); ;
        strcpy(name2,node->son->brother->str);
        strcpy(name2, name2 + 3);
        name2[strlen(name2)-1] = '\0';
        var2 = find_type(t,name2);
        if(strcmp(var2,"undef")==0){
          var2 = find_type(global,name2);
        }
      }
    }
  }
  if(tipo==0){
      return var1;
  }else if(tipo==2){
      if(onlychild){
        return var1;
      }else{
        return var2;
      }

  }else{
    if(strcmp(var1,"undef")==0 || strcmp(var2,"undef")==0){
      return "undef";
    }else if(strcmp(var1,"void")==0 || strcmp(var2,"void")==0){
      return "undef";
    }else if(strcmp(var1,var2)==0){
      return var1;
    }else if(strcmp(var1,"int")==0  && strcmp(var2,"int")!=0 && strcmp(var2,"double")!=0){
      return var1;
    }
    else if(strcmp(var2,"int")==0  && strcmp(var1,"int")!=0 && strcmp(var1,"double")!=0){
      return var2;
    }else if(strcmp(var1,"double")==0  &&  strcmp(var2,"double")!=0){
      return var1;
    }
    else if(strcmp(var2,"double")==0  &&  strcmp(var1,"double")!=0){
      return var2;
    }
    else if(strcmp(var1,"char")==0  &&  strcmp(var2,"short")==0){
      return var2;
    }
    else if(strcmp(var2,"char")==0  &&  strcmp(var1,"short")==0){
      return var1;
    }else{
      return var1;
    }
  }
}
char* find_type(tables* tab,  char* new_var){
  char* var1=NULL;
  //printf("%s\n",tab->str );
  //printf("%s\n",new_var );
  bool found = false;
  variable* aux= tab->first_var;


  while (aux != NULL){
    if (strcmp(aux->str, new_var) == 0){
      if(aux->tipo==1){
        if(aux->lido==1){
          found=true;
          var1 =  aux->type;
        }
      }else{
        if(aux->lido==1){
          found=true;
          var1 =  aux->type;
        }
      }
    }
    aux=aux->next;
  }
  if(found==false){
    var1 = "undef";
  }
  return var1;
}
int n_var_types(para_list* v_types){
  int count=0;
  if(v_types==NULL){
    return 0;
  } else{
    para_list* aux = v_types;
    while(aux!=NULL){
      count++;
      aux=aux->next;
    }
    return count;
  }
}
