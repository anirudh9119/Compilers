typedef union {
    int integerConstant;
    bool boolConstant;
    char *stringConstant;
    double doubleConstant;
    char identifier[MaxIdentLen+1]; // +1 for terminating null
    Decl *decl;
    List<Decl*> *declList;
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

#define	T_Void	258
#define	T_Bool	259
#define	T_Int	260
#define	T_Double	261
#define	T_String	262
#define	T_Class	263
#define	T_LessEqual	264
#define	T_GreaterEqual	265
#define	T_Equal	266
#define	T_NotEqual	267
#define	T_Dims	268
#define	T_And	269
#define	T_Or	270
#define	T_Null	271
#define	T_Extends	272
#define	T_This	273
#define	T_Interface	274
#define	T_Implements	275
#define	T_While	276
#define	T_For	277
#define	T_If	278
#define	T_Else	279
#define	T_Return	280
#define	T_Break	281
#define	T_New	282
#define	T_NewArray	283
#define	T_Print	284
#define	T_ReadInteger	285
#define	T_ReadLine	286
#define	T_Identifier	287
#define	T_StringConstant	288
#define	T_IntConstant	289
#define	T_DoubleConstant	290
#define	T_BoolConstant	291


extern YYSTYPE yylval;
