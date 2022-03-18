%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* outfile;

enum MARK 
{
    IDENTIFIER, 
    CHARACTOR,
    STRING,
    NUMBER, 
    DECIMAL,
    SCIENCE,

    // 关键字
    INT, 
    SHORT, 
    LONG, 
    CHAR, 
    VOID, 
    FLOAT, 
    DOUBLE, 

    IF, 
    ELSE, 
    FOR, 
    WHILE, 
    DO, 
    CONTINUE, 
    BREAK, 
    RETURN, 
    MAIN,


    // 界符
    PLUS, 
    MINUS, 
    MUL, 
    DIV, 
    MOD,
    PLUSPLUS,
    MINUSMINUS,

    EQ, 
    NE, 
    LT, 
    GT, 
    LE, 
    GE,

    AND, 
    OR, 
    NOT, 
    BITAND, 
    BITOR, 
    BITNOT, 
    XOR,

    ASSIGN, 
    PLUSASSIGN, 
    MINUSASSIGN, 
    MULASSIGN, 
    DIVASSIGN,
    MODASSIGN,

    LBRACE, 
    RBRACE, 
    LPAREN, 
    RPAREN, 
    LSQUARE, 
    RSQUARE, 
    COMMA, 
    SEMI,

    // 补充关键字
    TYPEDEF,
    SIZEOF,

    // 预处理相关
    LINECOMMENT,
    BLOCKCOMMENT,
    INCLUDE,
    DEFINE,
    HEAD_FILE
};

%}

digit   [0-9]
letter  [a-zA-Z]
whitespace   [\s]

%%
\#(\ )*include                      { fprintf(outfile, "%s  %d\n", yytext, INCLUDE); }
\#(\ )*define                       { fprintf(outfile, "%s  %d\n", yytext, DEFINE); }
\<.*\>                              { fprintf(outfile, "%s  %d\n", yytext, HEAD_FILE); }
"/*".*"*/"                          { fprintf(outfile, "%s  %d\n", yytext, BLOCKCOMMENT); }
"//"[^\n]*\n                        { fprintf(outfile, "%s  %d\n", yytext, LINECOMMENT); }
\"(.)*\"                            { fprintf(outfile, "%s  %d\n", yytext, STRING); }
\'.\'                               { fprintf(outfile, "%s  %d\n", yytext, CHARACTOR); }
(-|\+)?{digit}+(\.{digit}+)?e(-|\+)?{digit}+     { fprintf(outfile, "%s  %d\n", yytext, SCIENCE); }
(-|\+)?{digit}+\.{digit}+           { fprintf(outfile, "%s  %d\n", yytext, DECIMAL); }
(-|\+)?{digit}+                     { fprintf(outfile, "%s  %d\n", yytext, NUMBER); }
int                                 { fprintf(outfile, "%s  %d\n", yytext, INT); }
short                               { fprintf(outfile, "%s  %d\n", yytext, SHORT); }
long                                { fprintf(outfile, "%s  %d\n", yytext, LONG); }
char                                { fprintf(outfile, "%s  %d\n", yytext, CHAR); }
void                                { fprintf(outfile, "%s  %d\n", yytext, VOID); }
float                               { fprintf(outfile, "%s  %d\n", yytext, FLOAT); }
double                              { fprintf(outfile, "%s  %d\n", yytext, DOUBLE); }
if                                  { fprintf(outfile, "%s  %d\n", yytext, IF); }
else                                { fprintf(outfile, "%s  %d\n", yytext, ELSE); }
for                                 { fprintf(outfile, "%s  %d\n", yytext, FOR); }
while                               { fprintf(outfile, "%s  %d\n", yytext, WHILE); }
do                                  { fprintf(outfile, "%s  %d\n", yytext, DO); }
continue                            { fprintf(outfile, "%s  %d\n", yytext, CONTINUE); }
break                               { fprintf(outfile, "%s  %d\n", yytext, BREAK); }
return                              { fprintf(outfile, "%s  %d\n", yytext, RETURN); }
main                                { fprintf(outfile, "%s  %d\n", yytext, MAIN); }
typedef                             { fprintf(outfile, "%s  %d\n", yytext, TYPEDEF); }
sizeof                              { fprintf(outfile, "%s  %d\n", yytext, SIZEOF); }
({letter}|_)({letter}|{digit}|_)*   { fprintf(outfile, "%s  %d\n", yytext, IDENTIFIER); }
"+="                                { fprintf(outfile, "%s  %d\n", yytext, PLUSASSIGN); }
"++"                                { fprintf(outfile, "%s  %d\n", yytext, PLUSPLUS); }
"+"                                 { fprintf(outfile, "%s  %d\n", yytext, PLUS); }
"-="                                { fprintf(outfile, "%s  %d\n", yytext, MINUSASSIGN); }
"--"                                { fprintf(outfile, "%s  %d\n", yytext, MINUSMINUS); }
"-"                                 { fprintf(outfile, "%s  %d\n", yytext, MINUS); }
"*="                                { fprintf(outfile, "%s  %d\n", yytext, MULASSIGN); }
"*"                                 { fprintf(outfile, "%s  %d\n", yytext, MUL); }
"/="                                { fprintf(outfile, "%s  %d\n", yytext, DIVASSIGN); }
"/"                                 { fprintf(outfile, "%s  %d\n", yytext, DIV); }
"%="                                { fprintf(outfile, "%s  %d\n", yytext, MOD); }
"%"                                 { fprintf(outfile, "%s  %d\n", yytext, MODASSIGN); }
"=="                                { fprintf(outfile, "%s  %d\n", yytext, EQ); }
"="                                 { fprintf(outfile, "%s  %d\n", yytext, ASSIGN); }
"!="                                { fprintf(outfile, "%s  %d\n", yytext, NE); }
"!"                                 { fprintf(outfile, "%s  %d\n", yytext, NOT); }
"<="                                { fprintf(outfile, "%s  %d\n", yytext, LE); }
"<"                                 { fprintf(outfile, "%s  %d\n", yytext, LT); }
">="                                { fprintf(outfile, "%s  %d\n", yytext, GE); }
">"                                 { fprintf(outfile, "%s  %d\n", yytext, GT); }
"&&"                                { fprintf(outfile, "%s  %d\n", yytext, AND); }
"&"                                 { fprintf(outfile, "%s  %d\n", yytext, BITAND); }
"||"                                { fprintf(outfile, "%s  %d\n", yytext, OR); }
"|"                                 { fprintf(outfile, "%s  %d\n", yytext, BITNOT); }
"^"                                 { fprintf(outfile, "%s  %d\n", yytext, XOR); }
"("                                 { fprintf(outfile, "%s  %d\n", yytext, LPAREN); }
")"                                 { fprintf(outfile, "%s  %d\n", yytext, RPAREN); }
"["                                 { fprintf(outfile, "%s  %d\n", yytext, LSQUARE); }   
"]"                                 { fprintf(outfile, "%s  %d\n", yytext, RSQUARE); }
"{"                                 { fprintf(outfile, "%s  %d\n", yytext, LBRACE); }
"}"                                 { fprintf(outfile, "%s  %d\n", yytext, RBRACE); }
","                                 { fprintf(outfile, "%s  %d\n", yytext, COMMA); }
";"                                 { fprintf(outfile, "%s  %d\n", yytext, SEMI); }
"\n"       
" "                        

%%

int yywrap()
{
    return 1;
}

int main(int argc, char* argv[]) 
{
    if (argc > 2) 
    {
        printf("ERROR, arguement too much! You should only input a code file name!\n");
    } 
    else if (argc == 2) 
    {
        yyin = fopen(argv[1], "rb+");
    }
    outfile = fopen("./tokens.txt", "w+");
    fprintf(outfile, "value  enum\n");
    yylex();
    return 0;
}