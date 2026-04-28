#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include <vector>
#include <string>

struct ASTNode {
    std::string type;
    std::string value;
    ASTNode* left;
    ASTNode* right;

    ASTNode(std::string t, std::string v,
            ASTNode* l = nullptr, ASTNode* r = nullptr)
        : type(t), value(v), left(l), right(r) {}
};

class Parser {
private:
    std::vector<Token> tokens;
    int pos;

    Token current();
    void  advance();
    bool  match(TokenType type);
    void  expect(TokenType type, std::string errorMsg);

    ASTNode* parseProgram();
    ASTNode* parseStatement();
    ASTNode* parseVarDeclaration();
    ASTNode* parseAssignment();
    ASTNode* parseShowStatement();
    ASTNode* parseCheckStatement();
    ASTNode* parseWhileStatement();
    ASTNode* parseForStatement();
    ASTNode* parseBlock();

    ASTNode* parseExpression();
    ASTNode* parseComparison();
    ASTNode* parseAdditive();
    ASTNode* parseTerm();
    ASTNode* parseFactor();

public:
    explicit Parser(std::vector<Token> tokenList);
    ASTNode* parse();
};

#endif
