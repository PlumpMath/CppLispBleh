#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

#include "obj.h"
#include "read.h"
#include "ast.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Bitcode/ReaderWriter.h"

using namespace std;
using namespace llvm;

int main() {
    LLVMContext context;
    IRBuilder<> builder(context);
    Module mainModule("The Main Module", context);
    
    // llvm::Value *mainFn = buildMainFunction(mainModule, context, builder);
    //mainFn->dump();
    //cout << "---" << endl;
    
    string src = "(defn foo nil (+ 2 3))";
    unsigned long p = 0;
    Obj o = read(p, src);  
    cout << o.toString() << endl;
    AST *ast = objToAst(o);
    cout << ast->toString() << endl;
    llvm::Value *v = ast->emitIR(mainModule, context, builder);
    v->dump();

    /* REPL */
    string in = "";
    while(true) {
        cout << "λ ";
        getline(cin, in);
        if(in == ".") break;
        unsigned long p = 0;
        Obj o = read(p, in);
        AST *ast = objToAst(o);
        //cout << ast->toString() << endl;
        llvm::Value *v = ast->emitIR(mainModule, context, builder);
        v->dump();
    }

    // Dump final module
    cout << endl << " --- COMPLETE MODULE --- " << endl << endl;
    mainModule.dump();

    std::error_code EC;
    llvm::raw_fd_ostream OS("module.bc", EC, llvm::sys::fs::F_None);
    llvm::WriteBitcodeToFile(&mainModule, OS);
    OS.flush();
    //system("clang module.bc");
}

// clang++ main.cpp read.cpp ast.cpp obj.cpp -o Bleh `llvm-config --cxxflags --ldflags --system-libs --libs core`
