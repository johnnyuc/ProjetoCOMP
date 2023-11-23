#ifndef _AST_H
#define _AST_H

enum category { Program, Declaration, FuncDeclaration, FuncDefinition, ParamList, FuncBody, ParamDeclaration, StatList, If, While, Return, Or, And, Eq, Ne, Lt, Gt, Le, Ge, Add, Sub, Mul, Div, Mod, Not, Minus, Plus, Store, Comma, Call, BitWiseAnd, BitWiseXor, BitWiseOr, Char, ChrLit, Identifier, Int, Short, Natural, Double, Decimal, Void, Null };
#define names { "Program", "Declaration", "FuncDeclaration", "FuncDefinition", "ParamList", "FuncBody", "ParamDeclaration", "StatList", "If", "While", "Return", "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Add", "Sub", "Mul", "Div", "Mod", "Not", "Minus", "Plus", "Store", "Comma", "Call", "BitWiseAnd", "BitWiseXor", "BitWiseOr", "Char", "ChrLit", "Identifier", "Int", "Short", "Natural", "Double", "Decimal", "Void", "Null" }

enum type { integer_type, short_type, double_type, char_type, void_type, no_type };
#define type_name(type) ( \
    (type == integer_type) ? "int" : \
    ((type == short_type) ? "short" : \
    ((type == double_type) ? "double" : \
    ((type == char_type) ? "char" : \
    ((type == void_type) ? "void" : \
    "none")))))

#define category_type(category) (\
    (category == Int) ? integer_type : \
    ((category == Short) ? short_type : \
    ((category == Double) ? double_type : \
    ((category == Char) ? char_type : \
    ((category == Void) ? void_type : \
    no_type)))))

struct node {
    enum category category;
    char *token;
    int token_line, token_column;
    enum type type;
    struct node_list *children;
    struct node_list *brothers;
};

struct node_list {
    struct node *node;
    struct node_list *next;
};

struct node *newnode(enum category category, char *token);
void addchild(struct node *parent, struct node *child);
void addbrother(struct node *node1, struct node *new_brother);
struct node *getchild(struct node *parent, int position);
int countchildren(struct node *node);
int countbrother(struct node *node);
void show(struct node *root, int depth);
void deallocate(struct node *root);

void JohnnyTree(char* parent, char* type, struct node *root);
#endif
