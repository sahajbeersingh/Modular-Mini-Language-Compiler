#include <iostream>
#include "semantic.h"
using namespace std;

void SemanticAnalyzer::analyze(ASTNode* root) {
    if (!root) return;

    if (root->type == "PROGRAM" || root->type == "STMT" || root->type == "BLOCK") {
        analyze(root->left);
        analyze(root->right);
    }

    else if (root->type == "VAR_LIST") {
        analyze(root->left);
    }

    else if (root->type == "VAR_DECL") {
        string var = root->value;

        if (symbolTable.count(var)) {
            cout << "Semantic Error: '" << var << "' redeclared\n";
        } else {
            symbolTable[var] = "int";
            used[var] = false;
        }

        analyze(root->right); // only chain
    }

    else if (root->type == "ASSIGN") {
        string var = root->value;

        if (!symbolTable.count(var)) {
            cout << "Semantic Error: '" << var << "' not declared\n";
        } else {
            used[var] = true;
        }

        getType(root->left);
    }

    else if (root->type == "SHOW") {
        getType(root->left);
    }

    else if (root->type == "CHECK") {
        getType(root->left);
        analyze(root->right);
    }

    else if (root->type == "BRANCH") {
        analyze(root->left);
        analyze(root->right);
    }

    else if (root->type == "THEN" || root->type == "ELSE") {
        analyze(root->left);
    }

    else if (root->type == "WHILE") {
        getType(root->left);
        analyze(root->right);
    }

    else if (root->type == "FOR") {
        analyze(root->left);
        analyze(root->right);
    }

    else if (root->type == "BINARY") {
        getType(root);
    }
}

string SemanticAnalyzer::getType(ASTNode* node) {
    if (!node) return "int";

    if (node->type == "NUMBER") return "int";

    if (node->type == "IDENTIFIER") {
        if (!symbolTable.count(node->value)) {
            cout << "Semantic Error: '" << node->value << "' not declared\n";
        } else {
            used[node->value] = true;
        }
        return "int";
    }

    if (node->type == "BINARY") {
        string left = getType(node->left);
        string right = getType(node->right);

        if (left != right) {
            cout << "Semantic Error: Type mismatch\n";
        }
        return "int";
    }

    return "int";
}

void SemanticAnalyzer::printSymbolTable() {
    cout << "\nSymbol Table:\n";
    for (auto &entry : symbolTable) {
        cout << entry.first << " : " << entry.second << endl;
    }
}

void SemanticAnalyzer::checkUnused() {
    for (auto &entry : symbolTable) {
        if (!used[entry.first]) {
            cout << "Warning: '" << entry.first << "' declared but not used\n";
        }
    }
}