#include <IRGenerator.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>

using namespace llvm;

void IRGenerator::Generate(AST *tree) {
    FunctionType *mainFuncType = FunctionType::get(builder.getInt32Ty(), {builder.getInt32Ty(), builder.getPtrTy()}, false);
    Function *mainFunc = Function::Create(mainFuncType, Function::ExternalLinkage, "main", module);
    BasicBlock *entry = BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entry);

    tree->Accept(*this);

    FunctionType *calcWriteFuncType = FunctionType::get(builder.getVoidTy(), {builder.getInt32Ty()}, false);
    Function *calcWriteFunc = Function::Create(calcWriteFuncType, Function::ExternalLinkage, "calc_write", module);

    builder.CreateCall(calcWriteFunc, {returnValue});

    builder.CreateRet(builder.getInt32(0));
    module.print(outs(), nullptr);
}

void IRGenerator::Visit(Factor &node) {
    if (node.GetType() == Factor::kIdent) {
        returnValue = namedValues[node.GetVal()];
    } else {
        int intValue;
        node.GetVal().getAsInteger(10, intValue);
        returnValue = ConstantInt::get(builder.getInt32Ty(), intValue, true);
    }
}

void IRGenerator::Visit(BinaryOp &node) {
    node.GetLeft()->Accept(*this);
    Value *left = returnValue;

    node.GetRight()->Accept(*this);
    Value *right = returnValue;

    switch (node.GetOperator()) {
        case BinaryOp::kPlus:
            returnValue = builder.CreateNSWAdd(left, right);
            break;

        case BinaryOp::kMinus:
            returnValue = builder.CreateNSWSub(left, right);
            break;

        case BinaryOp::kMultiple:
            returnValue = builder.CreateNSWMul(left, right);
            break;

        case BinaryOp::kDivide:
            returnValue = builder.CreateSDiv(left, right);
            break;

        default:
            break;
    }
}

void IRGenerator::Visit(WithDeclaration& node) {
    FunctionType *calcReadFunctype = FunctionType::get(builder.getInt32Ty(), {builder.getPtrTy()}, false);
    Function *calcReadFunc = Function::Create(calcReadFunctype, Function::ExternalLinkage, "calc_read", module);

    for (const auto &param : node) {
        Constant *strText = ConstantDataArray::getString(context, param);
        GlobalVariable *str = new GlobalVariable(module, strText->getType(), true, GlobalValue::PrivateLinkage, strText, Twine(param).concat(".str"));
        Value *ptr = builder.CreateInBoundsGEP(strText->getType(), str, {builder.getInt32(0), builder.getInt32(0)}, "ptr");
        CallInst *call = builder.CreateCall(calcReadFunctype, calcReadFunc, {ptr});

        namedValues[param] = call;
    }

    node.GetExpr()->Accept(*this);
}
