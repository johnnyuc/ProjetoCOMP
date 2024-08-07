#ifndef _AST_H
#define _AST_H

// some useful includes
#include <string.h> // strcmp();
#include <stdio.h> // printf();

// some useful macros
enum category {  Program,   Function,   Parameters,   Parameter,   Arguments,   Integer,   Double,   Identifier,   Natural,   Decimal,   Call,   If,   Add,   Sub,   Mul,   Div };
#define names { "Program", "Function", "Parameters", "Parameter", "Arguments", "Integer", "Double", "Identifier", "Natural", "Decimal", "Call", "If", "Add", "Sub", "Mul", "Div" }

enum type {integer_type, double_type, no_type};
#define type_name(type) (type == integer_type ? "integer" : (type == double_type ? "double" : "none"))
#define category_type(category) (category == Integer ? integer_type : (category == Double ? double_type : no_type))

// node structure
struct node {
    enum category category;
    char *token;
    int token_line, token_column;
    enum type type;
    struct node_list *children;
};

// node list structure
struct node_list {
    struct node *node;
    struct node_list *next;
};

// function prototypes
struct node *newnode(enum category category, char *token);
void addchild(struct node *parent, struct node *child);
struct node *getchild(struct node *parent, int position);
int countchildren(struct node *node);
void show(struct node *root, int depth);
void deallocate(struct node *root);

#endif
