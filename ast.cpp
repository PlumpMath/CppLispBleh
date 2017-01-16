#include "ast.h"

#include <sstream>

using namespace llvm;

AST *objToAst(Obj& obj) {
    if(obj.tag == List &&
       obj.list.size() == 3 &&
       obj.list[0].tag == Symbol &&
       obj.list[0].symbol == "+") {
        AST *lhs = objToAst(obj.list[1]);
        AST *rhs = objToAst(obj.list[2]);
        return new AST_Binop(Add, lhs, rhs);
    }
    else if(obj.tag == List &&
            obj.list.size() == 3 &&
            obj.list[0].tag == Symbol &&
            obj.list[0].symbol == "-") {
        AST *lhs = objToAst(obj.list[1]);
        AST *rhs = objToAst(obj.list[2]);
        return new AST_Binop(Sub, lhs, rhs);
    }
    else if(obj.tag == List &&
            obj.list.size() == 3 &&
            obj.list[0].tag == Symbol &&
            obj.list[0].symbol == "*") {
        AST *lhs = objToAst(obj.list[1]);
        AST *rhs = objToAst(obj.list[2]);
        return new AST_Binop(Mul, lhs, rhs);
    }
    else if(obj.tag == List &&
            obj.list.size() > 0 &&
            obj.list[0].tag == Symbol &&
            obj.list[0].symbol == "defn") {
        if(obj.list.size() != 4) {
            cerr << "Wrong nr of arguments to 'defn' special form: " << obj.list.size() << ", " << obj.toString() << endl;
            exit(1);
        }
        string functionName = obj.list[1].symbol;
        AST *body = objToAst(obj.list[3]);
        return new AST_Defn(functionName, body);
    }
    else if(obj.tag == Number) {
        return new AST_Number(obj.number);
    }
    else if(obj.tag == Symbol) {
        cout << "Can't handle symbol: '" << obj.symbol << "'" << endl;
        exit(1);
    }
    else if(obj.tag == List) {
        return new AST_Number(666);
    }
    else {
        cout << "Can't handle form with tag: " << obj.tag << endl;
        exit(1);
    }
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

Value *AST_Binop::emitIR(llvm::Module& module, llvm::LLVMContext& context, llvm::IRBuilder<>& builder) {
    Value *l = lhs->emitIR(module, context, builder);
    Value *r = rhs->emitIR(module, context, builder);
    if(this->op == Add) return builder.CreateFAdd(l, r, "addtmp");
    else if(this->op == Sub) return builder.CreateFSub(l, r, "subtmp");
    else if(this->op == Mul) return builder.CreateFMul(l, r, "multmp");
    //else if(this->op == Div) return builder.CreateFAdd(l, r, "addtmp");
    else {
        cerr << "Unhandled op" << endl;
        exit(1);
    }
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

Value *AST_Call::emitIR(llvm::Module& module, llvm::LLVMContext& context, llvm::IRBuilder<>& builder) {
    return nullptr;
}

string AST_Call::toString() {
    return "AST_Call";
}

// AST_Number
AST_Number::AST_Number(double number) {
    this->number = number;
}

Value *AST_Number::emitIR(llvm::Module& module, llvm::LLVMContext& context, llvm::IRBuilder<>& builder) {
    return ConstantFP::get(context, APFloat(number));
}

string AST_Number::toString() {
    stringstream s;
    s << "AST_Number<";
    s << number;
    s << ">";
    return s.str();
}

// AST_Defn
AST_Defn::AST_Defn(string& functionName, AST *body) {
    this->functionName = functionName;
    this->body = body;
}

Value *AST_Defn::emitIR(llvm::Module& module, llvm::LLVMContext& context, llvm::IRBuilder<>& builder) {
    vector<Type*> args = {};
    FunctionType *ft = FunctionType::get(Type::getInt32Ty(context), args, false);
    Function *f = Function::Create(ft, Function::ExternalLinkage, this->functionName, &module);

    BasicBlock *bb = BasicBlock::Create(context, "entry", f);
    builder.SetInsertPoint(bb);

    Value *bodyIR = this->body->emitIR(module, context, builder);
    builder.CreateRet(bodyIR);

    return f;
}

string AST_Defn::toString() {
    stringstream s;
    s << "AST_Defn<";
    s << functionName;
    s << ", ";
    s << body->toString();
    s << ">";
    return s.str();
}
