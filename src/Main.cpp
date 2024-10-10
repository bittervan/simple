#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/CommandLine.h>

#include <Lexer.h>

static llvm::cl::opt<std::string> input(llvm::cl::Positional, llvm::cl::desc("<input expression>"), llvm::cl::init(""));

int main(int argc, const char **argv) {
    llvm::InitLLVM llvmInitializer(argc, argv);
    llvm::cl::ParseCommandLineOptions(argc, argv, "Simple - Simple\n");
    llvm::outs() << "Input: \"" << input << "\"\n";

    Lexer lex(input);

    Token token;
    lex.GetNext(token);

    while (token.GetType() != Token::kEOI) {
        llvm::outs() << "token type: " << token.GetType() << ", token text: \"" << token.GetText() << "\"\n";
        lex.GetNext(token);
    }

    return 0;
}