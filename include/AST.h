#pragma once

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Error.h>
class ASTVisitor;
class AST;
class Expr;
class Factor;
class BinaryOp;
class WithDeclaration;

class ASTVisitor {
public:
    virtual void Visit(AST &) {}

    virtual void Visit(Expr &) {}

    virtual void Visit(Factor &) = 0;
    virtual void Visit(BinaryOp &) = 0;
    virtual void Visit(WithDeclaration &) = 0;
};

class AST {
public:
    virtual ~AST() {}

    virtual void Accept(ASTVisitor &visitor) = 0;
};

class Expr : public AST {
public:
    Expr() {}
};

class Factor : public Expr {
public:
    enum ValueType { kIdent, kNumber };

private:
    ValueType type;
    llvm::StringRef value;

public:
    Factor(ValueType inType, llvm::StringRef inVal)
        : type(inType), value(inVal) {}

    ValueType GetType() { return type; }

    llvm::StringRef GetVal() { return value; }

    virtual void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }
};

class BinaryOp : public Expr {
public:
    enum Operator { kPlus, kMinus, kMultiple, kDivide };

private:
    Expr *left;
    Expr *right;
    Operator op;

public:
    BinaryOp(Operator inOp, Expr *inLeftExpr, Expr *inRightExpr)
        : op(inOp), left(inLeftExpr), right(inRightExpr) {}

    Expr *GetLeft() { return left; }

    Expr *GetRight() { return right; }

    Operator GetOperator() { return op; }

    virtual void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }

    std::string GetDisplayText() const {
        switch (op) {
        case kPlus:
            return "+";

        case kMinus:
            return "-";

        case kMultiple:
            return "*";

        case kDivide:
            return "/";

        default:
            break;
        }

        return "";
    }
};

class WithDeclaration : public AST {
    using VariableVector = llvm::SmallVector<llvm::StringRef, 8>;

public:
    WithDeclaration(llvm::SmallVector<llvm::StringRef, 8> inVars, Expr *inExpr)
        : variables(inVars), expr(inExpr) {}

    VariableVector::const_iterator begin() { return variables.begin(); }

    VariableVector::const_iterator end() { return variables.end(); }

    Expr *GetExpr() { return expr; }

    virtual void Accept(ASTVisitor &visitor) override { visitor.Visit(*this); }

private:
    VariableVector variables;
    Expr *expr;
};

class ASTPrinter : public ASTVisitor {
public:
    virtual void Visit(Factor &factor) override {
        llvm::outs() << "Factor";
        if (factor.GetType() == Factor::kIdent) {
            llvm::outs() << "(Ident): ";
        } else {
            llvm::outs() << "(Number): ";
        }
        llvm::outs() << factor.GetVal() << "\n";
    }

    virtual void Visit(BinaryOp &binaryOp) override {
        llvm::outs() << "BinaryOp: " << binaryOp.GetDisplayText() << "\n";
        if (binaryOp.GetLeft()) {
            binaryOp.GetLeft()->Accept(*this);
        }
        if (binaryOp.GetRight()) {
            binaryOp.GetRight()->Accept(*this);
        }
    }

    virtual void Visit(WithDeclaration &withDecl) override {
        llvm::outs() << "With Variables\n";
        for (auto var : withDecl) {
            llvm::outs() << var << " ";
        }

        llvm::outs() << "\n";

        if (withDecl.GetExpr()) {
            withDecl.GetExpr()->Accept(*this);
        }
    }
};