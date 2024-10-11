#include <AST.h>

#include <llvm/IR/Module.h>

class IRGenerator {
public:
    void Generate(AST *tree);
};