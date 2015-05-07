#include<bits/stdc++.h>
#include<iostream>
#include<stdlib.h>
#include<string>
#include<map>
#include<list>
#include "list.h"
#include "hashtable.h"
#include "location.h"
#include "codegen.h"
#include "mips.h"
#include "tac.h"
class Type;
class NamedType;
class Identifier;
class Stmt;
class FnDecl;


class Decl;
class VarDecl;
class Expr;
class Location;
class CodeGenerator;

class NamedType; // for new
class Type; // for NewArray


class Decl;
class NamedType;


//class Decl;

class Node  {
  protected:
    yyltype *location;
    Node *parent;

  public:
    Node(yyltype loc);
    Node();
    virtual ~Node() {}

    yyltype *GetLocation()   { return location; }
    void SetParent(Node *p)  { parent = p; }
    Node *GetParent()        { return parent; }
};


class Identifier : public Node
{
  protected:
    char *name;

  public:
    Identifier(yyltype loc, const char *name);
    friend ostream& operator<<(ostream& out, Identifier *id) { return out << id->name; }
    const char* GetName() { return name; }
};


class Decl : public Node
{
  protected:
    Identifier *id;

  public:
    Decl(Identifier *name);
    friend ostream& operator<<(ostream& out, Decl *d) { return out << d->id; }
    const char* GetName() { return id->GetName(); }
    Identifier* GetIdentifier() { return id; }
    virtual void Check(Hashtable<Decl *>* symbolTable) = 0;
    virtual bool CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable) = 0;

    virtual void Emit(CodeGenerator* cg) = 0;
};

class VarDecl : public Decl
{
  protected:
    Type *type;

  public:
    VarDecl(Identifier *name, Type *type);
    Type* GetType() { return type; }
    virtual void Check(Hashtable<Decl *>* symbolTable);
    virtual bool CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable);

    virtual void Emit(CodeGenerator* cg);
};

class ClassDecl : public Decl
{
  protected:
    List<Decl*> *members;
    NamedType *extends;
    List<NamedType*> *implements;

  public:
    List<VarDecl *>* allVariables;
    List<FnDecl *>* allMethods;
    List<List<FnDecl *>*>* allInterfaces;
    List<const char *>* ifName;

    int varOffset;
    int objectSize;

    void EmitHeader(CodeGenerator* cg, List<const char*>* names, int offsetCount);
    void AddSymbols(Hashtable<Decl *>* symbolTable, bool checkConflict, bool recurse);
    ClassDecl(Identifier *name, NamedType *extends,
              List<NamedType*> *implements, List<Decl*> *members);
    NamedType* GetExtends() { return extends; }
    List<NamedType*>* GetImplements() { return implements; }
    virtual void Check(Hashtable<Decl *>* symbolTable);
    virtual bool CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable) { return false; }
    void RecursiveAddList(List<VarDecl*>* v, List<FnDecl*>* m, List<List<FnDecl*>*>* i, List<const char *>* ifname);
    void RecursiveInterfaceAdd(List<VarDecl*>* v, List<FnDecl*>* m, List<List<FnDecl *>*>* interf, List<const char *>* ifname);
    virtual void Emit(CodeGenerator* cg);
};

class InterfaceDecl : public Decl
{
  protected:
    List<Decl*> *members;

  public:
    List<const char*>* allMethods;

    InterfaceDecl(Identifier *name, List<Decl*> *members);
    List<Decl*>* GetMembers() { return members; }
    virtual void Check(Hashtable<Decl *>* symbolTable);
    virtual bool CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable) { return false; }

    virtual void Emit(CodeGenerator* cg);
};

class FnDecl : public Decl
{
  protected:
    List<VarDecl*> *formals;
    Type *returnType;
    Stmt *body;

  public:
    FnDecl(Identifier *name, Type *returnType, List<VarDecl*> *formals);
    void SetFunctionBody(Stmt *b);
    virtual void Check(Hashtable<Decl *>* symbolTable);
    static bool CheckSignature(FnDecl *o_f, FnDecl *c_f);

    List<VarDecl*>* GetFormals() { return formals; }
    Type* GetReturnType() { return returnType; }
    virtual bool CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable);
    FnDecl* MakeCopy() { FnDecl* n = new FnDecl(id, returnType, formals); n->SetFunctionBody(body); return n; }
    virtual void Emit(CodeGenerator* cg);
};





class Program : public Node
{
  protected:
     List<Decl*> *decls;

  public:
     Program(List<Decl*> *declList);
     void Check();
     void Emit();
};

class Stmt : public Node
{
  public:
     char* exitLabel;

     Stmt() : Node() {}
     Stmt(yyltype loc) : Node(loc) {}
     virtual Type* Check(Hashtable<Decl*>* symbolTable) = 0;
     virtual Location* Emit(CodeGenerator *cg) = 0;
};

class StmtBlock : public Stmt
{
  protected:
    List<VarDecl*> *decls;
    List<Stmt*> *stmts;

  public:
    StmtBlock(List<VarDecl*> *variableDeclarations, List<Stmt*> *statements);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};


class ConditionalStmt : public Stmt
{
  protected:
    Expr *test;
    Stmt *body;

  public:
    ConditionalStmt(Expr *testExpr, Stmt *body);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class LoopStmt : public ConditionalStmt
{
  public:
    LoopStmt(Expr *testExpr, Stmt *body) : ConditionalStmt(testExpr, body) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable) = 0;
    virtual Location* Emit(CodeGenerator *cg) = 0;
};

class ForStmt : public LoopStmt
{
  protected:
    Expr *init, *step;

  public:
    ForStmt(Expr *init, Expr *test, Expr *step, Stmt *body);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class WhileStmt : public LoopStmt
{
  public:
    WhileStmt(Expr *test, Stmt *body) : LoopStmt(test, body) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class IfStmt : public ConditionalStmt
{
  protected:
    Stmt *elseBody;

  public:
    IfStmt(Expr *test, Stmt *thenBody, Stmt *elseBody);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class BreakStmt : public Stmt
{
  public:
    BreakStmt(yyltype loc) : Stmt(loc) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class ReturnStmt : public Stmt
{
  protected:
    Expr *expr;

  public:
    bool noReturn;
    ReturnStmt(yyltype loc, Expr *expr);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class PrintStmt : public Stmt
{
  protected:
    List<Expr*> *args;

  public:
    PrintStmt(List<Expr*> *arguments);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};
class Default : public Stmt
{
  protected:
    List<Stmt*>* s;

  public:
    Default(List<Stmt*>* statements);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class Case : public Stmt
{
  protected:
    Expr* e;
    List<Stmt*>* s;

  public:
    char* excLabel;
    Case(Expr* expr, List<Stmt*>* statements);
    Expr* GetExpr() { return e; }
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class SwitchStmt : public Stmt
{
  protected:
    Expr* e;
    List<Case*>* c;
    Default* d;

  public:
    SwitchStmt(Expr* exp, List<Case*>* cases, Default* def); // def can be null
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};
class Expr : public Stmt 
{
  public:
    Expr(yyltype loc) : Stmt(loc) {}
    Expr() : Stmt() {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
    Type* cachedType;
};

/* This node type is used for those places where an expression is optional.
 * We could use a NULL pointer, but then it adds a lot of checking for
 * NULL. By using a valid, but no-op, node, we save that trouble */

class EmptyExpr : public Expr
{
  public:
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class IntConstant : public Expr 
{
  protected:
    int value;
  
  public:
    int GetValue() { return value; }
    IntConstant(yyltype loc, int val);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class DoubleConstant : public Expr 
{
  protected:
    double value;
    
  public:
    DoubleConstant(yyltype loc, double val);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class BoolConstant : public Expr 
{
  protected:
    bool value;
    
  public:
    BoolConstant(yyltype loc, bool val);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class StringConstant : public Expr 
{ 
  protected:
    char *value;
    
  public:
    StringConstant(yyltype loc, const char *val);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class NullConstant: public Expr 
{
  public: 
    NullConstant(yyltype loc) : Expr(loc) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class ErrorConstant: public Expr 
{
  public: 
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class Operator : public Node 
{
  protected:
    char tokenString[4];
    
  public:
    Operator(yyltype loc, const char *tok);
    friend ostream& operator<<(ostream& out, Operator *o) { return out << o->tokenString; }
    virtual Type* Check(Hashtable<Decl*>* symbolTable) { return NULL; }
    virtual Location* Emit(CodeGenerator *cg) { return NULL; }
    char* GetToken() { return tokenString; }
};

class PostfixExpr : public Expr
{
  protected:
    Operator *op;
    Expr *left;
    
  public:
    PostfixExpr(Expr *lhs, Operator *op);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class CompoundExpr : public Expr
{
  protected:
    Operator *op;
    Expr *left, *right; // left will be NULL if unary
    
  public:
    CompoundExpr(Expr *lhs, Operator *op, Expr *rhs);  // for binary
    CompoundExpr(Operator *op, Expr *rhs);             // for unary
    virtual Type* Check(Hashtable<Decl*>* symbolTable) = 0;
    virtual Location* Emit(CodeGenerator *cg) = 0;
};

class ArithmeticExpr : public CompoundExpr 
{
  public:
    ArithmeticExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs,op,rhs) {}
    ArithmeticExpr(Operator *op, Expr *rhs) : CompoundExpr(op,rhs) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class RelationalExpr : public CompoundExpr 
{
  public:
    RelationalExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs,op,rhs) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class EqualityExpr : public CompoundExpr 
{
  public:
    EqualityExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs,op,rhs) {}
    const char *GetPrintNameForNode() { return "EqualityExpr"; }
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class LogicalExpr : public CompoundExpr 
{
  public:
    LogicalExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs,op,rhs) {}
    LogicalExpr(Operator *op, Expr *rhs) : CompoundExpr(op,rhs) {}
    const char *GetPrintNameForNode() { return "LogicalExpr"; }
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class AssignExpr : public CompoundExpr 
{
  public:
    AssignExpr(Expr *lhs, Operator *op, Expr *rhs) : CompoundExpr(lhs,op,rhs) { isLocalVariable = false; leftType = NULL; rightType = NULL; }
    const char *GetPrintNameForNode() { return "AssignExpr"; }
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);

    Type* leftType;
    Type* rightType;
    bool isLocalVariable;
};

class LValue : public Expr 
{
  public:
    LValue(yyltype loc) : Expr(loc) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable) = 0;
    virtual Location* Emit(CodeGenerator *cg) = 0;
};

class This : public Expr 
{
  public:
    This(yyltype loc) : Expr(loc) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class ArrayAccess : public LValue 
{
  protected:
    Expr *base, *subscript;
    
  public:
    ArrayAccess(yyltype loc, Expr *base, Expr *subscript);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
    Location* EmitMemoryLocation(CodeGenerator *cg);
};

/* Note that field access is used both for qualified names
 * base.field and just field without qualification. We don't
 * know for sure whether there is an implicit "this." in
 * front until later on, so we use one node type for either
 * and sort it out later. */
class FieldAccess : public LValue 
{
  protected:
    Expr *base;	// will be NULL if no explicit base
    Identifier *field;
    
  public:
    FieldAccess(Expr *base, Identifier *field); //ok to pass NULL base
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
    Type* baseType;
    Location* EmitReference(CodeGenerator *cg);
};

/* Like field access, call is used both for qualified base.field()
 * and unqualified field().  We won't figure out until later
 * whether we need implicit "this." so we use one node type for either
 * and sort it out later. */
class Call : public Expr 
{
  protected:
    Expr *base;	// will be NULL if no explicit base
    Identifier *field;
    List<Expr*> *actuals;
    
  public:
    Call(yyltype loc, Expr *base, Identifier *field, List<Expr*> *args);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);

    // For Code Generator
    char* fullyQualifiedFnName;
    bool isClassFunction;
    bool hasReturnType;
    bool isArrayLengthCheck;
    
    Decl* baseDecl;
};

class NewExpr : public Expr
{
  protected:
    NamedType *cType;
    
  public:
    NewExpr(yyltype loc, NamedType *clsType);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class NewArrayExpr : public Expr
{
  protected:
    Expr *size;
    Type *elemType;
    
  public:
    NewArrayExpr(yyltype loc, Expr *sizeExpr, Type *elemType);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class ReadIntegerExpr : public Expr
{
  public:
    ReadIntegerExpr(yyltype loc) : Expr(loc) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class ReadLineExpr : public Expr
{
  public:
    ReadLineExpr(yyltype loc) : Expr (loc) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};



class Type : public Node 
{
  protected:
    char *typeName;

  public :
    static Type *intType, *doubleType, *boolType, *voidType,
                *nullType, *stringType, *errorType;

    Type(yyltype loc) : Node(loc) {}
    Type(const char *str);

    virtual const char* GetName() { return typeName; }
    virtual void PrintToStream(ostream& out) { out << typeName; }
    friend ostream& operator<<(ostream& out, Type *t) { t->PrintToStream(out); return out; }
    virtual bool IsEquivalentTo(Type *other) { return this == other; }

    static bool ValidityChecker(Hashtable<Decl *>* symbolTable, Type* type);
    static bool TypeEquivalence(Type* a, Type *b, bool strict, Hashtable<Decl *>* symbolTable = NULL,  bool lcbOneWayCheck = true);
    static Type* LeastUpperBound(NamedType* a, NamedType* b, bool checkGreater = false); 
    
    virtual bool IsErrorType() { return strcmp(this->GetName(), Type::errorType->GetName()) == 0; }
    virtual bool IsNullType() { return strcmp(this->GetName(), Type::nullType->GetName()) == 0; }
    
    virtual bool Equivalence(Type* other, bool strict, Hashtable<Decl *>* parentST, bool lcbCheckGreater);
};

class NamedType : public Type 
{
  protected:
    Identifier *id;
    
  public:
    NamedType(Identifier *i);
    virtual const char* GetName() { return id->GetName(); }
    Identifier* GetIdentifier() { return id; }
    void PrintToStream(ostream& out) { out << id; }
    
    virtual bool IsErrorType() { return false; }
    virtual bool IsNullType() { return false; }
    
    virtual bool Equivalence(Type* other, bool strict, Hashtable<Decl *>* parentST, bool lcbCheckGreater);
};

class ArrayType : public Type 
{
  protected:
    Type *elemType;

  public:
    ArrayType(yyltype loc, Type *elemType);
    Type* BaseType() { return elemType; }
    void PrintToStream(ostream& out) { out << elemType << "[]"; }
    
    virtual const char* GetName() { return "Array"; }
    virtual bool IsErrorType() { return false; }
    virtual bool IsNullType() { return false; }
    
    virtual bool Equivalence(Type* other, bool strict, Hashtable<Decl *>* parentST, bool lcbCheckGreater);
};

