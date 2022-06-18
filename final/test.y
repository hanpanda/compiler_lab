%{
#include <cstdio>
#include <cstdlib>
#include <string>
#include "ast.h"
#include "symbol_table.h"
using namespace std;


SymbolTable symbolTable;
ASTnode* root;
int nodeId = 0;
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
%type <node> primary_expr unary_expr mul_expr add_expr and_expr or_expr assign_expr expr
%type <node> declaration_list declaration declarator_list type_specifier
%type <node> statement compound_statement expression_statement select_statement iteration_statement jump_statement block_item_list block_item 
%token <node> IDENTIFIER CONSTANT 
%token <node> IF ELSE WHILE BREAK CONTINUE 
%token <node> CHAR VOID FLOAT DOUBLE INT
%token <node> ADD MINUS MUL DIV MOD AND OR NOT ASSIGN LBRACKET RBRACKET LBRACE RBRACE SEMI COMM 

%%  
/* beginning of rules section */
expr : assign_expr
        {
            $$ = $1;
            root = $$;
        } 
    ;
assign_expr :   or_expr
        {
            $$ = $1;
        }
    |   unary_expr ASSIGN assign_expr
        {
            $$ = new ASTnode(NodeType::expr, nodeId++);
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
            $$ = new ASTnode(NodeType::expr, nodeId++);
            $$->setExprType(ExprType::orExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    ;
and_expr :  add_expr
        {
            $$ = $1;
        }
    |   and_expr AND add_expr
        {
            $$ = new ASTnode(NodeType::expr, nodeId++);
            $$->setExprType(ExprType::andExpr);
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
            $$ = new ASTnode(NodeType::expr, nodeId++);
            $$->setExprType(ExprType::addExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    |   add_expr MINUS mul_expr
        {
            $$ = new ASTnode(NodeType::expr, nodeId++);
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
            $$ = new ASTnode(NodeType::expr, nodeId++);
            $$->setExprType(ExprType::mulExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    |   mul_expr DIV unary_expr
        {
            $$ = new ASTnode(NodeType::expr, nodeId++);
            $$->setExprType(ExprType::divExpr);
            $$->addChild($1);
            $$->addChild($3);
        }
    |   mul_expr MOD unary_expr
        {
            $$ = new ASTnode(NodeType::expr, nodeId++);
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
        $$ = new ASTnode(NodeType::expr, nodeId++);
        $$->setExprType(ExprType::addExpr);
        $$->addChild($2);
    }
    |   MINUS primary_expr
    {
        $$ = new ASTnode(NodeType::expr, nodeId++);
        $$->setExprType(ExprType::minusExpr);
        $$->addChild($2);
    }
    |   NOT primary_expr
    {
        $$ = new ASTnode(NodeType::expr, nodeId++);
        $$->setExprType(ExprType::notExpr);
        $$->addChild($2);
    }
    ;
primary_expr : IDENTIFIER
        {
            $$ = new ASTnode(NodeType::token, nodeId++);
            $$->setTokenType(TokenType::identifier);
            $$->setTokenVal(yytext);

            printf("IDENTIFER: %s\n", yytext);
        }
    | CONSTANT
        {
            double constant = strtod(yytext, NULL);
            $$ = new ASTnode(NodeType::token, nodeId++);
            $$->setTokenType(TokenType::constant);
            $$->setTokenVal(constant);

            printf("CONSTANT: %f\n", constant);
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
                symbolTable.addItem(child->identifier, $1->typeSpec);
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

compound_statement : block_item_list
        {
            $$ = $1;
        }

block_item_list : block_item
        {
            $$ = new ASTnode(NodeType::stmt);
            $$->setStmtType(StmtType::compoundStmt);
        }
    |   block_item_list block_item
        {
            $$ = $1;
            $$->addChild($2);
        }
    ;

block_item : LBRACE declaration_list statement RBRACE
        {
            $$ = $2;
        }
    ;

expression_statement : expr SEMI
        {
            $$ = $1;
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
            $$ = $1;
            // root = $$;
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
    outfile = fopen("./tokens.txt", "w+");
    yyparse();
    printf("rootId: %d, children: %d\n", root->nodeId, (int)root->children.size());
    bfs(root);
    symbolTable.print();
    return 0;
}