#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Host.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "clang/Parse/Parser.h"
#include "clang/Parse/ParseAST.h"
#include <memory>
#include <iostream>
#include <string>

using namespace llvm;
using namespace clang;

static cl::opt<std::string> FileName(cl::Positional, cl::desc("Input file"), cl::Required);

int main(int argc, char** argv){
    cl::ParseCommandLineOptions(argc, argv, "My simple front end\n");
    CompilerInstance CI;
    DiagnosticOptions diagnosticOptions;
    CI.createDiagnostics();

    std::shared_ptr<clang::TargetOptions> PTO(new clang::TargetOptions());
    PTO.get()->Triple = sys::getDefaultTargetTriple();
    TargetInfo* PTI = TargetInfo::CreateTargetInfo(CI.getDiagnostics(), PTO);
    CI.setTarget(PTI);
    CI.createFileManager();
    CI.createSourceManager(CI.getFileManager());
    CI.createPreprocessor(TU_Complete);
    CI.getPreprocessorOpts().UsePredefines = false;
    std::unique_ptr<ASTConsumer> astConsumer = CreateASTPrinter(NULL, "");
    CI.setASTConsumer(std::move(astConsumer));

    CI.createASTContext();
    CI.createSema(TU_Complete, NULL);

    const FileEntry *pFile = CI.getFileManager().getFile(FileName);
    if(!pFile){
        std::cerr << "File not found: " << FileName << std::endl;
        return 1;
    } 
    CI.getSourceManager().setMainFileID(
        CI.getSourceManager().createFileID(
            pFile, SourceLocation(), SrcMgr::C_User
        )
    );
    CI.getDiagnosticClient().BeginSourceFile(CI.getLangOpts(),0);
    ParseAST(CI.getSema());
    CI.getASTContext().PrintStats();
    CI.getDiagnosticClient().EndSourceFile();
    return 0;
}