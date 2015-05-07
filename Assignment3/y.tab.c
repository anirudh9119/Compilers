
/*  A Bison parser, made from parser.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define YYLSP_NEEDED

#define	Void	258
#define	Bool	259
#define	Int	260
#define	Double	261
#define	String	262
#define	Class	263
#define	LessEqual	264
#define	GreaterEqual	265
#define	Equal	266
#define	NotEqual	267
#define	Dims	268
#define	And	269
#define	Or	270
#define	Null	271
#define	Extends	272
#define	This	273
#define	Interface	274
#define	Implements	275
#define	While	276
#define	For	277
#define	If	278
#define	Else	279
#define	Return	280
#define	Break	281
#define	New	282
#define	NewArray	283
#define	Print	284
#define	ReadInteger	285
#define	ReadLine	286
#define	Increment	287
#define	Decrement	288
#define	Switch	289
#define	Case	290
#define	Default	291
#define	T_Identifier	292
#define	StringConstant	293
#define	IntConstant	294
#define	DoubleConstant	295
#define	BoolConstant	296
#define	UnaryMinus	297
#define	NoElse	298

#line 1 "parser.y"

//#include "scanner.h" // for yylex
//#include "errors.h"
#include<stdio.h>
#include<string>
#include<stdlib.h>
#include<list>
#include "ast.h"
#define MaxIdentLen 31
using namespace std;

int yylex();
void yyerror(const char *msg); // standard error-handling routine

#line 16 "parser.y"
typedef union {
    int integer;
    bool bool_value;
    char *string;
    double float_value;
    char identifier[32]; // +1 for terminating null
    Decl *decl;
    List<Decl*> *declList;
    Type *type;
    VarDecl *varDecl;
    FnDecl *fnDecl;
    List<VarDecl*> *formals;
    Identifier *identObj;
    List<NamedType*>* nTList;
    ClassDecl* classDecl;
    InterfaceDecl* ifaceDecl;
    List<VarDecl*> *vardecls;
    List<Stmt*> *statements;
    Stmt* stmt;
    PrintStmt* printStmt;
    List<Expr*>* exprList;
    Expr* expr;
    StmtBlock* stmtBlock;
    LValue* lvalue;
    Call* call;
    IfStmt* ifStmt;
    WhileStmt* whileStmt;
    ForStmt* forStmt;
    ReturnStmt* returnStmt;
    BreakStmt* breakStmt;
    SwitchStmt* switchStmt;
    List<Case*>* caseStmts;
    Case* caseStmt;
    Default* defStmt;
} YYSTYPE;

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		248
#define	YYFLAG		-32768
#define	YYNTBASE	63

#define YYTRANSLATE(x) ((unsigned)(x) <= 298 ? yytranslate[x] : 102)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    52,     2,     2,     2,    51,     2,     2,    56,
    57,    49,    47,    58,    48,    54,    50,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    61,    55,    45,
    44,    46,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    53,     2,    62,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    59,     2,    60,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     7,     9,    11,    13,    15,    18,    23,
    26,    28,    30,    32,    34,    36,    39,    41,    48,    55,
    57,    58,    60,    64,    71,    80,    83,    84,    86,    90,
    93,    94,    96,    98,   104,   107,   108,   115,   122,   127,
   131,   135,   138,   142,   145,   147,   150,   152,   154,   156,
   159,   161,   163,   165,   167,   169,   171,   173,   176,   185,
   193,   201,   204,   206,   211,   215,   216,   218,   219,   227,
   233,   239,   245,   253,   262,   271,   281,   284,   288,   291,
   295,   297,   301,   303,   305,   307,   309,   311,   313,   315,
   317,   321,   325,   329,   333,   337,   341,   344,   348,   352,
   356,   360,   364,   368,   372,   376,   379,   382,   385,   389,
   393,   396,   403,   408,   415,   417,   418,   420,   424,   426,
   430
};

static const short yyrhs[] = {    64,
     0,    64,    65,     0,    65,     0,    66,     0,    70,     0,
    73,     0,    78,     0,    67,    55,     0,    67,    44,    97,
    55,     0,    68,    69,     0,     5,     0,     6,     0,     4,
     0,     7,     0,    69,     0,    68,    13,     0,    37,     0,
    68,    69,    56,    71,    57,    81,     0,     3,    69,    56,
    71,    57,    81,     0,    72,     0,     0,    67,     0,    72,
    58,    67,     0,     8,    69,    74,    59,    76,    60,     0,
     8,    69,    17,    69,    74,    59,    76,    60,     0,    20,
    75,     0,     0,    69,     0,    75,    58,    69,     0,    76,
    77,     0,     0,    66,     0,    70,     0,    19,    69,    59,
    79,    60,     0,    79,    80,     0,     0,    68,    69,    56,
    71,    57,    55,     0,     3,    69,    56,    71,    57,    55,
     0,    59,    82,    83,    60,     0,    59,    83,    60,     0,
    59,    82,    60,     0,    59,    60,     0,    59,     1,    60,
     0,    82,    66,     0,    66,     0,    83,    84,     0,    84,
     0,    92,     0,    55,     0,    97,    55,     0,    90,     0,
    91,     0,    93,     0,    95,     0,    94,     0,    85,     0,
    81,     0,     1,    55,     0,    34,    56,    97,    57,    59,
    86,    88,    60,     0,    34,    56,    97,    57,    59,    88,
    60,     0,    34,    56,    97,    57,    59,     1,    60,     0,
    86,    87,     0,    87,     0,    35,    97,    61,    89,     0,
    36,    61,    89,     0,     0,    83,     0,     0,    23,    56,
    97,    57,    84,    24,    84,     0,    23,    56,    97,    57,
    84,     0,    21,    56,    97,    57,    84,     0,    29,    56,
    96,    57,    55,     0,    22,    56,    55,    97,    55,    57,
    84,     0,    22,    56,    97,    55,    97,    55,    57,    84,
     0,    22,    56,    55,    97,    55,    97,    57,    84,     0,
    22,    56,    97,    55,    97,    55,    97,    57,    84,     0,
    25,    55,     0,    25,    97,    55,     0,    26,    55,     0,
    96,    58,    97,     0,    97,     0,   101,    44,    97,     0,
   101,     0,    38,     0,    39,     0,    40,     0,    41,     0,
    16,     0,    18,     0,    98,     0,    56,    97,    57,     0,
    97,    47,    97,     0,    97,    48,    97,     0,    97,    49,
    97,     0,    97,    50,    97,     0,    97,    51,    97,     0,
    48,    97,     0,    97,    45,    97,     0,    97,     9,    97,
     0,    97,    46,    97,     0,    97,    10,    97,     0,    97,
    11,    97,     0,    97,    12,    97,     0,    97,    14,    97,
     0,    97,    15,    97,     0,    52,    97,     0,   101,    32,
     0,   101,    33,     0,    30,    56,    57,     0,    31,    56,
    57,     0,    27,    69,     0,    28,    56,    97,    58,    68,
    57,     0,    69,    56,    99,    57,     0,    97,    54,    69,
    56,    99,    57,     0,   100,     0,     0,    97,     0,   100,
    58,    97,     0,    69,     0,    97,    54,    69,     0,    97,
    53,    97,    62,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   131,   144,   145,   148,   149,   150,   151,   154,   155,   159,
   162,   163,   164,   165,   166,   167,   170,   173,   174,   177,
   178,   181,   182,   185,   187,   191,   192,   195,   196,   199,
   200,   203,   204,   207,   210,   211,   214,   215,   218,   219,
   220,   221,   222,   227,   228,   231,   232,   235,   236,   237,
   238,   239,   240,   241,   242,   243,   244,   245,   250,   251,
   254,   259,   260,   263,   266,   267,   270,   271,   274,   275,
   278,   281,   284,   285,   286,   287,   290,   291,   294,   297,
   298,   301,   302,   305,   306,   307,   308,   309,   311,   312,
   313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
   323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
   333,   334,   337,   338,   341,   342,   345,   346,   349,   350,
   351
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","Void","Bool",
"Int","Double","String","Class","LessEqual","GreaterEqual","Equal","NotEqual",
"Dims","And","Or","Null","Extends","This","Interface","Implements","While","For",
"If","Else","Return","Break","New","NewArray","Print","ReadInteger","ReadLine",
"Increment","Decrement","Switch","Case","Default","T_Identifier","StringConstant",
"IntConstant","DoubleConstant","BoolConstant","UnaryMinus","NoElse","'='","'<'",
"'>'","'+'","'-'","'*'","'/'","'%'","'!'","'['","'.'","';'","'('","')'","','",
"'{'","'}'","':'","']'","Program","DeclList","Decl","VariableDecl","Variable",
"Type","Identifier","FunctionDecl","OptFormals","Formals","ClassDecl","ImplementsDecl",
"ImplementsList","FieldList","Field","InterfaceDecl","PrototypeList","Prototype",
"StmtBlock","VarDeclList","StmtList","Stmt","SwitchStmt","CaseStatements","CaseStatement",
"DefaultStatement","OptStmtList","IfStmt","WhileStmt","PrintStmt","ForStmt",
"ReturnStmt","BreakStmt","ExprList","Expr","Call","OptActuals","Actuals","LValue", NULL
};
#endif

static const short yyr1[] = {     0,
    63,    64,    64,    65,    65,    65,    65,    66,    66,    67,
    68,    68,    68,    68,    68,    68,    69,    70,    70,    71,
    71,    72,    72,    73,    73,    74,    74,    75,    75,    76,
    76,    77,    77,    78,    79,    79,    80,    80,    81,    81,
    81,    81,    81,    82,    82,    83,    83,    84,    84,    84,
    84,    84,    84,    84,    84,    84,    84,    84,    85,    85,
    85,    86,    86,    87,    88,    88,    89,    89,    90,    90,
    91,    92,    93,    93,    93,    93,    94,    94,    95,    96,
    96,    97,    97,    97,    97,    97,    97,    97,    97,    97,
    97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
    97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
    97,    97,    98,    98,    99,    99,   100,   100,   101,   101,
   101
};

static const short yyr2[] = {     0,
     1,     2,     1,     1,     1,     1,     1,     2,     4,     2,
     1,     1,     1,     1,     1,     2,     1,     6,     6,     1,
     0,     1,     3,     6,     8,     2,     0,     1,     3,     2,
     0,     1,     1,     5,     2,     0,     6,     6,     4,     3,
     3,     2,     3,     2,     1,     2,     1,     1,     1,     2,
     1,     1,     1,     1,     1,     1,     1,     2,     8,     7,
     7,     2,     1,     4,     3,     0,     1,     0,     7,     5,
     5,     5,     7,     8,     8,     9,     2,     3,     2,     3,
     1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
     3,     3,     3,     3,     3,     3,     2,     3,     3,     3,
     3,     3,     3,     3,     3,     2,     2,     2,     3,     3,
     2,     6,     4,     6,     1,     0,     1,     3,     1,     3,
     4
};

static const short yydefact[] = {     0,
     0,    13,    11,    12,    14,     0,     0,    17,     1,     3,
     4,     0,     0,    15,     5,     6,     7,     0,    27,     0,
     2,     0,     8,    16,    10,    21,     0,     0,     0,    36,
    88,    89,     0,     0,     0,     0,    84,    85,    86,    87,
     0,     0,     0,   119,     0,    90,    83,    21,    22,     0,
     0,    20,    27,    28,    26,    31,     0,   111,     0,     0,
     0,    97,   106,     0,   116,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     9,   107,   108,     0,     0,    10,     0,     0,     0,     0,
     0,     0,    34,     0,    35,     0,   109,   110,    91,   117,
     0,   115,    99,   101,   102,   103,   104,   105,    98,   100,
    92,    93,    94,    95,    96,     0,   120,    82,     0,     0,
    19,    23,    31,    29,    24,    32,    33,    30,     0,     0,
     0,   113,     0,   121,   116,    18,     0,     0,     0,     0,
     0,     0,     0,     0,    49,    42,    45,   119,    57,     0,
     0,    47,    56,    51,    52,    48,    53,    55,    54,     0,
     0,    21,    21,     0,   118,     0,    58,    43,     0,     0,
     0,    77,     0,    79,     0,     0,     0,    41,    44,     0,
    40,    46,    50,    25,     0,     0,   112,   114,     0,     0,
     0,     0,    78,     0,    81,     0,    39,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    38,    37,    71,     0,
     0,    70,    72,    80,     0,     0,     0,     0,     0,     0,
     0,     0,    66,    63,     0,    73,     0,     0,     0,    69,
    61,     0,     0,    62,     0,    60,    75,    74,     0,     0,
     0,    65,    59,    76,    64,     0,     0,     0
};

static const short yydefgoto[] = {   246,
     9,    10,    11,    12,    50,    44,    15,    51,    52,    16,
    29,    55,    91,   128,    17,    57,    95,   149,   150,   241,
   152,   153,   223,   224,   225,   242,   154,   155,   156,   157,
   158,   159,   194,   160,    46,   101,   102,    47
};

static const short yypact[] = {  1045,
    -2,-32768,-32768,-32768,-32768,    -2,    -2,-32768,  1045,-32768,
-32768,   -28,    -7,-32768,-32768,-32768,-32768,   -47,     5,   -26,
-32768,  1153,-32768,-32768,   -17,    95,    -2,    -2,     7,-32768,
-32768,-32768,    -2,   -14,    11,    12,-32768,-32768,-32768,-32768,
  1153,  1153,  1153,    16,   786,-32768,   -20,    95,-32768,    -7,
    17,    20,    60,-32768,    23,-32768,   137,-32768,  1153,    38,
    47,   -36,   -36,   636,  1153,  1153,  1153,  1153,  1153,  1153,
  1153,  1153,  1153,  1153,  1153,  1153,  1153,  1153,  1153,    -2,
-32768,-32768,-32768,  1153,    49,-32768,    35,    95,    48,    -2,
   250,    -2,-32768,    -7,-32768,   616,-32768,-32768,-32768,   924,
    52,    24,   559,   559,   980,   980,   990,   970,   559,   559,
    37,    37,   -36,   -36,   -36,   570,    54,   924,    35,   188,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    56,    58,
    95,-32768,  1153,-32768,  1153,-32768,   -45,    59,    67,    68,
  1091,    71,    69,    73,-32768,-32768,-32768,    28,-32768,   245,
   380,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   833,
   554,    95,    95,     1,   924,    74,-32768,-32768,  1153,  1122,
  1153,-32768,   844,-32768,  1153,  1153,    79,-32768,-32768,   425,
-32768,-32768,-32768,-32768,    78,    82,-32768,-32768,   685,  1153,
   855,   698,-32768,   -29,   924,   711,-32768,    81,    90,   515,
   902,  1153,   515,    91,  1153,    92,-32768,-32768,-32768,  1029,
   913,   128,-32768,   924,    25,   515,   760,  1060,   515,    96,
  1153,    94,    -4,-32768,    97,-32768,   515,   515,   773,-32768,
-32768,   588,   470,-32768,    99,-32768,-32768,-32768,   515,   290,
   335,-32768,-32768,-32768,-32768,   165,   166,-32768
};

static const short yypgoto[] = {-32768,
-32768,   158,   -86,   -25,     2,    70,   -88,   -41,-32768,-32768,
   116,-32768,    50,-32768,-32768,-32768,-32768,   -83,-32768,  -112,
  -111,-32768,-32768,   -53,   -52,   -68,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   -22,-32768,    40,-32768,-32768
};


#define	YYLAST		1209


static const short yytable[] = {    45,
    49,    13,   127,   121,   126,    24,    85,   151,    26,   167,
    13,    82,    83,    24,   168,    22,    79,    80,    62,    63,
    64,    27,    49,    84,    28,   220,    23,   204,   205,     8,
   221,   222,    30,   147,     8,   136,    96,   180,    48,   182,
   -15,    59,   100,   103,   104,   105,   106,   107,   108,   109,
   110,   111,   112,   113,   114,   115,   116,   187,    94,   221,
   222,   118,   122,   179,   -15,    56,    60,    61,   182,    14,
    18,    65,   127,    87,   126,    19,    20,    88,    14,    28,
    90,   133,    25,    65,   -66,    76,    77,    78,   209,    79,
    80,   212,    13,   120,    97,    14,    53,    54,     2,     3,
     4,     5,    58,    98,   226,   119,   123,   230,   132,   135,
   165,   162,   100,   163,   169,   237,   238,    14,   173,    86,
   185,   186,   170,   171,   175,   174,    14,   244,   176,   182,
   188,     8,   164,   167,   198,   207,    49,    49,   199,    92,
     2,     3,     4,     5,   208,   213,   189,   191,   192,   117,
   215,   219,   195,   196,   233,   231,   236,    14,   243,   124,
    14,   129,    13,   130,   247,   248,    21,   201,    89,   234,
   235,   245,   161,     8,   166,     0,     0,     0,     0,   211,
     0,     0,   214,     0,     0,     0,     0,   217,   137,   148,
     0,     2,     3,     4,     5,   229,    93,     0,   232,     0,
    14,     0,     0,    31,     0,    32,     0,     0,   138,   139,
   140,     0,   141,   142,    33,    34,   143,    35,    36,   148,
     0,   144,     0,     0,     8,    37,    38,    39,    40,     0,
    14,    14,    14,     0,     0,    41,     0,     0,     0,    42,
     0,     0,   145,    43,     0,   177,   120,   146,     2,     3,
     4,     5,     1,     2,     3,     4,     5,     0,     0,     0,
    31,     0,    32,     0,     0,   138,   139,   140,     0,   141,
   142,    33,    34,   143,    35,    36,     0,     0,   144,     0,
     0,     8,    37,    38,    39,    40,     8,     0,     0,     0,
   177,     0,    41,     0,     0,     0,    42,     0,     0,   145,
    43,     0,     0,   120,   178,    31,     0,    32,     0,   125,
   138,   139,   140,     0,   141,   142,    33,    34,   143,    35,
    36,     0,     0,   144,   -68,   -68,     8,    37,    38,    39,
    40,     0,     0,     0,     0,   177,     0,    41,     0,     0,
     0,    42,     0,     0,   145,    43,     0,     0,   120,   -68,
    31,     0,    32,     0,     0,   138,   139,   140,     0,   141,
   142,    33,    34,   143,    35,    36,     0,     0,   144,   -67,
   -67,     8,    37,    38,    39,    40,     0,     0,     0,     0,
   177,     0,    41,     0,     0,     0,    42,     0,     0,   145,
    43,     0,     0,   120,   -67,    31,     0,    32,     0,     0,
   138,   139,   140,     0,   141,   142,    33,    34,   143,    35,
    36,     0,     0,   144,     0,     0,     8,    37,    38,    39,
    40,     0,     0,     0,     0,   177,     0,    41,     0,     0,
     0,    42,     0,     0,   145,    43,     0,     0,   120,   181,
    31,     0,    32,     0,     0,   138,   139,   140,     0,   141,
   142,    33,    34,   143,    35,    36,     0,     0,   144,     0,
     0,     8,    37,    38,    39,    40,     0,     0,     0,     0,
   177,     0,    41,     0,     0,     0,    42,     0,     0,   145,
    43,     0,     0,   120,   197,    31,     0,    32,     0,     0,
   138,   139,   140,     0,   141,   142,    33,    34,   143,    35,
    36,     0,     0,   144,     0,     0,     8,    37,    38,    39,
    40,     0,     0,     0,     0,   177,     0,    41,     0,     0,
     0,    42,     0,     0,   145,    43,     0,     0,   120,   -68,
    31,     0,    32,     0,     0,   138,   139,   140,     0,   141,
   142,    33,    34,   143,    35,    36,     0,     0,   144,     0,
     0,     8,    37,    38,    39,    40,     1,     2,     3,     4,
     5,     0,    41,     0,     0,     0,    42,-32768,-32768,   145,
    43,     0,     0,   120,     0,     0,     0,     0,    66,    67,
    68,    69,     0,    70,    71,     0,     0,     0,     0,     0,
     8,     0,     0,     0,     0,     0,    66,    67,    68,    69,
     0,    70,    71,-32768,-32768,    74,    75,    76,    77,    78,
     0,    79,    80,   184,    72,    73,    74,    75,    76,    77,
    78,     0,    79,    80,    66,    67,    68,    69,     0,    70,
    71,   134,    72,    73,    74,    75,    76,    77,    78,     0,
    79,    80,     0,     0,    66,    67,    68,    69,   240,    70,
    71,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    72,    73,    74,    75,    76,    77,    78,     0,    79,    80,
     0,     0,     0,   131,     0,     0,     0,     0,     0,     0,
    72,    73,    74,    75,    76,    77,    78,     0,    79,    80,
     0,     0,    99,    66,    67,    68,    69,     0,    70,    71,
     0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
     0,    70,    71,     0,     0,     0,     0,     0,     0,    66,
    67,    68,    69,     0,    70,    71,     0,     0,     0,    72,
    73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
     0,   200,    72,    73,    74,    75,    76,    77,    78,     0,
    79,    80,     0,     0,   203,    72,    73,    74,    75,    76,
    77,    78,     0,    79,    80,     0,     0,   206,    66,    67,
    68,    69,     0,    70,    71,     0,     0,     0,     0,     0,
     0,    66,    67,    68,    69,     0,    70,    71,     0,     0,
     0,     0,     0,     0,    66,    67,    68,    69,     0,    70,
    71,     0,     0,     0,    72,    73,    74,    75,    76,    77,
    78,     0,    79,    80,     0,     0,   227,    72,    73,    74,
    75,    76,    77,    78,     0,    79,    80,     0,     0,   239,
    72,    73,    74,    75,    76,    77,    78,     0,    79,    80,
    81,    66,    67,    68,    69,     0,    70,    71,     0,     0,
     0,     0,    66,    67,    68,    69,     0,    70,    71,     0,
     0,     0,     0,    66,    67,    68,    69,     0,    70,    71,
     0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
    75,    76,    77,    78,     0,    79,    80,   183,    72,    73,
    74,    75,    76,    77,    78,     0,    79,    80,   193,    72,
    73,    74,    75,    76,    77,    78,     0,    79,    80,   202,
    66,    67,    68,    69,     0,    70,    71,     0,     0,     0,
     0,    66,    67,    68,    69,     0,    70,    71,     0,     0,
     0,     0,    66,    67,    68,    69,     0,    70,    71,     0,
     0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
    76,    77,    78,     0,    79,    80,   210,    72,    73,    74,
    75,    76,    77,    78,     0,    79,    80,   218,    72,    73,
    74,    75,    76,    77,    78,     0,    79,    80,    66,    67,
    68,    69,     0,    70,     0,     0,     0,     0,    66,    67,
-32768,-32768,     0,     0,     0,     0,     0,     0,    66,    67,
    68,    69,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    72,    73,    74,    75,    76,    77,
    78,     0,    79,    80,    72,    73,    74,    75,    76,    77,
    78,     0,    79,    80,    72,    73,    74,    75,    76,    77,
    78,     0,    79,    80,    31,     0,    32,     1,     2,     3,
     4,     5,     6,     0,     0,    33,    34,     0,    35,    36,
     0,     0,     0,     7,     0,     8,    37,    38,    39,    40,
     0,     0,     0,     0,     0,    31,    41,    32,     0,     0,
    42,     8,     0,     0,    43,   216,    33,    34,     0,    35,
    36,     0,     0,     0,     0,     0,     8,    37,    38,    39,
    40,     0,     0,     0,     0,     0,    31,    41,    32,     0,
     0,    42,     0,     0,     0,    43,   228,    33,    34,     0,
    35,    36,     0,     0,     0,     0,     0,     8,    37,    38,
    39,    40,     0,     0,     0,     0,     0,    31,    41,    32,
     0,     0,    42,     0,     0,   172,    43,     0,    33,    34,
     0,    35,    36,     0,     0,     0,     0,     0,     8,    37,
    38,    39,    40,     0,     0,     0,     0,     0,    31,    41,
    32,     0,     0,    42,     0,     0,   190,    43,     0,    33,
    34,     0,    35,    36,     0,     0,     0,     0,     0,     8,
    37,    38,    39,    40,     0,     0,     0,     0,     0,     0,
    41,     0,     0,     0,    42,     0,     0,     0,    43
};

static const short yycheck[] = {    22,
    26,     0,    91,    87,    91,    13,    48,   120,    56,    55,
     9,    32,    33,    13,    60,    44,    53,    54,    41,    42,
    43,    17,    48,    44,    20,     1,    55,    57,    58,    37,
    35,    36,    59,   120,    37,   119,    59,   150,    56,   151,
    13,    56,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    57,    57,    35,
    36,    84,    88,   150,    37,    59,    56,    56,   180,     0,
     1,    56,   161,    57,   161,     6,     7,    58,     9,    20,
    58,    58,    13,    56,    60,    49,    50,    51,   200,    53,
    54,   203,    91,    59,    57,    26,    27,    28,     4,     5,
     6,     7,    33,    57,   216,    57,    59,   219,    57,    56,
   133,    56,   135,    56,    56,   227,   228,    48,   141,    50,
   162,   163,    56,    56,    56,    55,    57,   239,    56,   241,
    57,    37,   131,    55,    57,    55,   162,   163,    57,     3,
     4,     5,     6,     7,    55,    55,   169,   170,   171,    80,
    59,    24,   175,   176,    61,    60,    60,    88,    60,    90,
    91,    92,   161,    94,     0,     0,     9,   190,    53,   223,
   223,   240,   123,    37,   135,    -1,    -1,    -1,    -1,   202,
    -1,    -1,   205,    -1,    -1,    -1,    -1,   210,     1,   120,
    -1,     4,     5,     6,     7,   218,    60,    -1,   221,    -1,
   131,    -1,    -1,    16,    -1,    18,    -1,    -1,    21,    22,
    23,    -1,    25,    26,    27,    28,    29,    30,    31,   150,
    -1,    34,    -1,    -1,    37,    38,    39,    40,    41,    -1,
   161,   162,   163,    -1,    -1,    48,    -1,    -1,    -1,    52,
    -1,    -1,    55,    56,    -1,     1,    59,    60,     4,     5,
     6,     7,     3,     4,     5,     6,     7,    -1,    -1,    -1,
    16,    -1,    18,    -1,    -1,    21,    22,    23,    -1,    25,
    26,    27,    28,    29,    30,    31,    -1,    -1,    34,    -1,
    -1,    37,    38,    39,    40,    41,    37,    -1,    -1,    -1,
     1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    55,
    56,    -1,    -1,    59,    60,    16,    -1,    18,    -1,    60,
    21,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
    31,    -1,    -1,    34,    35,    36,    37,    38,    39,    40,
    41,    -1,    -1,    -1,    -1,     1,    -1,    48,    -1,    -1,
    -1,    52,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
    16,    -1,    18,    -1,    -1,    21,    22,    23,    -1,    25,
    26,    27,    28,    29,    30,    31,    -1,    -1,    34,    35,
    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
     1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    55,
    56,    -1,    -1,    59,    60,    16,    -1,    18,    -1,    -1,
    21,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
    31,    -1,    -1,    34,    -1,    -1,    37,    38,    39,    40,
    41,    -1,    -1,    -1,    -1,     1,    -1,    48,    -1,    -1,
    -1,    52,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
    16,    -1,    18,    -1,    -1,    21,    22,    23,    -1,    25,
    26,    27,    28,    29,    30,    31,    -1,    -1,    34,    -1,
    -1,    37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
     1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    55,
    56,    -1,    -1,    59,    60,    16,    -1,    18,    -1,    -1,
    21,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
    31,    -1,    -1,    34,    -1,    -1,    37,    38,    39,    40,
    41,    -1,    -1,    -1,    -1,     1,    -1,    48,    -1,    -1,
    -1,    52,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
    16,    -1,    18,    -1,    -1,    21,    22,    23,    -1,    25,
    26,    27,    28,    29,    30,    31,    -1,    -1,    34,    -1,
    -1,    37,    38,    39,    40,    41,     3,     4,     5,     6,
     7,    -1,    48,    -1,    -1,    -1,    52,     9,    10,    55,
    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,     9,    10,
    11,    12,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,
    37,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
    -1,    14,    15,    45,    46,    47,    48,    49,    50,    51,
    -1,    53,    54,    60,    45,    46,    47,    48,    49,    50,
    51,    -1,    53,    54,     9,    10,    11,    12,    -1,    14,
    15,    62,    45,    46,    47,    48,    49,    50,    51,    -1,
    53,    54,    -1,    -1,     9,    10,    11,    12,    61,    14,
    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    45,    46,    47,    48,    49,    50,    51,    -1,    53,    54,
    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
    45,    46,    47,    48,    49,    50,    51,    -1,    53,    54,
    -1,    -1,    57,     9,    10,    11,    12,    -1,    14,    15,
    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,    11,    12,
    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,     9,
    10,    11,    12,    -1,    14,    15,    -1,    -1,    -1,    45,
    46,    47,    48,    49,    50,    51,    -1,    53,    54,    -1,
    -1,    57,    45,    46,    47,    48,    49,    50,    51,    -1,
    53,    54,    -1,    -1,    57,    45,    46,    47,    48,    49,
    50,    51,    -1,    53,    54,    -1,    -1,    57,     9,    10,
    11,    12,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,
    -1,     9,    10,    11,    12,    -1,    14,    15,    -1,    -1,
    -1,    -1,    -1,    -1,     9,    10,    11,    12,    -1,    14,
    15,    -1,    -1,    -1,    45,    46,    47,    48,    49,    50,
    51,    -1,    53,    54,    -1,    -1,    57,    45,    46,    47,
    48,    49,    50,    51,    -1,    53,    54,    -1,    -1,    57,
    45,    46,    47,    48,    49,    50,    51,    -1,    53,    54,
    55,     9,    10,    11,    12,    -1,    14,    15,    -1,    -1,
    -1,    -1,     9,    10,    11,    12,    -1,    14,    15,    -1,
    -1,    -1,    -1,     9,    10,    11,    12,    -1,    14,    15,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
    48,    49,    50,    51,    -1,    53,    54,    55,    45,    46,
    47,    48,    49,    50,    51,    -1,    53,    54,    55,    45,
    46,    47,    48,    49,    50,    51,    -1,    53,    54,    55,
     9,    10,    11,    12,    -1,    14,    15,    -1,    -1,    -1,
    -1,     9,    10,    11,    12,    -1,    14,    15,    -1,    -1,
    -1,    -1,     9,    10,    11,    12,    -1,    14,    15,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,    48,
    49,    50,    51,    -1,    53,    54,    55,    45,    46,    47,
    48,    49,    50,    51,    -1,    53,    54,    55,    45,    46,
    47,    48,    49,    50,    51,    -1,    53,    54,     9,    10,
    11,    12,    -1,    14,    -1,    -1,    -1,    -1,     9,    10,
    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,     9,    10,
    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,    50,
    51,    -1,    53,    54,    45,    46,    47,    48,    49,    50,
    51,    -1,    53,    54,    45,    46,    47,    48,    49,    50,
    51,    -1,    53,    54,    16,    -1,    18,     3,     4,     5,
     6,     7,     8,    -1,    -1,    27,    28,    -1,    30,    31,
    -1,    -1,    -1,    19,    -1,    37,    38,    39,    40,    41,
    -1,    -1,    -1,    -1,    -1,    16,    48,    18,    -1,    -1,
    52,    37,    -1,    -1,    56,    57,    27,    28,    -1,    30,
    31,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    40,
    41,    -1,    -1,    -1,    -1,    -1,    16,    48,    18,    -1,
    -1,    52,    -1,    -1,    -1,    56,    57,    27,    28,    -1,
    30,    31,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,
    40,    41,    -1,    -1,    -1,    -1,    -1,    16,    48,    18,
    -1,    -1,    52,    -1,    -1,    55,    56,    -1,    27,    28,
    -1,    30,    31,    -1,    -1,    -1,    -1,    -1,    37,    38,
    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    16,    48,
    18,    -1,    -1,    52,    -1,    -1,    55,    56,    -1,    27,
    28,    -1,    30,    31,    -1,    -1,    -1,    -1,    -1,    37,
    38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    56
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/local/share/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/usr/local/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 131 "parser.y"
{ 
                                      Program *program = new Program(yyvsp[0].declList);
                                      program->SetParent(NULL);
                                      // if no errors, advance to next phase
                                  //    if (ReportError::NumErrors() == 0) 
                                  //        program->Check(); 
                                      // comment out prev line to skip semantic analysis
                                      // actual code emission
                                  //    if (ReportError::NumErrors() == 0) 
                                  //        program->Emit();
                                    ;
    break;}
case 2:
#line 144 "parser.y"
{ (yyval.declList=yyvsp[-1].declList)->Append(yyvsp[0].decl); ;
    break;}
case 3:
#line 145 "parser.y"
{ (yyval.declList = new List<Decl*>)->Append(yyvsp[0].decl); ;
    break;}
case 4:
#line 148 "parser.y"
{ yyval.decl = yyvsp[0].varDecl; ;
    break;}
case 5:
#line 149 "parser.y"
{ yyval.decl = yyvsp[0].fnDecl; ;
    break;}
case 6:
#line 150 "parser.y"
{ yyval.decl = yyvsp[0].classDecl; ;
    break;}
case 7:
#line 151 "parser.y"
{ yyval.decl = yyvsp[0].ifaceDecl; ;
    break;}
case 8:
#line 154 "parser.y"
{ yyval.varDecl = yyvsp[-1].varDecl; ;
    break;}
case 9:
#line 155 "parser.y"
{ //ReportError::Formatted(&@$, "Cannot assign a value to a variable in declaration. Assignment ignored.\n"); $$ = $1; 
					    ;
    break;}
case 10:
#line 159 "parser.y"
{ yyval.varDecl = new VarDecl(yyvsp[0].identObj, yyvsp[-1].type); ;
    break;}
case 11:
#line 162 "parser.y"
{ yyval.type = Type::intType; ;
    break;}
case 12:
#line 163 "parser.y"
{ yyval.type = Type::doubleType; ;
    break;}
case 13:
#line 164 "parser.y"
{ yyval.type = Type::boolType; ;
    break;}
case 14:
#line 165 "parser.y"
{ yyval.type = Type::stringType; ;
    break;}
case 15:
#line 166 "parser.y"
{ yyval.type = new NamedType(yyvsp[0].identObj); ;
    break;}
case 16:
#line 167 "parser.y"
{ yyval.type = new ArrayType(yylsp[-1], yyvsp[-1].type); ;
    break;}
case 17:
#line 170 "parser.y"
{ yyval.identObj = new Identifier(yylsp[0], yyvsp[0].identifier); ;
    break;}
case 18:
#line 173 "parser.y"
{ yyval.fnDecl = new FnDecl(yyvsp[-4].identObj, yyvsp[-5].type, yyvsp[-2].formals); yyval.fnDecl->SetFunctionBody(yyvsp[0].stmtBlock); ;
    break;}
case 19:
#line 174 "parser.y"
{ yyval.fnDecl = new FnDecl(yyvsp[-4].identObj, Type::voidType, yyvsp[-2].formals); yyval.fnDecl->SetFunctionBody(yyvsp[0].stmtBlock); ;
    break;}
case 20:
#line 177 "parser.y"
{ yyval.formals = yyvsp[0].formals; ;
    break;}
case 21:
#line 178 "parser.y"
{ yyval.formals = new List<VarDecl *>; ;
    break;}
case 22:
#line 181 "parser.y"
{ (yyval.formals = new List<VarDecl *>)->Append(yyvsp[0].varDecl); ;
    break;}
case 23:
#line 182 "parser.y"
{ (yyval.formals = yyvsp[-2].formals)->Append(yyvsp[0].varDecl); ;
    break;}
case 24:
#line 186 "parser.y"
{ yyval.classDecl = new ClassDecl(yyvsp[-4].identObj, NULL, yyvsp[-3].nTList, yyvsp[-1].declList); ;
    break;}
case 25:
#line 188 "parser.y"
{ yyval.classDecl = new ClassDecl(yyvsp[-6].identObj, new NamedType(yyvsp[-4].identObj), yyvsp[-3].nTList, yyvsp[-1].declList); ;
    break;}
case 26:
#line 191 "parser.y"
{ yyval.nTList = yyvsp[0].nTList; ;
    break;}
case 27:
#line 192 "parser.y"
{ yyval.nTList = new List<NamedType *>; ;
    break;}
case 28:
#line 195 "parser.y"
{ (yyval.nTList = new List<NamedType *>)->Append(new NamedType(yyvsp[0].identObj)); ;
    break;}
case 29:
#line 196 "parser.y"
{ (yyval.nTList = yyvsp[-2].nTList)->Append(new NamedType(yyvsp[0].identObj)); ;
    break;}
case 30:
#line 199 "parser.y"
{ (yyval.declList=yyvsp[-1].declList)->Append(yyvsp[0].decl); ;
    break;}
case 31:
#line 200 "parser.y"
{ yyval.declList = new List<Decl*>; ;
    break;}
case 32:
#line 203 "parser.y"
{ yyval.decl = yyvsp[0].varDecl; ;
    break;}
case 33:
#line 204 "parser.y"
{ yyval.decl = yyvsp[0].fnDecl; ;
    break;}
case 34:
#line 207 "parser.y"
{ yyval.ifaceDecl = new InterfaceDecl(yyvsp[-3].identObj, yyvsp[-1].declList); ;
    break;}
case 35:
#line 210 "parser.y"
{ (yyval.declList=yyvsp[-1].declList)->Append(yyvsp[0].decl); ;
    break;}
case 36:
#line 211 "parser.y"
{ yyval.declList = new List<Decl*>; ;
    break;}
case 37:
#line 214 "parser.y"
{ yyval.decl = new FnDecl(yyvsp[-4].identObj, yyvsp[-5].type, yyvsp[-2].formals); ;
    break;}
case 38:
#line 215 "parser.y"
{ yyval.decl = new FnDecl(yyvsp[-4].identObj, Type::voidType, yyvsp[-2].formals); ;
    break;}
case 39:
#line 218 "parser.y"
{ yyval.stmtBlock = new StmtBlock(yyvsp[-2].vardecls, yyvsp[-1].statements); ;
    break;}
case 40:
#line 219 "parser.y"
{ yyval.stmtBlock = new StmtBlock(new List<VarDecl*>, yyvsp[-1].statements); ;
    break;}
case 41:
#line 220 "parser.y"
{ yyval.stmtBlock = new StmtBlock(yyvsp[-1].vardecls, new List<Stmt*>); ;
    break;}
case 42:
#line 221 "parser.y"
{ yyval.stmtBlock = new StmtBlock(new List<VarDecl*>, new List<Stmt*>); ;
    break;}
case 43:
#line 222 "parser.y"
{ 
				         //		ReportError::Formatted(&@$, "Can't parse statement block correctly. Attempting to skip."); $$ = new StmtBlock(new List<VarDecl*>, new List<Stmt*>); 
				    ;
    break;}
case 44:
#line 227 "parser.y"
{ (yyval.vardecls = yyvsp[-1].vardecls)->Append(yyvsp[0].varDecl); ;
    break;}
case 45:
#line 228 "parser.y"
{ (yyval.vardecls = new List<VarDecl*>)->Append(yyvsp[0].varDecl); ;
    break;}
case 46:
#line 231 "parser.y"
{ (yyval.statements = yyvsp[-1].statements)->Append(yyvsp[0].stmt); ;
    break;}
case 47:
#line 232 "parser.y"
{ (yyval.statements = new List<Stmt*>)->Append(yyvsp[0].stmt); ;
    break;}
case 48:
#line 235 "parser.y"
{ yyval.stmt = yyvsp[0].printStmt; ;
    break;}
case 49:
#line 236 "parser.y"
{ yyval.stmt = new EmptyExpr(); ;
    break;}
case 50:
#line 237 "parser.y"
{ yyval.stmt = yyvsp[-1].expr; ;
    break;}
case 51:
#line 238 "parser.y"
{ yyval.stmt = yyvsp[0].ifStmt; ;
    break;}
case 52:
#line 239 "parser.y"
{ yyval.stmt = yyvsp[0].whileStmt; ;
    break;}
case 53:
#line 240 "parser.y"
{ yyval.stmt = yyvsp[0].forStmt; ;
    break;}
case 54:
#line 241 "parser.y"
{ yyval.stmt = yyvsp[0].breakStmt; ;
    break;}
case 55:
#line 242 "parser.y"
{ yyval.stmt = yyvsp[0].returnStmt; ;
    break;}
case 56:
#line 243 "parser.y"
{ yyval.stmt = yyvsp[0].switchStmt; ;
    break;}
case 57:
#line 244 "parser.y"
{ yyval.stmt = yyvsp[0].stmtBlock; ;
    break;}
case 58:
#line 245 "parser.y"
{ 
					//ReportError::Formatted(&@$, "Cannot parse statement. Skipping statement."); $$ = new EmptyExpr(); 
 						  ;
    break;}
case 59:
#line 250 "parser.y"
{ yyval.switchStmt = new SwitchStmt(yyvsp[-5].expr, yyvsp[-2].caseStmts, yyvsp[-1].defStmt); ;
    break;}
case 60:
#line 251 "parser.y"
{ 
				// ReportError::Formatted(&@$, "Need at least one case statement. Skipping entire switch statement block."); $$ = new SwitchStmt($3, new List<Case *>, NULL); 
									;
    break;}
case 61:
#line 254 "parser.y"
{ 
		//ReportError::Formatted(&@$, "Cannot parse switch statement block. Skipping entire switch statement block."); $$ = new SwitchStmt($3, new List<Case *>, NULL); 
							;
    break;}
case 62:
#line 259 "parser.y"
{ (yyval.caseStmts = yyvsp[-1].caseStmts)->Append(yyvsp[0].caseStmt); ;
    break;}
case 63:
#line 260 "parser.y"
{ (yyval.caseStmts = new List<Case*>)->Append(yyvsp[0].caseStmt); ;
    break;}
case 64:
#line 263 "parser.y"
{ yyval.caseStmt = new Case(yyvsp[-2].expr, yyvsp[0].statements); ;
    break;}
case 65:
#line 266 "parser.y"
{ yyval.defStmt = new Default(yyvsp[0].statements); ;
    break;}
case 66:
#line 267 "parser.y"
{ yyval.defStmt = NULL; ;
    break;}
case 67:
#line 270 "parser.y"
{ yyval.statements = yyvsp[0].statements; ;
    break;}
case 68:
#line 271 "parser.y"
{ yyval.statements = new List<Stmt*>; ;
    break;}
case 69:
#line 274 "parser.y"
{ yyval.ifStmt = new IfStmt(yyvsp[-4].expr, yyvsp[-2].stmt, yyvsp[0].stmt); ;
    break;}
case 70:
#line 275 "parser.y"
{ yyval.ifStmt = new IfStmt(yyvsp[-2].expr, yyvsp[0].stmt, NULL); ;
    break;}
case 71:
#line 278 "parser.y"
{ yyval.whileStmt = new WhileStmt(yyvsp[-2].expr, yyvsp[0].stmt); ;
    break;}
case 72:
#line 281 "parser.y"
{ yyval.printStmt = new PrintStmt(yyvsp[-2].exprList); ;
    break;}
case 73:
#line 284 "parser.y"
{ yyval.forStmt = new ForStmt(new EmptyExpr(), yyvsp[-3].expr, new EmptyExpr(), yyvsp[0].stmt); ;
    break;}
case 74:
#line 285 "parser.y"
{ yyval.forStmt = new ForStmt(yyvsp[-5].expr, yyvsp[-3].expr, new EmptyExpr(), yyvsp[0].stmt); ;
    break;}
case 75:
#line 286 "parser.y"
{ yyval.forStmt = new ForStmt(new EmptyExpr(), yyvsp[-4].expr, yyvsp[-2].expr, yyvsp[0].stmt); ;
    break;}
case 76:
#line 287 "parser.y"
{ yyval.forStmt = new ForStmt(yyvsp[-6].expr, yyvsp[-4].expr, yyvsp[-2].expr, yyvsp[0].stmt); ;
    break;}
case 77:
#line 290 "parser.y"
{ /*$$ = new ReturnStmt(@$, new EmptyExpr());*/ ;
    break;}
case 78:
#line 291 "parser.y"
{ /*$$ = new ReturnStmt(@2, $2);*/ ;
    break;}
case 79:
#line 294 "parser.y"
{ yyval.breakStmt = new BreakStmt(yylsp[-1]); ;
    break;}
case 80:
#line 297 "parser.y"
{ (yyval.exprList = yyvsp[-2].exprList)->Append(yyvsp[0].expr); ;
    break;}
case 81:
#line 298 "parser.y"
{ (yyval.exprList = new List<Expr*>)->Append(yyvsp[0].expr); ;
    break;}
case 82:
#line 301 "parser.y"
{ yyval.expr = new AssignExpr(yyvsp[-2].lvalue, new Operator(yylsp[-1], "="), yyvsp[0].expr);  ;
    break;}
case 83:
#line 302 "parser.y"
{ yyval.expr = yyvsp[0].lvalue; ;
    break;}
case 84:
#line 305 "parser.y"
{ yyval.expr = new StringConstant(yylsp[0], yyvsp[0].string); ;
    break;}
case 85:
#line 306 "parser.y"
{ yyval.expr = new IntConstant(yylsp[0], yyvsp[0].integer); ;
    break;}
case 86:
#line 307 "parser.y"
{ yyval.expr = new DoubleConstant(yylsp[0], yyvsp[0].float_value); ;
    break;}
case 87:
#line 308 "parser.y"
{ yyval.expr = new BoolConstant(yylsp[0], yyvsp[0].bool_value); ;
    break;}
case 88:
#line 309 "parser.y"
{ yyval.expr = new NullConstant(yylsp[0]); ;
    break;}
case 89:
#line 311 "parser.y"
{ yyval.expr = new This(yylsp[0]); ;
    break;}
case 90:
#line 312 "parser.y"
{ yyval.expr = yyvsp[0].call; ;
    break;}
case 91:
#line 313 "parser.y"
{ yyval.expr = yyvsp[-1].expr; ;
    break;}
case 92:
#line 314 "parser.y"
{ yyval.expr = new ArithmeticExpr(yyvsp[-2].expr, new Operator(yylsp[-1], "+"), yyvsp[0].expr); ;
    break;}
case 93:
#line 315 "parser.y"
{ yyval.expr = new ArithmeticExpr(yyvsp[-2].expr, new Operator(yylsp[-1], "-"), yyvsp[0].expr); ;
    break;}
case 94:
#line 316 "parser.y"
{ yyval.expr = new ArithmeticExpr(yyvsp[-2].expr, new Operator(yylsp[-1], "*"), yyvsp[0].expr); ;
    break;}
case 95:
#line 317 "parser.y"
{ yyval.expr = new ArithmeticExpr(yyvsp[-2].expr, new Operator(yylsp[-1], "/"), yyvsp[0].expr); ;
    break;}
case 96:
#line 318 "parser.y"
{ yyval.expr = new ArithmeticExpr(yyvsp[-2].expr, new Operator(yylsp[-1], "%"), yyvsp[0].expr); ;
    break;}
case 97:
#line 319 "parser.y"
{ yyval.expr = new ArithmeticExpr(new Operator(yylsp[-1], "-"), yyvsp[0].expr); ;
    break;}
case 98:
#line 320 "parser.y"
{ yyval.expr = new RelationalExpr(yyvsp[-2].expr, new Operator(yylsp[-1], "<"), yyvsp[0].expr); ;
    break;}
case 99:
#line 321 "parser.y"
{ yyval.expr = new RelationalExpr(yyvsp[-2].expr, new Operator(yylsp[-1], "<="), yyvsp[0].expr); ;
    break;}
case 100:
#line 322 "parser.y"
{ yyval.expr = new RelationalExpr(yyvsp[-2].expr, new Operator(yylsp[-1], ">"), yyvsp[0].expr); ;
    break;}
case 101:
#line 323 "parser.y"
{ yyval.expr = new RelationalExpr(yyvsp[-2].expr, new Operator(yylsp[-1], ">="), yyvsp[0].expr); ;
    break;}
case 102:
#line 324 "parser.y"
{ yyval.expr = new EqualityExpr(yyvsp[-2].expr, new Operator(yylsp[-1], "=="), yyvsp[0].expr); ;
    break;}
case 103:
#line 325 "parser.y"
{ yyval.expr = new EqualityExpr(yyvsp[-2].expr, new Operator(yylsp[-1], "!="), yyvsp[0].expr); ;
    break;}
case 104:
#line 326 "parser.y"
{ yyval.expr = new LogicalExpr(yyvsp[-2].expr, new Operator(yylsp[-1], "&&"), yyvsp[0].expr); ;
    break;}
case 105:
#line 327 "parser.y"
{ yyval.expr = new LogicalExpr(yyvsp[-2].expr, new Operator(yylsp[-1], "||"), yyvsp[0].expr); ;
    break;}
case 106:
#line 328 "parser.y"
{ yyval.expr = new LogicalExpr(new Operator(yylsp[-1], "!"), yyvsp[0].expr); ;
    break;}
case 107:
#line 329 "parser.y"
{ yyval.expr = new PostfixExpr(yyvsp[-1].lvalue, new Operator(yylsp[0], "++")); ;
    break;}
case 108:
#line 330 "parser.y"
{ yyval.expr = new PostfixExpr(yyvsp[-1].lvalue, new Operator(yylsp[0], "--")); ;
    break;}
case 109:
#line 331 "parser.y"
{ /*$$ = new ReadIntegerExpr(@$);*/ ;
    break;}
case 110:
#line 332 "parser.y"
{ /*$$ = new ReadLineExpr(@$);*/ ;
    break;}
case 111:
#line 333 "parser.y"
{ /*$$ = new NewExpr(@$, new NamedType($2));*/ ;
    break;}
case 112:
#line 334 "parser.y"
{ /*$$ = new NewArrayExpr(@$, $3, $5);*/ ;
    break;}
case 113:
#line 337 "parser.y"
{ /*$$ = new Call(@$, NULL, $1, $3);*/ ;
    break;}
case 114:
#line 338 "parser.y"
{ /*$$ = new Call(@$, $1, $3, $5); */;
    break;}
case 115:
#line 341 "parser.y"
{ yyval.exprList = yyvsp[0].exprList; ;
    break;}
case 116:
#line 342 "parser.y"
{ yyval.exprList = new List<Expr*>; ;
    break;}
case 117:
#line 345 "parser.y"
{ (yyval.exprList = new List<Expr*>)->Append(yyvsp[0].expr); ;
    break;}
case 118:
#line 346 "parser.y"
{ (yyval.exprList = yyvsp[-2].exprList)->Append(yyvsp[0].expr); ;
    break;}
case 119:
#line 349 "parser.y"
{ yyval.lvalue = new FieldAccess(NULL, yyvsp[0].identObj); ;
    break;}
case 120:
#line 350 "parser.y"
{ yyval.lvalue = new FieldAccess(yyvsp[-2].expr, yyvsp[0].identObj); ;
    break;}
case 121:
#line 351 "parser.y"
{ /*$$ = new ArrayAccess(@$, $1, $3);*/ ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/usr/local/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 354 "parser.y"


void InitParser()
{
   PrintDebug("parser", "Initializing parser");
   yydebug = false;
}
