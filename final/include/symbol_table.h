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

enum class ItemType
{
    variable,
    constant,
    temp
};

class SymbolTable
{
public:
    struct Item
    {
        ItemType itemType;
        VarType varType;
        char varName[MAX_IDENT_LEN];
        union 
        {
            float constantFloat;
            int constantInt;
            char constantChar;
        };
        char addr[MAX_IDENT_LEN];
    };
    vector<Item> items;

    void addItemVariable(char* name, VarType type);
    void addItemConstantFloat(float constantVal);
    void addItemConstantInt(int constantVal);
    void addItemConstantChar(char constantVal);
    void addItemTemp();
    int getItemIdx(const char* name);
    int size();
    void print();
};

#endif