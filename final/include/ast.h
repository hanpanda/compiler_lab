#ifndef AST_H
#define AST_H
#include <string>
#include <vector>
#include "../include/symbol_table.h"
using namespace std;

#define MAX_IDENT_LEN 20

enum NodeType
{
    stmt,
    expr,
    token
};

enum StmtType
{
    ifStmt,
    whileStmt,
    breakStmt,
    continueStmt,
    exprStmt,
    compoundStmt,
};

enum ExprType
{
    addExpr,
    minusExpr,
    unaryAddExpr,
    unaryMinusExpr,
    mulExpr,
    divExpr,
    modExpr,
    andExpr,
    orExpr,
    notExpr,
    assignExpr,
    equExpr,
    neqExpr,
    ltExpr,
    gtExpr
};

enum TokenType
{
    constant,
    identifier
};

class ASTnode
{
public:
    int nodeId;
    NodeType nodetype;
    union
    {
        int tokenType;
        int stmtType;
        int exprType;
    };
    int symbolTableIdx;
    union
    {
        double constant;
        char identifier[MAX_IDENT_LEN];
        VarType typeSpec;
    };
    vector<ASTnode *> children;

    ASTnode(NodeType nodeKind = NodeType::token, int nodeId = -1, vector<ASTnode *> children = vector<ASTnode *>());
    void addChild(ASTnode *child);
    void setTokenType(int tokenType);
    void setStmtType(int stmtType);
    void setExprType(int exprType);
    void setTokenVal(char *identifier);
    void setTokenVal(double constant);
    void setTokenVal(VarType typeSpec);
};

void visitStmt(ASTnode* root);
int visitExpr(ASTnode* root);
#endif