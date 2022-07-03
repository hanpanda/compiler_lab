#include "../include/ast.h"
#include "../include/quat_table.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


extern QuatTable quatTable;
extern SymbolTable symbolTable;

ASTnode::ASTnode(NodeType nodeKind, int nodeId, vector<ASTnode *> children)
{
    this->nodeId = nodeId;
    this->nodetype = nodeKind;
    this->children = children;
}

void ASTnode::addChild(ASTnode *child)
{
    this->children.push_back(child);
}

void ASTnode::setTokenVal(char *identifier)
{
    memset(this->identifier, 0, MAX_IDENT_LEN);
    memcpy(this->identifier, identifier, strlen(identifier));
}

void ASTnode::setTokenVal(double constant)
{
    this->constant = constant;
}

void ASTnode::setTokenType(int tokenType)
{
    this->tokenType = tokenType;
}

void ASTnode::setTokenVal(VarType typeSpec)
{
    this->typeSpec = typeSpec;
}

void ASTnode::setStmtType(int stmtType)
{
    this->stmtType = stmtType;
}

void ASTnode::setExprType(int exprType)
{
    this->exprType = exprType;
}

void visitStmt(ASTnode *root)
{
    int varIdx, entryIdx, falsePartIdx;
    switch (root->stmtType)
    {
    case StmtType::compoundStmt:
        for (auto child : root->children)
        {
            visitStmt(child);
        }
        break;

    case StmtType::whileStmt:
        entryIdx = quatTable.size();
        varIdx = visitExpr(root->children[0]);
        quatTable.addQuat(Quat());
        if(root->children.size() > 0)
            visitStmt(root->children[1]);
        quatTable.setQuat(entryIdx + 1,
                          Quat(QuatOpType::jzOp, varIdx, -1, quatTable.size() + 1));
        quatTable.addQuat(Quat(QuatOpType::gotoOp, -1, -1, entryIdx));
        break;

    case StmtType::ifStmt:
        varIdx = visitExpr(root->children[0]); // expr
        entryIdx = quatTable.size();
        quatTable.addQuat(Quat());
        visitStmt(root->children[1]); // true part
        quatTable.addQuat(Quat());

        falsePartIdx = quatTable.size();
        if (root->children.size() > 2) // false part
            visitStmt(root->children[2]);

        quatTable.setQuat(entryIdx,
                          Quat(QuatOpType::jzOp, varIdx, -1, falsePartIdx));
        quatTable.setQuat(falsePartIdx - 1,
                          Quat(QuatOpType::gotoOp, -1, -1, quatTable.size()));
        break;

    case StmtType::exprStmt:
        visitExpr(root->children[0]);
        break;

    default:
        break;
    }
}

int visitExpr(ASTnode *root)
{
    vector<int> ids;
    for (auto child : root->children)
    {
        if (child->nodetype == NodeType::expr)
        {
            ids.push_back(visitExpr(child));
        }
        else if (child->nodetype == NodeType::token)
        {
            ids.push_back(child->symbolTableIdx);
        }
        else
        {
            printf("error in visitExpr!\n");
        }
    }

    QuatOpType op;
    switch (root->exprType)
    {
    case ExprType::orExpr:
        op = QuatOpType::orOp;
        break;

    case ExprType::andExpr:
        op = QuatOpType::andOp;
        break;

    case ExprType::notExpr:
        op = QuatOpType::notOp;
        break;

    case ExprType::addExpr:
        op = QuatOpType::addOp;
        break;

    case ExprType::minusExpr:
        op = QuatOpType::minusOp;
        break;

    case ExprType::mulExpr:
        op = QuatOpType::mulOp;
        break;

    case ExprType::divExpr:
        op = QuatOpType::divOp;
        break;

    case ExprType::modExpr:
        op = QuatOpType::modOp;
        break;

    case ExprType::assignExpr:
        op = QuatOpType::assignOp;
        printf("assign %d, %d\n", ids[0], ids[1]);
        break;

    default:
        break;
    }

    int tempVarIdx = symbolTable.size();
    symbolTable.addItemTemp();
    if (ids.size() == 2)
        quatTable.addQuat(Quat(op, ids[0], ids[1], tempVarIdx));
    else
        quatTable.addQuat(Quat(op, ids[0], -1, tempVarIdx));
    return tempVarIdx;
}