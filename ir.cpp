#include "ir.h"

IRGenerator::IRGenerator() : tempCounter(0), labelCounter(0) {}

std::string IRGenerator::newTemp() {
    tempCounter++;
    return "t" + std::to_string(tempCounter);
}

std::string IRGenerator::newLabel() {
    labelCounter++;
    return "L" + std::to_string(labelCounter);
}

void IRGenerator::emit(const std::string& line) {
    code.push_back(line);
}

std::vector<std::string> IRGenerator::generate(ASTNode* root) {
    code.clear();
    tempCounter = 0;
    labelCounter = 0;
    genStmt(root);
    return code;
}

void IRGenerator::genStmtList(ASTNode* node) {
    ASTNode* current = node;
    while (current) {
        if (current->type == "STMT") {
            genStmt(current->left);
        } else {
            genStmt(current);
        }
        current = current->right;
    }
}

void IRGenerator::genStmt(ASTNode* node) {
    if (!node) return;

    if (node->type == "PROGRAM") {
        genStmtList(node->left);
        return;
    }

    if (node->type == "STMT") {
        genStmt(node->left);
        genStmt(node->right);
        return;
    }

    if (node->type == "BLOCK") {
        genStmtList(node->left);
        return;
    }

    if (node->type == "VAR_LIST") {
        return;
    }

    if (node->type == "VAR_DECL") {
        return;
    }

    if (node->type == "ASSIGN") {
        std::string rhs = genExpr(node->left);
        emit(node->value + " = " + rhs);
        return;
    }

    if (node->type == "SHOW") {
        std::string value = genExpr(node->left);
        emit("print " + value);
        return;
    }

    if (node->type == "CHECK") {
        std::string thenLabel = newLabel();
        std::string elseLabel = newLabel();
        std::string endLabel = newLabel();

        genCondition(node->left, thenLabel, elseLabel);
        emit(thenLabel + ":");
        if (node->right && node->right->left) {
            genStmt(node->right->left);
        }
        emit("goto " + endLabel);
        emit(elseLabel + ":");
        if (node->right && node->right->right) {
            genStmt(node->right->right);
        }
        emit(endLabel + ":");
        return;
    }

    if (node->type == "BRANCH") {
        genStmt(node->left);
        genStmt(node->right);
        return;
    }

    if (node->type == "THEN" || node->type == "ELSE") {
        genStmt(node->left);
        return;
    }

    if (node->type == "WHILE") {
        std::string startLabel = newLabel();
        std::string bodyLabel = newLabel();
        std::string endLabel = newLabel();

        emit(startLabel + ":");
        genCondition(node->left, bodyLabel, endLabel);
        emit(bodyLabel + ":");
        genStmt(node->right);
        emit("goto " + startLabel);
        emit(endLabel + ":");
        return;
    }

    if (node->type == "FOR") {
        ASTNode* control = node->left;
        ASTNode* body = node->right;

        if (control && control->left) {
            genStmt(control->left);
        }

        std::string startLabel = newLabel();
        std::string bodyLabel = newLabel();
        std::string endLabel = newLabel();

        emit(startLabel + ":");

        ASTNode* condUpd = control ? control->right : nullptr;
        ASTNode* condition = condUpd ? condUpd->left : nullptr;
        ASTNode* update = condUpd ? condUpd->right : nullptr;

        if (condition) {
            genCondition(condition, bodyLabel, endLabel);
        } else {
            emit("goto " + bodyLabel);
        }

        emit(bodyLabel + ":");
        genStmt(body);
        if (update) {
            genStmt(update);
        }
        emit("goto " + startLabel);
        emit(endLabel + ":");
        return;
    }
}

std::string IRGenerator::genExpr(ASTNode* node) {
    if (!node) return "";

    if (node->type == "NUMBER") {
        return node->value;
    }

    if (node->type == "IDENTIFIER") {
        return node->value;
    }

    if (node->type == "BINARY") {
        std::string left = genExpr(node->left);
        std::string right = genExpr(node->right);
        std::string temp = newTemp();
        emit(temp + " = " + left + " " + node->value + " " + right);
        return temp;
    }

    return "";
}

void IRGenerator::genCondition(ASTNode* node, const std::string& trueLabel, const std::string& falseLabel) {
    if (!node) {
        emit("goto " + falseLabel);
        return;
    }

    if (node->type == "BINARY" && node->left && node->right) {
        std::string left = genExpr(node->left);
        std::string right = genExpr(node->right);
        emit("if " + left + " " + node->value + " " + right + " goto " + trueLabel);
        emit("goto " + falseLabel);
        return;
    }

    std::string value = genExpr(node);
    emit("if " + value + " goto " + trueLabel);
    emit("goto " + falseLabel);
}
