#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include <vector>
#include <memory>
#include <string>

struct ASTNode {
    virtual ~ASTNode() {}
    virtual std::string toString() = 0;
};

struct Program : ASTNode {
    std::vector<std::unique_ptr<ASTNode>> statements;
    std::string toString() override { return "Program"; }
};

struct Declaration : ASTNode {
    std::string varName;
    Declaration(std::string name) : varName(name) {}
    std::string toString() override { return "Declare: " + varName; }
};

struct Assignment : ASTNode {
    std::string varName;
    std::string expr;
    Assignment(std::string name, std::string e) : varName(name), expr(e) {}
    std::string toString() override { return varName + " = " + expr; }
};

struct Print : ASTNode {
    std::string expr;
    Print(std::string e) : expr(e) {}
    std::string toString() override { return "Print: " + expr; }
};

struct IfStatement : ASTNode {
    std::string condition;
    std::vector<std::unique_ptr<ASTNode>> thenBlock;
    std::vector<std::unique_ptr<ASTNode>> elseBlock;
    std::string toString() override { return "If: " + condition; }
};

#endif