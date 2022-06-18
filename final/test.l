%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype
{
IDENTIFIER = 258,
CONSTANT = 259,
IF = 260,
ELSE = 261,
WHILE = 262,
BREAK = 263,
CONTINUE = 264,
CHAR = 265,
VOID = 266,
FLOAT = 267,
DOUBLE = 268,
INT = 269,
ADD = 270,
MINUS = 271,
MUL = 272,
DIV = 273,
MOD = 274,
AND = 275,
OR = 276,
NOT = 277,
ASSIGN = 278, 
LBRACKET = 279,
RBRACKET = 280,
LBRACE = 281,
RBRACE = 282,
SEMI = 283,
COMM =  284
};

extern FILE* outfile;
extern "C" int yylex();
%}


digit   [0-9]
letter  [a-zA-Z]

%%
"/*".*"*/"                          {  }
"//"[^\n]*\n                        {  }
\"(.)*\"                            {  }
\'.\'                               {  }
{digit}+(\.{digit}+)?e(-|\+)?{digit}+   {
                                            fprintf(outfile, "%s  %d\n", yytext, CONSTANT); 
                                            return CONSTANT;
                                        }
{digit}+\.{digit}+      {
                            fprintf(outfile, "%s  %d\n", yytext, CONSTANT); 
                            return CONSTANT;
                        }
{digit}+    { 
                fprintf(outfile, "%s  %d\n", yytext, CONSTANT); 
                return CONSTANT;
            }
int { 
        fprintf(outfile, "%s  %d\n", yytext, INT); 
        return INT;
    }
char    { 
            fprintf(outfile, "%s  %d\n", yytext, CHAR); 
            return CHAR;
        }
void    { 
            fprintf(outfile, "%s  %d\n", yytext, VOID); 
            return VOID;
        }
float   { 
            fprintf(outfile, "%s  %d\n", yytext, FLOAT); 
            return FLOAT;
        }
double  { 
            fprintf(outfile, "%s  %d\n", yytext, DOUBLE); 
            return DOUBLE;
        }
if  { 
        fprintf(outfile, "%s  %d\n", yytext, IF); 
        return IF;
    }
else    { 
            fprintf(outfile, "%s  %d\n", yytext, ELSE); 
            return ELSE;
        }
while   { 
            fprintf(outfile, "%s  %d\n", yytext, WHILE); 
            return WHILE;
        }
continue    { 
                fprintf(outfile, "%s  %d\n", yytext, CONTINUE); 
                return CONTINUE;
            }
break   { 
            fprintf(outfile, "%s  %d\n", yytext, BREAK); 
            return BREAK;
        }
({letter}|_)({letter}|{digit}|_)*   { 
                                        fprintf(outfile, "%s  %d\n", yytext, IDENTIFIER); 
                                        return IDENTIFIER;
                                    }
"+" {   return ADD; }
"-" {   return MINUS; }
"*" {   return MUL; }
"/" {   return DIV; }
"%" {   return MOD; }
"!" {   return NOT; }
"&&"    {   return AND;  }
"||"    {   return OR;  }
"="     {   return ASSIGN;  }
"("     {   return LBRACKET;  }
")"     {   return RBRACKET;  }
"{"     {   return LBRACE;  }
"}"     {   return RBRACE;  }
";"     {   return SEMI;    }
","     {   return COMM;    }
[ \t]+      /* ignore whitespace */;
\n          /* ignore EOL */;



%%

