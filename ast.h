#pragma once

#include <iostream>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include "obj.h"

class AST {
  public:
    virtual ~AST() {}
    virtual llvm::Value *emitIR(llvm::Module& module, llvm::LLVMContext& context, llvm::IRBuilder<>& builder) = 0;
    virtual string toString() = 0;
};

enum Op {
    Add,
    Sub,
    Mul,
    Div
};

class AST_Binop : public AST {
  public:
    AST_Binop(Op op, AST *lhs, AST *rhs);
    virtual ~AST_Binop() {}
    Op op;
    AST *lhs;
    AST *rhs;
    llvm::Value *emitIR(llvm::Module& module, llvm::LLVMContext& context, llvm::IRBuilder<>& builder);
    string toString();
};

class AST_Call : public AST {
  public:
    AST_Call(string& functionName);
    virtual ~AST_Call() {}
    string functionName;
    llvm::Value *emitIR(llvm::Module& module, llvm::LLVMContext& context, llvm::IRBuilder<>& builder);
    string toString();
};

class AST_Number : public AST {
  public:
    AST_Number(double number);
    virtual ~AST_Number() {}
    double number;
    llvm::Value *emitIR(llvm::Module& module, llvm::LLVMContext& context, llvm::IRBuilder<>& builder);
    string toString();
};

class AST_Defn : public AST {
  public:
    AST_Defn(string& functionName, AST *body);
    virtual ~AST_Defn() {}
    string functionName;
    AST *body;
    llvm::Value *emitIR(llvm::Module& module, llvm::LLVMContext& context, llvm::IRBuilder<>& builder);
    string toString();
};

AST *objToAst(Obj& obj);
llvm::Value *buildMainFunction(llvm::Module& module, llvm::LLVMContext& context, llvm::IRBuilder<>& builder);
