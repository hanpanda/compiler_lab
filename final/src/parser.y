%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "../include/ast.h"
#include "../include/symbol_table.h"
#include "../include/quat_table.h"
#include "../include/asm_generator.h"
using namespace std;


SymbolTable symbolTable;
QuatTable quatTable;
AsmGenerator asmGenerator;
ASTnode* root;

FILE* outfile;
extern FILE *yyin, *yyout;
extern char* yytext;
extern "C" int yylex(void);
extern "C" int yywrap();
int yyerror(char *s);
%}

%union {
    ASTnode* node;
}
%start program
/* %start expr */
/* %start statement */
/* %start declaration_list */
%type <node> program
%type <node> primary_expr unary_expr mul_expr add_expr equality_expr relational_expr and_expr or_expr assign_expr expr
%type <node> declaration_list declaration declarator_list type_specifier
%type <node> statement compound_statement expression_statement select_statement iteration_statement jump_statement block_item_list block_item 
%token <node> IDENTIFIER CONSTANT_FLOAT CONSTANT_INT CONSTANT_CHAR 
%token <node> IF ELSE WHILE BREAK CONTINUE 
%token <node> CHAR VOID FLOAT DOUBLE INT
%token <node> ADD MINUS MUL DIV MOD AND OR NOT ASSIGN LT GT EQU NEQ
%token <node> LBRACKET RBRACKET LBRACE RBRACE SEMI COMM 

%%  
/* beginning of rules section */
expr : assign_expr
        {
            $$ = $1;
        } 
    ;
assign_expr :   or_expr
        {
            $$ = $1;
        }
    |   unary_expr ASSIGN assign_expr
        {
            $$ = new ASTnode(NodeType::expr);
            $$->setExprType(ExprType::assignExpr);            
            $$->addChild($1);
            $$->addChild($3);
        }
    ;
or_expr :   and_expr
        {
            $$ = $1;
        }
    |   or_expr OR and_expr 
        {
            $$ = new ASTnode(NodeType::expr);
            $$->setExprType(ExprType::orExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    ;
and_expr :  equality_expr
        {
            $$ = $1;
        }
    |   and_expr AND equality_expr
        {
            $$ = new ASTnode(NodeType::expr);
            $$->setExprType(ExprType::andExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    ;

equality_expr : relational_expr 
        {
            $$ = $1;
        }
    |   equality_expr EQU relational_expr
        {
            $$ = new ASTnode(NodeType::expr);
            $$->setExprType(ExprType::equExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    |   equality_expr NEQ relational_expr
        {
            $$ = new ASTnode(NodeType::expr);
            $$->setExprType(ExprType::neqExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    ;

relational_expr :   add_expr
        {
            $$ = $1;
        }
    |   relational_expr LT add_expr
        {
            $$ = new ASTnode(NodeType::expr);
            $$->setExprType(ExprType::ltExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    |   relational_expr GT add_expr
        {
            $$ = new ASTnode(NodeType::expr);
            $$->setExprType(ExprType::gtExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    ;

add_expr :  mul_expr
        {
            $$ = $1;
        }
    |   add_expr ADD mul_expr
        {
            $$ = new ASTnode(NodeType::expr);
            $$->setExprType(ExprType::addExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    |   add_expr MINUS mul_expr
        {
            $$ = new ASTnode(NodeType::expr);
            $$->setExprType(ExprType::minusExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    ;
mul_expr :  unary_expr
        {
            $$ = $1;
        }
    |   mul_expr MUL unary_expr
        {
            $$ = new ASTnode(NodeType::expr);
            $$->setExprType(ExprType::mulExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    |   mul_expr DIV unary_expr
        {
            $$ = new ASTnode(NodeType::expr);
            $$->setExprType(ExprType::divExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    |   mul_expr MOD unary_expr
        {
            $$ = new ASTnode(NodeType::expr);
            $$->setExprType(ExprType::modExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    ;
unary_expr : primary_expr
    {
        $$ = $1;
    }
    |   ADD primary_expr
    {
        $$ = new ASTnode(NodeType::expr);
        $$->setExprType(ExprType::addExpr);
        $$->addChild($2);
    }
    |   MINUS primary_expr
    {
        $$ = new ASTnode(NodeType::expr);
        $$->setExprType(ExprType::minusExpr);
        $$->addChild($2);
    }
    |   NOT primary_expr
    {
        $$ = new ASTnode(NodeType::expr);
        $$->setExprType(ExprType::notExpr);
        $$->addChild($2);
    }
    ;
primary_expr : IDENTIFIER
        {
            $$ = new ASTnode(NodeType::token);
            $$->setTokenType(TokenType::identifier);
            $$->setTokenVal(yytext);

            $$->symbolTableIdx = symbolTable.getItemIdx(yytext);
            if($$->symbolTableIdx == -1)
            {
                printf("Undefined Variable!\n");
            }
        }
    | CONSTANT_FLOAT
        {
            float constant = strtof(yytext, NULL);
            $$ = new ASTnode(NodeType::token);
            $$->setTokenType(TokenType::constant);
            $$->setTokenVal(constant);

            $$->symbolTableIdx = symbolTable.size();
            symbolTable.addItemConstantFloat(constant);
        }
    | CONSTANT_INT
        {
            int constant = atoi(yytext);
            $$ = new ASTnode(NodeType::token);
            $$->setTokenType(TokenType::constant);
            $$->setTokenVal(constant);

            $$->symbolTableIdx = symbolTable.size();
            symbolTable.addItemConstantInt(constant);   
        }
    | CONSTANT_CHAR
        {
            char constant = yytext[0];
            $$ = new ASTnode(NodeType::token);
            $$->setTokenType(TokenType::constant);
            $$->setTokenVal(constant);

            $$->symbolTableIdx = symbolTable.size();
            symbolTable.addItemConstantChar(constant);
        }
    | LBRACKET expr RBRACKET
        {
            $$ = $2;
        }
    ;

declaration_list : declaration
    | declaration_list declaration
    ;

declaration : type_specifier declarator_list SEMI
        {
            for(auto child: $2->children)
            {
                child->symbolTableIdx = symbolTable.size();
                symbolTable.addItemVariable(child->identifier, $1->typeSpec);
            }
        }
    ;

type_specifier : INT
        {
            $$ = new ASTnode();
            $$->setTokenVal(VarType::INT);
        }
    | CHAR
        {
            $$ = new ASTnode();
            $$->setTokenVal(VarType::CHAR);
        }
    | FLOAT
        {
            $$ = new ASTnode();
            $$->setTokenVal(VarType::FLOAT);
        }
    ;

declarator_list : IDENTIFIER
        {
            $1 = new ASTnode();
            $1->setTokenType(TokenType::identifier);
            $1->setTokenVal(yytext);

            $$ = new ASTnode();
            $$->addChild($1);
        }
    | declarator_list COMM IDENTIFIER
        {
            $3 = new ASTnode();
            $3->setTokenType(TokenType::identifier);
            $3->setTokenVal(yytext);

            $1->addChild($3);
            $$ = $1;
        }
    ;

statement : compound_statement
        {
            $$ = $1;
        }
    |   expression_statement
        {
            $$ = $1;
        }
    |   select_statement
        {
            $$ = $1;
        }
    |   iteration_statement
        {
            $$ = $1;
        }
    |   jump_statement
        {
            $$ = $1;
        }
    ;

compound_statement : LBRACE block_item_list RBRACE
        {
            $$ = $2;
        }

block_item_list : block_item
        {
            $$ = new ASTnode(NodeType::stmt);
            $$->setStmtType(StmtType::compoundStmt);
            $$->addChild($1);
        }
    |   block_item_list block_item
        {
            $$ = $1;
            $$->addChild($2);
        }
    ;

block_item : declaration_list statement
        {
            $$ = $2;
        }
    |   statement
        {
            $$ = $1;
        }
    ;

expression_statement : expr SEMI
        {
            $$ = new ASTnode(NodeType::stmt);
            $$->setStmtType(StmtType::exprStmt);
            $$->addChild($1);
        }
    ;

select_statement : IF LBRACKET expr RBRACKET statement
        {
            $$ = new ASTnode(NodeType::stmt);
            $$->setStmtType(StmtType::ifStmt);
            $$->addChild($3);
            $$->addChild($5);
        }
    |   IF LBRACKET expr RBRACKET statement ELSE statement
        {
            $$ = new ASTnode(NodeType::stmt);
            $$->setStmtType(StmtType::ifStmt);
            $$->addChild($3);
            $$->addChild($5);
            $$->addChild($7);
        }
    ;

iteration_statement : WHILE LBRACKET expr RBRACKET statement
        {
            $$ = new ASTnode(NodeType::stmt);
            $$->setStmtType(StmtType::whileStmt);
            $$->addChild($3);
            $$->addChild($5);
        }
    ;

jump_statement : CONTINUE SEMI
        {
            $$ = new ASTnode(NodeType::stmt);
            $$->setStmtType(StmtType::continueStmt);
        }
    |   BREAK SEMI
        {
            $$ = new ASTnode(NodeType::stmt);
            $$->setStmtType(StmtType::breakStmt);
        }
    ;

program : declaration_list statement 
        {
            $$ = $2;
            root = $$;
        }
    ;

%%
/* beginning of program section */

int yyerror(char *s) 
{ 
    fprintf(stderr, "%s\n", s); 
    return 0; 
}

int yywrap()
{
    return 1;
}

int main(int argc, char* argv[]) 
{
    if (argc > 2) 
    {
        printf("ERROR, arguement too much! You should only input a code file name!\n");
    } 
    else if (argc == 2) 
    {
        yyin = fopen(argv[1], "rb+");
    }
    int len = strlen(argv[1]);
    char filename[len + 1] = {0};
    
    memcpy(filename, argv[1], len * sizeof(char));
    memcpy(&filename[len - 3], "tok", 3 * sizeof(char));

    outfile = fopen(filename, "w+");
    yyparse();
    vector<int> a, b;
    visitStmt(root, a, b);

    memcpy(&filename[len - 3], "asm", 3 * sizeof(char));
    asmGenerator.generate(string(filename));

    symbolTable.print();
    
    memcpy(&filename[len - 3], "qua", 3 * sizeof(char));
    quatTable.print(string(filename));

    return 0;
}