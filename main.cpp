#include "lexer.h"
#include "semantic.h"
#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void printAST(ASTNode* node, const std::string& prefix = "", bool isLast = true) {
    if (!node) return;

    std::cout << prefix << (isLast ? "|__ " : "|-- ");
    std::cout << "[" << node->type << "]";
    if (!node->value.empty()) std::cout << "  \"" << node->value << "\"";
    std::cout << "\n";

    std::string childPrefix = prefix + (isLast ? "   " : "|  ");

    bool isStatementChain = (node->type == "PROGRAM" ||
                             node->type == "VAR_LIST" ||
                             node->type == "VAR_DECL" ||
                             node->type == "ASSIGN"   ||
                             node->type == "SHOW"     ||
                             node->type == "THEN"     ||
                             node->type == "ELSE"     ||
                             node->type == "BLOCK");

    if (isStatementChain) {
        if (node->left)  printAST(node->left,  childPrefix, true);
        if (node->right) printAST(node->right, prefix, isLast);  
    } else {
        bool hasRight = (node->right != nullptr);
        if (node->left)  printAST(node->left,  childPrefix, !hasRight);
        if (node->right) printAST(node->right, childPrefix, true);
    }
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file '" << filename << "'\n";
        exit(1);
    }
    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

int main(int argc, char* argv[]) {
    std::string filename = (argc > 1) ? argv[1] : "sample.txt";
    if (argc == 1)
        std::cout << "No input file specified. Using default: sample.txt\n";

    std::string source = readFile(filename);

    std::cout << "\nCUSTOM SYNTAX COMPILER\n";
    std::cout << "Source file: " << filename << "\n";

    std::cout << "\n- SOURCE CODE -\n" << source << "\n";

    std::cout << "\n- TOKENS -\n";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    int tokenCount = 0;
    for (auto& tok : tokens) {
        if (tok.type != TOKEN_EOF) {
            std::cout << tok.toString() << "\n";
            tokenCount++;
        }
    }
    std::cout << "\nTokens generated: " << tokenCount << "\n";

    std::cout << "\n- AST -\n";
    Parser parser(tokens);

    try {
        ASTNode* root = parser.parse();
        std::cout << "Parsing successful!\n\n";
        printAST(root);
        cout << "\n- SEMANTIC ANALYSIS -\n";

        SemanticAnalyzer sa;
        sa.analyze(root);
        sa.printSymbolTable();
        sa.checkUnused();
    } catch (const std::exception& e) {
        std::cerr << "Parse error: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\nCOMPILATION SUCCESSFUL.\n";
    return 0;
}
