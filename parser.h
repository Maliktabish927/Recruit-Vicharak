#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

class Parser {
public:
    Parser(Lexer& lexer);
    std::shared_ptr<ASTNode> parse();
private:
    Token getNextToken();
    std::shared_ptr<ASTNode> program();
    std::shared_ptr<ASTNode> statement();
    std::shared_ptr<ASTNode> assignment();
    std::shared_ptr<ASTNode> expression();
    std::shared_ptr<ASTNode> term();
    std::shared_ptr<ASTNode> factor();

    Lexer& lexer;
    Token currentToken;
};

#endif
