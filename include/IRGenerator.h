#include <AST.h>

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/ADT/StringMap.h>

class IRGenerator : ASTVisitor {
    llvm::LLVMContext context;
    llvm::Module module;
    llvm::IRBuilder<> builder;

    llvm::StringMap<llvm::Value *> namedValues;
    llvm::Value *returnValue;
public:
    void Generate(AST *tree);
    IRGenerator() : module("Simple", context), builder(context) {}
    void Visit(Factor &node) override;
    void Visit(BinaryOp &node) override;
    void Visit(WithDeclaration &node) override;
};