#ifndef _AST_H
#define _AST_H

enum category { Program, Declaration, FuncDeclaration, FuncDefinition, ParamList, FuncBody, ParamDeclaration, StatList, If, While, Return,
                Or, And, Eq, Ne, Lt, Gt, Le, Ge, Add, Sub, Mul, Div, Mod, Not, Minus, Plus, Store, Comma, Call, BitWiseAnd, BitWiseXor, BitWiseOr,
                Char, ChrLit, Identifier, Int, Short, Natural, Double, Decimal, Void, Null };
#define names { "Program", "Declaration", "FuncDeclaration", "FuncDefinition", "ParamList", "FuncBody", "ParamDeclaration", "StatList", "If", "While", "Return", "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Add", "Sub", "Mul", "Div", "Mod", "Not", "Minus", "Plus", "Store", "Comma", "Call", "BitWiseAnd", "BitWiseXor", "BitWiseOr", "Char", "ChrLit", "Identifier", "Int", "Short", "Natural", "Double", "Decimal", "Void", "Null" }

enum type {integer_type, double_type, void_type, char_type, short_type, undef_type,no_type};

#define type_name(type) \
    (type == integer_type ? "int" : \
     (type == double_type ? "double" : \
     (type == void_type ? "void" : \
     (type == char_type ? "char" : \
     (type == short_type ? "short" : \
     (type == undef_type ? "undef" : "none"))))))

#define category_type(category) \
    (category == Int ? integer_type : \
     (category == Double ? double_type : \
     (category == Void ? void_type : \
     (category == Char ? char_type : \
     (category == Short ? short_type : \
     (category == Natural ? integer_type : no_type))))))

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
void append_brothers_to_children(struct node *parent, struct node_list *brothers);
int count_elements1(struct node_list *head);
void addchild(struct node *parent, struct node *child);
void free_brothers_list(struct node_list *brothers);
void addbrother(struct node *node1, struct node *brother);
struct node *getchild(struct node *parent, int position);
int countchildren(struct node *node);
int countbrothers(struct node *node);
void show(struct node *root, int depth);
void deallocate(struct node *root);
int check_types(struct node *node);


#endif