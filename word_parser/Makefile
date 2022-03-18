LEX_FILE = clang.l
INPUT_FILE = demo.c

build:
	lex $(LEX_FILE)
	gcc lex.yy.c -o lex.yy
run:
	./lex.yy $(INPUT_FILE)