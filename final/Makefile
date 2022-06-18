LEX_FILE = test.l
YACC_FILE = test.y
INPUT_FILE = test.txt

build:
	flex  $(LEX_FILE) 
	bison $(YACC_FILE) -o test.tab.cpp
	g++ test.tab.cpp lex.yy.c ast.cpp symbol_table.cpp -o parser

run:
	./parser $(INPUT_FILE)

clean:
	rm test.tab.cpp test.tab.c lex.yy.c lex.yy.cpp parser