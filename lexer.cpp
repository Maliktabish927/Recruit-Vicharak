#include "lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& input) : input(input), pos(0) {
    currentChar = input[pos];
}

char Lexer::getNextChar() {
    pos++;
    if (pos >= input.size()) {
        return '\0'; // End of input
    } else {
        return input[pos];
    }
}

void Lexer::skipWhitespace() {
    while (isspace(currentChar)) {
        currentChar = getNextChar();
    }
}

Token Lexer::identifier() {
    std::string result;
    while (isalnum(currentChar)) {
        result += currentChar;
        currentChar = getNextChar();
    }
    return {T_IDENTIFIER, result};
}

Token Lexer::number() {
    std::string result;
    while (isdigit(currentChar)) {
        result += currentChar;
        currentChar = getNextChar();
    }
    return {T_NUMBER, result};
}

Token Lexer::getNextToken() {
    while (currentChar != '\0') {
        if (isspace(currentChar)) {
            skipWhitespace();
            continue;
        }
        if (isalpha(currentChar)) {
            return identifier();
        }
        if (isdigit(currentChar)) {
            return number();
        }
        switch (currentChar) {
            case '=': currentChar = getNextChar(); return {T_ASSIGN, "="};
            case '+': currentChar = getNextChar(); return {T_ADD, "+"};
            case '-': currentChar = getNextChar(); return {T_SUB, "-"};
            case '*': currentChar = getNextChar(); return {T_MUL, "*"};
            case '/': currentChar = getNextChar(); return {T_DIV, "/"};
            case '(': currentChar = getNextChar(); return {T_LPAREN, "("};
            case ')': currentChar = getNextChar(); return {T_RPAREN, ")"};
            case ';': currentChar = getNextChar(); return {T_SEMICOLON, ";"};
            default:
                currentChar = getNextChar();
                return {T_UNKNOWN, std::string(1, currentChar)};
        }
    }
    return {T_EOF, ""};
}
