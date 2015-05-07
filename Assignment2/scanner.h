#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#define true 1 
#define false 0
//#include <vector>
//using namespace std;
//static vector<const char*> debugKeys;
static const int BufferSize = 2048;
#define MaxIdentLen 31    // Maximum length for identifiers

  
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

/* We have to specify all the information about the token read by the flex in the structure yylval which has to be of the type YYSTYPE
   so that it is accesible by the bison 
*/
 
typedef union {
    int integer;
    int  bool_value;
    char *string;
    double float_value;
    char identifier[MaxIdentLen+1]; 
} YYSTYPE;

 
/* Global variable: yylval
 */
extern YYSTYPE yylval;
extern char *yytext;      // Text of lexeme just scanned
int yylex();              // Defined in the generated lex.yy.c file
void InitScanner();                
 

