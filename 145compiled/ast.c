#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

// create a node of a given category with a given lexical symbol
struct node *newnode(enum category category, char *token) {
    struct node *new = malloc(sizeof(struct node));
    new->category = category;
    new->token = token;
    new->type = no_type;
    new->children = malloc(sizeof(struct node_list));
    new->children->node = NULL;
    new->children->next = NULL;
    new->brothers = NULL;
    new->error =1;
    return new;
}

// append a node to the list of children of the parent node
void addchild(struct node *parent, struct node *child) {

    child->parent = parent;
    // Append the child node to the list of children
    struct node_list *new_node = malloc(sizeof(struct node_list));
    new_node->node = child;
    new_node->next = NULL;

    if (parent->children == NULL) {
        parent->children = new_node;
    } else {
        struct node_list *last_child = parent->children;
        while (last_child->next != NULL) {
            last_child = last_child->next;
        }
        last_child->next = new_node;
    }

    // Recursively append brothers to the list of children
    append_brothers_to_children(parent, child->brothers);
    
    // Set the brothers list of the child to NULL
    child->brothers = NULL;
}

// Recursively append a list of brothers to the list of children
void append_brothers_to_children(struct node *parent, struct node_list *brothers) {
    while (brothers != NULL) {
        if (brothers->node != NULL) {
            addchild(parent, brothers->node);
        }
        brothers = brothers->next;
    }
}

// get a pointer to a specific child, numbered 0, 1, 2, ...
struct node *getchild(struct node *parent, int position) {
    if(parent!=NULL){
        struct node_list *children = parent->children;
        while((children = children->next) != NULL)
            if(position-- == 0)
                return children->node;
        return NULL;
    }
    else{
        return NULL;
    }

}

// count the children of a node
int countchildren(struct node *node) {
    int i = 0;
    while(getchild(node, i) != NULL)
        i++;
    return i;
}

// category names #defined in ast.h
char *category_name[] = names;

// traverse the AST and print its content
void show(struct node *node, int depth) {
    if (node == NULL) return; // if tree is empty
    
    for (int i= 0; i < depth; i++)
        printf("..");
    if (node->token == NULL)
        printf("%s\n", category_name[node->category]);
    else
        printf("%s(%s)\n", category_name[node->category], node->token);
    
    // Iterate over the children of the current node
    struct node_list *child = node->children;
    while (child != NULL) {
        show(child->node, depth + 1);
        child = child->next;
    }

    // Iterate over the brothers of the current node
    struct node_list *brother = node->brothers;
    while (brother != NULL) {
        show(brother->node, depth);
        brother = brother->next;
    }
}

// free the AST
void deallocate(struct node *node) {
    if(node != NULL) {
        struct node_list *child = node->children;
        while(child != NULL) {
            deallocate(child->node);
            child = child->next;
        }
        if(node->token != NULL)
            free(node->token);
    }
}

void addbrother(struct node *node1, struct node *new_brother) {
    if (node1 == NULL) {
        fprintf(stderr, "Error: node1 is NULL.\n");
        return;
    }

    // Cria e verifica a alocação de memória para o novo nó na lista de irmãos
    struct node_list *new_node_list = (struct node_list *) malloc(sizeof(struct node_list));
    if (new_node_list == NULL) {
        fprintf(stderr, "Failed to allocate memory for a new node list.\n");
        return;
    }

    new_node_list->node = new_brother;
    new_node_list->next = NULL;

    // Se não houver irmãos ainda, define o novo nó como o primeiro irmão
    if (node1->brothers == NULL) {
        node1->brothers = new_node_list;
    } else {
        // Encontra o último nó da lista de irmãos de forma iterativa
        struct node_list *current = node1->brothers;
        while (current->next != NULL) {
            current = current->next;
        }
        // Adiciona o novo nó ao final da lista
        current->next = new_node_list;
    }

    // Atribui NULL ao ponteiro new_node_list após a alocação
    new_node_list = NULL;
    free(new_node_list);
}
int countbrother(struct node *node) {
    int count = 0;

    // Verificar se o nó é nulo
    if (node == NULL || node->brothers == NULL) {
        return 0;
    }

    // Iterar sobre os irmãos do nó
    struct node_list *current = node->brothers;
    while (current != NULL) {
        if (current->node != NULL) {
            count++;
        }
        current = current->next; // Avançar para o próximo item na lista de irmãos
    }

    return count;
}
