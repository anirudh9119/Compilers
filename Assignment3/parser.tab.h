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


extern YYSTYPE yylval;
