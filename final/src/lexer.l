%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

  enum yytokentype
  {
    IDENTIFIER = 258,
    CONSTANT_FLOAT = 259,
    CONSTANT_INT = 260,
    CONSTANT_CHAR = 261,
    IF = 262,
    ELSE = 263,
    WHILE = 264,
    BREAK = 265,
    CONTINUE = 266,
    CHAR = 267,
    VOID = 268,
    FLOAT = 269,
    DOUBLE = 270,
    INT = 271,
    ADD = 272,
    MINUS = 273,
    MUL = 274,
    DIV = 275,
    MOD = 276,
    AND = 277,
    OR = 278,
    NOT = 279,
    ASSIGN = 280,
    LT = 281,
    GT = 282,
    EQU = 283,
    NEQ = 284,
    LBRACKET = 285,
    RBRACKET = 286,
    LBRACE = 287,
    RBRACE = 288,
    SEMI = 289,
    COMM = 290
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
                                            fprintf(outfile, "%s  %d\n", yytext, CONSTANT_FLOAT); 
                                            return CONSTANT_FLOAT;
                                        }
{digit}+\.{digit}+      {
                            fprintf(outfile, "%s  %d\n", yytext, CONSTANT_FLOAT); 
                            return CONSTANT_FLOAT;
                        }
{digit}+    { 
                fprintf(outfile, "%s  %d\n", yytext, CONSTANT_INT); 
                return CONSTANT_INT;
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
'{letter}'  {
                fprintf(outfile, "%s  %d\n", yytext, CONSTANT_CHAR); 
                return CONSTANT_CHAR;
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
"<"     {   return LT;    }
">"     {   return GT;    }
"=="    {   return EQU;  }
"!="    {   return NEQ;     }
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

