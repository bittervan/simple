#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>

#include <Lexer.h>
#include <Parser.h>
#include <IRGenerator.h>
#include <SemanticAnalyzer.h>

using namespace llvm;

static llvm::cl::opt<std::string> inputFile(cl::Positional,
                                        cl::desc("<input expression>"),
                                        cl::init(""));

int main(int argc, const char **argv) {
    InitLLVM llvmInitializer(argc, argv);
    cl::ParseCommandLineOptions(argc, argv, "Simple - Simple\n");
    errs() << "InputFile: \"" << inputFile << "\"\n";

    auto fileOrErr = MemoryBuffer::getFile(inputFile);
    std::unique_ptr<MemoryBuffer> inputBuffer = std::move(fileOrErr.get());

    Lexer lex(inputBuffer->getBuffer());

    Token token;
    Parser parse(lex);
    AST *tree = parse.Parse();

    if (!tree || parse.HasError()) {
        errs() << "Error parsing input\n";
        return 1;
    }

    SemanticAnalyzer semanticAnalyzer;
    if (semanticAnalyzer.Analyze(tree)) {
        errs() << "Semantic errors occured\n";
        return 1;
    }

    IRGenerator generator;
    generator.Generate(tree);

    return 0;
}