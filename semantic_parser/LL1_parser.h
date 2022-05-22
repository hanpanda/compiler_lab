/* *********************************
 * remove left recursive, LL(1) grammar
 * E -> TX
 * X -> +TX {GEQ(+)}|-TX {GEQ(-)}|e
 * T -> FY
 * Y -> *FY {GEQ(*)}|/FY {GEQ(/)}|%FY {GEQ(%)}|e
 * F -> NZ
 * Z -> ^F {GEQ(^)}|e
 * N -> (E)|i {PUSH(i)}
 * ********************************/

#include <set>
#include <map>
#include <vector>
#include <stack>
using namespace std;

enum ActionType
{
    NONE = 1,
    PUSH,
    GEQ
};

enum TokenType
{
    IDENTIFIER = 1,
    OTHER
};

enum NodeType
{
    ACTION = 1,
    VN,
    VT
};

struct Node
{
    int type;
    int tokenType;
    int actionType;
    string value;
    Node(int type = 0, string value = "", int tokenType = 0, int actionType = 0)
    {
        this->value = value;
        this->type = type;
        this->tokenType = tokenType;
        this->actionType = actionType;
    }
};

struct Generator
{
    char left;
    string right;
    int action;
    char actionObj;
    Generator(char left, string right, int action = NONE, char actionObj = 0)
    {
        this->left = left;
        this->right = right;
        this->action = action;
        this->actionObj = actionObj;
    }
};

struct Quaternary
{
    string op;
    string operand1;
    string operand2;
    string result;
    Quaternary(string op = "", string operand1 = "", string operand2 = "", string result = "")
    {
        this->op = op;
        this->operand1 = operand1;
        this->operand2 = operand2;
        this->result = result;
    }
    string str() 
    {
        return "(" + op + ", " + operand1 + ", " + operand2 + ", " + result + ")";
    }
};

class LL1Parser
{
    char start;
    set<char> VN;
    set<char> VT;
    map<char, vector<Generator>> generators;

    map<char, set<char>> first;
    map<char, set<char>> follow;

    map<char, map<char, Generator *>> parseTable;
    vector<Quaternary> quaternaries;

public:
    LL1Parser();

    bool isInVT(char ch);

    void buildFirst(char key);
 
    void buildFirst();
  
    void buildFollow(char key);
   
    void buildFollow();

    void buildParseTable();

    void drawCell(int width, string str = "", char padding = ' ', 
        string lBorder = "", string rBorder = "|");

    void errorHandling(string msg);

    void parse(vector<Node> &tokens);

    void log();
};

