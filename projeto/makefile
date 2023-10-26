# Define the lexer source file
LEXER_SOURCE = uccompiler.l
PARSER_SOURCE = uccompiler.y

# Define the name of the output executables
LEXER_EXECUTABLE = lexer
PARSER_EXECUTABLE = uccompiler

# Default target (build the compiler)
all: $(PARSER_EXECUTABLE) $(LEXER_EXECUTABLE)

# Target to generate the parser C code using yacc
y.tab.c: $(PARSER_SOURCE)
	yacc -dv $(PARSER_SOURCE)

# Target to generate the lexer C code using lex
lex.yy.c: $(LEXER_SOURCE)
	lex $(LEXER_SOURCE)

# Target to build the compiler using GCC
$(PARSER_EXECUTABLE): y.tab.c lex.yy.c
	cc y.tab.c lex.yy.c -o $(PARSER_EXECUTABLE)

# Target to build the lexer using GCC
$(LEXER_EXECUTABLE): lex.yy.c
	gcc -o $(LEXER_EXECUTABLE) lex.yy.c -ll

# Clean up intermediate and output files
clean:
	rm -f $(PARSER_EXECUTABLE) $(LEXER_EXECUTABLE) y.tab.c y.tab.h lex.yy.c y.output
