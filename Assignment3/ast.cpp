#include "ast.h"
#include <string.h> // strdup
#include <stdio.h>  // printf
#include "list.h"
#include "hashtable.h"
#include "tac.h" 

Node::Node(yyltype loc) {
    location = new yyltype(loc);
    parent = NULL;
}

Node::Node() {
    location = NULL;
    parent = NULL;
}

Identifier::Identifier(yyltype loc, const char *n) : Node(loc) {
    name = strdup(n);
}

extern Hashtable<Decl *>* __globalST;

Program::Program(List<Decl*> *d) {
    Assert(d != NULL);
    (decls=d)->SetParentAll(this);
}

void Program::Check() {
    Hashtable<Decl *>* symbolTable = new Hashtable<Decl *>();

    /* Insert and check for identifier consistency */
    for(int i = 0; i < decls->NumElements(); i++) {
        const char* declName = decls->Nth(i)->GetName();

        /* Checks whether the id exists and if it does, if the id is at the same level */
        if(symbolTable->Lookup(declName) == NULL || symbolTable->Lookup(declName)->GetParent() != this) {
            symbolTable->Enter(declName, decls->Nth(i));
        } else {
            ReportError::DeclConflict(decls->Nth(i), symbolTable->Lookup(declName));
        }
    }

    __globalST = symbolTable->MakeCopy();

    /* Recursively check subcomponents */
    for(int i = 0; i < decls->NumElements(); i++) {
        decls->Nth(i)->Check(symbolTable);
    }
}

void Program::Emit() {
    CodeGenerator* cg = new CodeGenerator();
    Hashtable<Location *>* next = new Hashtable<Location *>(); 
    cg->varLocs->Append(next);
    
    for(int i = 0; i < decls->NumElements(); i++) {
        if(dynamic_cast<VarDecl *>(decls->Nth(i)) != NULL)
            decls->Nth(i)->Emit(cg);
    }
    
    for(int i = 0; i < decls->NumElements(); i++) {
        if(dynamic_cast<VarDecl *>(decls->Nth(i)) == NULL)
            decls->Nth(i)->Emit(cg);
    }

    if(__globalST->Lookup("main") == NULL) {
        ReportError::NoMainFound();
    } else {
        #ifdef DCC_OPTIMIZATION
        cg->OptimizeIR();
        #endif
        cg->DoFinalCodeGen();
    }
}



StmtBlock::StmtBlock(List<VarDecl*> *d, List<Stmt*> *s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

Type* StmtBlock::Check(Hashtable<Decl*>* parentST) { 
    Hashtable<Decl*>* symbolTable = parentST->MakeCopy();

    /* Check declaration validity */
    for(int i = 0; i < decls->NumElements(); i++) {
        const char* declName = decls->Nth(i)->GetName();
        Type::ValidityChecker(parentST, decls->Nth(i)->GetType());
        /* Checks whether the id exists and if it does, if the id is at the same level */
        if(symbolTable->Lookup(declName) == NULL || symbolTable->Lookup(declName)->GetParent() != this) {
            symbolTable->Enter(declName, decls->Nth(i));
        } else {
            ReportError::DeclConflict(decls->Nth(i), symbolTable->Lookup(declName));
        }
    }
    
    for(int i = 0; i < stmts->NumElements(); i++) {
        stmts->Nth(i)->Check(symbolTable);
    }

    delete symbolTable;
    return NULL;
}

Location* StmtBlock::Emit(CodeGenerator *cg) {
    cg->NewScope();

    for(int i = 0; i < decls->NumElements(); i++) {
        decls->Nth(i)->Emit(cg);
    }
    
    for(int i = 0; i < stmts->NumElements(); i++) {
        stmts->Nth(i)->Emit(cg);
    }

    cg->RemoveScope();
    return NULL;
}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) { 
    Assert(t != NULL && b != NULL);
    (test=t)->SetParent(this); 
    (body=b)->SetParent(this);
}

Type* ConditionalStmt::Check(Hashtable<Decl*>* parentST) {
    Hashtable<Decl*>* symbolTable = parentST->MakeCopy(); 

    Type* t = test->Check(symbolTable);
    if(!Type::TypeEquivalence(t, Type::boolType, true)) ReportError::TestNotBoolean(test);
    body->Check(symbolTable);

    delete symbolTable;
    return NULL;
}

Location* ConditionalStmt::Emit(CodeGenerator *cg) {
    return NULL;
}

ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b): LoopStmt(t, b) { 
    Assert(i != NULL && t != NULL && s != NULL && b != NULL);
    (init=i)->SetParent(this);
    (step=s)->SetParent(this);
}

Type* ForStmt::Check(Hashtable<Decl*>* parentST) { 
    Hashtable<Decl*>* symbolTable = parentST->MakeCopy();

    init->Check(symbolTable);
    Type* t = test->Check(symbolTable);
    if(!Type::TypeEquivalence(t, Type::boolType, true)) ReportError::TestNotBoolean(test);
    step->Check(symbolTable);
    body->Check(symbolTable);

    delete symbolTable;
    return NULL;
}

Location* ForStmt::Emit(CodeGenerator *cg) {
    init->Emit(cg);
    char* loopStartL = cg->NewLabel();
    char* loopEndL = exitLabel = cg->NewLabel();
    
    cg->GenLabel(loopStartL);
    Location* testL = test->Emit(cg);
    cg->GenIfZ(testL, loopEndL);
    
    body->Emit(cg);
    step->Emit(cg);
    cg->GenGoto(loopStartL);
    
    cg->GenLabel(loopEndL);
    return NULL;
}

Type* WhileStmt::Check(Hashtable<Decl*>* parentST) {  
    Hashtable<Decl*>* symbolTable = parentST->MakeCopy(); 

    Type* t = test->Check(symbolTable);
    if(!Type::TypeEquivalence(t, Type::boolType, true)) ReportError::TestNotBoolean(test);
    body->Check(symbolTable);

    delete symbolTable;
    return NULL;
}

Location* WhileStmt::Emit(CodeGenerator *cg) {
    char* loopStartL = cg->NewLabel();
    char* loopEndL = exitLabel = cg->NewLabel();
    
    cg->GenLabel(loopStartL);
    Location* testL = test->Emit(cg);
    cg->GenIfZ(testL, loopEndL);
    
    body->Emit(cg);
    cg->GenGoto(loopStartL);
    
    cg->GenLabel(loopEndL);
    return NULL;
}

Type* BreakStmt::Check(Hashtable<Decl*>* parentST) {
    
    Node* p = this;
    while((p = p->GetParent()) != NULL)
        if(dynamic_cast<WhileStmt *>(p) != NULL || 
           dynamic_cast<ForStmt *>(p) != NULL || 
           dynamic_cast<Case *>(p) != NULL ||
           dynamic_cast<Default *>(p) != NULL)
            return NULL;

    ReportError::BreakOutsideLoop(this);
    return NULL;
}

Location* BreakStmt::Emit(CodeGenerator *cg) {
    Node* p = this;
    while((p = p->GetParent()) != NULL)
        if(dynamic_cast<WhileStmt *>(p) != NULL || 
           dynamic_cast<ForStmt *>(p) != NULL || 
           dynamic_cast<Case *>(p) != NULL ||
           dynamic_cast<Default *>(p) != NULL) {
             cg->GenGoto(dynamic_cast<Stmt *>(p)->exitLabel);
             break;
        }
    return NULL;
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) { 
    Assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}

Type* IfStmt::Check(Hashtable<Decl*>* parentST) { 
    Hashtable<Decl*>* symbolTable = parentST->MakeCopy(); 
    
    Type* t = test->Check(symbolTable);
    if(!Type::TypeEquivalence(t, Type::boolType, true)) ReportError::TestNotBoolean(test);
    
    body->Check(symbolTable);
    if(elseBody != NULL) 
        elseBody->Check(symbolTable);

    delete symbolTable;
    return NULL;
}

Location* IfStmt::Emit(CodeGenerator *cg) {
    Location* testL = test->Emit(cg);
    char* nextLabel = cg->NewLabel();
    
    if(elseBody != NULL) {
        char* elseLabel = cg->NewLabel();
        cg->GenIfZ(testL, elseLabel);
        body->Emit(cg);
        cg->GenGoto(nextLabel);
        cg->GenLabel(elseLabel);
        elseBody->Emit(cg);
    } else {
        cg->GenIfZ(testL, nextLabel);
        body->Emit(cg);
    }

    cg->GenLabel(nextLabel);
    return NULL;
}

ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) { 
    Assert(e != NULL);
    (expr=e)->SetParent(this);
}

Type* ReturnStmt::Check(Hashtable<Decl*>* parentST) {
    Hashtable<Decl*>* symbolTable = parentST->MakeCopy();

    Node* p = this;
    while((p = p->GetParent()) != NULL)
        if(dynamic_cast<FnDecl *>(p) != NULL)
            break;

    FnDecl* fn = dynamic_cast<FnDecl *>(p);
    Type* t = expr->Check(symbolTable);
    
    if(t == Type::voidType) noReturn = true; else noReturn = false;

    if(!Type::TypeEquivalence(t, fn->GetReturnType(), false, symbolTable))
        ReportError::ReturnMismatch(this, t, fn->GetReturnType());

    delete symbolTable;
    return NULL;
}

Location* ReturnStmt::Emit(CodeGenerator *cg) {
    if(noReturn) {
        cg->GenReturn();
    } else {
        Location* l = expr->Emit(cg);
        cg->GenReturn(l);
    }
    return NULL;
}

PrintStmt::PrintStmt(List<Expr*> *a) {    
    Assert(a != NULL);
    (args=a)->SetParentAll(this);
}

Type* PrintStmt::Check(Hashtable<Decl*>* parentST) {
    for(int i = 0; i < args->NumElements(); ++i) {
        Type* t = args->Nth(i)->Check(parentST);
        if(!Type::TypeEquivalence(t, Type::stringType, true) && 
           !Type::TypeEquivalence(t, Type::boolType, true) &&
           !Type::TypeEquivalence(t, Type::intType, true)) {
            ReportError::PrintArgMismatch(args->Nth(i), i + 1, t);
        }
        args->Nth(i)->cachedType = t;
    }
    return NULL;
}

Location* PrintStmt::Emit(CodeGenerator *cg) {
    for(int i = 0; i < args->NumElements(); ++i) {
        Type* t = args->Nth(i)->cachedType;
        Location* l = args->Nth(i)->Emit(cg);
        if(Type::TypeEquivalence(t, Type::stringType, true)) {
            cg->GenBuiltInCall(PrintString, l, NULL);
        } else if (Type::TypeEquivalence(t, Type::boolType, true)) {
            cg->GenBuiltInCall(PrintBool, l, NULL);
        } else if (Type::TypeEquivalence(t, Type::intType, true)) {
            cg->GenBuiltInCall(PrintInt, l, NULL);
        }
    }
    return NULL;
}

SwitchStmt::SwitchStmt(Expr* exp, List<Case*>* cases, Default* def) {
    Assert(exp != NULL && cases != NULL);
    e = exp;
    c = cases;
    d = def;
    e->SetParent(this);
    c->SetParentAll(this);
    if(d != NULL) d->SetParent(this);
}

Type* SwitchStmt::Check(Hashtable<Decl*>* parentST) {
    Type *t_e = e->Check(parentST);
    if(!Type::TypeEquivalence(t_e, Type::intType, true)) 
        ReportError::ArgMismatch(e, 1, t_e, Type::intType);
    for(int i = 0; i < c->NumElements(); ++i) {
        c->Nth(i)->Check(parentST);
    }
    if(d != NULL)
        d->Check(parentST);
    return NULL;
}

Location* SwitchStmt::Emit(CodeGenerator *cg) {
    Location* exprL = e->Emit(cg);
    Location* zeroL = cg->GenLoadConstant(0);
    char* endLabel = cg->NewLabel();
    char* defaultLabel = cg->NewLabel();
    
    for(int i = 0; i < c->NumElements(); i++) {
        Location* myL = c->Nth(i)->GetExpr()->Emit(cg);
        c->Nth(i)->excLabel = cg->NewLabel();
        cg->GenIfZ(cg->GenBinaryOp("==", cg->GenBinaryOp("==", exprL, myL), zeroL), c->Nth(i)->excLabel);
    }
    
    if(d != NULL) {
        cg->GenGoto(defaultLabel);
    } else {
        cg->GenGoto(endLabel);
    }
    
    for(int i = 0; i < c->NumElements(); i++) {
        cg->GenLabel(c->Nth(i)->excLabel);
        c->Nth(i)->exitLabel = endLabel;
        c->Nth(i)->Emit(cg);
    }
    
    if(d != NULL) {
        cg->GenLabel(defaultLabel);
        d->Emit(cg);
    }
    
    cg->GenLabel(endLabel);
    
    return NULL;
}

Case::Case(Expr* expr, List<Stmt*>* statements) {
    Assert(expr != NULL && statements != NULL);
    e = expr;
    s = statements;
    s->SetParentAll(this);
}

Type* Case::Check(Hashtable<Decl*>* parentST) { 
    Type* z = e->Check(parentST);
    if(!Type::TypeEquivalence(z, Type::intType, true)) {
	ReportError::ArgMismatch(e, 0, z, Type::intType);
    }
    for(int i = 0; i < s->NumElements(); i++) {
        s->Nth(i)->Check(parentST);
    }
    return NULL;
}

Location* Case::Emit(CodeGenerator *cg) {
    for(int i = 0; i < s->NumElements(); i++) {
        s->Nth(i)->Emit(cg);
    }
    return NULL;
}

Default::Default(List<Stmt*>* statements) {
    Assert(statements != NULL);
    s = statements;
    s->SetParentAll(this);
}

Type* Default::Check(Hashtable<Decl*>* parentST) { 
    for(int i = 0; i < s->NumElements(); i++) {
        s->Nth(i)->Check(parentST);
    }
    return NULL;
}

Location* Default::Emit(CodeGenerator *cg) {
    for(int i = 0; i < s->NumElements(); i++) {
        s->Nth(i)->Emit(cg);
    }
    return NULL;
}







Decl::Decl(Identifier *n) : Node(*n->GetLocation()) {
    Assert(n != NULL);
    (id=n)->SetParent(this); 
}


VarDecl::VarDecl(Identifier *n, Type *t) : Decl(n) {
    Assert(n != NULL && t != NULL);
    (type=t)->SetParent(this);
}

void VarDecl::Emit(CodeGenerator* cg) {
    // Either a StmtBlock or a ClassDecl or Program
    if(ClassDecl* parent = dynamic_cast<ClassDecl*>(this->GetParent())) {
        // TODO: Decl in class
    } else if (StmtBlock* parent = dynamic_cast<StmtBlock*>(this->GetParent())) {
        Location* l = cg->GetNewLocationOnStack(id->GetName());
        cg->InsertLocation(id->GetName(), l);
    } else {
        Location* l = cg->GenGlobalVar(id->GetName());
        cg->InsertLocation(id->GetName(), l);
    }
}

void VarDecl::Check(Hashtable<Decl *>* symbolTable) {
    Type::ValidityChecker(symbolTable, type);
}

ClassDecl::ClassDecl(Identifier *n, NamedType *ex, List<NamedType*> *imp, List<Decl*> *m) : Decl(n) {
    // extends can be NULL, impl & mem may be empty lists but cannot be NULL
    Assert(n != NULL && imp != NULL && m != NULL);     
    extends = ex;
    if (extends) extends->SetParent(this);
    (implements=imp)->SetParentAll(this);
    (members=m)->SetParentAll(this);
}

void ClassDecl::RecursiveAddList(List<VarDecl*>* v, List<FnDecl*>* m, List<List<FnDecl *>*>* interf, List<const char *>* ifname) {
    if(extends != NULL) {
        ClassDecl* parent = dynamic_cast<ClassDecl *>(__globalST->Lookup(extends->GetName()));
        if(parent != NULL) {
            parent->RecursiveAddList(v, m, interf, ifname);
        }
    }

    for(int i = 0; i < members->NumElements(); i++) {
        if(FnDecl* fn = dynamic_cast<FnDecl*>(members->Nth(i))) {
            bool found = false;
            for(int i = 0; i < m->NumElements(); i++) 
                if(strcmp(m->Nth(i)->GetName(), fn->GetName()) == 0) {
                    found = true;
                    m->RemoveAt(i);
                    m->InsertAt(fn, i);
                }
            if(!found)
                m->Append(fn);
        } else if(VarDecl* vd = dynamic_cast<VarDecl*>(members->Nth(i))) {
            v->Append(vd);
        }
    }
}

void ClassDecl::RecursiveInterfaceAdd(List<VarDecl*>* v, List<FnDecl*>* m, List<List<FnDecl *>*>* interf, List<const char *>* ifname) {
    if(extends != NULL) {
        ClassDecl* parent = dynamic_cast<ClassDecl *>(__globalST->Lookup(extends->GetName()));
        if(parent != NULL) {
            parent->RecursiveInterfaceAdd(v, m, interf, ifname);
        }
    }

    for(int i = 0; i < implements->NumElements(); ++i) {
        InterfaceDecl* imDecl = dynamic_cast<InterfaceDecl *>(__globalST->Lookup(implements->Nth(i)->GetName()));
        ifname->Append(strdup(imDecl->GetName()));
        List<Decl*>* mem = imDecl->GetMembers();
        List<FnDecl*>* currI = new List<FnDecl*>();

        for(int j = 0; j < mem->NumElements(); j++) {
            FnDecl* i_f = dynamic_cast<FnDecl *>(mem->Nth(j));
            
            for(int k = 0; k < m->NumElements(); k++) {
                if(strcmp(m->Nth(k)->GetName(), i_f->GetName()) == 0) {
                    currI->Append(m->Nth(k));
                    break;
                }
            }
        }
        interf->Append(currI);
    }
}

void ClassDecl::EmitHeader(CodeGenerator* cg, List<const char*>* names, int offsetCount) {
    char tmpName[100];
    snprintf(tmpName, 100, "_offset_%s_%d", this->GetName(), offsetCount);
    tmpName[99] = '\0';
    
    cg->GenLabel(tmpName);
    BeginFunc* bf = cg->GenBeginFunc();
    cg->GenReturn(cg->GenLoadConstant(offsetCount));
    bf->SetFrameSize(cg->currentStackSize * 4);
    cg->GenEndFunc();

    names->Append(strdup(tmpName));
}

void ClassDecl::Emit(CodeGenerator* cg) {
    int offsetCount = 0;

    List<const char *>* names = new List<const char *>();
    
    EmitHeader(cg, names, offsetCount);
    offsetCount += 4;

    for(int i = 0; i < allMethods->NumElements(); i++) {
        if(allMethods->Nth(i)->GetParent() == this) {
            string tmpName = "f_";
            tmpName += dynamic_cast<Decl*>(allMethods->Nth(i)->GetParent())->GetName();
            tmpName += ".";
            tmpName += allMethods->Nth(i)->GetName();
            names->Append(strdup(tmpName.c_str()));
        } else {
            FnDecl* myFn = allMethods->Nth(i)->MakeCopy();
            myFn->SetParent(this);
            allMethods->RemoveAt(i);
            allMethods->InsertAt(myFn, i);
            
            string tmpName = "f_";
            tmpName += dynamic_cast<Decl*>(allMethods->Nth(i)->GetParent())->GetName();
            tmpName += ".";
            tmpName += allMethods->Nth(i)->GetName();
            names->Append(strdup(tmpName.c_str()));
            
            members->Append(myFn);
        }
    }
    
    cg->GenVTable(this->GetName(), names);
    for(int j = 0; j < ifName->NumElements(); j++) {
        string vtableName;
        vtableName += this->GetName();
        vtableName += "_";
        vtableName += ifName->Nth(j);

        List<const char *>* names = new List<const char *>();
        
        EmitHeader(cg, names, offsetCount);
        offsetCount += 4;
        
        for(int i = 0; i < allInterfaces->Nth(j)->NumElements(); i++) {
            const char* fnName = allInterfaces->Nth(j)->Nth(i)->GetName();
            for(int k = 0; k < allMethods->NumElements(); k++) {
                if(strcmp(allMethods->Nth(k)->GetName(), fnName) == 0) {
                    string tmpName = "f_";
                    tmpName += dynamic_cast<Decl*>(allMethods->Nth(k)->GetParent())->GetName();
                    tmpName += ".";
                    tmpName += allMethods->Nth(k)->GetName();
                    names->Append(strdup(tmpName.c_str()));
                }
            }
        }
        cg->GenVTable(strdup(vtableName.c_str()), names);
    }

    for(int i = 0; i < members->NumElements(); i++) {
        if(FnDecl* fn = dynamic_cast<FnDecl*>(members->Nth(i))) {
            fn->Emit(cg);
        }
    }
}

bool VarDecl::CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable) {
    if(dynamic_cast<ClassDecl*>(this->GetParent()) != NULL) {
        ReportError::DeclConflict(newDecl, this);
        return false;
    }
    return true;
}

bool FnDecl::CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable) {
    //FnDecl* o_f = this;
    FnDecl* c_f = dynamic_cast<FnDecl*>(newDecl);
    
    if(dynamic_cast<ClassDecl*>(this->GetParent()) != NULL) {
        if(c_f == NULL || this->GetParent() == newDecl->GetParent()) {
            ReportError::DeclConflict(newDecl, this);
            return false;
        }

        if(!FnDecl::CheckSignature(this, c_f)) {
            ReportError::OverrideMismatch(newDecl);
            return false;
        }
    }
    return true;
}

void ClassDecl::AddSymbols(Hashtable<Decl *>* symbolTable, bool checkConflict, bool recurse) {
    if(recurse && extends != NULL) {
        ClassDecl* parent = dynamic_cast<ClassDecl *>(__globalST->Lookup(extends->GetName()));
        if(parent != NULL) {
            parent->AddSymbols(symbolTable, checkConflict, recurse);
        }
    }

    for(int i = 0; i < members->NumElements(); ++i) {
        const char* declName = members->Nth(i)->GetName();
        if(!checkConflict || symbolTable->Lookup(declName) == NULL) {
            symbolTable->Enter(declName, members->Nth(i));
        } else {
            Decl* d = symbolTable->Lookup(declName);
            if(d != NULL && !d->CheckCompatibilityInClass(members->Nth(i), symbolTable)) continue;
            symbolTable->Enter(declName, members->Nth(i));
        }
    }
}

void ClassDecl::Check(Hashtable<Decl *>* parentST) {
    Hashtable<Decl *>* symbolTable = parentST->MakeCopy();

    /* First, we check whether extends and implements are being implemented from correctly declared things */
    if(extends != NULL) {
        Decl* exDecl = symbolTable->Lookup(extends->GetName());
        if(exDecl == NULL || dynamic_cast<ClassDecl *>(exDecl) == NULL)
            ReportError::IdentifierNotDeclared(extends->GetIdentifier(), LookingForClass);

        /* Check recursive whether extends form a loop */
        Hashtable<Decl*> h_temp;
        h_temp.Enter(this->GetName(), this);
        while(true) {
            if(exDecl == NULL || dynamic_cast<ClassDecl *>(exDecl) == NULL) break;
            ClassDecl* cdec = dynamic_cast<ClassDecl *>(exDecl);
            if(h_temp.Lookup(cdec->GetName()) != NULL) {
                ReportError::Formatted(this->GetIdentifier()->GetLocation(), "The inheritance hierarchy of this class forms a loop.");
                return;
            }
            h_temp.Enter(cdec->GetName(), cdec);
            if(cdec->GetExtends() != NULL)
                exDecl = symbolTable->Lookup(cdec->GetExtends()->GetName());
            else
                exDecl = NULL;
        }
    }

    
    for(int i = 0; i < implements->NumElements(); ++i) {
        Decl* imDecl = symbolTable->Lookup(implements->Nth(i)->GetName());
        if(imDecl == NULL || dynamic_cast<InterfaceDecl *>(imDecl) == NULL)
            ReportError::IdentifierNotDeclared(implements->Nth(i)->GetIdentifier(), LookingForInterface);
    }

    /* Add everything ever inherited this into the class, starting from the top */
    if(extends != NULL) {
        ClassDecl* c = dynamic_cast<ClassDecl *>(symbolTable->Lookup(extends->GetName()));
        if(c != NULL) c->AddSymbols(symbolTable, false, true);
    }
    
    /* Add everything implemented in this class itself and check for dups and inheritance */
    this->AddSymbols(symbolTable, true, false);

    /* Now check whether interfaces are implemented properly */
    for(int i = 0; i < implements->NumElements(); ++i) {
        InterfaceDecl* imDecl = dynamic_cast<InterfaceDecl *>(symbolTable->Lookup(implements->Nth(i)->GetName()));
        if(imDecl != NULL) {
            List<Decl*>* m = imDecl->GetMembers();
            for(int j = 0; j < m->NumElements(); j++) {
                FnDecl* i_f = dynamic_cast<FnDecl *>(m->Nth(j));
                FnDecl* m_f = dynamic_cast<FnDecl *>(symbolTable->Lookup(m->Nth(j)->GetName()));

                if(m_f == NULL || dynamic_cast<ClassDecl *>(m_f->GetParent()) == NULL) {
                    ReportError::InterfaceNotImplemented(this, implements->Nth(i));
                    continue;
                }
                
                if(!FnDecl::CheckSignature(m_f, i_f)) {
                    ReportError::OverrideMismatch(m_f);
                    continue;
                }
            }
        }
    }

    /* Recursively check whether each function / var decls is proper */
    for(int i = 0; i < members->NumElements(); ++i) {
        members->Nth(i)->Check(symbolTable);
    }

    /* NOTE NOTE NOTE : CODE GEN CODE GEN CODE GEN PRELIM WORKS */
    allVariables = new List<VarDecl *>();
    allMethods = new List<FnDecl *>();
    allInterfaces = new List<List<FnDecl *>*>();
    ifName = new List<const char *>();

    RecursiveAddList(allVariables, allMethods, allInterfaces, ifName);
    RecursiveInterfaceAdd(allVariables, allMethods, allInterfaces, ifName);

    this->varOffset = (allInterfaces->NumElements() + 1) * 4;
    this->objectSize = (allInterfaces->NumElements() + 1 + allVariables->NumElements()) * 4;
    
    /* NOTE NOTE NOTE : CODE GEN CODE GEN CODE GEN PRELIM WORKS */

    delete symbolTable;
}

InterfaceDecl::InterfaceDecl(Identifier *n, List<Decl*> *m) : Decl(n) {
    Assert(n != NULL && m != NULL);
    (members=m)->SetParentAll(this);
}

void InterfaceDecl::Emit(CodeGenerator* cg) {
    allMethods = new List<const char *>();
    
    for(int i = 0; i < members->NumElements(); i++) {
        if(FnDecl* fn = dynamic_cast<FnDecl*>(members->Nth(i))) {
            allMethods->Append(strdup(fn->GetName()));
        } 
    }
}

void InterfaceDecl::Check(Hashtable<Decl *>* parentST) {
    Hashtable<Decl *>* symbolTable = parentST->MakeCopy();

    /* Insert and check for identifier consistency */
    for(int i = 0; i < members->NumElements(); i++) {
        const char* declName = members->Nth(i)->GetName();

        /* Checks whether the id exists and if it does, if the id is at the same level */
        if(symbolTable->Lookup(declName) == NULL || symbolTable->Lookup(declName)->GetParent() != this) {
            symbolTable->Enter(declName, members->Nth(i));
        } else {
            ReportError::DeclConflict(members->Nth(i), symbolTable->Lookup(declName));
        }
    }

    /* Recursively check function declarations */
    for(int i = 0; i < members->NumElements(); i++) {
        members->Nth(i)->Check(symbolTable);
    }
    
    delete symbolTable;
}

FnDecl::FnDecl(Identifier *n, Type *r, List<VarDecl*> *d) : Decl(n) {
    Assert(n != NULL && r!= NULL && d != NULL);
    (returnType=r)->SetParent(this);
    (formals=d)->SetParentAll(this);
    body = NULL;
}

void FnDecl::Emit(CodeGenerator* cg) {
    Assert(body != NULL);

    cg->NewScope();

    if(ClassDecl* p = dynamic_cast<ClassDecl*>(this->GetParent())) {
        string tmp = "f_";
        tmp += p->GetName();
        tmp += ".";
        tmp += this->GetName();
        cg->GenLabel(tmp.c_str());
    } else {
        string tmp = "f_";
        tmp += id->GetName();
        cg->GenLabel(tmp.c_str());
        // We generate two labels right next to each other for main so main can be recursive too
        // without too many special cases littered around
        if(strcmp(id->GetName(), "main") == 0)
            cg->GenLabel(id->GetName());
    }
    BeginFunc* bf = cg->GenBeginFunc();

    int globals = 0;

    if(dynamic_cast<ClassDecl *>(this->GetParent())) {
        cg->InsertLocation("this", new Location(fpRelative, 4 + globals * 4, "this"));
        globals++;
    }

    for(int i = 0; i < formals->NumElements(); ++i) {
        cg->InsertLocation(formals->Nth(i)->GetName(), new Location(fpRelative, 4 + globals * 4, formals->Nth(i)->GetName()));
        globals++;
    }

    body->Emit(cg);

    bf->SetFrameSize(cg->currentStackSize * 4);
    cg->GenEndFunc();
    
    cg->RemoveScope();
}

void FnDecl::Check(Hashtable<Decl *>* parentST) {
    Hashtable<Decl *>* symbolTable = parentST->MakeCopy();

    /* Verify return type validity */
    Type::ValidityChecker(symbolTable, returnType);

    /* Verify formals and variable declaration validity */
    for(int i = 0; i < formals->NumElements(); ++i) {
        const char* declName = formals->Nth(i)->GetName();
        Type::ValidityChecker(symbolTable, formals->Nth(i)->GetType());
        if(symbolTable->Lookup(declName) == NULL || symbolTable->Lookup(declName)->GetParent() != this) {
            symbolTable->Enter(declName, formals->Nth(i));
        } else {
            ReportError::DeclConflict(formals->Nth(i), symbolTable->Lookup(declName));
        }
    }

    /* Check statement block */
    if(body != NULL) {
        body->Check(symbolTable);
    }
    
    delete symbolTable;
}

bool FnDecl::CheckSignature(FnDecl *o_f, FnDecl *c_f) {
    /* We need to check for strict type adherence now */
    if(!Type::TypeEquivalence(o_f->GetReturnType(), c_f->GetReturnType(), true)) {
        return false;
    }

    if(o_f->GetFormals()->NumElements() != c_f->GetFormals()->NumElements()) {
        return false;
    }

    for(int j = 0; j < o_f->GetFormals()->NumElements(); j++) {
        if(!Type::TypeEquivalence(o_f->GetFormals()->Nth(j)->GetType(), c_f->GetFormals()->Nth(j)->GetType(), true)) {
            return false;
        }
    }
    
    return true;
}

void FnDecl::SetFunctionBody(Stmt *b) { 
    (body=b)->SetParent(this);
}

Type* Expr::Check(Hashtable<Decl*>* parentST) { 
    return NULL;
}

Location* Expr::Emit(CodeGenerator *cg) {
    return NULL;
}

Type* EmptyExpr::Check(Hashtable<Decl*>* parentST) {
    return Type::voidType;
}

Location* EmptyExpr::Emit(CodeGenerator *cg) {
    return NULL;
}

Type* NullConstant::Check(Hashtable<Decl*>* parentST) {
    return Type::nullType;
}

Location* NullConstant::Emit(CodeGenerator *cg) {
    return NULL;
}

IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
}

Type* IntConstant::Check(Hashtable<Decl*>* parentST) { 
    return Type::intType;
}

Location* IntConstant::Emit(CodeGenerator *cg) {
    return cg->GenLoadConstant(value);
}

DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}

Type* DoubleConstant::Check(Hashtable<Decl*>* parentST) {
    return Type::doubleType;
}

Location* DoubleConstant::Emit(CodeGenerator *cg) {
    return NULL;
}

Type* ErrorConstant::Check(Hashtable<Decl*>* parentST) {
    return Type::errorType;
}

Location* ErrorConstant::Emit(CodeGenerator *cg) {
    return NULL;
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}

Type* BoolConstant::Check(Hashtable<Decl*>* parentST) { 
    return Type::boolType;
}

Location* BoolConstant::Emit(CodeGenerator *cg) {
    return cg->GenLoadConstant(value ? 1 : 0);
}

StringConstant::StringConstant(yyltype loc, const char *val) : Expr(loc) {
    Assert(val != NULL);
    value = strdup(val);
}

Type* StringConstant::Check(Hashtable<Decl*>* parentST) { 
    return Type::stringType;
}

Location* StringConstant::Emit(CodeGenerator *cg) {
    return cg->GenLoadConstant(value);
}

PostfixExpr::PostfixExpr(Expr *l, Operator *o) 
  : Expr(Join(l->GetLocation(), o->GetLocation())) {
    Assert(l != NULL && o != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this); 
}

Type* PostfixExpr::Check(Hashtable<Decl*>* parentST) { 
    /* NOTE: Need to be integer */
    if(!Type::TypeEquivalence(left->Check(parentST), Type::intType, true)) {
        ReportError::IncompatibleOperand(op, left->Check(parentST));
        return Type::errorType;
    }
    return Type::intType;
}

Location* PostfixExpr::Emit(CodeGenerator *cg) {
    Location* one = cg->GenLoadConstant(1);

    // need to handle what L is a local/global variable or something I malloc'd
    if(dynamic_cast<ArrayAccess*>(left) != NULL) {
        Location* l_ref = dynamic_cast<ArrayAccess*>(left)->EmitMemoryLocation(cg);
        Location* l = cg->GenLoad(l_ref);
        if(strcmp(op->GetToken(), "++") == 0) {
            cg->GenStore(l_ref, cg->GenBinaryOp("+", one, l));
        } else {
            cg->GenStore(l_ref, cg->GenBinaryOp("-", one, l));
        }
        return l;
    } else {
        if(FieldAccess* fa = dynamic_cast<FieldAccess *>(left)) {
            Location* l_ref = fa->EmitReference(cg);
            if(l_ref == NULL) {
                Location* l = fa->Emit(cg);
                Location* tmp = cg->GenTempVar();
                cg->GenAssign(tmp, l);
                if(strcmp(op->GetToken(), "++") == 0)
                    cg->GenAssign(l, cg->GenBinaryOp("+", tmp, one));
                else
                    cg->GenAssign(l, cg->GenBinaryOp("-", tmp, one));
                return tmp;
            } else {
                Location* l = cg->GenLoad(l_ref);
                if(strcmp(op->GetToken(), "++") == 0) {
                    cg->GenStore(l_ref, cg->GenBinaryOp("+", one, l));
                } else {
                    cg->GenStore(l_ref, cg->GenBinaryOp("-", one, l));
                }
                return l;
            }
        } else {
            Location* l = left->Emit(cg);
            Location* tmp = cg->GenTempVar();
            cg->GenAssign(tmp, l);
            if(strcmp(op->GetToken(), "++") == 0)
                cg->GenAssign(l, cg->GenBinaryOp("+", tmp, one));
            else
                cg->GenAssign(l, cg->GenBinaryOp("-", tmp, one));
            return tmp;
        } 
    }
    return NULL;
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r) 
  : Expr(Join(l->GetLocation(), r->GetLocation())) {
    Assert(l != NULL && o != NULL && r != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this); 
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r) 
  : Expr(Join(o->GetLocation(), r->GetLocation())) {
    Assert(o != NULL && r != NULL);
    left = NULL; 
    (op=o)->SetParent(this);
    (right=r)->SetParent(this);
}
  
ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base=b)->SetParent(this); 
    (subscript=s)->SetParent(this);
}

Type* ArrayAccess::Check(Hashtable<Decl*>* parentST) {
    Type* b_t = base->Check(parentST);
    Type* s_t = subscript->Check(parentST);

    if(dynamic_cast<ArrayType *>(b_t) == NULL) {
        ReportError::BracketsOnNonArray(base);
        return Type::errorType;
    }
    
    if(!Type::TypeEquivalence(s_t, Type::intType, true)) {
        ReportError::SubscriptNotInteger(subscript);
        return Type::errorType;
    }
    
    return (dynamic_cast<ArrayType *>(b_t))->BaseType();
}

Location* ArrayAccess::Emit(CodeGenerator *cg) {
    return cg->GenLoad(this->EmitMemoryLocation(cg));
}

Location* ArrayAccess::EmitMemoryLocation(CodeGenerator *cg) {
    Location* baseL = base->Emit(cg);
    Location* sscrL = subscript->Emit(cg);
    
    Location* zeroL = cg->GenLoadConstant(0);
    Location* fourL = cg->GenLoadConstant(4);
    
    char* contLabel = cg->NewLabel();
    
    Location* sizeL = cg->GenLoad(baseL, 0);
    
    // Runtime Check
    Location* tempL = cg->GenBinaryOp("||", 
                          cg->GenBinaryOp("<", sscrL, zeroL), 
                          cg->GenBinaryOp("||", 
                              cg->GenBinaryOp("<", sizeL, sscrL),
                              cg->GenBinaryOp("==", sizeL, sscrL)
                          )
                      );
    
    cg->GenIfZ(tempL, contLabel);
    cg->GenBuiltInCall(PrintString, cg->GenLoadConstant(err_arr_out_of_bounds), NULL);
    cg->GenBuiltInCall(Halt, NULL, NULL);
    
    cg->GenLabel(contLabel);
    Location* fL = cg->GenBinaryOp("+", baseL, cg->GenBinaryOp("+", cg->GenBinaryOp("*", sscrL, fourL), fourL));
    fL->locationRef = true;
    return fL;
}

FieldAccess::FieldAccess(Expr *b, Identifier *f) 
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b; 
    if (base) base->SetParent(this); 
    (field=f)->SetParent(this);
    baseType = NULL;
}

Type* FieldAccess::Check(Hashtable<Decl*>* parentST) {
    if(base == NULL) {
        Decl* d = parentST->Lookup(field->GetName());
        if(d == NULL) {
            ReportError::IdentifierNotDeclared(field, LookingForVariable);
            return Type::errorType;
        }

        if(dynamic_cast<VarDecl *>(d) != NULL) {
            return (dynamic_cast<VarDecl *>(d))->GetType();
        } else {
            ReportError::IdentifierNotDeclared(field, LookingForVariable);
            return Type::errorType;
        }
    } else {
        Type* base_type = baseType = base->Check(parentST);

        if(base_type == Type::errorType) return Type::errorType;
        
        /* NOTE: for conformance reasons, we check whether there is a class with that field */
        NamedType* nt = dynamic_cast<NamedType *>(base_type);
        
        if(nt != NULL) {
            ClassDecl* cd = dynamic_cast<ClassDecl *>(__globalST->Lookup(nt->GetName()));
            if(cd != NULL) {
                Hashtable<Decl*>* cleanTable = new Hashtable<Decl*>();
                cd->AddSymbols(cleanTable, false, true);
                
                if(dynamic_cast<VarDecl *>(cleanTable->Lookup(field->GetName())) == NULL) {
                    ReportError::FieldNotFoundInBase(field, base_type);
                    delete cleanTable;
                    return Type::errorType;
                }
                delete cleanTable;
            }
        }

        NamedType* currclass = NULL;
        Node* p = this;
        while((p = p->GetParent()) != NULL)
            if(dynamic_cast<ClassDecl *>(p) != NULL)
                currclass = new NamedType((dynamic_cast<ClassDecl *>(p))->GetIdentifier());
                
        if(currclass == NULL && dynamic_cast<NamedType *>(base_type) != NULL) {
            ReportError::InaccessibleField(field, base_type);
            return Type::errorType;
        } else if(currclass == NULL && dynamic_cast<NamedType *>(base_type) == NULL) {
            ReportError::FieldNotFoundInBase(field, base_type);
            return Type::errorType;
        }

        if(!Type::TypeEquivalence(currclass, base_type, false, parentST, true)) {
            ReportError::InaccessibleField(field, base_type);
            return Type::errorType;
        }

        Decl* d = parentST->Lookup(field->GetName());
        if(dynamic_cast<VarDecl *>(d) != NULL) {
            return (dynamic_cast<VarDecl *>(d))->GetType();
        } else {
            ReportError::IdentifierNotDeclared(field, LookingForVariable);
            return Type::errorType;
        }
    }
    Assert(0);
}

Location* FieldAccess::Emit(CodeGenerator *cg) {
    Location* l = EmitReference(cg);
    if(l == NULL) {
        return cg->GetLocation(field->GetName());
    } else {
        return cg->GenLoad(l);
    }
}

Location* FieldAccess::EmitReference(CodeGenerator *cg) {
    ClassDecl* cd = NULL;
    Node* p = this;
    if(baseType != NULL) cd = dynamic_cast<ClassDecl* >(__globalST->Lookup(baseType->GetName()));
    
    if(cd == NULL)
        while((p = p->GetParent()) != NULL)
            if(dynamic_cast<ClassDecl *>(p) != NULL)
                cd = dynamic_cast<ClassDecl *>(p);
    
    if(cd == NULL) {
        return NULL;
    } else {
        if(base == NULL && cg->GetLocation(field->GetName()) != NULL) return NULL;
    
        int actualOffset = -1;
        for(int i = 0; i < cd->allVariables->NumElements(); i++) {
            if(strcmp(cd->allVariables->Nth(i)->GetName(), field->GetName()) == 0) {
                actualOffset = cd->varOffset + 4 * i;
                break;
            }
        }

        if(actualOffset == -1) return NULL;

        if(base != NULL) {
            Location* baseL = base->Emit(cg);
            return cg->GenBinaryOp("+", baseL, cg->GenLoadConstant(actualOffset));
        } else {
            return cg->GenBinaryOp("+", new Location(fpRelative, 4, "this"), cg->GenLoadConstant(actualOffset));
        }
    }
}

Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  {
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
    isArrayLengthCheck = false;
}

Type* Call::Check(Hashtable<Decl*>* parentST) {
    Type* returnType = Type::errorType;
    FnDecl* actualFn = NULL;

    for(int j = 0; j < actuals->NumElements(); j++) {
        if(actuals->Nth(j)->Check(parentST)->IsErrorType()) {
            actuals->RemoveAt(j);
            actuals->InsertAt(new ErrorConstant(), j);
        }
    }

    if(base == NULL) {
        Decl* d = parentST->Lookup(field->GetName());
        if(d == NULL) {
            ReportError::IdentifierNotDeclared(field, LookingForFunction);
            return Type::errorType;
        }

        if(dynamic_cast<FnDecl *>(d) != NULL) {
            actualFn = (dynamic_cast<FnDecl *>(d));
            returnType = (dynamic_cast<FnDecl *>(d))->GetReturnType();
            
            // For Code Generator
            if(ClassDecl* cd = dynamic_cast<ClassDecl* >(actualFn->GetParent())) {
                this->fullyQualifiedFnName = NULL;
                this->isClassFunction = true;
                this->baseDecl = cd;
            } else {
                string tmp = "f_";
                tmp += actualFn->GetName();
                
                this->fullyQualifiedFnName = strdup(tmp.c_str());
                this->isClassFunction = false;
            }
            
            goto CheckParams;
        } else {
            ReportError::IdentifierNotDeclared(field, LookingForFunction);
            return Type::errorType;
        }
    } else {
        Hashtable<Decl*>* symbolTable = parentST->MakeCopy();
        Type* base_type = base->Check(symbolTable);
        if(dynamic_cast<NamedType *>(base_type) == NULL) {
            if(dynamic_cast<ArrayType *>(base_type) != NULL && strcmp(field->GetName(), "length") == 0) {
                // Special handling of length
                if(actuals->NumElements() != 0) {
                    ReportError::NumArgsMismatch(field, 0, actuals->NumElements());
                    return Type::errorType;
                }
                this->isArrayLengthCheck = true;
                return Type::intType;
            } else {
                ReportError::FieldNotFoundInBase(field, base_type);
                delete symbolTable;
                return Type::errorType;
            }
        } else {
            NamedType* nt = dynamic_cast<NamedType *>(base_type);
            ClassDecl* cd = dynamic_cast<ClassDecl *>(__globalST->Lookup(nt->GetName()));
            InterfaceDecl* in = dynamic_cast<InterfaceDecl *>(__globalST->Lookup(nt->GetName()));
            
            this->fullyQualifiedFnName = NULL;
            this->isClassFunction = true;
            
            if(in == NULL && cd == NULL) {
                // Can't even find the identifier
                // NOTE: NOT REPORTING ANY ERROR HERE - SINCE ALREADY WARNED
                delete symbolTable;
                return Type::errorType;
            }
            if(cd != NULL) {
                this->baseDecl = cd;
                Hashtable<Decl*>* cleanTable = new Hashtable<Decl*>();
                cd->AddSymbols(cleanTable, false, true);
                
                Decl* childDecl = cleanTable->Lookup(field->GetName());
                if(dynamic_cast<FnDecl *>(childDecl) != NULL) {
                    actualFn = (dynamic_cast<FnDecl *>(childDecl));
                    returnType = (dynamic_cast<FnDecl *>(childDecl))->GetReturnType();
                    goto CheckParams;
                } else {
                    ReportError::FieldNotFoundInBase(field, base_type); 
                    return Type::errorType;
                }
            } else if(in != NULL) {
                this->baseDecl = in;
                for(int i = 0; i < in->GetMembers()->NumElements(); ++i) {
                    if(strcmp(in->GetMembers()->Nth(i)->GetName(), field->GetName()) == 0) {
                        actualFn = (dynamic_cast<FnDecl *>(in->GetMembers()->Nth(i)));
                        returnType = (dynamic_cast<FnDecl *>(in->GetMembers()->Nth(i)))->GetReturnType();
                        goto CheckParams;
                    }
                }
                ReportError::FieldNotFoundInBase(field, base_type);
                return Type::errorType;
            }
        }
    }
    
CheckParams:
    if(returnType != Type::voidType) {
        hasReturnType = true;
    } else {
        hasReturnType = false;
    }

    if(actualFn->GetFormals()->NumElements() != actuals->NumElements()) {
        ReportError::NumArgsMismatch(field, actualFn->GetFormals()->NumElements(), actuals->NumElements());
        return Type::errorType;
    }

    for(int j = 0; j < actualFn->GetFormals()->NumElements(); j++) {
        if(!Type::TypeEquivalence(actuals->Nth(j)->Check(parentST), actualFn->GetFormals()->Nth(j)->GetType(), false, parentST, true)) {
            ReportError::ArgMismatch(actuals->Nth(j), j + 1, actuals->Nth(j)->Check(parentST), actualFn->GetFormals()->Nth(j)->GetType());
            return Type::errorType;
        }
    }

    return returnType;
}

Location* Call::Emit(CodeGenerator *cg) {
    if(this->isArrayLengthCheck) {
        Location* baseL = base->Emit(cg);
        return cg->GenLoad(baseL);
    } else if(this->isClassFunction) {
        Location* baseL;
        if(base != NULL) {
            baseL = base->Emit(cg);
        } else {
            baseL = new Location(fpRelative, 4, "this");
        }
        int callCounter = 0;
        Location* rtn = NULL;
    
        for(int i = actuals->NumElements() - 1; i >= 0; i--) {
            Location* tmp = actuals->Nth(i)->Emit(cg);
            cg->GenPushParam(tmp);
            callCounter++;
        }
        
        Location* myVTable = cg->GenLoad(baseL);
        Location* offsetFunction = cg->GenLoad(myVTable);
        Location* offsetValue = cg->GenACall(offsetFunction, true);
        Location* actualBaseL = cg->GenBinaryOp("-", baseL, offsetValue);
        
        int actualFnOffset = 0;
        
        if(ClassDecl* cd = dynamic_cast<ClassDecl *>(baseDecl)) {
            for(int j = 0; j < cd->allMethods->NumElements(); j++) {
                if(strcmp(cd->allMethods->Nth(j)->GetName(), field->GetName()) == 0) {
                    actualFnOffset = (j + 1) * 4;
                    break;
                }
            }
        } else if(InterfaceDecl* in = dynamic_cast<InterfaceDecl *>(baseDecl)) {
            for(int j = 0; j < in->allMethods->NumElements(); j++) {
                if(strcmp(in->allMethods->Nth(j), field->GetName()) == 0) {
                    actualFnOffset = (j + 1) * 4;
                    break;
                }
            }
        }

        Location* actualFunctionLoc = cg->GenLoad(myVTable, actualFnOffset);
        
        cg->GenPushParam(actualBaseL);
        callCounter++;
        
        rtn = cg->GenACall(actualFunctionLoc, hasReturnType);
        if(!hasReturnType) rtn = cg->GenLoadConstant(0);
        cg->GenPopParams(callCounter * 4);
        
        return rtn;
    } else {

        int callCounter = 0;
        Location* rtn = NULL;
    
        for(int i = actuals->NumElements() - 1; i >= 0; i--) {
            Location* tmp = actuals->Nth(i)->Emit(cg);
            cg->GenPushParam(tmp);
            callCounter++;
        }
        
        rtn = cg->GenLCall(this->fullyQualifiedFnName, hasReturnType);
        if(!hasReturnType) rtn = cg->GenLoadConstant(0);
        cg->GenPopParams(callCounter * 4);
    
        return rtn;
    }
}

NewExpr::NewExpr(yyltype loc, NamedType *c) : Expr(loc) { 
  Assert(c != NULL);
  (cType=c)->SetParent(this);
}

Type* NewExpr::Check(Hashtable<Decl*>* parentST) { 
    /* NOTE: NEW CLASS ONLY - NOT NEW INTERFACES */
    Decl* d = parentST->Lookup(cType->GetName());
    if(dynamic_cast<ClassDecl *>(d) == NULL) {
        ReportError::IdentifierNotDeclared(cType->GetIdentifier(), LookingForClass);
        return Type::errorType;
    }
    return cType;
}

Location* NewExpr::Emit(CodeGenerator *cg) {
    ClassDecl* c = dynamic_cast<ClassDecl *>(__globalST->Lookup(cType->GetName()));
    Location* start = cg->GenBuiltInCall(Alloc, cg->GenLoadConstant(c->objectSize));

    cg->GenStore(start, cg->GenLoadLabel(cType->GetName()));
    for(int i = 0; i < c->ifName->NumElements(); i++) {
        string vtableName;
        vtableName += c->GetName();
        vtableName += "_";
        vtableName += c->ifName->Nth(i);
        cg->GenStore(start, cg->GenLoadLabel(strdup(vtableName.c_str())), (i + 1) * 4);
    }

    return start;
}

NewArrayExpr::NewArrayExpr(yyltype loc, Expr *sz, Type *et) : Expr(loc) {
    Assert(sz != NULL && et != NULL);
    (size=sz)->SetParent(this); 
    (elemType=et)->SetParent(this);
}

Type* NewArrayExpr::Check(Hashtable<Decl*>* parentST) { 
    bool error = false;
    Type* sizeType = size->Check(parentST);
    if(!Type::TypeEquivalence(sizeType, Type::intType, true)) {
        ReportError::NewArraySizeNotInteger(size);
        error = true;
    }
    error = !Type::ValidityChecker(parentST, elemType);
    if(!error)
        return new ArrayType(*(this->GetLocation()), elemType);
    return Type::errorType;
}

Location* NewArrayExpr::Emit(CodeGenerator *cg) {
    char* haltLabel = cg->NewLabel();
    char* continueLabel = cg->NewLabel();
    Location* sizeL = size->Emit(cg);
    Location* zeroL = cg->GenLoadConstant(0);
    Location* fourL = cg->GenLoadConstant(4);
    
    /* NOTE: Runtime Check - Whether arrsize is positive */
    Location* isLessL = cg->GenBinaryOp("<", zeroL, sizeL);
    cg->GenIfZ(isLessL, haltLabel);
    cg->GenGoto(continueLabel);
    
    cg->GenLabel(haltLabel);
    cg->GenBuiltInCall(PrintString, cg->GenLoadConstant(err_arr_bad_size), NULL);
    cg->GenBuiltInCall(Halt, NULL, NULL);
    
    cg->GenLabel(continueLabel);
    Location* start = cg->GenBuiltInCall(Alloc, cg->GenBinaryOp("+" ,cg->GenBinaryOp("*", sizeL, fourL), fourL), NULL);
    cg->GenStore(start, sizeL, 0);

    return start;
}

Type* This::Check(Hashtable<Decl*>* parentST) {
    Node* p = this;
    while((p = p->GetParent()) != NULL) {
    	if(dynamic_cast<ClassDecl *>(p) != NULL) { 
            return new NamedType((dynamic_cast<ClassDecl *>(p))->GetIdentifier());
    	}
    }

    ReportError::ThisOutsideClassScope(this);
    return Type::errorType;
}

Location* This::Emit(CodeGenerator *cg) {
    return new Location(fpRelative, 4, "this");
}

Type* ReadIntegerExpr::Check(Hashtable<Decl*>* parentST) {
    return Type::intType;
}

Location* ReadIntegerExpr::Emit(CodeGenerator *cg) {
    return cg->GenBuiltInCall(ReadInteger, NULL, NULL);
}

Type* ReadLineExpr::Check(Hashtable<Decl*>* parentST) {
    return Type::stringType;
}

Location* ReadLineExpr::Emit(CodeGenerator *cg) {
    return cg->GenBuiltInCall(ReadLine, NULL, NULL);
}

Type* AssignExpr::Check(Hashtable<Decl*>* parentST) {
    Type* l = leftType = left->Check(parentST);
    Type* r = rightType = right->Check(parentST);

    if(!Type::TypeEquivalence(r, l, false, parentST)) {
        ReportError::IncompatibleOperands(op, l, r);
        return Type::errorType;
    }
    
    return r;
}

Location* AssignExpr::Emit(CodeGenerator *cg) {
    Location* l = left->Emit(cg);
    Location* r = right->Emit(cg);

    // need to handle what L is a local/global variable or something I malloc'd
    if(dynamic_cast<ArrayAccess*>(left) != NULL) {
        Location* l_ref = dynamic_cast<ArrayAccess*>(left)->EmitMemoryLocation(cg);
        cg->GenStore(l_ref, r);
    } else {
        Decl* leftDecl = (leftType != NULL && leftType->GetName() != NULL) ? __globalST->Lookup(leftType->GetName()) : NULL;
        Decl* rightDecl = (rightType != NULL && rightType->GetName() != NULL) ? __globalST->Lookup(rightType->GetName()) : NULL;
        if(dynamic_cast<InterfaceDecl *>(leftDecl) != NULL && dynamic_cast<ClassDecl *>(rightDecl) != NULL) {
            ClassDecl* rc = dynamic_cast<ClassDecl *>(rightDecl);
            int offset = 0;
            for(int i = 0; i < rc->ifName->NumElements(); i++) {
                if(strcmp(rc->ifName->Nth(i), leftDecl->GetName()) == 0) {
                    offset = (i + 1) * 4;
                    break;
                }
            }
            cg->GenAssign(l, cg->GenBinaryOp("+", r, cg->GenLoadConstant(offset)));
        } else if(FieldAccess* fa = dynamic_cast<FieldAccess *>(left)) {
            //cg->GenAssign(l, r);
            Location* lref = fa->EmitReference(cg);
            if(lref == NULL) {
                lref = fa->Emit(cg);
                cg->GenAssign(lref, r);
            } else {
                cg->GenStore(lref, r);
            }
        } else {
            cg->GenAssign(l, r);
        } 
    }
    return l;
}

Type* ArithmeticExpr::Check(Hashtable<Decl*>* parentST) { 
    if(left != NULL) {
        Type* l = left->Check(parentST);
        Type* r = right->Check(parentST);
        if(Type::TypeEquivalence(l, r, true) && (Type::TypeEquivalence(l, Type::doubleType, true) || Type::TypeEquivalence(l, Type::intType, true)))
            return l;
        else {
            ReportError::IncompatibleOperands(op, l, r);
            return Type::errorType;
        }
    } else {
        Type* r = right->Check(parentST);
        if(Type::TypeEquivalence(r, Type::doubleType, true) || Type::TypeEquivalence(r, Type::intType, true))
            return r;
        else {
            ReportError::IncompatibleOperand(op, r);
            return Type::errorType;
        }
    }
}

Location* ArithmeticExpr::Emit(CodeGenerator *cg) {
    if(left != NULL) {
        Location* l = left->Emit(cg);
        Location* r = right->Emit(cg);
        return cg->GenBinaryOp((op->GetToken()), l, r);
    } else {
        Location* l = cg->GenLoadConstant(0);
        Location* r = right->Emit(cg);
        return cg->GenBinaryOp((op->GetToken()), l, r);
    }
}

Type* RelationalExpr::Check(Hashtable<Decl*>* parentST) { 
    Type* l = left->Check(parentST);
    Type* r = right->Check(parentST);
    if(Type::TypeEquivalence(l, r, true) && (Type::TypeEquivalence(l, Type::doubleType, true) || Type::TypeEquivalence(l, Type::intType, true)))
        return Type::boolType;
    else {
        ReportError::IncompatibleOperands(op, l, r);
        return Type::errorType;
    }
}

Location* RelationalExpr::Emit(CodeGenerator *cg) {
    char* o = op->GetToken();
    Location* l = left->Emit(cg);
    Location* r = right->Emit(cg);
    
    if(strcmp(o, "<") == 0) {
        return cg->GenBinaryOp("<", l, r);
    } else if(strcmp(o, "<=") == 0) {
        return cg->GenBinaryOp("||", cg->GenBinaryOp("<", l, r), cg->GenBinaryOp("==", l, r));
    } else if(strcmp(o, ">") == 0) {
        return cg->GenBinaryOp("<", r, l);
    } else if(strcmp(o, ">=") == 0) {
        return cg->GenBinaryOp("||", cg->GenBinaryOp("<", r, l), cg->GenBinaryOp("==", l, r));
    }
    return NULL;
}

Type* EqualityExpr::Check(Hashtable<Decl*>* parentST) {
    Type* l = left->cachedType = left->Check(parentST);
    Type* r = right->cachedType = right->Check(parentST);

    if(Type::TypeEquivalence(l, r, false, parentST, false)) 
        return Type::boolType;
    else {
        ReportError::IncompatibleOperands(op, l, r);
        return Type::errorType;
    }
}

Location* EqualityExpr::Emit(CodeGenerator *cg) {
    Location* l = left->Emit(cg);
    Location* r = right->Emit(cg);

    if(left->cachedType == Type::stringType) {
        if(strcmp(op->GetToken(), "==") == 0)
            return cg->GenBuiltInCall(StringEqual, l, r);
        else {
            Location* zero = cg->GenLoadConstant(0);
            return cg->GenBinaryOp("==", cg->GenBuiltInCall(StringEqual, l, r), zero);
        }
    } else {
        if(strcmp(op->GetToken(), "==") == 0)
            return cg->GenBinaryOp("==", l, r);
        else {
            Location* zero = cg->GenLoadConstant(0);
            return cg->GenBinaryOp("==", cg->GenBinaryOp("==", l, r), zero);
        }
    }
    return NULL;
}

Type* LogicalExpr::Check(Hashtable<Decl*>* parentST) { 
    if(left != NULL) {
        Type* l = left->Check(parentST);
        Type* r = right->Check(parentST);
        if(Type::TypeEquivalence(l, r, true) && (Type::TypeEquivalence(l, Type::boolType, true)))
            return Type::boolType;
        else {
            ReportError::IncompatibleOperands(op, l, r);
            return Type::errorType;
        }
    } else {
        Type* r = right->Check(parentST);
        if(Type::TypeEquivalence(r, Type::boolType, true))
            return Type::boolType;
        else {
            ReportError::IncompatibleOperand(op, r);
            return Type::errorType;
        }
    }
}

Location* LogicalExpr::Emit(CodeGenerator *cg) {
    if(strcmp(op->GetToken(), "&&") == 0) {
        Location* l = left->Emit(cg);
        Location* r = right->Emit(cg);
        return cg->GenBinaryOp("&&", l, r);
    } else if(strcmp(op->GetToken(), "||") == 0) {
        Location* l = left->Emit(cg);
        Location* r = right->Emit(cg);
        return cg->GenBinaryOp("||", l, r);
    } else if(strcmp(op->GetToken(), "!") == 0) {
        Location* r = right->Emit(cg);
        Location* zero = cg->GenLoadConstant(0);
        return cg->GenBinaryOp("==", r, zero);
    }
    return NULL;
}



Type *Type::intType    = new Type("int");
Type *Type::doubleType = new Type("double");
Type *Type::voidType   = new Type("void");
Type *Type::boolType   = new Type("bool");
Type *Type::nullType   = new Type("null");
Type *Type::stringType = new Type("string");
Type *Type::errorType  = new Type("error"); 

Type::Type(const char *n) {
    Assert(n);
    typeName = strdup(n);
}

bool Type::ValidityChecker(Hashtable<Decl *>* parentST, Type* type) {
    NamedType* nT = dynamic_cast<NamedType *>(type);
    ArrayType *aT = dynamic_cast<ArrayType *>(type);
    
    Hashtable<Decl *>* symbolTable = __globalST->MakeCopy();

    if(nT != NULL) {
        Decl* dT = symbolTable->Lookup(nT->GetName());
        if(dT == NULL || dynamic_cast<VarDecl *>(dT) != NULL || dynamic_cast<FnDecl *>(dT) != NULL) {
            ReportError::IdentifierNotDeclared(nT->GetIdentifier(), LookingForType);
            return false;
        }
        return true;
    } else if(aT != NULL) {
        return Type::ValidityChecker(symbolTable, aT->BaseType());
    } else {
        return true;
    }
    return false;
}

/* Calculates the least upper bound type 
   If checkGreater is enabled, NamedType a must be <= NamedType b.
 */
Type* Type::LeastUpperBound(NamedType* a, NamedType* b, bool checkGreater) {
    Hashtable<Decl *>* symbolTable = __globalST->MakeCopy();

    Decl* a_decl = symbolTable->Lookup(a->GetName());
    Decl* b_decl = symbolTable->Lookup(b->GetName());

    if(a_decl == NULL || b_decl == NULL) return Type::errorType;

    ClassDecl* a_class = dynamic_cast<ClassDecl *>(a_decl);
    ClassDecl* b_class = dynamic_cast<ClassDecl *>(b_decl);
    InterfaceDecl* a_if = dynamic_cast<InterfaceDecl *>(a_decl);
    InterfaceDecl* b_if = dynamic_cast<InterfaceDecl *>(b_decl);

    if(a_if != NULL && b_if != NULL) {
        /* They are both interfaces, so either they are completely compatible or nothing */
        if(strcmp(a_if->GetName(), b_if->GetName()) == 0) return a;
        return Type::errorType;
    }

    if((a_class != NULL && b_if != NULL) || (a_if != NULL && b_class != NULL)) {
        /* One is a class and another is an interface */
        if(!checkGreater && a_if != NULL) { 
            swap(a_if, b_if); swap(a_class, b_class); // ensures that a is always the class
        } else if(checkGreater && a_if != NULL) {
            return Type::errorType; // NOTE: this means requires class but you give it an interface 
        }
        ClassDecl* savedClass = a_class;
        List<NamedType*>* a_list = a_class->GetImplements();
        while(true) {
            for(int i = 0; i < a_list->NumElements(); ++i) {
                if(strcmp(a_list->Nth(i)->GetName(), b_if->GetName()) == 0) 
                    return new NamedType(savedClass->GetIdentifier());
            }
            if(a_class->GetExtends() == NULL) break;
            if(((a_decl = symbolTable->Lookup(a_class->GetExtends()->GetName())) != NULL) && dynamic_cast<ClassDecl *>(a_decl) != NULL) {
                a_class = dynamic_cast<ClassDecl *>(a_decl);
                a_list = a_class->GetImplements();
            }
        }
        /* Drops down to nothing */
        return Type::errorType;
    }
    
    // if they are both classes then we need to walk the hierarchy
    Hashtable<ClassDecl *> a_ancestors;
    a_ancestors.Enter(a_class->GetName(), a_class);
    
    while(true) {
        if(a_class->GetExtends() == NULL) break;
        while(((a_decl = symbolTable->Lookup(a_class->GetExtends()->GetName())) != NULL) && dynamic_cast<ClassDecl *>(a_decl) == NULL)
            symbolTable->Remove(a_class->GetExtends()->GetName(), a_decl);
        if(a_decl == NULL) break;
        a_class = dynamic_cast<ClassDecl *>(a_decl);
        a_ancestors.Enter(a_class->GetName(), a_class);
    }

    if(a_ancestors.Lookup(b_class->GetName()) != NULL) {
        /* Then B is an ancestor of A and we can just return B's type */
        return new NamedType(b_class->GetIdentifier());
    }

    /* If the checkGreater flag is not flag then we can compare both ways */
    if(!checkGreater) {
        while(true) {
            if(b_class->GetExtends() == NULL) break;
            while(((b_decl = symbolTable->Lookup(b_class->GetExtends()->GetName())) != NULL) && dynamic_cast<ClassDecl *>(b_decl) == NULL)
                symbolTable->Remove(b_class->GetExtends()->GetName(), b_decl);
            if(b_decl == NULL) break;
            b_class = dynamic_cast<ClassDecl *>(b_decl);
            if(a_ancestors.Lookup(b_class->GetName()) != NULL) {
                return new NamedType(b_class->GetIdentifier());
            }
        }
    }
    
    delete symbolTable;
    return Type::errorType;
}

/* If not strict, then A <= B is okay */
bool Type::TypeEquivalence(Type* a, Type *b, bool strict, Hashtable<Decl *>* parentST, bool lcbCheckGreater) {
    Assert(strict || parentST != NULL);
    Assert(a != NULL);
    Assert(b != NULL);

    return a->Equivalence(b, strict, parentST, lcbCheckGreater);
}

bool Type::Equivalence(Type* other, bool strict, Hashtable<Decl *>* parentST, bool lcbCheckGreater) {
    if(this->IsErrorType() || other->IsErrorType())
        return true;

    if(this->IsNullType() && other->IsNullType())
        return true;

    if(this->IsNullType() && !other->IsNullType()) 
        return other->Equivalence(this, strict, parentST, lcbCheckGreater);

    if(dynamic_cast<NamedType*>(other) != NULL || dynamic_cast<ArrayType*>(other) != NULL)
        return false;

    return strcmp(this->typeName, other->typeName) == 0;
}

bool NamedType::Equivalence(Type* other, bool strict, Hashtable<Decl *>* parentST, bool lcbCheckGreater) {
    if(this->IsErrorType() || other->IsErrorType())
        return true;
        
    if(!strict && (this->IsNullType() || other->IsNullType()))
        return true;

    if(dynamic_cast<NamedType*>(other) == NULL)
        return false;
    
    if(strict) return (strcmp(this->GetName(), other->GetName()) == 0);

    Hashtable<Decl *>* symbolTable = parentST->MakeCopy();
    if(LeastUpperBound(this, dynamic_cast<NamedType*>(other), lcbCheckGreater) == Type::errorType) {
        delete symbolTable;
        return false;
    } else {
        delete symbolTable;
        return true;
    }
}

bool ArrayType::Equivalence(Type* other, bool strict, Hashtable<Decl *>* parentST, bool lcbCheckGreater) {
    if(this->IsErrorType() || other->IsErrorType())
        return true;
    
    if(dynamic_cast<ArrayType*>(other) == NULL) return false;

    return this->BaseType()->Equivalence(dynamic_cast<ArrayType*>(other)->BaseType(), strict, parentST, lcbCheckGreater);
}

NamedType::NamedType(Identifier *i) : Type(*i->GetLocation()) {
    Assert(i != NULL);
    (id=i)->SetParent(this);
} 


ArrayType::ArrayType(yyltype loc, Type *et) : Type(loc) {
    Assert(et != NULL);
    (elemType=et)->SetParent(this);
}
