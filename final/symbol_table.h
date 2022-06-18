#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <vector>
#include <unordered_map>
using namespace std;

#define MAX_IDENT_LEN 20

enum class VarType
{
    INT,
    CHAR,
    FLOAT  
};

struct SymbolTableItem
{
    char name[MAX_IDENT_LEN];
    VarType type;
};

class SymbolTable
{
    vector<SymbolTableItem*> items;
public:
    void addItem(char* name, VarType type);
    SymbolTableItem* getItem(const char* name);
    void print();
};
#endif