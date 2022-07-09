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

void QuatTable::print(string filename)
{
    FILE* fptr = fopen(filename.c_str(), "w");
    printf("\n----------Quat table----------\n");
    fprintf(fptr, "\n----------Quat table----------\n");
    for(int i = 0; i < table.size(); i++)
    {
        printf("%d: (", i);
        fprintf(fptr, "%d: (", i);
        switch (table[i].optype)
        {
        case QuatOpType::addOp:
            printf("+, ");
            fprintf(fptr, "+, ");
            break;
        case QuatOpType::minusOp:
            printf("-, ");
            fprintf(fptr, "-, ");
            break;
        case QuatOpType::mulOp:
            printf("*, ");
            fprintf(fptr, "*, ");
            break;
        case QuatOpType::divOp:
            printf("/, ");
            fprintf(fptr, "/, ");
            break;
        case QuatOpType::modOp:
            printf("%%, ");
            fprintf(fptr, "%%, ");
            break;
        case QuatOpType::andOp:
            printf("&&, ");
            fprintf(fptr, "&&, ");
            break;
        case QuatOpType::orOp:
            printf("||, ");
            fprintf(fptr, "||, ");
            break;
        case QuatOpType::notOp:
            printf("!, ");
            fprintf(fptr, "!, ");
            break;
        case QuatOpType::assignOp:
            printf("=, ");
            fprintf(fptr, "=, ");
            break;
        case QuatOpType::jzOp:
            printf("jz, ");
            fprintf(fptr, "jz, ");
            break;
        case QuatOpType::gotoOp:
            printf("goto, ");
            fprintf(fptr, "goto, ");
            break;
        case QuatOpType::equOp:
            printf("==, ");
            fprintf(fptr, "==, ");
            break;
        case QuatOpType::neqOp:
            printf("!=, ");
            fprintf(fptr, "!=, ");
            break;
        case QuatOpType::ltOp:
            printf("<, ");
            fprintf(fptr, "<, ");
            break;
        case QuatOpType::gtOp:
            printf(">, ");
            fprintf(fptr, ">, ");
            break;  
        case QuatOpType::noneOp:
            printf("none, ");
            fprintf(fptr, "none, ");
            break;
        }
        printf("%d, %d, %d)\n", table[i].operandIdx1, table[i].operandIdx2, table[i].resultIdx);
        fprintf(fptr, "%d, %d, %d)\n", table[i].operandIdx1, table[i].operandIdx2, table[i].resultIdx);
    }
}