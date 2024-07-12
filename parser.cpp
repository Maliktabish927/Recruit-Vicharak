#include "parser.h"
#include <stdexcept>

Parser::Parser(Lexer& lexer) : lexer(lexer) {
    currentToken = lexer.getNextToken();
}

Token Parser::getNextToken() {
    return currentToken = lexer.getNextToken();
}

std::shared_ptr<ASTNode> Parser::parse() {
    return program();
}

std::shared_ptr<ASTNode> Parser::program() {
    auto node = std::make_shared<ASTNode>();
    node->type = N_PROGRAM;
    while (currentToken.type != T_EOF) {
        node->children.push_back(statement());
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::statement() {
    if (currentToken.type == T_IDENTIFIER) {
        return assignment();
    }
    throw std::runtime_error("Unexpected token: " + currentToken.value);
}

std::shared_ptr<ASTNode> Parser::assignment() {
    auto node = std::make_shared<ASTNode>();
    node->type = N_ASSIGNMENT;
    node->value = currentToken.value;
    getNextToken(); // consume identifier
    if (currentToken.type != T_ASSIGN) {
        throw std::runtime_error("Expected '=' after identifier");
    }
    getNextToken(); // consume '='
    node->children.push_back(expression());
    if (currentToken.type != T_SEMICOLON) {
        throw std::runtime_error("Expected ';' after assignment");
    }
    getNextToken(); // consume ';'
    return node;
}

std::shared_ptr<ASTNode> Parser::expression() {
    auto node = term();
    while (currentToken.type == T_ADD || currentToken.type == T_SUB) {
        auto opNode = std::make_shared<ASTNode>();
        opNode->type = N_EXPRESSION;
        opNode->value = currentToken.value;
        opNode->children.push_back(node);
        getNextToken(); // consume operator
        opNode->children.push_back(term());
        node = opNode;
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::term() {
    auto node = factor();
    while (currentToken.type == T_MUL || currentToken.type == T_DIV) {
        auto opNode = std::make_shared<ASTNode>();
        opNode->type = N_TERM;
        opNode->value = currentToken.value;
        opNode->children.push_back(node);
        getNextToken(); // consume operator
        opNode->children.push_back(factor());
        node = opNode;
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::factor() {
    auto node = std::make_shared<ASTNode>();
    node->type = N_FACTOR;
    if (currentToken.type == T_IDENTIFIER || currentToken.type == T_NUMBER) {
        node->value = currentToken.value;
        getNextToken(); // consume factor
    } else if (currentToken.type == T_LPAREN) {
        getNextToken(); // consume '('
        node = expression();
        if (currentToken.type != T_RPAREN) {
            throw std::runtime_error("Expected ')'");
        }
        getNextToken(); // consume ')'
    } else {
        throw std::runtime_error("Unexpected token: " + currentToken.value);
    }
    return node;
}
