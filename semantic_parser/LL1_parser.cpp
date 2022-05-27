#include <iostream>
#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <stack>
#include "LL1_parser.h"
using namespace std;

LL1Parser::LL1Parser()
{
    // hard encode LL(1) grammar
    start = 'E';
    VN = {'E', 'T', 'F', 'N', 'X', 'Y', 'Z'};
    VT = {'+', '-', '*', '/', '%', '(', ')', '^', 'i'};
    vector<Generator> temp;
    temp = {Generator('E', "TX")};
    generators['E'] = temp;

    temp = {
        Generator('X', "+TX", GEQ, '+'),
        Generator('X', "-TX", GEQ, '-'),
        Generator('X', "e")};
    generators['X'] = temp;

    temp = {Generator('T', "FY")};
    generators['T'] = temp;

    temp = {
        Generator('Y', "*FY", GEQ, '*'),
        Generator('Y', "/FY", GEQ, '/'),
        Generator('Y', "%FY", GEQ, '%'),
        Generator('Y', "e")};
    generators['Y'] = temp;

    temp = {
        Generator('F', "NZ")};
    generators['F'] = temp;

    temp = {
        Generator('Z', "^F", GEQ, '^'),
        Generator('Z', "e")};
    generators['Z'] = temp;

    temp = {
        Generator('N', "(E)"),
        Generator('N', "i", PUSH)};
    generators['N'] = temp;

    buildFirst();
    buildFollow();
    buildParseTable();
}

bool LL1Parser::isInVT(char ch)
{
    return VT.count(ch) == 1;
}

void LL1Parser::buildFirst(char key)
{
    printf("key: %c\n", key);

    for (auto generator : generators[key])
    {
        char firstCh = generator.right[0];
        if (VT.count(firstCh) != 0 || firstCh == 'e')
        {
            first[key].insert(firstCh);
        }
        else
        {
            for (int i = 0; i < generator.right.size(); i++)
            {
                char ch = generator.right[i];
                if (VT.count(ch) != 0)
                {
                    first[key].insert(ch);
                    break;
                }
                else if (VN.count(ch) != 0)
                {
                    if (first[ch].empty())
                    {
                        buildFirst(ch);
                    }
                    for (auto c : first[ch])
                    {
                        if (c != 'e')
                        {
                            first[key].insert(c);
                        }
                    }
                }

                if (first[ch].count('e') == 0)
                {
                    break;
                }
                if (i == generator.right.size() - 1)
                {
                    first[key].insert('e');
                }
            }
        }
    }
}

void LL1Parser::buildFirst()
{
    for (auto key : VT)
    {
        first[key].insert(key);
    }
    first['e'].insert('e');

    for (auto key : VN)
    {
        if (first[key].empty())
            buildFirst(key);
    }
}

void LL1Parser::buildFollow(char key)
{
    if (key == start)
        follow[start].insert('#');

    for (auto item : generators)
    {
        char key2 = item.first;
        for (auto generator : item.second)
        {
            for (int i = 0; i < generator.right.size(); i++)
            {
                char ch = generator.right[i];
                if (ch != key)
                    continue;

                int j;
                // add FIRST(...)
                for (j = i + 1; j < generator.right.size(); j++)
                {
                    ch = generator.right[j];
                    follow[key].insert(first[ch].begin(), first[ch].end());
                    if (first[ch].count('e') == 0)
                        break;
                    else
                        follow[key].erase('e');
                }
                // add FOLLOW(key2)
                if ((j == generator.right.size() || i == generator.right.size() - 1) && key2 != key)
                {
                    if (follow[key2].empty())
                        buildFollow(key2);
                    follow[key].insert(follow[key2].begin(), follow[key2].end());
                }
            }
        }
    }
}

void LL1Parser::buildFollow()
{
    for (auto key : VN)
    {
        if (follow[key].empty())
            buildFollow(key);
    }
}

void LL1Parser::buildParseTable()
{
    for (auto &item : generators)
    {
        char key = item.first;
        for (auto &generator : item.second)
        {
            set<char> s;
            char ch;
            for (int i = 0; i < generator.right.size(); i++)
            {
                ch = generator.right[i];
                s.insert(first[ch].begin(), first[ch].end());
                if (first[ch].count('e') == 0)
                {
                    s.erase('e');
                    break;
                }
            }
            for (auto a : s)
            {
                parseTable[key][a] = &generator;
            }
            if (s.count('e') != 0)
            {
                for (auto a : follow[key])
                {
                    parseTable[key][a] = &generator;
                }
            }
        }
    }
}

void LL1Parser::drawCell(int width, string str, char padding,
                         string lBorder, string rBorder)
{
    // printf("%s", lBorder.c_str());
    cout << lBorder;
    for (auto item : str)
    {
        printf("%c", item);
    }
    for (int i = str.size(); i < width; i++)
    {
        printf("%c", padding);
    }
    // printf("%s", rBorder.c_str());
    cout << rBorder;
}

void LL1Parser::errorHandling(string msg)
{
    cout << "Error: " << msg << endl;
    return;
}

void LL1Parser::parse(vector<Node> &tokens)
{
    // draw a table header
    drawCell(40, "", '-', "", "");
    drawCell(20, "Parsing procedure", '-', "", "");
    drawCell(40, "", '-', "", "\n");
    drawCell(5, string("step"), ' ', "|");
    drawCell(40, string("stack"));
    drawCell(20, string("input"));
    drawCell(10, string("generator"));
    drawCell(20, string("quaternary"));
    drawCell(100, "", '-', "\n|", "|\n");

    tokens.push_back(Node(NodeType::VT, "#"));
    vector<Node> grammarStack;
    vector<string> semanticStack;
    Generator *generator;
    grammarStack.push_back(Node(NodeType::VT, "#"));
    grammarStack.push_back(Node(NodeType::VN, string(1, start)));
    quaternaries.clear();

    for (int i = 0, step = 0, tmp = 0; i < tokens.size(); step++)
    {
        // draw a table line
        drawCell(5, to_string(step), ' ', "|");
        string str = "";
        for (auto &node : grammarStack)
        {
            if (node.type == NodeType::ACTION)
            {
                if (node.actionType == ActionType::GEQ)
                {
                    str += "{GEQ ";
                    str += node.value;
                    str += "}";
                }
                else if (node.actionType == ActionType::PUSH)
                {
                    str += "{PUSH}";
                }
            }
            else
            {
                str += node.value;
            }
        }
        drawCell(40, str);
        str = "";
        for (int j = i; j < tokens.size(); j++)
        {
            str += tokens[j].value;
        }
        drawCell(20, str);

        Node curNode = tokens[i];
        Node topNode = *grammarStack.rbegin();

        if (topNode.type == NodeType::VT)
        {
            if (!(topNode.value == curNode.value &&
                      topNode.type == curNode.type ||
                  topNode.tokenType == TokenType::IDENTIFIER &&
                      curNode.tokenType == TokenType::IDENTIFIER))
            {
                errorHandling("1");
                return;
            }
            drawCell(10);
            drawCell(20);
            printf("\n");
            if (topNode.value == "#")
            {
                printf("meet # stop! success!\n");
                return;
            }
            else
            {
                grammarStack.pop_back();
                i++;
            }
        }
        else if (topNode.type == NodeType::VN)
        {
            char ch1 = topNode.value[0];
            char ch2 = curNode.value[0];
            if (curNode.tokenType == TokenType::IDENTIFIER)
            {
                ch2 = 'i';
            }
            if (parseTable.count(ch1) == 0 || parseTable[ch1].count(ch2) == 0)
            {
                errorHandling("2");
                return;
            }
            grammarStack.pop_back();
            generator = parseTable[ch1][ch2];

            drawCell(10, topNode.value + "->" + generator->right);
            drawCell(20);
            printf("\n");

            if (generator->right == "e")
                continue;
            if (generator->action != NONE)
            {
                // errorHandling(string(1, generator->actionObj));
                Node node(NodeType::ACTION, string(1, generator->actionObj), 0, generator->action);
                if (generator->action == ActionType::PUSH)
                {
                    node.value = curNode.value;
                }
                grammarStack.push_back(node);
            }
            for (int i = generator->right.size() - 1; i >= 0; i--)
            {
                Node node(NodeType::VN, string(1, generator->right[i]));
                if (VT.count(generator->right[i]) == 1)
                {
                    node.type = NodeType::VT;
                    if (generator->right[i] == 'i')
                    {
                        node.tokenType = TokenType::IDENTIFIER;
                    }
                }
                grammarStack.push_back(node);
            }
        }
        else if (topNode.type == NodeType::ACTION)
        {
            drawCell(10, string("Action"));
            if (topNode.actionType == ActionType::PUSH)
            {
                semanticStack.push_back(topNode.value);

                drawCell(20);
                printf("\n");
            }
            else if (topNode.actionType == ActionType::GEQ)
            {
                Quaternary q;
                q.operand2 = *semanticStack.rbegin();
                semanticStack.pop_back();
                q.operand1 = *semanticStack.rbegin();
                semanticStack.pop_back();
                q.op = topNode.value;
                q.result = "t" + to_string(tmp++);

                semanticStack.push_back(q.result);
                quaternaries.push_back(q);

                drawCell(20, q.str());
                printf("\n");
            }
            grammarStack.pop_back();
        }
    }
}

void LL1Parser::log()
{
    printf("-----------Grammar generators-----------\n");
    for (auto item : generators)
    {
        printf("%c -> ", item.first);
        for (auto g : item.second)
        {
            printf("%s |", g.right.c_str());
        }
        printf("\n");
    }

    printf("------------FIRST SET-----------\n");
    for (auto item : first)
    {
        printf("FIRST(%c): ", item.first);
        for (auto ch : item.second)
        {
            printf("%c ", ch);
        }
        printf("\n");
    }

    printf("------------FOLLOW SET-----------\n");
    for (auto item : follow)
    {
        printf("FOLLOW(%c): ", item.first);
        for (auto ch : item.second)
        {
            printf("%c ", ch);
        }
        printf("\n");
    }

    drawCell(150, "PARSE TABLE", '-', "|", "|\n");
    printf("|");
    drawCell(13);
    for (auto key : VT)
    {
        drawCell(13, "   " + string(1, key));
    }
    drawCell(13, string("   #"));
    printf("\n|");
    for (int i = 0; i < VT.size() + 2; i++)
    {
        drawCell(13, "", '-');
    }
    printf("\n");
    for (auto line : parseTable)
    {
        string str;
        printf("|");
        char key1 = line.first;
        drawCell(13, "   " + string(1, line.first));
        for (auto key2 : VT)
        {
            // cout << key2 << "p1\n";
            str = "";
            if (parseTable[key1].count(key2) != 0)
            {
                str = string(1, key1) + "->" + parseTable[key1][key2]->right;
                if(parseTable[key1][key2]->action == ActionType::GEQ) {
                    str += "{GEQ " + string(1, parseTable[key1][key2]->actionObj) + "}";
                } else {
                    str += "{PUSH}";
                }
            }
            drawCell(13, str);
        }
        str = "";
        if (parseTable[key1].count('#') != 0)
            str = string(1, key1) + "->" + parseTable[key1]['#']->right;
        drawCell(13, str);
        printf("\n");
    }
}
