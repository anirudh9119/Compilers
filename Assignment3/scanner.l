%{
#include <string.h>
#include <ctype.h>
#include<stdlib.h>
#include<stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ast.h"
//#include "scanner.h"
#define true 1 
#define false 0
#include "parser.tab.h"
#define MaxIdentLen 31
//int yylex();              // Defined in the generated lex.yy.c file


#define TAB_SIZE 8
int lineno;
int  colno;
%}

PUNCTUATION        ([!;,.[\]{}()])
ARITHMETIC         ([-+*/%])
RELATIONAL         ([<>=])
OPERATOR           ({ARITHMETIC}|{RELATIONAL})
DECIMAL            ([0-9]+)
HEXADECIMAL        (0[xX][0-9a-fA-F]+)
CHARS              (\"[^\"\n]*\")
BOOLEAN            (true|false)   
INTEGER            ({DECIMAL}|{HEXADECIMAL})   
FLOAT              ({DECIMAL}\.{DECIMAL}?((E|e)(\+|\-)?{DECIMAL})?)
IDENTIFIER         ([a-zA-Z][a-zA-Z0-9_]*)

%x COMMENT
%% 
[\n]                   { colno = 1; lineno++; }
[\t]                   { colno += TAB_SIZE - colno % TAB_SIZE + 1; }
[ ]                    ;

void                   { return Void; }
int                    { return Int; }
double                 { return Double; }
bool                   { return Bool; }
string                 { return String; }
class                  { return Class; }
interface              { return Interface; }
null                   { return Null; }
this                   { return This; }
extends                { return Extends; }
implements             { return Implements; }
for                    { return For; }
while                  { return While; }
if                     { return If; }
else                   { return Else; }
return                 { return Return; }
break                  { return Break; }
new                    { return New; }
NewArray               { return NewArray; }
Print                  { return Print; }
ReadInteger            { return ReadInteger; }
ReadLine               { return ReadLine; }

 
{PUNCTUATION} |         
{OPERATOR}             { return yytext[0]; }


\<=                    { return LessEqual; }
\>=                    { return GreaterEqual; }
==                     { return Equal; }
!=                     { return NotEqual; }
\[\]                   { return Dims; }
&&                     { return And; }
\|\|                   { return Or; }

{CHARS}                  { 
                           yylval.string = strdup(yytext);
                           return StringConstant;
                         }
{BOOLEAN}                { 
                           if (strcmp("true", yytext) == 0)
                               yylval.bool_value = true;
                           else
			       yylval.bool_value = false;
                           return BoolConstant;
                         }
{DECIMAL}                { 
                           yylval.integer = strtol(yytext, NULL, 10); 
			   return IntConstant; 
			 }
{HEXADECIMAL}            {
                          yylval.integer = strtol(yytext, NULL, 16); 
			  return IntConstant;
                         }
{FLOAT}                  { 
                           yylval.float_value = atof(yytext); 
			   return DoubleConstant; 
			 }

{IDENTIFIER}             { 
                           if (yyleng < MaxIdentLen)
			   {
			  	strncpy(yylval.identifier, yytext, MaxIdentLen);
			  	yylval.identifier[MaxIdentLen] = '\0';   
			  	return T_Identifier;
			   }
		        }


%%
void InitScanner()
{
    yy_flex_debug = false;
    lineno = 1;
    colno = 1;
}

