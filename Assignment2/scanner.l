%{
#include <string.h>
#include <ctype.h>
#include<stdlib.h>
#include<stdio.h>
#include "scanner.h"
/* 
	Global variable: yylval
*/
YYSTYPE yylval;  

/* Global variable: yylloc
 * -----------------------
 */
typedef struct yyltype yylloc; 

/* 
	Macro: YY_USER_ACTION 
*/
//static void DoBeforeEachAction(); 
//#define YY_USER_ACTION DoBeforeEachAction();
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
	 /* Skip  white Spaces */
[\n]                   { colno = 1; lineno++; }
[\t]                   { colno += TAB_SIZE - colno % TAB_SIZE + 1; }
[ ]                    ;

 /* identify all the  keywords and return the correct token associated with that keyword. */
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


 /* recognize two-character operators and return the correct token */
\<=                    { return LessEqual; }
\>=                    { return GreaterEqual; }
==                     { return Equal; }
!=                     { return NotEqual; }
\[\]                   { return Dims; }
&&                     { return And; }
\|\|                   { return Or; }

 /* recognize int, double, bool and string constants,
  * return the correct token
  * and set appropriate filed of yylval 
  */
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

 /* recognize identifiers, 
  * return the correct token and set appropriate fields of yylval 
  */
{IDENTIFIER}             { 
                           if (yyleng < MaxIdentLen)
			   {
			  	strncpy(yylval.identifier, yytext, MaxIdentLen);
			  	yylval.identifier[MaxIdentLen] = '\0';   
			  	return Identifier;
			   }
		        }

 /* consume single-line comment */
\/\/.*\n                { 
                          lineno++; 
			  colno = 1;
			}
\/\*.*\*\/              ;

 /* consume multi-line comments 
  * report unterminated comment
  */

\/\*                    { BEGIN COMMENT; }
<COMMENT>.              ;
<COMMENT>[\r\n]         { 
                          lineno++; 
                          colno = 1;
			}
<COMMENT>\*\/           { BEGIN INITIAL; }


<COMMENT><<EOF>>        { 
			  BEGIN INITIAL;
			}


%%
void InitScanner()
{
    yy_flex_debug = false;
    lineno = 1;
    colno = 1;
}

