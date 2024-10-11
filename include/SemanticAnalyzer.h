#pragma oncde

#include <AST.h>

class SemanticAnalyzer {
public:
    SemanticAnalyzer() = default;
    ~SemanticAnalyzer() = default;

    bool Analyze(AST *tree);
};