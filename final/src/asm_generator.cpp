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
            fprintf(fptr, "const_%d dd %f\n", i, symbolTable.items[i].constantVal);
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

    const SymbolTable::Item* item;
    unordered_set<int> labels;

    for(int i = 0; i < quatTable.table.size(); i++)
    {
        if(labels.count(i))
            fprintf(fptr, "L%d:\n", i);

        switch(quatTable.table[i].optype)
        {
        case QuatOpType::addOp:
            item = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "mov eax, %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "add eax, %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, eax\n", item->addr);
            break;

        case QuatOpType::minusOp:
            item = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "mov eax, %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "sub eax, %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, eax\n", item->addr);
            break;

        case QuatOpType::mulOp:
            item = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "mov eax, %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "imul dword %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, eax\n", item->addr);
            break;
        
        case QuatOpType::divOp:
            fprintf(fptr, "mov edx, 0\n");
            item = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "mov eax, %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "idiv dword %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, eax\n", item->addr);
            break;
        
        case QuatOpType::modOp:
            fprintf(fptr, "mov edx, 0\n");
            item = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "mov eax, %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "idiv dword %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, edx\n", item->addr);
            break;
        
        case QuatOpType::andOp:
            item = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "mov eax, %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "and eax, dword %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, eax\n", item->addr);
            break;
        
        case QuatOpType::orOp:
            item = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "mov eax, %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "or eax, dword %s\n", item->addr);
            item = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, eax\n", item->addr);
            break;

        case QuatOpType::notOp:
            item = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "mov eax, %s\n", item->addr);
            fprintf(fptr, "not eax");
            item = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, eax\n", item->addr);
            break;

        case QuatOpType::jzOp:
            item = &symbolTable.items[quatTable.table[i].operandIdx1];
            fprintf(fptr, "cmp dword %s, 0\n", item->addr);
            fprintf(fptr, "je L%d\n", quatTable.table[i].resultIdx);
            labels.insert(quatTable.table[i].resultIdx);
            break;
        
        case QuatOpType::gotoOp:
            fprintf(fptr, "jmp L%d\n", quatTable.table[i].resultIdx);
            labels.insert(quatTable.table[i].resultIdx);
            break;
        
        case QuatOpType::assignOp:
            item = &symbolTable.items[quatTable.table[i].operandIdx2];
            fprintf(fptr, "mov eax, dword %s\n", item->addr);

            item = &symbolTable.items[quatTable.table[i].operandIdx1];
            if(item->varType == VarType::CHAR)
                fprintf(fptr, "mov %s, al\n", item->addr);
            else
                fprintf(fptr, "mov %s, eax\n", item->addr);

            item = &symbolTable.items[quatTable.table[i].resultIdx];
            fprintf(fptr, "mov %s, eax\n", item->addr);
            break;
        
        case QuatOpType::noneOp:
            break;
        }
    }
    fprintf(fptr, "L%d:\n", quatTable.table.size());

    fclose(fptr);
}