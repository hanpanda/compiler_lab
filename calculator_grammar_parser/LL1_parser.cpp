/* *********************************
 * remove left recursive, LL(1) grammar
 * E -> TX
 * X -> +TX|-TX|e
 * T -> FY
 * Y -> *FY|/FY|%FY|e
 * F -> NZ
 * Z -> ^F|e
 * N -> (E)|i
 * ********************************/

#include <iostream>
#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <stack>
using namespace std;

class LL1Parser{
    char start;
    set<char> VN;
    set<char> VT;
    map<char, vector<string>> generators;

    map<char, set<char>> first;
    map<char, set<char>> follow; 

    map<char, map<char, string>> parseTable;
public:
    LL1Parser(){
        // hard encode LL(1) grammar
        start = 'E';
        VN = {'E', 'T', 'F', 'N', 'X', 'Y', 'Z'};
        VT = {'+', '-', '*', '/', '%', '(', ')', '^', 'i'};
        vector<string> temp;
        temp = {"TX"};
        generators['E'] = temp;

        temp = {"+TX", "-TX", "e"};
        generators['X'] = temp;

        temp = {"FY"};
        generators['T'] = temp;

        temp = {"*FY", "/FY", "%FY", "e"};
        generators['Y'] = temp;

        temp = {"NZ"};
        generators['F'] = temp;

        temp = {"^F", "e"};
        generators['Z'] = temp;

        temp = {"(E)", "i"};
        generators['N'] = temp;

        buildFirst();
        buildFollow();
        buildParseTable();
    }

    void buildFirst(char key)
    {
        printf("key: %c\n", key);

        for(auto generator: generators[key])
        {
            char firstCh = generator[0];
            if(VT.count(firstCh) != 0 || firstCh == 'e')
            {
                first[key].insert(firstCh);
            }
            else
            {
                for(int i = 0; i < generator.size(); i++)
                {
                    char ch = generator[i];
                    if(VT.count(ch) != 0)
                    {
                        first[key].insert(ch);
                        break;
                    }
                    else if(VN.count(ch) != 0)
                    {
                        if(first[ch].empty())
                        {
                            buildFirst(ch);
                        }
                        for(auto c: first[ch])
                        {
                            if(c != 'e')
                            {
                                first[key].insert(c);
                            }
                        }
                    }
                    
                    if(first[ch].count('e') == 0)
                    {
                        break;
                    }
                    if(i == generator.size() - 1)
                    {
                        first[key].insert('e');
                    }
                }
            }
        }
    }

    void buildFirst()
    {
        for(auto key: VT)
        {
            first[key].insert(key);
        }
        first['e'].insert('e');

        for(auto key: VN)
        {
            if(first[key].empty())
                buildFirst(key);
        }
    }

    void buildFollow(char key)
    {
        if(key == start)
            follow[start].insert('#');

        for(auto item: generators)
        {
            char key2 = item.first;
            for(auto generator: item.second)
            {
                for(int i = 0; i < generator.size(); i++)
                {
                    char ch = generator[i];
                    if(ch != key)
                        continue;

                    int j;
                    // add FIRST(...)
                    for(j = i + 1; j < generator.size(); j++)
                    {
                        ch = generator[j];
                        follow[key].insert(first[ch].begin(), first[ch].end());
                        if(first[ch].count('e') == 0)
                            break;
                        else
                            follow[key].erase('e');
                    }
                    // add FOLLOW(key2)
                    if((j == generator.size() || i == generator.size() - 1) && key2 != key)
                    {
                        if(follow[key2].empty())
                            buildFollow(key2);
                        follow[key].insert(follow[key2].begin(), follow[key2].end());                            
                    }
                }
            }
        }
    }

    void buildFollow()
    {
        for(auto key: VN)
        {
            if(follow[key].empty())
                buildFollow(key);
        }
    }

    void buildParseTable()
    {
        for(auto item: generators)
        {
            char key = item.first;
            for(auto generator: item.second)
            {
                set<char> s;
                char ch;
                for(int i = 0; i < generator.size(); i++)
                {
                    ch = generator[i];
                    s.insert(first[ch].begin(), first[ch].end());
                    if(first[ch].count('e') == 0)
                    {
                        s.erase('e');
                        break;
                    }
                }
                for(auto a: s)
                {
                    parseTable[key][a] = generator;
                }
                if(s.count('e') != 0)
                {
                    for(auto a: follow[key])
                    {
                        parseTable[key][a] = generator;
                    }
                }
            }
        }
    }

    template<class T>
    void drawCeil(int width, T container)
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

    void parse(string str)
    {
        // draw a table header
        printf("------------Parsing procedure------------\n");
        printf("|");
        drawCeil(5, string("step"));
        drawCeil(10, string("stack"));
        drawCeil(10, string("input"));
        drawCeil(10, string("generator"));
        printf("\n|--------------------------------------|\n");

        str += '#';
        vector<char> parseStack;
        string generator;
        char topCh;
        char ch;
        parseStack.push_back('#');
        parseStack.push_back(start);
        for(int i = 0, step = 0; i < str.size(); step++)
        {
            ch = str[i];
            topCh = *parseStack.rbegin();
            
            // draw a table line
            printf("|");
            drawCeil(5, to_string(step));
            drawCeil(10, parseStack);
            drawCeil(10, string(str.begin() + i, str.end()));

            if(topCh == ch)
            {
                drawCeil(10, string(""));
                printf("\n");
                if(topCh == '#')
                {
                    printf("meet # stop! success!\n");
                    return;
                } 
                else
                {
                    parseStack.pop_back();
                    i++;
                }
            }
            else if(VN.count(topCh) == 1)
            {
                if(parseTable.count(topCh) == 0 || parseTable[topCh].count(ch) == 0)
                {
                    printf("\nError!\n");
                    return;
                }
                parseStack.pop_back();
                generator = parseTable[topCh][ch];
                drawCeil(10,  string(1, topCh) + "->" + generator);
                printf("\n");
                if(generator == "e")
                    continue;
                for(int i = generator.size() - 1; i >= 0; i--)
                {
                    parseStack.push_back(generator[i]);
                }
            }
            else
            {
                printf("\nError!\n");
                return;
            }
        }
    }

    void log() 
    {
        printf("-----------Grammar generators-----------\n");
        for(auto item: generators)
        {
            printf("%c -> ", item.first);
            for(auto str: item.second)
            {
                printf("%s |", str.c_str());
            }
            printf("\n");
        }

        printf("------------FIRST SET-----------\n");
        for(auto item: first)
        {
            printf("FIRST(%c): ", item.first);
            for(auto ch: item.second)
            {
                printf("%c ", ch);
            }
            printf("\n");
        }

        printf("------------FOLLOW SET-----------\n");
        for(auto item: follow)
        {
            printf("FOLLOW(%c): ", item.first);
            for(auto ch: item.second)
            {
                printf("%c ", ch);
            }
            printf("\n");
        }

        printf("--------------------------------------------PARSE TABLE---------------------------------------------\n");
        printf("|");
        drawCeil(8, string(""));
        for(auto key: VT)
        {
            drawCeil(8, "   " + string(1, key));
        }
        drawCeil(8, string("   #"));
        printf("\n|");
        for(int i = 0; i < VT.size() + 2; i++)
        {
            drawCeil(8, string("--------"));
        }
        printf("\n");
        for(auto line: parseTable)
        {
            string str;
            printf("|");
            char key1 = line.first;
            drawCeil(8, "   " + string(1, line.first));
            for(auto key2: VT)
            {
                str = "";
                if(parseTable[key1].count(key2) != 0)
                    str = string(1, key1) + "->" + parseTable[key1][key2];
                drawCeil(8, str);
            }
            str = "";
            if(parseTable[key1].count('#') != 0)
                str = string(1, key1) + "->" + parseTable[key1]['#'];
            drawCeil(8, str);
            printf("\n");
        }
    }

};


int main()
{
    LL1Parser parser;
    parser.log();

    printf("Welcome to use this LL(1) grammar parser!\n");
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