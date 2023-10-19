# Makefile for building the lexer

# Define the lexer source file
LEXER_SOURCE = calc.l
YACC_SOURCE = calc.y

# Define the name of the output executable
OUTPUT_EXECUTABLE = calc

# Default target (build the lexer)
all: $(OUTPUT_EXECUTABLE)

# Target to generate the lexer C code using lex
lex.yy.c: $(LEXER_SOURCE)
	lex $(LEXER_SOURCE)

y.tab.c y.tab.h y.output: $(YACC_SOURCE)
	yacc -dv $(YACC_SOURCE)

# Target to build the lexer using GCC
$(OUTPUT_EXECUTABLE): lex.yy.c y.tab.c y.tab.h
	gcc -o $(OUTPUT_EXECUTABLE) y.tab.c lex.yy.c -ll

# Clean up intermediate and output files
clean:
	rm -f lex.yy.c y.tab.c y.tab.h y.output $(OUTPUT_EXECUTABLE)
