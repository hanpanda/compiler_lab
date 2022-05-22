#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include "LL1_parser.h"
using namespace std;

int main()
{
    LL1Parser parser;
    parser.log();

    printf("Welcome to use this LL(1) grammar parser!\n");
    string input, str;
    while (true)
    {
        printf("Please input a string for parsing:\n");
        getline(cin, input);
        str = "";
        for (auto ch : input)
        {
            if (ch != ' ')
            {
                str += ch;
            }
        }
        vector<Node> tokens;
        string temp = "";
        for (auto ch : str)
        {
            if (parser.isInVT(ch) && ch != 'i')
            {
                if (temp != "")
                {
                    Node token(NodeType::VT, temp, TokenType::IDENTIFIER);
                    tokens.push_back(token);
                    temp = "";
                }
                Node token(NodeType::VT, string(1, ch), TokenType::OTHER);
                tokens.push_back(token);
            }
            else
            {
                temp += ch;
            }
        }
        if (temp != "")
        {
            Node token(NodeType::VT, temp, TokenType::IDENTIFIER);
            tokens.push_back(token);
        }
        // examine
        for (auto &token : tokens)
        {
            cout << "(" << token.value << " " << token.tokenType << ")\n";
        }
        parser.parse(tokens);
    }

    return 0;
}