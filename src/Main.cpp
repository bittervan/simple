#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/InitLLVM.h>

#include <Lexer.h>
#include <Parser.h>
#include <SemanticAnalyzer.h>

static llvm::cl::opt<std::string> input(llvm::cl::Positional,
                                        llvm::cl::desc("<input expression>"),
                                        llvm::cl::init(""));

int main(int argc, const char **argv) {
  llvm::InitLLVM llvmInitializer(argc, argv);
  llvm::cl::ParseCommandLineOptions(argc, argv, "Simple - Simple\n");
  llvm::outs() << "Input: \"" << input << "\"\n";

  Lexer lex(input);

  Token token;
  Parser parse(lex);
  AST *tree = parse.Parse();

  if (!tree || parse.HasError()) {
    llvm::errs() << "Error parsing input\n";
    return 1;
  }

  SemanticAnalyzer semanticAnalyzer;
  if (semanticAnalyzer.Analyze(tree)) {
    llvm::errs() << "Semantic errors occured\n";
    return 1;
  }

  return 0;
}