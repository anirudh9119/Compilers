#include <string.h>
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include "scanner.h"
#include "parser.h"

static void PrintOneToken(TokenType token, const char *text, YYSTYPE value)
{
  char buffer[] = {'\'', token, '\'', '\0'};
  const char *name = token >= Void ? gTokenNames[token - Void] : buffer;
  printf("%-12s is %s ", text,name);
  switch(token) {
    case IntConstant:     
      printf("(value = %d)\n", value.integer); break;
    case DoubleConstant:   
      printf("(value = %g)\n", value.float_value); break;
    case StringConstant:  
      printf("(value = %s)\n", value.string); break;
    case BoolConstant:    
      printf("(value = %s)\n", value.bool_value ? "true" : "false"); break;
    case Identifier:
	if (strcmp(text, value.identifier)) {
	  printf("(truncated to %s)\n", value.identifier);
	  break;
	}
    default:
      printf("\n"); break;
  }
}
int main(int argc, char *argv[])
{
    InitScanner();
    TokenType token;
    while ((token = (TokenType)yylex()) != 0) 
        PrintOneToken(token, yytext, yylval);
    InitParser();
  //  yyparse();
}
