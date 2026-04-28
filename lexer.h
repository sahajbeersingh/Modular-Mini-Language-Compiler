#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <vector>
#include <cctype>

class Lexer {
private:
    std::string input;
    int pos;
    int line;
    int col;
    std::vector<Token> tokens;

    char current() {
        if (pos >= input.length()) return '\0';
        return input[pos];
    }

    void advance() {
        if (current() == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }
        pos++;
    }

    void skipWhitespace() {
        while (current() == ' ' || current() == '\t' || current() == '\n' || current() == '\r') {
            advance();
        }
    }

public:
    Lexer(std::string source) : input(source), pos(0), line(1), col(1) {}

    std::vector<Token> tokenize() {
        tokens.clear();

        while (pos < input.length()) {
            skipWhitespace();
            if (pos >= input.length()) break;

            char c = current();
            int startLine = line;
            int startCol = col;

            if (c == '/' && pos + 1 < input.length() && input[pos + 1] == '/') {
                while (current() != '\n' && pos < input.length()) {
                    advance();
                }
                continue;
            }

            if (isalpha(c) || c == '_') {
                std::string value;
                while (isalnum(current()) || current() == '_') {
                    value += current();
                    advance();
                }

                TokenType type;
                if (value == "var") type = TOKEN_VAR;
                else if (value == "check") type = TOKEN_CHECK;
                else if (value == "otherwise") type = TOKEN_OTHERWISE;
                else if (value == "show") type = TOKEN_SHOW;
                else if (value == "while") type = TOKEN_WHILE;
                else if (value == "for") type = TOKEN_FOR;
                else type = TOKEN_IDENTIFIER;

                tokens.push_back(Token(type, value, startLine, startCol));
                continue;
            }

            if (isdigit(c)) {
                std::string value;
                while (isdigit(current())) {
                    value += current();
                    advance();
                }
                tokens.push_back(Token(TOKEN_NUMBER, value, startLine, startCol));
                continue;
            }

            advance();

            if (c == '=') {
                if (current() == '=') {
                    advance();
                    tokens.push_back(Token(TOKEN_EQUAL, "==", startLine, startCol));
                } else {
                    tokens.push_back(Token(TOKEN_ASSIGN, "=", startLine, startCol));
                }
            }
            else if (c == '!') {
                if (current() == '=') {
                    advance();
                    tokens.push_back(Token(TOKEN_NOT_EQUAL, "!=", startLine, startCol));
                } else {
                    tokens.push_back(Token(TOKEN_ERROR, "!", startLine, startCol));
                }
            }
            else if (c == '<') {
                if (current() == '=') {
                    advance();
                    tokens.push_back(Token(TOKEN_LESS_EQUAL, "<=", startLine, startCol));
                } else {
                    tokens.push_back(Token(TOKEN_LESS, "<", startLine, startCol));
                }
            }
            else if (c == '>') {
                if (current() == '=') {
                    advance();
                    tokens.push_back(Token(TOKEN_GREATER_EQUAL, ">=", startLine, startCol));
                } else {
                    tokens.push_back(Token(TOKEN_GREATER, ">", startLine, startCol));
                }
            }
            else if (c == '+') tokens.push_back(Token(TOKEN_PLUS, "+", startLine, startCol));
            else if (c == '-') tokens.push_back(Token(TOKEN_MINUS, "-", startLine, startCol));
            else if (c == '*') tokens.push_back(Token(TOKEN_MULTIPLY, "*", startLine, startCol));
            else if (c == '/') tokens.push_back(Token(TOKEN_DIVIDE, "/", startLine, startCol));
            else if (c == ';') tokens.push_back(Token(TOKEN_SEMICOLON, ";", startLine, startCol));
            else if (c == ',') tokens.push_back(Token(TOKEN_COMMA, ",", startLine, startCol));
            else if (c == '(') tokens.push_back(Token(TOKEN_LPAREN, "(", startLine, startCol));
            else if (c == ')') tokens.push_back(Token(TOKEN_RPAREN, ")", startLine, startCol));
            else if (c == '{') tokens.push_back(Token(TOKEN_LBRACE, "{", startLine, startCol));
            else if (c == '}') tokens.push_back(Token(TOKEN_RBRACE, "}", startLine, startCol));
            else {
                tokens.push_back(Token(TOKEN_ERROR, std::string(1, c), startLine, startCol));
            }
        }

        tokens.push_back(Token(TOKEN_EOF, "", line, col));
        return tokens;
    }
};

#endif
