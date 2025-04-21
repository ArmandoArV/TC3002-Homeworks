#ifndef LEXER_H
#define LEXER_H

#include "../Token/Token.h"
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

class Lexer {
private:
    std::string source;
    size_t position = 0;
    size_t line = 1;
    size_t column = 1;
    std::vector<Token> tokens;

    static const std::unordered_set<std::string> keywords;

    char currentChar() const;
    char peekChar() const;
    void advance();
    void skipWhitespace();
    Token readNumber();
    Token readIdentifier();
    Token readString();
    Token readChar();
    Token readLineComment();
    Token readBlockComment();
    TokenKind TokenKindFromString(const std::string& str);

public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();
    static std::string tokenKindToString(TokenKind kind);
};

#endif // LEXER_H