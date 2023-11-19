# Define the lexer source file
LEXER_SOURCE = uccompiler.l
YACC_SOURCE = uccompiler.y

# Define the name of the output executables
OUTPUT_EXECUTABLE = uccompiler

# Default target (build the compiler)
all: $(OUTPUT_EXECUTABLE)

# Target to generate the lexer C code using lex
lex.yy.c: $(LEXER_SOURCE)
	lex $(LEXER_SOURCE)

# Target to generate the parser C code using yacc
y.tab.c y.tab.h y.output: $(YACC_SOURCE)
	yacc -dv -Wconflicts-rr -Wconflicts-sr -Wcounterexamples -Wother $(YACC_SOURCE)

# Target to build the lexer using GCC
$(OUTPUT_EXECUTABLE): lex.yy.c y.tab.c y.tab.h
	gcc -o $(OUTPUT_EXECUTABLE) y.tab.c lex.yy.c ast.c -Wall -Wno-unused-function

# Clean up intermediate and output files
clean:
	rm -f $(OUTPUT_EXECUTABLE) y.tab.c y.tab.h lex.yy.c y.output

# Clean *.res files
cleanres:
	rm -f ../mooshak_testing/meta1/*.res
	rm -f ../mooshak_testing/meta2/*.res
	rm -f ../mooshak_testing/meta3/*.res
	rm -f ../mooshak_testing/meta4/*.res