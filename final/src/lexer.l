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
float   { 
            fprintf(outfile, "%s  %d\n", yytext, FLOAT); 
            return FLOAT;
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
"+" {   fprintf(outfile, "%s  %d\n", yytext, ADD); return ADD;  }
"-" {   fprintf(outfile, "%s  %d\n", yytext, MINUS); return MINUS;  }
"*" {   fprintf(outfile, "%s  %d\n", yytext, MUL); return MUL;  }
"/" {   fprintf(outfile, "%s  %d\n", yytext, DIV); return DIV;  }
"%" {   fprintf(outfile, "%s  %d\n", yytext, MOD); return MOD;  }
"<"     {   fprintf(outfile, "%s  %d\n", yytext, LT); return LT;     }
">"     {   fprintf(outfile, "%s  %d\n", yytext, GT); return GT;    }
"=="    {   fprintf(outfile, "%s  %d\n", yytext, EQU); return EQU;  }
"!="    {   fprintf(outfile, "%s  %d\n", yytext, NEQ); return NEQ;     }
"!" {   fprintf(outfile, "%s  %d\n", yytext, NOT); return NOT;  }
"&&"    {   fprintf(outfile, "%s  %d\n", yytext, AND); return AND;  }
"||"    {   fprintf(outfile, "%s  %d\n", yytext, OR); return OR;   }
"="     {   fprintf(outfile, "%s  %d\n", yytext, ASSIGN); return ASSIGN;   }
"("     {   fprintf(outfile, "%s  %d\n", yytext, LBRACKET); return LBRACKET;   }
")"     {   fprintf(outfile, "%s  %d\n", yytext, RBRACKET); return RBRACKET;   }
"{"     {   fprintf(outfile, "%s  %d\n", yytext, LBRACE); return LBRACE;   }
"}"     {   fprintf(outfile, "%s  %d\n", yytext, RBRACE); return RBRACE;   }
";"     {   fprintf(outfile, "%s  %d\n", yytext, SEMI); return SEMI;     }
","     {   fprintf(outfile, "%s  %d\n", yytext, COMM); return COMM;     }
[ \t]+      /* ignore whitespace */;
\n          /* ignore EOL */;



%%

