#include <cstring>
#include <string>
#include "symbol_table.h"
using namespace std;


void SymbolTable::addItem(char* name, VarType type)
{
    SymbolTableItem* item = new SymbolTableItem();
    memcpy(item->name, name, strlen(name));
    item->type = type;
    items.push_back(item);
}

SymbolTableItem* SymbolTable::getItem(const char* name)
{
    for(auto item: items)
    {
        if(!strcmp(name, item->name))
        {
            return item;
        }
    }
    return nullptr;
}

void SymbolTable::print()
{
    printf("\nsymbol table:\n");
    string str;
    for(auto item: items)
    {
        switch (item->type)
        {
        case VarType::INT:
            str = "int";
            break;
        
        case VarType::CHAR:
            str = "char";
            break;

        case VarType::FLOAT:
            str = "float";
            break;
        }
        printf("%s, %s\n", item->name, str.c_str());
    }
}
