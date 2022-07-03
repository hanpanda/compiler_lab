SRC_PATH = ./src/
LEX_FILE = $(wildcard $(SRC_PATH)*.l)
YACC_FILE = $(wildcard $(SRC_PATH)*.y)
INPUT_FILE = ./test/test.txt
APP = complier

$(APP): 
	flex  -o $(SRC_PATH)lexer.c $(LEX_FILE) 
	bison $(YACC_FILE) -o $(SRC_PATH)parser.cpp
	g++ $(SRC_PATH)*.cpp $(SRC_PATH)*.c -o $@

run:
	./$(APP) $(INPUT_FILE)

clean:
	rm $(APP) $(SRC_PATH)lexer.c $(SRC_PATH)parser.cpp