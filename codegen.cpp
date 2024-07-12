#include "codegen.h"

CodeGen::CodeGen() : code("") {}

std::string CodeGen::generateCode(std::shared_ptr<ASTNode> root) {
    genProgram(root);
    return code;
}

void CodeGen::genProgram(std::shared_ptr<ASTNode> node) {
    for (const auto& child : node->children) {
        genStatement(child);
    }
}

void CodeGen::genStatement(std::shared_ptr<ASTNode> node) {
    if (node->type == N_ASSIGNMENT) {
        genAssignment(node);
    }
}

void CodeGen::genAssignment(std::shared_ptr<ASTNode> node) {
    code += "LOAD " + node->value + "\n";
    genExpression(node->children[0]);
    code += "STORE " + node->value + "\n";
}

void CodeGen::genExpression(std::shared_ptr<ASTNode> node) {
    if (node->children.size() == 1) {
        genTerm(node->children[0]);
    } else {
        genTerm(node->children[0]);
        if (node->value == "+") {
            code += "ADD ";
        } else if (node->value == "-") {
            code += "SUB ";
        }
        genTerm(node->children[1]);
    }
}

void CodeGen::genTerm(std::shared_ptr<ASTNode> node) {
    if (node->children.size() == 1) {
        genFactor(node->children[0]);
    } else {
        genFactor(node->children[0]);
        if (node->value == "*") {
            code += "MUL ";
        } else if (node->value == "/") {
            code += "DIV ";
        }
        genFactor(node->children[1]);
    }
}

void CodeGen::genFactor(std::shared_ptr<ASTNode> node) {
    if (node->type == N_FACTOR) {
        if (std::isdigit(node->value[0])) {
            code += "LOAD #" + node->value + "\n";
        } else {
            code += "LOAD " + node->value + "\n";
        }
    } else {
        genExpression(node);
    }
}
