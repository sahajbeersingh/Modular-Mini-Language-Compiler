#ifndef IR_H
#define IR_H

#include "parser.h"
#include <string>
#include <vector>

class IRGenerator {
private:
    int tempCounter;
    int labelCounter;
    std::vector<std::string> code;

    std::string newTemp();
    std::string newLabel();

    void emit(const std::string& line);
    void genStmt(ASTNode* node);
    void genStmtList(ASTNode* node);
    std::string genExpr(ASTNode* node);
    void genCondition(ASTNode* node, const std::string& trueLabel, const std::string& falseLabel);

public:
    IRGenerator();
    std::vector<std::string> generate(ASTNode* root);
};

#endif
