%{
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
%}

%union {
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
};
%token   Void Bool Int Double String Class 
%token   LessEqual GreaterEqual Equal NotEqual Dims
%token   And Or Null Extends This Interface Implements
%token   While For If Else Return Break
%token   New NewArray Print ReadInteger ReadLine
%token   Increment Decrement Switch Case Default

%token   <identifier> T_Identifier
%token   <string> StringConstant 
%token   <integer> IntConstant
%token   <float_value> DoubleConstant
%token   <bool_value> BoolConstant

%token   UnaryMinus
%token   NoElse

%type <declList>   DeclList 
%type <decl>       Decl
%type <type>       Type
%type <varDecl>    VariableDecl
%type <varDecl>    Variable
%type <fnDecl>     FunctionDecl
%type <formals>    OptFormals
%type <formals>    Formals
%type <identObj>   Identifier
%type <declList>   FieldList
%type <decl>       Field
%type <nTList>     ImplementsDecl
%type <nTList>     ImplementsList
%type <classDecl>  ClassDecl
%type <decl>       Prototype
%type <declList>   PrototypeList
%type <ifaceDecl>  InterfaceDecl
%type <vardecls>   VarDeclList
%type <stmtBlock>  StmtBlock
%type <statements> StmtList
%type <stmt>       Stmt
%type <printStmt>  PrintStmt
%type <exprList>   ExprList
%type <expr>       Expr
%type <lvalue>     LValue
%type <exprList>   Actuals
%type <exprList>   OptActuals
%type <call>       Call
%type <ifStmt>     IfStmt
%type <whileStmt>  WhileStmt
%type <forStmt>    ForStmt
%type <returnStmt> ReturnStmt
%type <breakStmt>  BreakStmt
%type <switchStmt> SwitchStmt
%type <caseStmts>  CaseStatements
%type <caseStmt>   CaseStatement
%type <defStmt>    DefaultStatement
%type <statements> OptStmtList

/* Precedence Assignment
 * ---------------------
 * Here we do some precedence assignments. Ones at the botoom have higher
 * priority than those at the top 
 */

%nonassoc   '='
%left       Or
%left       And
%nonassoc   Equal NotEqual
%nonassoc   '<' LessEqual '>' GreaterEqual
%left       '+' '-'
%left       '*' '/' '%'
%nonassoc   '!' UnaryMinus Increment Decrement
%nonassoc   '[' '.'
%nonassoc   NoElse
%nonassoc   Else

%%
/* Rules
 * -----
 * All productions and actions should be placed between the start and stop
 * %% markers which delimit the Rules section.
	 
 */
Program           :    DeclList    { 
                                      Program *program = new Program($1);
                                      program->SetParent(NULL);
                                      // if no errors, advance to next phase
                                  //    if (ReportError::NumErrors() == 0) 
                                  //        program->Check(); 
                                      // comment out prev line to skip semantic analysis
                                      // actual code emission
                                  //    if (ReportError::NumErrors() == 0) 
                                  //        program->Emit();
                                    }
                  ;

DeclList          :    DeclList Decl        { ($$=$1)->Append($2); }
                  |    Decl                 { ($$ = new List<Decl*>)->Append($1); }
                  ;

Decl              :    VariableDecl         { $$ = $1; }
                  |    FunctionDecl         { $$ = $1; }
                  |    ClassDecl            { $$ = $1; }
                  |    InterfaceDecl        { $$ = $1; }
                  ;

VariableDecl      : Variable ';'            { $$ = $1; }
                  | Variable '=' Expr ';'   { //ReportError::Formatted(&@$, "Cannot assign a value to a variable in declaration. Assignment ignored.\n"); $$ = $1; 
					    }
                  ;

Variable          : Type Identifier         { $$ = new VarDecl($2, $1); }
                  ;

Type              : Int                   { $$ = Type::intType; }
                  | Double                { $$ = Type::doubleType; }
                  | Bool                  { $$ = Type::boolType; }
                  | String                { $$ = Type::stringType; }
                  | Identifier              { $$ = new NamedType($1); }
                  | Type Dims             { $$ = new ArrayType(@1, $1); }
                  ;

Identifier        : T_Identifier            { $$ = new Identifier(@1, $1); }
                  ;

FunctionDecl      : Type Identifier '(' OptFormals ')' StmtBlock     { $$ = new FnDecl($2, $1, $4); $$->SetFunctionBody($6); }
                  | Void Identifier '(' OptFormals ')' StmtBlock   { $$ = new FnDecl($2, Type::voidType, $4); $$->SetFunctionBody($6); }
                  ;

OptFormals        : Formals                 { $$ = $1; }
                  |                         { $$ = new List<VarDecl *>; }
                  ;

Formals           : Variable                { ($$ = new List<VarDecl *>)->Append($1); }
                  | Formals ',' Variable    { ($$ = $1)->Append($3); }
                  ; 

ClassDecl         : Class Identifier ImplementsDecl '{' FieldList '}'
                    { $$ = new ClassDecl($2, NULL, $3, $5); }
                  | Class Identifier Extends Identifier ImplementsDecl '{' FieldList '}'
                    { $$ = new ClassDecl($2, new NamedType($4), $5, $7); }
                  ;

ImplementsDecl    : Implements ImplementsList       { $$ = $2; }
                  |                                   { $$ = new List<NamedType *>; }
                  ;
                  
ImplementsList    : Identifier                        { ($$ = new List<NamedType *>)->Append(new NamedType($1)); }
                  | ImplementsList ',' Identifier     { ($$ = $1)->Append(new NamedType($3)); }
                  ;
                  
FieldList         : FieldList Field       { ($$=$1)->Append($2); }
                  |                       { $$ = new List<Decl*>; }
                  ;

Field             : VariableDecl          { $$ = $1; }
                  | FunctionDecl          { $$ = $1; }
                  ;

InterfaceDecl     : Interface Identifier '{' PrototypeList '}'      { $$ = new InterfaceDecl($2, $4); }
                  ;

PrototypeList     : PrototypeList Prototype    { ($$=$1)->Append($2); }
                  |                            { $$ = new List<Decl*>; }
                  ;

Prototype         : Type Identifier '(' OptFormals ')' ';'   { $$ = new FnDecl($2, $1, $4); }
                  | Void Identifier '(' OptFormals ')' ';' { $$ = new FnDecl($2, Type::voidType, $4); }
                  ;

StmtBlock         : '{' VarDeclList StmtList '}'   { $$ = new StmtBlock($2, $3); }
                  | '{' StmtList '}'               { $$ = new StmtBlock(new List<VarDecl*>, $2); }
                  | '{' VarDeclList '}'            { $$ = new StmtBlock($2, new List<Stmt*>); }
                  | '{' '}'                        { $$ = new StmtBlock(new List<VarDecl*>, new List<Stmt*>); }
                  | '{' error '}'  { 
				         //		ReportError::Formatted(&@$, "Can't parse statement block correctly. Attempting to skip."); $$ = new StmtBlock(new List<VarDecl*>, new List<Stmt*>); 
				    }
                  ;

VarDeclList       : VarDeclList VariableDecl    { ($$ = $1)->Append($2); }
                  | VariableDecl                { ($$ = new List<VarDecl*>)->Append($1); }
                  ;

StmtList          : StmtList Stmt               { ($$ = $1)->Append($2); }
                  | Stmt                        { ($$ = new List<Stmt*>)->Append($1); }
                  ;

Stmt              : PrintStmt                     { $$ = $1; }
                  | ';'                           { $$ = new EmptyExpr(); }
                  | Expr ';'                      { $$ = $1; }
                  | IfStmt                        { $$ = $1; }
                  | WhileStmt                     { $$ = $1; }
                  | ForStmt                       { $$ = $1; }
                  | BreakStmt                     { $$ = $1; }
                  | ReturnStmt                    { $$ = $1; }
                  | SwitchStmt                    { $$ = $1; }
                  | StmtBlock                     { $$ = $1; }
                  | error ';'                     { 
					//ReportError::Formatted(&@$, "Cannot parse statement. Skipping statement."); $$ = new EmptyExpr(); 
 						  }
                  ;
                  
SwitchStmt        : Switch '(' Expr ')' '{' CaseStatements DefaultStatement '}'  { $$ = new SwitchStmt($3, $6, $7); }
                  | Switch '(' Expr ')' '{' DefaultStatement '}' { 
				// ReportError::Formatted(&@$, "Need at least one case statement. Skipping entire switch statement block."); $$ = new SwitchStmt($3, new List<Case *>, NULL); 
									}
                  | Switch '(' Expr ')' '{' error '}' { 
		//ReportError::Formatted(&@$, "Cannot parse switch statement block. Skipping entire switch statement block."); $$ = new SwitchStmt($3, new List<Case *>, NULL); 
							}
                  ;

CaseStatements    : CaseStatements CaseStatement            { ($$ = $1)->Append($2); }
                  | CaseStatement                           { ($$ = new List<Case*>)->Append($1); }
                  ;

CaseStatement     : Case Expr ':' OptStmtList             { $$ = new Case($2, $4); }
                  ;

DefaultStatement  : Default ':' OptStmtList               { $$ = new Default($3); }
                  |                                         { $$ = NULL; }
                  ;

OptStmtList       : StmtList                                { $$ = $1; }
                  |                                         { $$ = new List<Stmt*>; }
                  ;

IfStmt            : If '(' Expr ')' Stmt Else Stmt       { $$ = new IfStmt($3, $5, $7); }
                  | If '(' Expr ')' Stmt %prec NoElse    { $$ = new IfStmt($3, $5, NULL); }
                  ;

WhileStmt         : While '(' Expr ')' Stmt     { $$ = new WhileStmt($3, $5); }
                  ;

PrintStmt         : Print '(' ExprList ')' ';'  { $$ = new PrintStmt($3); }
                  ;

ForStmt           : For '(' ';' Expr ';' ')' Stmt               { $$ = new ForStmt(new EmptyExpr(), $4, new EmptyExpr(), $7); }
                  | For '(' Expr ';' Expr ';' ')' Stmt          { $$ = new ForStmt($3, $5, new EmptyExpr(), $8); }
                  | For '(' ';' Expr ';' Expr ')' Stmt          { $$ = new ForStmt(new EmptyExpr(), $4, $6, $8); }
                  | For '(' Expr ';' Expr ';' Expr ')' Stmt     { $$ = new ForStmt($3, $5, $7, $9); }
                  ;

ReturnStmt        : Return ';'      { /*$$ = new ReturnStmt(@$, new EmptyExpr());*/ }
                  | Return Expr ';' { /*$$ = new ReturnStmt(@2, $2);*/ }
                  ;

BreakStmt         : Break ';' { $$ = new BreakStmt(@1); }
                  ;

ExprList          : ExprList ',' Expr             { ($$ = $1)->Append($3); }
                  | Expr                          { ($$ = new List<Expr*>)->Append($1); }
                  ;

Expr              : LValue '=' Expr               { $$ = new AssignExpr($1, new Operator(@2, "="), $3);  }
                  | LValue                        { $$ = $1; }
                  
                  /* Constants here */
                  | StringConstant              { $$ = new StringConstant(@1, $1); }
                  | IntConstant                 { $$ = new IntConstant(@1, $1); }
                  | DoubleConstant              { $$ = new DoubleConstant(@1, $1); }
                  | BoolConstant                { $$ = new BoolConstant(@1, $1); }
                  | Null                        { $$ = new NullConstant(@1); }
                  
                  | This                        { $$ = new This(@1); }
                  | Call                          { $$ = $1; }
                  | '(' Expr ')'                  { $$ = $2; }
                  | Expr '+' Expr                 { $$ = new ArithmeticExpr($1, new Operator(@2, "+"), $3); }
                  | Expr '-' Expr                 { $$ = new ArithmeticExpr($1, new Operator(@2, "-"), $3); }
                  | Expr '*' Expr                 { $$ = new ArithmeticExpr($1, new Operator(@2, "*"), $3); }
                  | Expr '/' Expr                 { $$ = new ArithmeticExpr($1, new Operator(@2, "/"), $3); }
                  | Expr '%' Expr                 { $$ = new ArithmeticExpr($1, new Operator(@2, "%"), $3); }
                  | '-' Expr %prec UnaryMinus   { $$ = new ArithmeticExpr(new Operator(@1, "-"), $2); }
                  | Expr '<' Expr                 { $$ = new RelationalExpr($1, new Operator(@2, "<"), $3); }
                  | Expr LessEqual Expr         { $$ = new RelationalExpr($1, new Operator(@2, "<="), $3); }
                  | Expr '>' Expr                 { $$ = new RelationalExpr($1, new Operator(@2, ">"), $3); }
                  | Expr GreaterEqual Expr      { $$ = new RelationalExpr($1, new Operator(@2, ">="), $3); }
                  | Expr Equal Expr             { $$ = new EqualityExpr($1, new Operator(@2, "=="), $3); }
                  | Expr NotEqual Expr          { $$ = new EqualityExpr($1, new Operator(@2, "!="), $3); }
                  | Expr And Expr               { $$ = new LogicalExpr($1, new Operator(@2, "&&"), $3); }
                  | Expr Or Expr                { $$ = new LogicalExpr($1, new Operator(@2, "||"), $3); }
                  | '!' Expr                      { $$ = new LogicalExpr(new Operator(@1, "!"), $2); }
                  | LValue Increment              { $$ = new PostfixExpr($1, new Operator(@2, "++")); }
                  | LValue Decrement              { $$ = new PostfixExpr($1, new Operator(@2, "--")); }
                  | ReadInteger '(' ')'         { /*$$ = new ReadIntegerExpr(@$);*/ }
                  | ReadLine '(' ')'            { /*$$ = new ReadLineExpr(@$);*/ }
                  | New Identifier              { /*$$ = new NewExpr(@$, new NamedType($2));*/ }
                  | NewArray '(' Expr ',' Type ')'  { /*$$ = new NewArrayExpr(@$, $3, $5);*/ }
                  ;

Call              : Identifier '(' OptActuals ')'            { /*$$ = new Call(@$, NULL, $1, $3);*/ }
                  | Expr '.' Identifier '(' OptActuals ')'   { /*$$ = new Call(@$, $1, $3, $5); */}
                  ;

OptActuals        : Actuals                       { $$ = $1; }
                  |                               { $$ = new List<Expr*>; }
                  ;

Actuals           : Expr                          { ($$ = new List<Expr*>)->Append($1); }
                  | Actuals ',' Expr              { ($$ = $1)->Append($3); }
                  ;

LValue            : Identifier                    { $$ = new FieldAccess(NULL, $1); }
                  | Expr '.' Identifier           { $$ = new FieldAccess($1, $3); }
                  | Expr '[' Expr ']'             { /*$$ = new ArrayAccess(@$, $1, $3);*/ }
                  ;

%%

void InitParser()
{
   PrintDebug("parser", "Initializing parser");
   yydebug = false;
}
