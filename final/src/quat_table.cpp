#include "../include/quat_table.h"
#include <cstdio>


void QuatTable::addQuat(Quat q)
{
    table.push_back(q);
}

int QuatTable::size()
{
    return table.size();
}

void QuatTable::setQuat(int idx, Quat q)
{
    table[idx] = q;
}

void QuatTable::print()
{
    printf("\n----------Quat table----------\n");
    for(int i = 0; i < table.size(); i++)
    {
        printf("%d: (", i);
        switch (table[i].optype)
        {
        case QuatOpType::addOp:
            printf("+, ");
            break;
        case QuatOpType::minusOp:
            printf("-, ");
            break;
        case QuatOpType::mulOp:
            printf("*, ");
            break;
        case QuatOpType::divOp:
            printf("/, ");
            break;
        case QuatOpType::modOp:
            printf("%%, ");
            break;
        case QuatOpType::andOp:
            printf("&&, ");
            break;
        case QuatOpType::orOp:
            printf("||, ");
            break;
        case QuatOpType::notOp:
            printf("!, ");
            break;
        case QuatOpType::assignOp:
            printf("=, ");
            break;
        case QuatOpType::jzOp:
            printf("jz, ");
            break;
        case QuatOpType::gotoOp:
            printf("goto, ");
            break;
        case QuatOpType::equOp:
            printf("==, ");
            break;
        case QuatOpType::neqOp:
            printf("!=, ");
            break;
        case QuatOpType::ltOp:
            printf("<, ");
            break;
        case QuatOpType::gtOp:
            printf(">, ");
            break;  
        case QuatOpType::noneOp:
            printf("none, ");
            break;
        }
        printf("%d, %d, %d)\n", table[i].operandIdx1, table[i].operandIdx2, table[i].resultIdx);
    }
}