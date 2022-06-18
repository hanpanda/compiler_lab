#include "ast.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <queue>
#include <stack>


struct Quat
{
    int optype;
    int operandId1;
    int operandId2;
    int resultId;
    Quat(int optype, int operandId1, int operandId2, int resultId) 
    {
        this->optype = optype;
        this->operandId1 = operandId1;
        this->operandId2 = operandId2;
        this->resultId = resultId;
    }
};

void bfs(ASTnode *root)
{
    printf("bfs:\n");

    stack<string> quatStack;
    queue<ASTnode *> q;
    ASTnode *cur;
    q.push(root);

    while (!q.empty())
    {
        cur = q.front();
        q.pop();
        int nChild = cur->children.size();
        // printf("cur id: %d, nodetype: %d\n", cur->nodeId, cur->nodetype);

        if (cur->nodetype == NodeType::expr)
        {
            string quat = "";       

            switch (cur->exprType)
            {
            case ExprType::mulExpr:
                quat += "*,";
                break;
            
            case ExprType::divExpr:
                quat += "/,";
                break;

            case ExprType::modExpr:
                quat += "%,";
                break;

            case ExprType::addExpr:
                quat += "+,";
                break;

            case ExprType::minusExpr:
                quat += "-,";
                break;

            case ExprType::notExpr:
                quat += "!,";
                break;

            case ExprType::andExpr:
                quat += "&&";
                break;

            case ExprType::orExpr:
                quat += "||";
                break;

            case ExprType::assignExpr:
                quat += "=";
                break;

            default:
                break;
            }

            for(int i = 0; i < nChild; i++)
            {
                quat += to_string(cur->children[i]->nodeId);
                quat += ",";
            }
            if(nChild < 2) 
            {
                quat += "_,";
            }
            quat += to_string(cur->nodeId); 
            quatStack.push(quat);
        } 
        else if(cur->nodetype == NodeType::token)
        {
            switch (cur->tokenType)
            {
            case TokenType::constant:
                printf("node id: %d, value: %f\n", cur->nodeId, cur->constant);
                break;
            
            case TokenType::identifier:
                printf("node id: %d, value: %s\n", cur->nodeId, cur->identifier);
                break;

            default:
                break;
            }
        }
        // push children into q
        for(int i = nChild - 1; i >= 0; i--)
        {
            q.push(cur->children[i]);
        }
    }

    printf("\nprint quat:\n");
    while(!quatStack.empty())
    {
        printf("%s\n", quatStack.top().c_str());
        quatStack.pop();
    }
}

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

void ASTnode::setTokenVal(char* identifier)
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