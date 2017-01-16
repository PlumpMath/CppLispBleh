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
    
    llvm::Value *mainFn = buildMainFunction(mainModule, context, builder);
    mainFn->dump();
    cout << "---" << endl;
    
    // string src = "(+ 3 5)";
    // unsigned long p = 0;
    // Obj o = read(p, src);  
    // cout << o.toString() << endl;

    // AST *ast = objToAst(o);
    // cout << ast->toString() << endl;

    // llvm::Value *v = ast->emitIR(context, builder);
    // v->dump();

    std::error_code EC;
    llvm::raw_fd_ostream OS("module.bc", EC, llvm::sys::fs::F_None);
    llvm::WriteBitcodeToFile(&mainModule, OS);
    OS.flush();

    system("clang module.bc");

    // // Save as executable
    // auto TargetTriple = sys::getDefaultTargetTriple();
    // InitializeAllTargetInfos();
    // InitializeAllTargets();
    // InitializeAllTargetMCs();
    // InitializeAllAsmParsers();
    // InitializeAllAsmPrinters();
    // std::string Error;
    // auto target = TargetRegistry::lookupTarget(TargetTriple, Error);
    // if (!target) {
    //     errs() << Error;
    //     return 1;
    // }
    // cout << "Target: " << target << endl;
    // auto CPU = "generic";
    // auto Features = "";
    // TargetOptions opt;
    // auto RM = Optional<Reloc::Model>();
    // auto TargetMachine = target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);
    // mainModule.setDataLayout(TargetMachine->createDataLayout());
    // mainModule.setTargetTriple(TargetTriple);
    
    // auto Filename = "prog";
    // std::error_code EC;
    // raw_fd_ostream dest(Filename, EC, sys::fs::F_None);
    // if (EC) {
    //     errs() << "Could not open file: " << EC.message();
    //     return 1;
    // }
    
    // legacy::PassManager pass;
    // auto FileType = TargetMachine::CGFT_ObjectFile;
    // //auto FileType = TargetMachine::CGFT_AssemblyFile;
    
    // if (TargetMachine->addPassesToEmitFile(pass, dest, FileType)) {
    //     errs() << "TargetMachine can't emit a file of this type";
    //     return 1;
    // }
    
    // pass.run(mainModule);    
    // dest.flush();

    // Dump final module
    cout << endl << " --- COMPLETE MODULE --- " << endl << endl;
    mainModule.dump();

    // string in = "";
    // while(true) {
    //     getline(cin, in);
    //     if(in == "exit") break;
    //     unsigned long p = 0;
    //     cout << read(p, in).toString() << endl;
    // }
}

// clang++ main.cpp read.cpp ast.cpp obj.cpp -o Bleh `llvm-config --cxxflags --ldflags --system-libs --libs core`
