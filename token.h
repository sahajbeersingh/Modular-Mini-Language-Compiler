#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

enum TokenType {
    // keywords
    TOKEN_INT,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_PRINT,
    
    // identifiers & literals
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    
    // operators
    TOKEN_ASSIGN,    // =
    TOKEN_PLUS,      // +
    TOKEN_MINUS,     // -
    TOKEN_MULTIPLY,  // *
    TOKEN_DIVIDE,    // /
    TOKEN_EQUAL,     // ==
    TOKEN_NOT_EQUAL, // !=
    TOKEN_LESS,      // <
    TOKEN_GREATER,   // >
    TOKEN_LESS_EQUAL,    // <=
    TOKEN_GREATER_EQUAL, // >=
    
    // delimiters
    TOKEN_SEMICOLON, // ;
    TOKEN_COMMA,     // ,
    TOKEN_LPAREN,    // (
    TOKEN_RPAREN,    // )
    TOKEN_LBRACE,    // {
    TOKEN_RBRACE,    // }
    
    // special
    TOKEN_EOF,
    TOKEN_ERROR
};

class Token {
public:
    TokenType type;
    std::string value;
    int line;
    int col;
    
    Token(TokenType t, std::string v, int l, int c) 
        : type(t), value(v), line(l), col(c) {}
    
    std::string toString() {
        std::string typeStr;
        switch(type) {
            case TOKEN_INT: typeStr = "INT"; break;
            case TOKEN_IF: typeStr = "IF"; break;
            case TOKEN_ELSE: typeStr = "ELSE"; break;
            case TOKEN_PRINT: typeStr = "PRINT"; break;
            case TOKEN_IDENTIFIER: typeStr = "IDENTIFIER"; break;
            case TOKEN_NUMBER: typeStr = "NUMBER"; break;
            case TOKEN_ASSIGN: typeStr = "ASSIGN"; break;
            case TOKEN_PLUS: typeStr = "PLUS"; break;
            case TOKEN_MINUS: typeStr = "MINUS"; break;
            case TOKEN_MULTIPLY: typeStr = "MULTIPLY"; break;
            case TOKEN_DIVIDE: typeStr = "DIVIDE"; break;
            case TOKEN_EQUAL: typeStr = "EQUAL"; break;
            case TOKEN_NOT_EQUAL: typeStr = "NOT_EQUAL"; break;
            case TOKEN_LESS: typeStr = "LESS"; break;
            case TOKEN_GREATER: typeStr = "GREATER"; break;
            case TOKEN_LESS_EQUAL: typeStr = "LESS_EQUAL"; break;
            case TOKEN_GREATER_EQUAL: typeStr = "GREATER_EQUAL"; break;
            case TOKEN_SEMICOLON: typeStr = "SEMICOLON"; break;
            case TOKEN_COMMA: typeStr = "COMMA"; break;
            case TOKEN_LPAREN: typeStr = "LPAREN"; break;
            case TOKEN_RPAREN: typeStr = "RPAREN"; break;
            case TOKEN_LBRACE: typeStr = "LBRACE"; break;
            case TOKEN_RBRACE: typeStr = "RBRACE"; break;
            case TOKEN_EOF: typeStr = "EOF"; break;
            default: typeStr = "ERROR";
        }
        return "Token(" + typeStr + ", '" + value + "')";
    }
};

#endif