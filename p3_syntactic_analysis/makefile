# Makefile for building the lexer

# Define the lexer source file
LEXER_SOURCE = uccompiler.l

# Define the name of the output executable
OUTPUT_EXECUTABLE = lexer

# Default target (build the lexer)
all: $(OUTPUT_EXECUTABLE)

# Target to generate the lexer C code using lex
lex.yy.c: $(LEXER_SOURCE)
	lex $(LEXER_SOURCE)

# Target to build the lexer using GCC
$(OUTPUT_EXECUTABLE): lex.yy.c
	gcc -o $(OUTPUT_EXECUTABLE) lex.yy.c -ll

# Clean up intermediate and output files
clean:
	rm -f lex.yy.c $(OUTPUT_EXECUTABLE)
