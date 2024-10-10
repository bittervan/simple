#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/Error.h>

int main(int argc, const char **argv) {
    llvm::InitLLVM llvmInitializer(argc, argv);
    llvm::outs() << "Hello, World!" << "\n";
}