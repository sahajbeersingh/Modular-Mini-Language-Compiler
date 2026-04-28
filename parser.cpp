#include "parser.h"
#include <stdexcept>
#include <vector>

Parser::Parser(std::vector<Token> tokenList) : tokens(tokenList), pos(0) {}

Token Parser::current() {
    if (pos >= (int)tokens.size())
        return Token(TOKEN_EOF, "", 0, 0);
    return tokens[pos];
}

void Parser::advance() {
    if (pos < (int)tokens.size()) pos++;
}

bool Parser::match(TokenType type) {
    if (current().type == type) { advance(); return true; }
    return false;
}

void Parser::expect(TokenType type, std::string errorMsg) {
    if (current().type != type)
        throw std::runtime_error(errorMsg + " at line " +
                                 std::to_string(current().line));
    advance();
}

ASTNode* Parser::parse() {
    return parseProgram();
}

static ASTNode* buildStmtChain(std::vector<ASTNode*>& stmts) {
    ASTNode* head = nullptr;
    ASTNode* tail = nullptr;
    for (ASTNode* s : stmts) {
        ASTNode* env = new ASTNode("STMT", "", s);
        if (!head) { head = tail = env; }
        else       { tail->right = env; tail = env; }
    }
    return head;
}

ASTNode* Parser::parseProgram() {
    ASTNode* root = new ASTNode("PROGRAM", "");
    std::vector<ASTNode*> stmts;
    while (current().type != TOKEN_EOF)
        stmts.push_back(parseStatement());
    root->left = buildStmtChain(stmts);
    return root;
}

ASTNode* Parser::parseStatement() {
    switch (current().type) {
        case TOKEN_VAR:        return parseVarDeclaration();
        case TOKEN_IDENTIFIER: return parseAssignment();
        case TOKEN_SHOW:       return parseShowStatement();
        case TOKEN_CHECK:      return parseCheckStatement();
        case TOKEN_WHILE:      return parseWhileStatement();
        case TOKEN_FOR:        return parseForStatement();
        default:
            throw std::runtime_error("Unexpected token: " + current().toString());
    }
}

ASTNode* Parser::parseBlock() {
    expect(TOKEN_LBRACE, "Expected '{'");
    std::vector<ASTNode*> stmts;
    while (current().type != TOKEN_RBRACE && current().type != TOKEN_EOF)
        stmts.push_back(parseStatement());
    expect(TOKEN_RBRACE, "Expected '}'");
    return new ASTNode("BLOCK", "", buildStmtChain(stmts));
}

ASTNode* Parser::parseVarDeclaration() {
    expect(TOKEN_VAR, "Expected 'var'");

    std::string firstName = current().value;
    expect(TOKEN_IDENTIFIER, "Expected variable name after 'var'");

    ASTNode* head = new ASTNode("VAR_DECL", firstName);
    ASTNode* tail = head;

    while (match(TOKEN_COMMA)) {
        std::string name = current().value;
        expect(TOKEN_IDENTIFIER, "Expected variable name after ','");
        ASTNode* next = new ASTNode("VAR_DECL", name);
        tail->right = next;
        tail = next;
    }

    expect(TOKEN_SEMICOLON, "Expected ';' after variable declaration");
    return new ASTNode("VAR_LIST", "", head);
}

ASTNode* Parser::parseAssignment() {
    std::string name = current().value;
    expect(TOKEN_IDENTIFIER, "Expected variable name");
    expect(TOKEN_ASSIGN,     "Expected '='");
    ASTNode* expr = parseExpression();
    expect(TOKEN_SEMICOLON,  "Expected ';'");
    return new ASTNode("ASSIGN", name, expr);
}

ASTNode* Parser::parseShowStatement() {
    expect(TOKEN_SHOW,      "Expected 'show'");
    ASTNode* expr = parseExpression();
    expect(TOKEN_SEMICOLON, "Expected ';'");
    return new ASTNode("SHOW", "", expr);
}

ASTNode* Parser::parseCheckStatement() {
    expect(TOKEN_CHECK, "Expected 'check'");
    ASTNode* condition = parseExpression();

    ASTNode* thenBlock = parseBlock();
    ASTNode* thenNode  = new ASTNode("THEN", "", thenBlock);

    ASTNode* elseBlock = nullptr;
    if (current().type == TOKEN_OTHERWISE) {
        advance();
        elseBlock = parseBlock();
    }
    ASTNode* elseNode = new ASTNode("ELSE", "", elseBlock);  

    ASTNode* branch = new ASTNode("BRANCH", "", thenNode, elseNode);
    return new ASTNode("CHECK", "", condition, branch);
}

ASTNode* Parser::parseWhileStatement() {
    expect(TOKEN_WHILE, "Expected 'while'");
    ASTNode* condition = parseExpression();
    ASTNode* body      = parseBlock();
    return new ASTNode("WHILE", "", condition, body);
}


ASTNode* Parser::parseForStatement() {
    expect(TOKEN_FOR,    "Expected 'for'");
    expect(TOKEN_LPAREN, "Expected '('");

    ASTNode* init      = parseAssignment();           
    ASTNode* condition = parseExpression();
    expect(TOKEN_SEMICOLON, "Expected ';' after for-condition");

    std::string updateName = current().value;
    expect(TOKEN_IDENTIFIER, "Expected variable name in for-update");
    expect(TOKEN_ASSIGN,     "Expected '=' in for-update");
    ASTNode* updateExpr = parseExpression();
    ASTNode* update = new ASTNode("ASSIGN", updateName, updateExpr);

    expect(TOKEN_RPAREN, "Expected ')'");
    ASTNode* body = parseBlock();

    ASTNode* condUpd    = new ASTNode("COND_UPD", "", condition, update);
    ASTNode* loopControl = new ASTNode("FOR_CTRL", "", init, condUpd);
    return new ASTNode("FOR", "", loopControl, body);
}


ASTNode* Parser::parseExpression() {
    return parseComparison();
}

ASTNode* Parser::parseComparison() {
    ASTNode* left = parseAdditive();

    if (match(TOKEN_EQUAL)      || match(TOKEN_NOT_EQUAL)     ||
        match(TOKEN_LESS)       || match(TOKEN_GREATER)       ||
        match(TOKEN_LESS_EQUAL) || match(TOKEN_GREATER_EQUAL)) {

        std::string op = tokens[pos - 1].value;
        ASTNode* right = parseAdditive();
        return new ASTNode("BINARY", op, left, right);
    }

    return left;
}

ASTNode* Parser::parseAdditive() {
    ASTNode* left = parseTerm();

    while (match(TOKEN_PLUS) || match(TOKEN_MINUS)) {
        std::string op = tokens[pos - 1].value;
        ASTNode* right = parseTerm();
        left = new ASTNode("BINARY", op, left, right);
    }

    return left;
}

ASTNode* Parser::parseTerm() {
    ASTNode* left = parseFactor();

    while (match(TOKEN_MULTIPLY) || match(TOKEN_DIVIDE)) {
        std::string op = tokens[pos - 1].value;
        ASTNode* right = parseFactor();
        left = new ASTNode("BINARY", op, left, right);
    }

    return left;
}

ASTNode* Parser::parseFactor() {
    if (match(TOKEN_NUMBER))
        return new ASTNode("NUMBER", tokens[pos - 1].value);
    if (match(TOKEN_IDENTIFIER))
        return new ASTNode("IDENTIFIER", tokens[pos - 1].value);
    if (match(TOKEN_LPAREN)) {
        ASTNode* expr = parseExpression();
        expect(TOKEN_RPAREN, "Expected ')'");
        return expr;
    }
    throw std::runtime_error("Expected expression, got: " + current().toString());
}
