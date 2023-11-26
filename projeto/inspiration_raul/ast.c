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
    return new;
}

// append a node to the list of children of the parent node
void addchild(struct node *parent, struct node *child) {
    struct node_list *new = malloc(sizeof(struct node_list));
    new->node = child;
    new->next = NULL;
    struct node_list *children = parent->children;
    while(children->next != NULL)
        children = children->next;
    children->next = new;
    
    // Adiciona os irmãos do nó filho à lista de filhos do nó pai
struct node_list *current_brother = child->brothers;
struct node_list *last_child = parent->children;

// Encontra o último filho da lista atual de filhos do pai
while (last_child != NULL && last_child->next != NULL) {
    last_child = last_child->next;
}

// Adiciona os irmãos como filhos ao nó pai
while (current_brother != NULL) {
    struct node_list *new_brother = malloc(sizeof(struct node_list));
    new_brother->node = current_brother->node;
    new_brother->next = NULL;

    // Adiciona o novo irmão ao final da lista de filhos do pai
    if (last_child == NULL) {
        parent->children = new_brother;
    } else {
        last_child->next = new_brother;
    }
    last_child = new_brother;

    // Move para o próximo irmão
    struct node_list *temp = current_brother;
    current_brother = current_brother->next;

    // Libera o irmão da lista de irmãos do nó filho
    free(temp);
}

// Atualiza a lista de irmãos do nó filho para NULL
child->brothers = NULL;
} 


// get a pointer to a specific child, numbered 0, 1, 2, ...
struct node *getchild(struct node *parent, int position) {
    struct node_list *children = parent->children;
    while((children = children->next) != NULL)
        if(position-- == 0)
            return children->node;
    return NULL;
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
    int i;
    for (i = 0; i < depth; i++)
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
            struct node_list *tmp = child;
            child = child->next;
        }
        if(node->token != NULL)
            free(node->token);
    }
}


void addbrother(struct node *node1, struct node *new_brother) {
    // Aloca memória para o novo nó na lista de irmãos
    struct node_list *new_node_list = malloc(sizeof(struct node_list));

    // Verifica a alocação de memória
    if (new_node_list == NULL) {
        // Lidar com a falha na alocação (por exemplo, retornar ou sair)
        fprintf(stderr, "Failed to allocate memory for a new node list.\n");
        return;
    }

    new_node_list->node = new_brother;
    new_node_list->next = NULL;

    // Obtém uma referência à lista atual de irmãos do nó
    struct node_list *brothers = node1->brothers;

    // Se não houver irmãos ainda, define o novo nó como o primeiro irmão
    if (brothers == NULL) {
        node1->brothers = new_node_list;
    } else {
        // Caso contrário, percorre a lista para encontrar o último nó
        while (brothers->next != NULL) {
            brothers = brothers->next;
        }
        // Adiciona o novo nó ao final da lista
        brothers->next = new_node_list;
    }
}


int countbrothers(struct node *node) {
    int count = 0;

    if (node == NULL || node->brothers == NULL) {
        return 0;
    }

    struct node_list *current = node->brothers;
    while (current != NULL) {
        if (current->node != NULL) {
            count++;
        }
        current = current->next; 
    }

    return count;
}
