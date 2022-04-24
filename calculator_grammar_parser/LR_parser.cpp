/* *********************************
 * LR grammar
 * E -> E+T|E-T|T
 * T -> T*F|T/F|T%F|F
 * F -> G^F|G
 * G -> (E)|i
 * ********************************/

#include <iostream>
#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;

typedef pair<char, string> Item;
typedef set<Item> ItemSet;

class LRParser
{
    char extendStart;
    char start;
    set<char> VN;
    set<char> VT;
    map<char, vector<string>> generators;

    vector<ItemSet> itemSets;
    map<int, map<char, int>> goTable;

    map<int, map<char, pair<char, string>>> actionTable;
    map<int, map<char, int>> gotoTable;

public:
    LRParser()
    {
        // hard encode LR grammar
        start = 'E';
        VN = {'E', 'T', 'F', 'G'};
        VT = {'+', '-', '*', '/', '%', '(', ')', '^', 'i'};
        vector<string> temp;
        temp = {"E+T", "E-T", "T"};
        generators['E'] = temp;

        temp = {"T*F", "T/F", "T%F", "F"};
        generators['T'] = temp;

        temp = {"G^F", "G"};
        generators['F'] = temp;

        temp = {"(E)", "i"};
        generators['G'] = temp;

        // case 2:
        // start = 'E';
        // VN = {'E', 'A', 'B'};
        // VT = {'a', 'b', 'c', 'd'};
        // vector<string> temp;
        // temp = {"aA", "bB"};
        // generators['E'] = temp;

        // temp = {"cA", "d"};
        // generators['A'] = temp;

        // temp = {"cB", "d"};
        // generators['B'] = temp;

        // case 3:
        // start = 'E';
        // VN = {'E', 'F', 'T'};
        // VT = {'+', '*', '(', ')', 'i'};
        // vector<string> temp;
        // temp = {"E+T", "T"};
        // generators['E'] = temp;

        // temp = {"(E)", "i"};
        // generators['F'] = temp;

        // temp = {"F", "T*F"};
        // generators['T'] = temp;

        // extend grammar
        temp = {string(1, start)};
        extendStart = 'S';
        generators[extendStart] = temp;

        printf("Building item sets...\n");
        buildItemSets();
        printf("Building parse table...\n");
        buildParseTable();
        printf("LR Parser initialized!\n");
    }

    ItemSet closure(ItemSet s)
    {
        queue<Item> q;
        ItemSet closure_s;
        for (auto item : s)
        {
            q.push(item);
            closure_s.insert(item);
        }

        while (!q.empty())
        {
            Item item = q.front();
            q.pop();

            char A = 0; // 'e'?
            for (int i = 0; i < item.second.size(); i++)
            {
                if (item.second[i] == '.' && i != item.second.size() - 1)
                {
                    A = item.second[i + 1];
                    break;
                }
            }
            if (A != 0)
            {
                Item i;
                i.first = A;
                for (auto g : generators[A])
                {
                    i.second = "." + g;
                    if(closure_s.count(i) == 0)
                    {
                        q.push(i);
                        closure_s.insert(i);
                    }
                }
            }
        }
        return closure_s;
    }

    ItemSet go(ItemSet I, char A)
    {
        ItemSet J;
        for (auto item : I)
        {
            for (int i = 0; i < item.second.size(); i++)
            {
                if (item.second[i] == '.' && i != item.second.size() - 1)
                {
                    if (item.second[i + 1] == A)
                    {
                        item.second[i] = A;
                        item.second[i + 1] = '.';
                        J.insert(item);
                        break;
                    }
                }
            }
        }
        return closure(J);
    }

    int idxInItemSets(ItemSet s)
    {
        for (int i = 0; i < itemSets.size(); i++)
        {
            if (itemSets[i] == s)
                return i;
        }
        return itemSets.size();
    }

    void buildItemSets()
    {
        Item i;
        ItemSet s;
        i.first = extendStart;
        i.second = "." + string(1, start);
        s.insert(i);
        s = closure(s);

        queue<ItemSet> q;
        ItemSet s1, s2;
        int s1_idx, s2_idx = 0;
        q.push(s);
        itemSets.push_back(s);
        while (!q.empty())
        {
            s1 = q.front();
            q.pop();
            s1_idx = idxInItemSets(s1);
            
            for (auto A : VN)
            {
                s2 = go(s1, A);
                if (s2.empty())   
                    continue;
                s2_idx = idxInItemSets(s2);
                goTable[s1_idx][A] = s2_idx;
                if(s2_idx == itemSets.size())
                {
                    q.push(s2);
                    itemSets.push_back(s2);
                }
            }
            for (auto A : VT)
            {
                s2 = go(s1, A);
                if (s2.empty())   
                    continue;
                s2_idx = idxInItemSets(s2);
                goTable[s1_idx][A] = s2_idx;
                if(s2_idx == itemSets.size())
                {
                    q.push(s2);
                    itemSets.push_back(s2);
                }
            }
        }
        printf("itemSets size: %ld\n", itemSets.size());
    }

    void buildParseTable()
    {
        string generator;
        for(int i = 0; i < itemSets.size(); i++)
        {
            for(auto item: itemSets[i])
            {
                // rule 3: 接受。S -> E.
                if(item.first == extendStart && item.second == string(1, start) + ".")
                {
                    actionTable[i]['#'].first = 'a';
                    actionTable[i]['#'].second = "";
                    continue;
                }
                for(int j = 0; j < item.second.size(); j++)
                {
                    if(item.second[j] == '.')
                    {
                        generator = item.second;
                        generator.erase(generator.begin() + j);
                        generator = string(1, item.first) + ":" + generator;
                        // rule 1: 归约。 A -> B.
                        if(j == item.second.size() - 1)
                        {
                            for(auto a: VT)
                            {
                                actionTable[i][a].first = 'r';
                                actionTable[i][a].second = generator;
                            }
                            actionTable[i]['#'].first = 'r';
                            actionTable[i]['#'].second = generator;
                        }
                        // rule 2: 移进。 A -> B.a
                        else 
                        {
                            char ch = item.second[j + 1];
                            if(VT.count(ch) && goTable[i].count(ch)) 
                            {
                                actionTable[i][ch].first = 's';
                                actionTable[i][ch].second = to_string(goTable[i][ch]);
                            }
                        }
                    }
                }
                // rule 4: 状态转移
                for(auto element: goTable[i])
                {
                    if(VN.count(element.first) != 0)
                    {
                        gotoTable[i][element.first] = element.second;
                    }
                }
            }
        }
    }

    void drawParseProcedure(int step, stack<int> tempStack, stack<char> tempChStack, string str)
    {
        if(step == 0)
        {
            printf("-------------------------Parsing procedure-------------------------\n");
            drawCeil(8, "| step");
            drawCeil(20, "  state stack");
            drawCeil(20, "  charactor stack");
            drawCeil(15, "  input string");
            printf("\n");
        }

        // step
        drawCeil(8, "|" + to_string(step));

        // state stack
        string tempStr = "";
        while(!tempStack.empty())
        {
            tempStr += to_string(tempStack.top()) + " ";
            tempStack.pop();
        }
        tempStr.reserve();
        drawCeil(20, tempStr);

        // charactor stack
        tempStr = "";
        while(!tempChStack.empty())
        {
            tempStr += tempChStack.top();
            tempChStack.pop();
        }
        tempStr.reserve();
        drawCeil(20, tempStr);

        // input string
        drawCeil(15, str);
        printf("\n");
    }

    void parse(string str)
    {
        str += '#';
        int state;
        char ch, generatorLeft;
        stack<int> stateStack;
        stack<char> chStack;
        stateStack.push(0);
        chStack.push('#');
        for(int i = 0, step = 0; i < str.size(); step++)
        {
            drawParseProcedure(step, stateStack, chStack, str.substr(i));
            
            state = stateStack.top();
            ch = str[i];
            if(VT.count(ch) != 0 || ch == '#')
            {
                if(actionTable[state][ch].first == 'r')
                {
                    // printf("recusive: charactor: %c\n", ch);
                    // printf("size: %ld\n", actionTable[state][ch].second.size());
                    for(int j = 0; j < actionTable[state][ch].second.size() - 2; j++)
                    {
                        if(stateStack.empty() || chStack.empty())
                        {
                            printf("empty. %d, %c\n", i, str[i]);
                        }
                        stateStack.pop();
                        chStack.pop();
                    }
                    generatorLeft = actionTable[state][ch].second[0];
                    stateStack.push(gotoTable[stateStack.top()][generatorLeft]);
                    chStack.push(generatorLeft);
                }
                else if(actionTable[state][ch].first == 's')
                {
                    state = stoi(actionTable[state][ch].second);
                    // printf("shift: state: %d, charactor: %c\n", state, ch);
                    stateStack.push(state);
                    chStack.push(ch);
                    i++;
                }
                else if (actionTable[state][ch].first == 'a')
                {
                    printf("Analysis successfully! Stop!\n");
                    break;
                }
                else 
                {
                    printf("Grammar error!\n");
                    break;
                }
            }
            else
            {
                printf("Word error!\n");
                break;
            }
        }
    }

    void printItemSet(ItemSet s)
    {
        for (auto item : s)
        {
            printf("%c -> %s\n", item.first, item.second.c_str());
        }
    }

    void drawCeil(int width, string container)
    {
        for(auto item: container)
        {
            printf("%c", item);
        }
        for(int i = container.size(); i < width; i++)
        {
            printf(" ");
        }
        printf("|");
    }

    void log()
    {
        printf("-----------Grammar generators-----------\n");
        for(auto item: generators)
        {
            if(VT.count(item.first) != 0)
                continue;
            printf("%c -> ", item.first);
            for(auto str: item.second)
            {
                printf("%s |", str.c_str());
            }
            printf("\n");
        }

        // draw parse table
        printf("--------------------------------------------PARSE TABLE---------------------------------------------\n");
        vector<char> tempVN, tempVT;
        drawCeil(6, "|");
        for(auto a: VT)
        {
            drawCeil(8, string(1, a));
            tempVT.push_back(a);
        }
        drawCeil(8, string(1, '#'));
        tempVT.push_back('#');
        for(auto A: VN)
        {
            drawCeil(4, string(1, A));
            tempVN.push_back(A);
        }
        printf("\n");
        drawCeil(6, "|state");
        drawCeil(9 * (1 + VT.size()) - 1, "ACTION TABLE");
        drawCeil(5 * VN.size() - 1, "GOTO TABLE");
        printf("\n");

        for(int state = 0; state < itemSets.size(); state++)
        {
            drawCeil(6, "|" + to_string(state));
            for(auto a: tempVT)
            {
                if(actionTable.count(state) == 0 || actionTable[state].count(a) == 0)
                {
                    drawCeil(8, "");
                    continue;
                }
                printf("%c ", actionTable[state][a].first);
                drawCeil(6, actionTable[state][a].second);
            }
            for(auto A: tempVN)
            {
                if(gotoTable.count(state) == 0 || gotoTable[state].count(A) == 0)
                {
                    drawCeil(4, "");
                    continue;
                }
                drawCeil(4, to_string(gotoTable[state][A]));
            }
            printf("\n");
        }

        string str = "i*(i+i)/i";
        printf("\nThis is a example case:\n");
        printf("input string: %s\n", str.c_str());
        parse(str);
    }
};

int main()
{
    LRParser parser;
    parser.log();

    printf("\nWelcome to use this LR grammar parser!\n");
    string input, str; 
    while(true)
    {
        printf("Please input a string for parsing:\n");
        getline(cin, input);
        str = "";
        for(auto ch: input)
        {
            if(ch != ' ')
            {
                str += ch;
            }
        }
        parser.parse(str);
    }

    return 0;
}