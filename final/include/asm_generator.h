#ifndef ASM_GENERATOR_H
#define ASM_GENERATOR_H
#include "symbol_table.h"
#include "quat_table.h"

extern QuatTable quatTable;
extern SymbolTable symbolTable;

class AsmGenerator
{
    string filename;
public:
    void generate(string filename = "./test/a.asm");
    void defineDataSeg(SymbolTable& symbolTable);
    void defineTextSeg(const SymbolTable& symbolTable, const QuatTable& quatTable);
};

#endif