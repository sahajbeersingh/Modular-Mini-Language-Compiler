#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file '" << filename << "'" << std::endl;
        std::cerr << "Make sure the file exists in the same folder as the program." << std::endl;
        exit(1);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    std::string filename;
    std::string source;
    
    if (argc > 1) {
        filename = argv[1];
    } else {
        filename = "sample.txt";
        std::cout << "No file specified. Using default: sample.txt" << std::endl;
    }
    source = readFile(filename);
    
    std::cout << "\nREADING FILE: " << filename << "\n";
    std::cout << "\nSOURCE CODE\n";
    std::cout << source << std::endl;
    
    std::cout << "\nTOKENS\n";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    int tokenCount = 0;
    for (auto& token : tokens) {
        if (token.type != TOKEN_EOF) {
            std::cout << token.toString() << std::endl;
            tokenCount++;
        }
    }
    std::cout << "\nCOMPILATION SUCCESSFUL\n";
    std::cout << "Lexical analysis completed." << std::endl;
    std::cout << "File: " << filename << std::endl;
    std::cout << "Tokens found: " << tokenCount << std::endl;
    std::cout << "Lines processed: " << lexer.tokenize().back().line - 1 << std::endl;
    
    return 0;
}