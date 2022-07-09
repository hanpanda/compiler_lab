#include "../include/asm_generator.h"
#include <cstdio>
#include <cstring>
#include <unordered_set>
using namespace std;


void AsmGenerator::generate(string filename)
{
    this->filename = filename;
    FILE* fptr = fopen(filename.c_str(), "w");
    
    fprintf(fptr, "%*sglobal main\n%*sextern printf\n", 
        4, "", 4, "");
    fclose(fptr);

    defineDataSeg(symbolTable);
    defineTextSeg(symbolTable, quatTable);

}

void AsmGenerator::defineDataSeg(SymbolTable& symbolTable)
{
    FILE* fptr = fopen(filename.c_str(), "a");
    fprintf(fptr, "\n%*ssection .data\n", 4, "");
    
    for(int i = 0; i < symbolTable.items.size(); i++)
    {
        string asmName;
        switch (symbolTable.items[i].itemType)
        {
        case ItemType::variable:
            asmName = symbolTable.items[i].varName;
        case ItemType::temp:
            if(symbolTable.items[i].itemType == ItemType::temp)
                asmName = "temp_" + to_string(i);
            
            fprintf(fptr, "%s ", asmName.c_str());
            if(symbolTable.items[i].varType == VarType::CHAR)
            {
                fprintf(fptr, "db 0\n");
            }
            else 
            {
                fprintf(fptr, "dd 0\n");
            }
            break;
        
        case ItemType::constant:
            fprintf(fptr, "const_%d dd ", i);
            switch (symbolTable.items[i].varType)
            {
            case VarType::INT:
                fprintf(fptr, "%d\n", symbolTable.items[i].constantInt);
                break;
            
            case VarType::FLOAT:
                fprintf(fptr, "%f\n", symbolTable.items[i].constantFloat);
                break;

            case VarType::CHAR:
                fprintf(fptr, "%c\n", symbolTable.items[i].constantChar);
                break;
            }
            asmName = "const_" + to_string(i);
            break;
        }
        asmName = "[" + asmName + "]";
        memset(symbolTable.items[i].addr, 0, MAX_IDENT_LEN);
        memcpy(symbolTable.items[i].addr, asmName.c_str(), asmName.size());
    }
    
    fclose(fptr);
}

void AsmGenerator::defineTextSeg(const SymbolTable& symbolTable, 
    const QuatTable& quatTable)
{
    FILE* fptr = fopen(filename.c_str(), "a");
    fprintf(fptr, "\n%*ssection .text\nmain:\n", 4, "");

    const SymbolTable::Item* item1, *item2, *item3;
    unordered_set<int> labels;

    for(int i = 0; i < quatTable.table.size(); i++)
    {
        fprintf(fptr, "L%d:\n", i);

        switch(quatTable.table[i].optype)
        {
        case QuatOpType::addOp:
            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            if(quatTable.table[i].operandIdx2 != -1)
                item2 = &symbolTable.items[quatTable.table[i].operandIdx2];
            else 
                item2 = nullptr;
            item3 = &symbolTable.items[quatTable.table[i].resultIdx];
            if(item2 != nullptr)
            {
                fprintf(fptr, "mov eax, %s\n"
                    "add eax, %s\n"
                    "mov %s, eax\n",
                    item1->addr, item2->addr, item3->addr);
            }
            else
            {
                fprintf(fptr, "mov eax, %s\n"
                    "mov %s, eax\n",
                    item1->addr, item3->addr);
            }
            break;

        case QuatOpType::minusOp:
            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            if(quatTable.table[i].operandIdx2 != -1)
                item2 = &symbolTable.items[quatTable.table[i].operandIdx2];
            else 
                item2 = nullptr;
            item3 = &symbolTable.items[quatTable.table[i].resultIdx];
            if(item2 != nullptr)
            {
                fprintf(fptr, "mov eax, %s\n"
                    "sub eax, %s\n"
                    "mov %s, eax\n",
                    item1->addr, item2->addr, item3->addr);
            }
            else
            {
                fprintf(fptr, "mov eax, 0\n"
                    "sub eax, %s\n"
                    "mov %s, eax\n",
                    item1->addr, item3->addr);
            }
            break;

        case QuatOpType::mulOp:
            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "mov eax, %s\n", item1->addr);
            item1 = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "imul dword %s\n", item1->addr);
            item1 = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, eax\n", item1->addr);
            break;
        
        case QuatOpType::divOp:
            fprintf(fptr, "mov edx, 0\n");
            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "mov eax, %s\n", item1->addr);
            item1 = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "idiv dword %s\n", item1->addr);
            item1 = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, eax\n", item1->addr);
            break;
        
        case QuatOpType::modOp:
            fprintf(fptr, "mov edx, 0\n");
            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "mov eax, %s\n", item1->addr);
            item1 = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "idiv dword %s\n", item1->addr);
            item1 = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, edx\n", item1->addr);
            break;
        
        case QuatOpType::andOp:
            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "mov eax, %s\n", item1->addr);
            item1 = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "and eax, dword %s\n", item1->addr);
            item1 = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, eax\n", item1->addr);
            break;
        
        case QuatOpType::orOp:
            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "mov eax, %s\n", item1->addr);
            item1 = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "or eax, dword %s\n", item1->addr);
            item1 = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, eax\n", item1->addr);
            break;

        case QuatOpType::notOp:
            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            item2 = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(
                fptr, 
                "mov eax, %s\n"
                "cmp eax, 0\n"
                "je LL%d\n"
                "mov dword %s, 0\n"
                "jmp LLL%d\n"
                "LL%d:\n"
                "mov dword %s, 1\n"
                "LLL%d:\n",
                item1->addr, i, item2->addr, i, i, item2->addr, i);
            break;

        case QuatOpType::jzOp:
            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "cmp dword %s, 0\n", item1->addr);
            fprintf(fptr, "je L%d\n", quatTable.table[i].resultIdx);
            labels.insert(quatTable.table[i].resultIdx);
            break;
        
        case QuatOpType::gotoOp:
            fprintf(fptr, "jmp L%d\n", quatTable.table[i].resultIdx);
            labels.insert(quatTable.table[i].resultIdx);
            break;
        
        case QuatOpType::assignOp:
            item1 = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "mov eax, dword %s\n", item1->addr);

            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            if(item1->varType == VarType::CHAR)
                fprintf(fptr, "mov %s, al\n", item1->addr);
            else
                fprintf(fptr, "mov %s, eax\n", item1->addr);

            item1 = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, eax\n", item1->addr);
            break;
        
        case QuatOpType::equOp:
            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            item2 = &symbolTable.items[quatTable.table[i].operandIdx2];
            item3 = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(
                fptr, 
                "mov eax, dword %s\n" \
                "cmp eax, dword %s\n" \
                "je LL%d\n" \
                "mov dword %s, 0\n" \
                "jmp LLL%d\n" \
                "LL%d: mov dword %s, 1\n" \
                "LLL%d:\n",
                item1->addr,
                item2->addr,
                i, item3->addr, i, 
                i, item3->addr, i);
            break;

        case QuatOpType::neqOp:
            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            item2 = &symbolTable.items[quatTable.table[i].operandIdx2];
            item3 = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(
                fptr, 
                "mov eax, dword %s\n" \
                "cmp eax, dword %s\n" \
                "jne LL%d\n" \
                "mov dword %s, 0\n" \
                "jmp LLL%d\n" \
                "LL%d: mov dword %s, 1\n" \
                "LLL%d:\n", 
                item1->addr,
                item2->addr,
                i, item3->addr, i, 
                i, item3->addr, i);
            break;

        case QuatOpType::ltOp:
            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            item2 = &symbolTable.items[quatTable.table[i].operandIdx2];
            item3 = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(
                fptr, 
                "mov eax, dword %s\n" \
                "cmp eax, dword %s\n" \
                "jl LL%d\n" \
                "mov dword %s, 0\n" \
                "jmp LLL%d\n" \
                "LL%d: mov dword %s, 1\n" \
                "LLL%d:\n", 
                item1->addr,
                item2->addr,
                i, item3->addr, i, 
                i, item3->addr, i);
            break;

        case QuatOpType::gtOp:
            item1 = &symbolTable.items[quatTable.table[i].operandIdx1];
            item2 = &symbolTable.items[quatTable.table[i].operandIdx2];
            item3 = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(
                fptr, 
                "mov eax, dword %s\n" \
                "cmp eax, dword %s\n" \
                "jg LL%d\n" \
                "mov dword %s, 0\n" \
                "jmp LLL%d\n" \
                "LL%d: mov dword %s, 1\n" \
                "LLL%d:\n", 
                item1->addr,
                item2->addr,
                i, item3->addr, i, 
                i, item3->addr, i);
            break;

        case QuatOpType::noneOp:
            break;
        }
    }
    fprintf(fptr, "L%ld:\n", quatTable.table.size());

    fclose(fptr);
}