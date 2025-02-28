#pragma once

#include <AST.h>
#include <Lexer.h>
#include <llvm/Support/Error.h>

class Parser {
public:
    Parser(Lexer &lexer) : lexer(lexer), hasError(false) { Advance(); }

    AST *Parse();

    bool HasError() { return hasError; }

private:
    AST *ParseCalc();
    Expr *ParseExpr();
    Expr *ParseTerm();
    Expr *ParseFactor();

    void AddError() {
        llvm::errs() << "Unexpected: " << token.GetText() << "\n";
        hasError = true;
    }

    void Advance() { lexer.GetNext(token); }

    bool IsNextTokenOfType(Token::TokenType tokenType) {
        if (!token.Is(tokenType)) {
            AddError();
            return true;
        }

        return false;
    }

    bool Consume(Token::TokenType tokenType) {
        if (IsNextTokenOfType(tokenType)) {
            return true;
        }

        Advance();
        return false;
    }

private:
    Lexer &lexer;
    Token token;
    bool hasError;
};