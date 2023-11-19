compile: uccompiler.l uccompiler.y functions.c
	lex uccompiler.l
	yacc -d -v uccompiler.y
	gcc -Wall -Wextra -Wno-unused-function lex.yy.c y.tab.c functions.c -o uccompiler 

debug: uccompiler.l uccompiler.y functions.c
	lex uccompiler.l
	yacc -d uccompiler.y
	gcc -Wall -g lex.yy.c y.tab.c functions.c -o uccompiler