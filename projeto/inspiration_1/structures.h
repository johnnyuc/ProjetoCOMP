#ifndef STRUCTURES_H
#define STRUCTURES_H


// META 2
	typedef enum {
		// _type
		Char, Short, Int, Double, Void, Id,
		// _program
		is_program,
		// _declaration
		is_declaration, is_functionDeclaration, is_parameterDeclaration,
		// _list
		is_parameterList, is_functionBody, is_statementList,
		// _functionDefinition
		is_functionDefinition,
		// _if
		is_if,
		// _binaryOp
		is_or, is_and, is_eq, is_ne, is_lt, is_gt, is_le, is_ge, is_add, is_sub, is_mul, is_div, is_mod, is_store, is_comma, is_bitwiseand, is_bitwisexor, is_bitwiseor,
		is_while, is_call,
		// _unaryOp
		is_not, is_minus, is_plus, is_return,
		// _terminal
		is_chrLit, is_intLit, is_realLit,
		// _null
		is_null /* , 
		// _type
		Undef */
	} node;

	extern char* names[];

	typedef struct _s0 _program;					typedef struct _s9 _while;
	typedef struct _s1 _declaration;				typedef struct _s10 _return;
	typedef struct _s2 _functionDeclaration;		typedef struct _s11 _binaryOp;
	typedef struct _s3 _functionDefinition;			typedef struct _s12 _unaryOp;
	typedef struct _s4 _parameterList;				typedef struct _s13 _call;
	typedef struct _s5 _parameterDeclaration;		typedef struct _s14 _terminal;
	typedef struct _s6 _functionBody;				typedef struct _s15 _typeSpec;
	typedef struct _s7 _statementList;
	typedef struct _s8 _if;

	typedef enum {tipo_char = Char, tipo_short = Short, tipo_int = Int, tipo_double = Double, tipo_void = Void, tipo_undef, nulo} t_basico;

	typedef struct _s0{
		node type;
		void* next;
		t_basico anotation;
	} _program;

	typedef struct _s1{
		node type;
		void* next;
		_typeSpec* typespec;
		_terminal* id;
		void* expression;
		t_basico anotation;
	} _declaration;

	typedef struct _s2{
		node type;
		void* next;
		_typeSpec* typespec;
		_terminal* id;
		_parameterList* parameter_list;
		t_basico anotation;
	} _functionDeclaration;

	typedef struct _s3{
		node type;
		void* next;
		_typeSpec* typespec;
		_terminal* id;
		_parameterList* parameter_list;
		_functionBody* function_body;
		t_basico anotation;
	} _functionDefinition;

	typedef struct _s4{
		node type;
		_parameterDeclaration* first_parameter;
		t_basico anotation;
	} _parameterList;

	typedef struct _s5{
		node type;	
		_parameterDeclaration* next;
		_typeSpec* typespec;
		_terminal* id;
		t_basico anotation;
	} _parameterDeclaration;

	typedef struct _s6{
		node type;
		void* declaration_or_statement;
		t_basico anotation;
	} _functionBody;

	typedef struct _s7{
		node type;
		void* next;
		void* first_statement;
		t_basico anotation;
	} _statementList;

	typedef struct _s8{
		node type;
		void* next;
		void* condition; // expression
		void* if_branch; // statement
		void* else_branch; // statement
		t_basico anotation;
	} _if;

	typedef struct _s9{
		node type;
		void* next;
		void* condition; // expression
		void* branch; // statement;
		t_basico anotation;
	} _while;

	typedef struct _s10{
		node type;
		void* next;
		void* expression;
		t_basico anotation;
	} _return;

	typedef struct _s11{
		node type;
		void* next;
		void* left_expression;
		void* right_expression;
		t_basico anotation;
	} _binaryOp;

	typedef struct _s12{
		node type;
		void* next;
		void* expression;
		t_basico anotation;
	} _unaryOp;

	typedef struct _s13{
		node type;
		void* next;
		_terminal* id;
		void* first_parameter; // expression
		t_basico anotation;
	} _call;

	typedef struct _s14{
		node type;
		void* next;
		char* value;		
		t_basico anotation;
	} _terminal;

	typedef struct _s15{
		node type;
		t_basico anotation;
	} _typeSpec; // ou null





// META 3
	typedef enum {tipo_var, tipo_funcao} t_elemento;


	typedef struct _et {																			// Elemento da Tabela [GLOBAL]
		
		t_elemento tipo_elemento;																	// Tipo Elemento: var ou funcao

		char id[64];																				// ID
		t_basico tipo_basico;																		// Tipo do ID
		
		struct _etf * lista_param_func;																// Exclusivo Ambiente Func [Lista PARAM]
		struct _etf * lista_vars_func;																// Exclusivo Ambiente Func [Lista VAR-LOCAL]
		short flag_def;																				// Exclusivo Ambiente Func [Func Definida? Default: 0 == False] Modificar depois
		
		struct _et * next;																			// ->Proximo
		
	} elemento_tabela;



	typedef struct _etf {																			// Elemento do ambiente da Funcao (Param ou Vars Locais) [PARTICULAR FUNC]
		
		char id[64];																				// ID
		t_basico tipo_basico;																		// Tipo do ID
		
		struct _etf * next;																			// ->Proximo
		
	} elemento_funcao;


#endif

