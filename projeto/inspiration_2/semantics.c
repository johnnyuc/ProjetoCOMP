#include<stdio.h>
#include<string.h>
#include "semantics.h"
//#include "symbol_table.h"

void check_program(ast_node* node) {
    char* nome;
    ast_node* aux = node->son;
    global = register_table("Global",0,0);
    tables* tab = global;
    para_list* para = register_para("int");
    para->first=para;
    variable* new_var1 = register_var("putchar",0,0,1,"int",para,1);
    add_var(global,new_var1);
    para_list* para1 = register_para("void");
    para1->first=para1;
    variable* new_var2 = register_var("getchar",0,0,1,"int",para1,1);
    add_var(global,new_var2);
    while(aux!=NULL){
      if(strcmp(aux->str, "FuncDefinition") == 0 ){
        ast_node* node3 = aux->son->brother->brother->son;
        ast_node* node4 = NULL;
        int count=0;
        bool f_void=false;
        while(node3!=NULL){
          if(check_types(node3->son)==1){
            f_void=true;

            node4 = node3;
          }
          count++;
          node3=node3->brother;
        }
        if(count>1 && f_void==true){
        printf("Line %d, col %d: Invalid use of void type in declaration\n",node4->son->lin,node4->son->col);

        }else{
          nome = malloc(strlen(  aux->son->brother->str) + 1);
          strcpy(nome,aux->son->brother->str);
          strcpy(nome, nome + 3);
          nome[strlen(nome)-1] = '\0';
          if(find_tab(global,nome)==1){
            tables* tab1=global;
            while(strcmp(tab1->str,nome)!=0  ){
              tab1=tab1->next_table;
            }
            if(strcmp(tab1->first_var->type,var_return(aux->son->str))==0){
              if(compare_Param_List(global,aux->son->brother->brother->son,nome)==0){
                tab1->type=0;
                tab1->first_var=NULL;
              }else{
                variable* v = global->first_var;
                while(strcmp(v->str,nome)!=0  ){
                  v=v->next;
                }
                check_para(v,aux->son,nome);
              }
              variable* new_var = register_var("return",0,0,0,var_return(aux->son->str),NULL,0);
              add_var(tab1,new_var);
              ast_node* node2 = aux->son->brother->brother->son;
              check_ParamDeclaration(global,tab1,node2,aux->son->str,nome);
              node2 = aux->son->brother->brother->brother;
              check_Declaration(global,tab1,node2);
            }
            else{
              variable* v = global->first_var;
              while(strcmp(v->str,nome)!=0  ){
                v=v->next;
              }
              check_para(v,aux->son,nome);
            }
          }else if(find_tab(global,nome)==0){
            tables* tab1=register_table(nome,1,0);
            tab->next_table=tab1;
            tab=tab->next_table;
            variable* new_var = register_var("return",0,0,0,var_return(aux->son->str),NULL,0);
            add_var(tab,new_var);
            ast_node* node2 = aux->son->brother->brother->son;
            check_ParamDeclaration(global,tab,node2,aux->son->str,nome);
            node2 = aux->son->brother->brother->brother;
            check_Declaration(global,tab,node2);
          }
        }
        check_functionbody(global,tab,aux->son->brother->brother->brother->son);
      }else if(strcmp(aux->str, "FuncDeclaration") == 0){
        ast_node* node3 = aux->son->brother->brother->son;
        ast_node* node4 = NULL;
        int count=0;
        bool f_void=false;
        while(node3!=NULL){
          if(check_types(node3->son)==1){
            f_void=true;
            node4 = node3;
          }
          count++;
          node3=node3->brother;
        }
        if(count>1 && f_void==true){
        printf("Line %d, col %d: Invalid use of void type in declaration\n",node4->son->lin,node4->son->col);

        }else{
          nome = malloc(strlen(  aux->son->brother->str) + 1);
          strcpy(nome,aux->son->brother->str);
          strcpy(nome, nome + 3);
          nome[strlen(nome)-1] = '\0';
          if(find_tab(global,nome)==0){
            tables* tab1=register_table(nome,1,1);
            tab->next_table=tab1;
            tab=tab->next_table;
            variable* new_var = register_var("return",0,0,0,var_return(aux->son->str),NULL,0);
            add_var(tab,new_var);
            ast_node* node2 = aux->son->brother->brother->son;
            check_ParamDeclaration(global,tab,node2,aux->son->str,nome);
            node2 = aux->son->brother->brother->brother;
            if(node2!=NULL){
              check_Declaration(global,tab,node2);
            }
          }else{
            variable* v = global->first_var;
            while(strcmp(v->str,nome)!=0  ){
              v=v->next;
            }
            check_para(v,aux->son,nome);
          }
        }
      }else if(strcmp(aux->str, "Declaration") == 0){
        int erro = check_types( aux->son);
        if(erro==0){
          nome = malloc(strlen(  aux->son->brother->str) + 1); ;
          strcpy(nome,aux->son->brother->str);
          strcpy(nome, nome + 3);
          nome[strlen(nome)-1] = '\0';
          variable* new_var = register_var(nome,0,0,0,var_return(aux->son->str),NULL,0);
          add_var(global,new_var);
          if(aux->son->brother->brother!=NULL){
            check_functionbody(global,tab,aux->son->brother->brother);
          }
        }else{
          printf("Line %d, col %d: Invalid use of void type in declaration\n",aux->son->brother->lin,aux->son->brother->col );
        }
      }
      aux=aux->brother;
    }
}

void check_para(variable* v,ast_node* ast,char* nome){
  ast_node* a =ast->brother->brother->son;
  int l,c;
  para_list* p = v->next_para->first->next;
  bool equal=true;
  int c1 = n_var_types(v->next_para);
  int c2=1;
  while(a!=NULL && p!=NULL){
    c2++;
    if(strcmp(var_return(a->son->str),p->type)!=0){

      equal=false;
      l=a->son->lin;
      c=a->son->col;
    }
    a=a->brother;
    p=p->next;
  }
  c1-=1;
  c2-=1;
  if(equal==false && c1==c2){
    printf("Line %d, col %d: Conflicting types (got", ast->brother->lin,ast->brother->col);
    printf(" %s(",var_return(ast->str));
    ast_node* a1 =ast->brother->brother->son;
    printf("%s",var_return(a1->son->str));
    a1=a1->brother;
    while(a1!=NULL){
      printf(",%s",var_return(a1->son->str));
      a1=a1->brother;
    }
    printf("), expected ");
    para_list* p = v->next_para->first;
    printf(" %s(",p->type);
    p=p->next;
    printf("%s",p->type);
    p=p->next;
    while(p!=NULL){
      printf(",%s",p->type);
      p=p->next;
    }
    printf("))\n");
  }else  if(c1!=c2){
    printf("Line %d, col %d: Wrong number of arguments to function %s (got %d, required %d)\n",l,c,nome,c2,c1);
  }

}
void check_ParamDeclaration(tables* tab,tables* tab1, ast_node* node,char* type,char* nome){
  para_list* para = NULL;
  para_list* first = NULL;
  char* name;
  para = register_para(var_return(node->son->str));
  para->first=para;
  first=para;
  while(node!=NULL){
    para->next = register_para(var_return(node->son->str));
    if(node->son->brother!=NULL){
      name = malloc(strlen( node->son->brother->str) + 1); ;
      strcpy(name,node->son->brother->str);
      strcpy(name, name + 3);
      name[strlen(name)-1] = '\0';
      variable* new_var1 = register_var(name,0,0,2,var_return(node->son->str),NULL,0);
      add_var(tab1,new_var1);
    }
    para=para->next;
    para->first=first;
    node=node->brother;
  }
  variable* new_var = register_var(nome,0,0,1,var_return(type),para->first,0);
  add_var(tab,new_var);
}
void check_Declaration(tables* global,tables* tab, ast_node* node){
  char* name;
  if(node->son!=NULL){
    ast_node* n = node->son;
    while(n!=NULL){
      if(strcmp(n->str, "Declaration") == 0){
        int erro = check_types( n->son);
        if(erro==0){
          name = malloc(strlen( n->son->brother->str) + 1); ;
          strcpy(name, n->son->brother->str);
          strcpy(name, name + 3);
          name[strlen(name)-1] = '\0';
          variable* new_var = register_var(name,0,0,0,var_return(n->son->str),NULL,0);
          //printf("%s--%s\n",tab->str,new_var->str);
          add_var(tab,new_var);
        }else{
          printf("Line %d, col %d: Invalid use of void type in declaration\n",n->son->brother->lin,n->son->brother->col );
          //aux->son->brother->lin,aux->son->brother->col);
        }
      }
      n=n->brother;
    }
  }
}
int check_types(ast_node* node){
  ast_node* aux = node;
  //printf("%s\n",node->str);
  if(strcmp(aux->str,"Void")==0){
    return 1;
  }
  return 0;
}
int compare_Param_List(tables* global,ast_node* n2,char* nome){
  bool comp = true;
  int c = 0;
  int o = 0;
  ast_node* aux = n2;
  variable* v = global->first_var;
  while(strcmp(v->str,nome)!=0  ){
    v=v->next;
  }
  para_list* p = v->next_para->first->next;
  c = n_var_types(v->next_para->first->next);
  while(comp){
    if(aux==NULL || p==NULL){
      comp=false;
    }else{
      if(strcmp(var_return(aux->son->str),p->type)==0){
        o++;
        aux=aux->brother;
        p=p->next;
      }else{
        printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n",aux->son->lin,aux->son->col,var_return(aux->son->str),p->type );
        comp=false;
      }
    }
  }
  if(c==o){
    return 0;
  }
  else{
    return 1;
  }
}

void check_functionbody(tables* tab,tables* tab1, ast_node* node){
  ast_node* n1 = node;
  if(n1!=NULL){
    while(n1!=NULL){
      if(strcmp(n1->str,"If")==0){
        check_if(tab,tab1,n1);
      }else if(strcmp(n1->str,"Return")==0){
        check_return(tab,tab1,n1);
      }else if(strcmp(n1->str,"While")==0){
        //check_while(tab,tab1,n1);
      }else if(strcmp(n1->str,"Call")==0){
        check_number(tab,tab1,n1);
      }else if(strcmp(n1->str,"Store")==0){
        check_store(tab,tab1,n1);
      }else if(strcmp(n1->str,"Statlist")==0){

      }else if(strcmp(n1->str,"Declaration")==0){
        check_decla(tab,tab1,n1);
      }else if(strcmp(n1->str,"And")==0){
        check_statment(tab,tab1,n1);
      }else if(strcmp(n1->str,"Or")==0){
        check_statment(tab,tab1,n1);
      }else if(strcmp(n1->str,"Mod")==0){
        check_statment(tab,tab1,n1);
      }else if(strcmp(n1->str,"BitWiseAnd")==0){
        check_statment(tab,tab1,n1);
      }else if(strcmp(n1->str,"BitWiseOr")==0){
        check_statment(tab,tab1,n1);
      }else if(strcmp(n1->str,"BitWiseXor")==0){
        check_statment(tab,tab1,n1);
      }else if(strcmp(n1->type,"Eq")==0){
        check_statment(tab,tab1,n1);
      }else if(strcmp(n1->type,"Ne")==0){
        check_statment(tab,tab1,n1);
      }else if(strcmp(n1->str,"Le")==0){
        check_statment(tab,tab1,n1);
      }else if(strcmp(n1->str,"Ge")==0){
        check_statment(tab,tab1,n1);
      }else if(strcmp(n1->str,"Gt")==0){
        check_statment(tab,tab1,n1);
      }else if(strcmp(n1->str,"Lt")==0){
        check_statment(tab,tab1,n1);
      }else if(strcmp(n1->str,"Plus")==0){
      }else if(strcmp(n1->str,"Minus")==0){
      }else if(strcmp(n1->str,"Not")==0){
      }else if(strcmp(n1->type,"tipo1")==0){
        check_statment(tab,tab1,n1);
      }else if(strcmp(n1->type,"tipo")==0){
        check_statment(tab,tab1,n1);
      }
      n1=n1->brother;
    }
  }
}

void check_number(tables* tab,tables* tab1, ast_node* node){
  char* name = malloc(strlen( node->son->str) + 1); ;
  strcpy(name, node->son->str);
  strcpy(name, name + 3);
  name[strlen(name)-1] = '\0';
  bool comp = true;
  bool erro = false;
  int c = 0;
  int o=0;
  ast_node* aux = node->son->brother;
  ast_node* aux1 = NULL;
  variable* v = tab->first_var;
  while(comp){
    if (strcmp(v->str,name)==0) {
      comp=false;
    }else{
      if(v->next==NULL){
        erro = true;
        comp = false;
      }else{
        v=v->next;
      }
    }
  }
  if(erro){
    while(aux!=NULL){
      o++;
      aux1=aux;
      aux=aux->brother;
    }
    printf("Line %d, col %d: Unknown symbol %s\n",node->son->lin,node->son->col,name);
    printf("Line %d, col %d: Wrong number of arguments to function %s (got %d, required 0)\n",node->son->lin,node->son->col,name,o);
  }else{
    para_list* p = v->next_para->first->next;
    c = n_var_types(v->next_para->first->next);
    while(aux!=NULL){
      if(aux->id != true || strcmp(aux->type,"null")!=0){
        //printf("%s\n",aux->str );
        check_functionbody(tab,tab1,aux);
      }
      o++;
      //printf("%s .. %s .. %d .. %d\n",aux->str,aux->type,aux->lin,aux->col);
      aux1 =aux;
      aux=aux->brother;
    }
    //printf("%d--%d\n",o,c);
    if(o!=c){
      printf("Line %d, col %d: Wrong number of arguments to function %s (got %d, required %d)\n",node->son->lin,node->son->col,name,o,c);
    }else if(c==o){
      check_param(p,node->son,tab,tab1,c);
    }
  }
}

void check_param(para_list* para,ast_node* node,tables* tab,tables* tab1,int count){
  ast_node* n=node->brother;
  int k = 0;
  char* t1 = malloc(10);
  char* t2 = malloc(10);
  int l,c;
  bool equal = true;
  for(k=0;k<count;k++){
    if(n->id)
      check_id(tab,tab1,n);
    //printf("%s   ---  %s   ---  %s\n",n->str,ftype(tab,tab1,n),para->type );
    if(strcmp(ftype(tab,tab1,n),para->type)!=0){
      equal = false;
      t1 = ftype(tab,tab1,n);
      t2 =para->type;
      l = n->lin;
      c = n->col;
    }
    n=n->brother;
    para=para->next;
  }
  if(equal==false){
    printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n",l,c,t1,t2 );
  }
}

void check_id(tables* global,tables* tab,ast_node* n){
  bool found = false;
  char* name = malloc(strlen( n->str) + 1);
  strcpy(name, n->str);
  strcpy(name, name + 3);
  name[strlen(name)-1] = '\0';
  variable* v = global->first_var;
  while(v!=NULL){
    if(strcmp(v->str,name)==0){
      found=true;
    }
    v=v->next;
  }
  v = tab->first_var;
  while(v!=NULL){
    if(strcmp(v->str,name)==0){
      found=true;
    }
    v=v->next;
  }
  if(!found){
      printf("Line %d, col %d: Unknown symbol %s\n", n->lin,n->col,name);
  }
}
char* ftype(tables* global,tables* tab,ast_node* n){
  bool found = false;
  char* aux;
  char* name = malloc(strlen( n->str) + 1);
  strcpy(name, n->str);
  strcpy(name, name + 3);
  name[strlen(name)-1] = '\0';
  variable* v = global->first_var;
  while(v!=NULL){
    if(strcmp(v->str,name)==0){
      found=true;
      aux = v->type;
    }
    v=v->next;
  }
  if(found==false){
    v = tab->first_var;
    while(v!=NULL){
      if(strcmp(v->str,name)==0){
        found=true;
        aux = v->type;
      }
      v=v->next;
    }
  }
  if(found == false){
    aux = "undef";
  }
  if(strcmp(n->type,"null")!=0){
    return n->type;
  }else{
    return aux;
  }

}
void check_statment(tables* global,tables* tab,ast_node* aux){
  char* t1 = malloc(10);
  char* t2 = malloc(10);
  printf("%s -- %s\n",aux->str ,aux->type);
  if((aux->son->id != true || strcmp(aux->son->type,"int")!=0 || strcmp(aux->son->type,"double")!=0)){
    //printf("----    %s -- %s\n",aux->son->str,aux->son->type );
    check_functionbody(global,tab,aux->son);
  }else{
    if(aux->son->id)
      check_id(global,tab,aux->son);
    printf("%s -- %s\n",aux->son->str,aux->son->type );
    if(strcmp(aux->son->type,"null")!=0){
      t1=aux->son->type;
    }else{
      t1 = ftype(global,tab,aux->son);
    }

  }
  if(aux->son->brother!=NULL){
    if((aux->son->brother->id != true || strcmp(aux->son->brother->type,"int")!=0 || strcmp(aux->son->brother->type,"double")!=0)){
      //printf("----    %s -- %s\n",aux->son->brother->str,aux->son->brother->type );
      check_functionbody(global,tab,aux->son->brother);
    }else{
      if(aux->son->brother->id)
        check_id(global,tab,aux->son->brother);
      printf("%s -- %s\n",aux->son->brother->str,aux->son->brother->type );
      if(strcmp(aux->son->brother->type,"null")!=0){
        t2=aux->son->brother->type;
      }else{
        t2 = ftype(global,tab,aux->son->brother);
      }
    }
  }
  if(strcmp(t1,"undef")==0 && strcmp("undef",t2)==0){
    t1 = "NULL";
  }
  if(strcmp(t1,t2)!=0){
    if(strcmp(aux->str,"Add")==0){
      printf("Line %d, col %d: Operator + cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->str,"Sub")==0){
      printf("Line %d, col %d: Operator - cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->str,"Mul")==0){
      printf("Line %d, col %d: Operator * cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->str,"Div")==0){
      printf("Line %d, col %d: Operator / cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->str,"And")==0){
      printf("Line %d, col %d: Operator && cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->str,"Or")==0){
      printf("Line %d, col %d: Operator || cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->str,"Mod")==0){
      printf("Line %d, col %d: Operator %% cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->str,"BitWiseAnd")==0){
      printf("Line %d, col %d: Operator & cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->str,"BitWiseOr")==0){
      printf("Line %d, col %d: Operator | cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->str,"BitWiseXor")==0){
      printf("Line %d, col %d: Operator ^ cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->type,"Eq")==0){
      printf("Line %d, col %d: Operator == cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->type,"Ne")==0){
      printf("Line %d, col %d: Operator != cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->str,"Le")==0){
      printf("Line %d, col %d: Operator <= cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->str,"Ge")==0){
      printf("Line %d, col %d: Operator >= cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->str,"Gt")==0){
      printf("Line %d, col %d: Operator > cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }else if(strcmp(aux->str,"Lt")==0){
      printf("Line %d, col %d: Operator < cannot be applied to types %s,%s\n",aux->son->lin,aux->son->col,t1,t2);
    }
  }
  //printf("%s\n", n->son->brother->str);
}
void check_store(tables* global,tables* tab,ast_node* aux){
  char* t1 = malloc(10);
  char* t2 = malloc(10);
  if(aux->son->son==NULL && aux->son->brother->son==NULL){
    if(strcmp(aux->son->type,aux->son->brother->type)==0 && strcmp(aux->son->type,"null")!=0 && aux->son->id==false){
      printf("Line %d, col %d: Lvalue required\n",aux->lin,aux->col );
    }
    if(aux->son->id)
      check_id(global,tab,aux->son);
    if(strcmp(aux->son->type,"null")!=0){
      t1=aux->son->type;
    }else{
      t1 = ftype(global,tab,aux->son);
    }
    if(aux->son->brother->id)
      check_id(global,tab,aux->son->brother);
    if(strcmp(aux->son->brother->type,"null")!=0){
      t2=aux->son->brother->type;
    }else{
      t2 = ftype(global,tab,aux->son->brother);
    }
    if(strcmp(t1,"undef")==0 && strcmp("undef",t2)==0){
      t1 = "NULL";
    }
    if(strcmp(t1,t2)!=0){
      printf("Line %d, col %d: Operator = cannot be applied to types %s,%s\n",aux->son->brother->lin,aux->son->brother->col,t1,t2);
    }
  }else{
    if((aux->son->id != true || strcmp(aux->son->type,"int")!=0 || strcmp(aux->son->type,"double")!=0)){
      check_functionbody(global,tab,aux->son);
    }
    if(aux->son->brother!=NULL){
      if((aux->son->brother->id != true || strcmp(aux->son->brother->type,"null")!=0 || strcmp(aux->son->brother->type,"int")!=0 || strcmp(aux->son->brother->type,"double")!=0)){
        check_functionbody(global,tab,aux->son->brother);
      }
    }
  }
}

void check_return(tables* global,tables* tab,ast_node* aux){
  if((aux->son->id != true || strcmp(aux->son->type,"int")!=0 || strcmp(aux->son->type,"double")!=0)){
    check_functionbody(global,tab,aux->son);
  }else{
    if(aux->son->id)
      check_id(global,tab,aux->son);
  }
  if(aux->son->brother!=NULL){
    if((aux->son->brother->id != true || strcmp(aux->son->brother->type,"int")!=0 || strcmp(aux->son->brother->type,"double")!=0)){
      check_functionbody(global,tab,aux->son->brother);
    }else{
      if(aux->son->brother->id)
        check_id(global,tab,aux->son->brother);
    }
  }
}

void check_decla(tables* global,tables* tab,ast_node* aux){
  if(aux->son->brother->brother!=NULL){
    if((aux->son->brother->brother->id != true || strcmp(aux->son->brother->brother->type,"int")!=0 || strcmp(aux->son->brother->brother->type,"double")!=0)){
      check_functionbody(global,tab,aux->son->brother);
    }else{
      if(aux->son->brother->id)
        check_id(global,tab,aux->son->brother);
    }
  }
}

void check_if(tables* global,tables* tab,ast_node* aux){
  if(aux->son->id){
    check_id(global,tab,aux->son);
  }else{
    check_functionbody(global,tab,aux->son);
  }
  if(strcmp("Null",aux->son->brother->str)!=0){
    check_functionbody(global,tab,aux->son->brother);
    //printf("%s\n",aux->son->brother->str);
  }
  if(strcmp("Null",aux->son->brother->brother->str)!=0){
  //printf("%s\n",aux->son->brother->brother->str);
    check_functionbody(global,tab,aux->son->brother->brother);
  }
}
