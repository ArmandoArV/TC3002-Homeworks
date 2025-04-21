#include "Lexer.h"
#include <cctype>
#include <stdexcept>
#include <iostream>
#include "../Token/Token.h"

const std::unordered_set<std::string> Lexer::keywords = {
    "and", "break", "dec", "elif", "else", "false", "if", "inc",
    "loop", "not", "or", "return", "true", "var",
    "printi", "printc", "prints", "println",
    "readi", "reads", "new", "size", "add", "get", "set", "main"
};

Lexer::Lexer(const std::string& source) : source(source) {}

char Lexer::currentChar() const {
    return position < source.length() ? source[position] : '\0';
}

char Lexer::peekChar() const {
    return (position + 1) < source.length() ? source[position + 1] : '\0';
}

void Lexer::advance() {
    if (position >= source.length()) return;
    if (source[position] == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    position++;
}

void Lexer::skipWhitespace() {
    while (std::isspace(currentChar())) {
        advance();
    }
}

Token Lexer::readNumber() {
    size_t startLine = line;
    size_t startColumn = column;
    std::string numStr;

    while (std::isdigit(currentChar())) {
        numStr += currentChar();
        advance();
    }

    return {TokenKind::LIT_INT, numStr, startLine, startColumn};
}

Token Lexer::readIdentifier() {
    size_t startLine = line;
    size_t startColumn = column;
    std::string ident;

    while (std::isalnum(currentChar()) || currentChar() == '_') {
        ident += currentChar();
        advance();
    }

    if (keywords.count(ident)) {
        return {TokenKindFromString(ident), ident, startLine, startColumn};
    }

    return {TokenKind::IDENTIFIER, ident, startLine, startColumn};
}

TokenKind Lexer::TokenKindFromString(const std::string& str) {
    static const std::unordered_map<std::string, TokenKind> keywordMap = {
        {"and", TokenKind::AND}, {"break", TokenKind::BREAK}, {"dec", TokenKind::DEC},
        {"elif", TokenKind::ELIF}, {"else", TokenKind::ELSE}, {"false", TokenKind::FALSE},
        {"if", TokenKind::IF}, {"inc", TokenKind::INC}, {"loop", TokenKind::LOOP},
        {"not", TokenKind::NOT}, {"or", TokenKind::OR}, {"return", TokenKind::RETURN},
        {"true", TokenKind::TRUE}, {"var", TokenKind::VAR}, {"printi", TokenKind::PRINTI},
        {"printc", TokenKind::PRINTC}, {"prints", TokenKind::PRINTS}, {"println", TokenKind::PRINTLN},
        {"readi", TokenKind::READI}, {"reads", TokenKind::READS}, {"new", TokenKind::NEW},
        {"size", TokenKind::SIZE}, {"add", TokenKind::ADD}, {"get", TokenKind::GET},
        {"set", TokenKind::SET}, {"main", TokenKind::MAIN}
    };

    auto it = keywordMap.find(str);
    return it != keywordMap.end() ? it->second : TokenKind::IDENTIFIER;
}

std::vector<Token> Lexer::tokenize() {
    while (position < source.length()) {
        char c = currentChar();

        if (std::isspace(c)) {
            skipWhitespace();
            continue;
        }

        if (std::isdigit(c)) {
            tokens.push_back(readNumber());
            continue;
        }

        if (std::isalpha(c) || c == '_') {
            tokens.push_back(readIdentifier());
            continue;
        }

        // Handle other cases (e.g., operators, strings, etc.)
        advance();
    }

    tokens.push_back({TokenKind::END_OF_FILE, "", line, column});
    return tokens;
}