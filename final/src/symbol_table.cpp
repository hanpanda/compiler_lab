#include <cstring>
#include <string>
#include "../include/symbol_table.h"
using namespace std;

void printCeil(string str = "", int w = 15, char lCh = 0, char rCh = '|')
{
    if(lCh != 0)
        printf("%c", lCh);
    printf("%s", str.c_str());
    for(int i = 0; i < w - str.length(); i++)
        printf(" ");
    printf("%c", rCh);
}

int SymbolTable::size()
{
    return items.size();
}

void SymbolTable::addItemVariable(char* name, VarType type)
{
    SymbolTable::Item item;
    item.itemType = ItemType::variable;
    memset(item.varName, 0, MAX_IDENT_LEN);
    memcpy(item.varName, name, strlen(name));
    item.varType = type;
    items.push_back(item);
}

void SymbolTable::addItemConstant(double constantVal)
{
    SymbolTable::Item item;
    item.itemType = ItemType::constant;
    item.constantVal = constantVal;
    items.push_back(item);
}

void SymbolTable::addItemTemp()
{
    SymbolTable::Item item;
    item.itemType = ItemType::temp;
    items.push_back(item);
}

int SymbolTable::getItemIdx(const char* name)
{
    for(int i = 0; i < items.size(); i++)
    {
        if(!strcmp(items[i].varName, name))
        {
            return i;
        }
    }
    return -1;
}

void SymbolTable::print()
{
    printf("\n----------Symbol table----------\n");
    printCeil("id", 15, '|');
    printCeil("item type");
    printCeil("type");
    printCeil("value");
    printf("\n");

    for(int i = 0; i < items.size(); i++)
    {
        string itemType = "";
        string type = "";
        string value = "";
        printCeil(to_string(i), 15, '|');

        if(items[i].itemType == ItemType::constant)
        {
            itemType = "constant";
            value = to_string(items[i].constantVal);
        }
        else if(items[i].itemType == ItemType::temp)
        {
            itemType = "temp";
        }
        else if(items[i].itemType == ItemType::variable)
        {
            itemType = "variable";
            value = items[i].varName;
            switch (items[i].varType)
            {
            case VarType::INT:
                type = "int";
                break;
            
            case VarType::CHAR:
                type = "char";
                break;

            case VarType::FLOAT:
                type = "float";
                break;
            }
        }

        printCeil(itemType);
        printCeil(type);
        printCeil(value);
        printf("\n");
    }
}

