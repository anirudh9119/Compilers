#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#define true 1 
#define false 0
static const int BufferSize = 2048;
#define MaxIdentLen 31  

  
typedef enum { 
    Void = 256, Int, Double, Bool, String, Class, Null, Dims,LessEqual, GreaterEqual, Equal, NotEqual, And, Or, While, For, If, Else, Return, 
    Break, Extends, This, Implements, Interface, New, NewArray, Identifier, StringConstant, IntConstant, DoubleConstant, BoolConstant, 
    Print, ReadInteger, ReadLine, NumTokenTypes
} TokenType;

 
static const char *gTokenNames[NumTokenTypes] = {
  "Void", "Int", "Double", "Bool", "String", "Class", "Null", "Dims", "LessEqual", "GreaterEqual", "Equal", "NotEqual", "And", "Or", 
  "While", "For", "If", "Else", "Return", "Break", "Extends", "This", "Implements", "Interface", "New", "NewArray","Identifier", 
  "StringConstant", "IntConstant", "DoubleConstant", "BoolConstant", "Print", "ReadInteger", "ReadLine"
}; 

extern char *yytext;      // Text of lexeme just scanned
int yylex();              // Defined in the generated lex.yy.c file

void InitScanner();                
 



