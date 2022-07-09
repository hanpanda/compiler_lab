#include "../include/ast.h"
#include "../include/quat_table.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


extern QuatTable quatTable;
extern SymbolTable symbolTable;

ASTnode::ASTnode(NodeType nodeKind, vector<ASTnode *> children)
{
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

void visitStmt(ASTnode *root, 
    vector<int>& continueStmtIndices, vector<int>& breakStmtIndices)
{
    int varIdx, idx1, idx2;
    vector<int> tmpBreakStmtIndices, tmpContinueStmtIndices;
    switch (root->stmtType)
    {
    case StmtType::compoundStmt:
        for (auto child : root->children)
        {
            visitStmt(child, continueStmtIndices, breakStmtIndices);
        }
        break;

    case StmtType::whileStmt:
        idx1 = quatTable.size();
        varIdx = visitExpr(root->children[0]);  // while条件

        idx2 = quatTable.size();
        quatTable.addQuat(Quat());      // 占位
        
        if(root->children.size() > 0)
            visitStmt(root->children[1], tmpContinueStmtIndices, tmpBreakStmtIndices);   // 循环体

        quatTable.setQuat(idx2,
                          Quat(QuatOpType::jzOp, varIdx, -1, quatTable.size() + 1));
        quatTable.addQuat(Quat(QuatOpType::gotoOp, -1, -1, idx1));

        for(auto i: tmpBreakStmtIndices)
        {
            quatTable.setQuat(i, Quat(QuatOpType::gotoOp, -1, -1, quatTable.size()));
        }

        for(auto i: tmpContinueStmtIndices)
        {
            quatTable.setQuat(i, Quat(QuatOpType::gotoOp, -1, -1, idx1));
        }
        break;

    case StmtType::ifStmt:
        varIdx = visitExpr(root->children[0]); // expr
        idx1 = quatTable.size();
        quatTable.addQuat(Quat());
        visitStmt(root->children[1], continueStmtIndices, breakStmtIndices); // true part
        quatTable.addQuat(Quat());

        idx2 = quatTable.size();
        if (root->children.size() > 2) // false part
            visitStmt(root->children[2], continueStmtIndices, breakStmtIndices);

        quatTable.setQuat(idx1,
                          Quat(QuatOpType::jzOp, varIdx, -1, idx2));
        quatTable.setQuat(idx2 - 1,
                          Quat(QuatOpType::gotoOp, -1, -1, quatTable.size()));
        break;

    case StmtType::exprStmt:
        visitExpr(root->children[0]);
        break;

    case StmtType::continueStmt:
        continueStmtIndices.push_back(quatTable.size());
        quatTable.addQuat(Quat());
        break;

    case StmtType::breakStmt:
        breakStmtIndices.push_back(quatTable.size());
        quatTable.addQuat(Quat());
        break;

    default:
        break;
    }
}

int visitExpr(ASTnode *root)
{
    if(root->nodetype == NodeType::token)
    {
        return root->symbolTableIdx;
    }

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
        break;

    case ExprType::equExpr:
        op = QuatOpType::equOp;
        break;

    case ExprType::neqExpr:
        op = QuatOpType::neqOp;
        break;

    case ExprType::ltExpr:
        op = QuatOpType::ltOp;
        break;

    case ExprType::gtExpr:
        op = QuatOpType::gtOp;
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