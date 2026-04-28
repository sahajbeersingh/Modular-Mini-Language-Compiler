#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "parser.h"
#include <unordered_map>
#include <string>

using namespace std;

class SemanticAnalyzer {
private:
    unordered_map<string, string> symbolTable;
    unordered_map<string, bool> used;

    string getType(ASTNode* node);

public:
    void analyze(ASTNode* root);
    void printSymbolTable();
    void checkUnused();
};

#endif