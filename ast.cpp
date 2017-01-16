#include "ast.h"

#include <sstream>

using namespace llvm;

AST *objToAst(Obj& obj) {
    if(obj.tag == List) {
        if(obj.list.size() == 3 &&
            obj.list[0].tag == Symbol &&
            obj.list[0].symbol == "+") {
            AST *lhs = objToAst(obj.list[1]);
            AST *rhs = objToAst(obj.list[2]);
            return new AST_Binop(Add, lhs, rhs);
        }
    }
    else if(obj.tag == Number) {
        return new AST_Number(obj.number);
    }
    else if(obj.tag == Symbol) {
        
    }    
    return nullptr;
}

Value *buildMainFunction(llvm::Module& module, llvm::LLVMContext& context, llvm::IRBuilder<>& builder) {
    vector<Type*> args = {};
    FunctionType *ft = FunctionType::get(Type::getInt32Ty(context), args, false);
    Function *f = Function::Create(ft, Function::ExternalLinkage, "main", &module);

    BasicBlock *bb = BasicBlock::Create(context, "entry", f);
    builder.SetInsertPoint(bb);

    auto zero = ConstantInt::get(Type::getInt32Ty(context), 123, false);
    builder.CreateRet(zero);

    return f;
}

// AST_Binop
AST_Binop::AST_Binop(Op op, AST *lhs, AST *rhs) {
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
}

Value *AST_Binop::emitIR(llvm::LLVMContext& context, llvm::IRBuilder<>& builder) {
    Value *l = lhs->emitIR(context, builder);
    Value *r = rhs->emitIR(context, builder);
    return builder.CreateFAdd(l, r, "addtmp");
}

string AST_Binop::toString() {
    stringstream s;
    s << "AST_Binop<+, ";
    s << lhs->toString();
    s << ", ";
    s << rhs->toString();
    s << ">";
    return s.str();
}

// AST_Call
AST_Call::AST_Call(string& functionName) {
    this->functionName = functionName;
}

Value *AST_Call::emitIR(llvm::LLVMContext& context, llvm::IRBuilder<>& builder) {
    return nullptr;
}

string AST_Call::toString() {
    return "AST_Call";
}

// AST_Number
AST_Number::AST_Number(double number) {
    this->number = number;
}

Value *AST_Number::emitIR(llvm::LLVMContext& context, llvm::IRBuilder<>& builder) {
    return ConstantFP::get(context, APFloat(number));
}

string AST_Number::toString() {
    stringstream s;
    s << "AST_Number<";
    s << number;
    s << ">";
    return s.str();
}
