#include "Lexer.h"
#include <cctype>
#include <stdexcept>
#include <iostream>

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
    while (position < source.length()) {
        char c = currentChar();
        if (c == '\n') {
            line++;
            column = 1;
            position++;
        } else if (isspace(c)) {
            advance();
        } else {
            break;
        }
    }
}

bool isSpecialChar(char c) {
    return static_cast<unsigned char>(c) > 127;
}

bool isLatinLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

Token Lexer::readNumber() {
    size_t startLine = line;
    size_t startColumn = column;
    std::string numStr;

    if (currentChar() == '-') {
        numStr += '-';
        advance();
    }

    while (isdigit(currentChar())) {
        numStr += currentChar();
        advance();
    }

    return {TokenKind::LIT_INT, numStr, startLine, startColumn};
}

Token Lexer::readIdentifier() {
    size_t startLine = line;
    size_t startColumn = column;
    std::string ident;

    if (isLatinLetter(currentChar()) || currentChar() == '_') {
        ident += currentChar();
        advance();
    } else {
        return {TokenKind::UNKNOWN, std::string(1, currentChar()), startLine, startColumn};
    }

    while (isalnum(currentChar()) || currentChar() == '_') {
        ident += currentChar();
        advance();
    }

    if (keywords.count(ident)) {
        if (ident == "true" || ident == "false") {
            return {TokenKind::LIT_BOOL, ident, startLine, startColumn};
        }
        return {TokenKindFromString(ident), ident, startLine, startColumn};
    }

    return {TokenKind::IDENTIFIER, ident, startLine, startColumn};
}

Token Lexer::readString() {
    size_t startLine = line;
    size_t startColumn = column;
    std::string str;
    advance(); // Skip opening quote

    while (currentChar() != '"' && currentChar() != '\0') {
        if (currentChar() == '\\') {
            advance();
            switch (currentChar()) {
                case 'n': str += '\n'; break;
                case 'r': str += '\r'; break;
                case 't': str += '\t'; break;
                case '\\': str += '\\'; break;
                case '"': str += '"'; break;
                case '\'': str += '\''; break;
                default:
                    str += '\\';
                    str += currentChar();
                    break;
            }
        } else {
            str += currentChar();
        }
        advance();
    }

    if (currentChar() != '"') {
        throw std::runtime_error("Unterminated string literal");
    }
    advance(); // Skip closing quote

    return {TokenKind::LIT_STR, str, startLine, startColumn};
}

Token Lexer::readChar() {
    size_t startLine = line;
    size_t startColumn = column;
    std::string ch;
    advance(); // Skip opening quote

    if (currentChar() == '\\') {
        advance();
        switch (currentChar()) {
            case 'n': ch = "\n"; break;
            case 'r': ch = "\r"; break;
            case 't': ch = "\t"; break;
            case '\\': ch = "\\"; break;
            case '\'': ch = "'"; break;
            default:
                ch = "\\";
                ch += currentChar();
                break;
        }
        advance();
    } else {
        if (currentChar() == '\'') {
            throw std::runtime_error("Empty character literal");
        }
        ch += currentChar();
        advance();
    }

    if (currentChar() != '\'') {
        throw std::runtime_error("Unterminated character literal");
    }
    advance(); // Skip closing quote

    return {TokenKind::LIT_CHAR, ch, startLine, startColumn};
}

Token Lexer::readLineComment() {
    size_t startLine = line;
    size_t startColumn = column;
    std::string comment;
    advance(); advance(); // Skip initial //

    while (currentChar() != '\n' && currentChar() != '\0') {
        comment += currentChar();
        advance();
    }

    return {TokenKind::LINE_COMMENT, comment, startLine, startColumn};
}

Token Lexer::readBlockComment() {
    size_t startLine = line;
    size_t startColumn = column;
    std::string comment;
    advance(); advance(); // Skip /*

    while (position < source.length()) {
        if (currentChar() == '*' && peekChar() == '/') {
            advance(); advance();
            return {TokenKind::BLOCK_COMMENT, comment, startLine, startColumn};
        }
        comment += currentChar();
        advance();
    }

    throw std::runtime_error("Unterminated block comment");
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

        if (isspace(c)) {
            skipWhitespace();
            continue;
        }

        if (static_cast<unsigned char>(c) > 127) {
            tokens.push_back({TokenKind::UNKNOWN, std::string(1, c), line, column});
            advance();
            continue;
        }

        if (c == '/' && peekChar() == '/') {
            tokens.push_back(readLineComment());
            continue;
        }

        if (c == '/' && peekChar() == '*') {
            tokens.push_back(readBlockComment());
            continue;
        }

        if (isdigit(c) || (c == '-' && isdigit(peekChar()))) {
            tokens.push_back(readNumber());
            continue;
        }

        if (isalpha(c) || c == '_') {
            tokens.push_back(readIdentifier());
            continue;
        }

        if (c == '"') {
            tokens.push_back(readString());
            continue;
        }

        if (c == '\'') {
            tokens.push_back(readChar());
            continue;
        }

        switch (c) {
            case '=':
                advance();
                if (currentChar() == '=') {
                    advance();
                    tokens.push_back({TokenKind::EQUAL, "==", line, column - 2});
                } else {
                    tokens.push_back({TokenKind::ASSIGN, "=", line, column - 1});
                }
                continue;
            case '!':
                advance();
                if (currentChar() == '=') {
                    advance();
                    tokens.push_back({TokenKind::NOT_EQUAL, "!=", line, column - 2});
                } else {
                    tokens.push_back({TokenKind::UNKNOWN, "!", line, column - 1});
                }
                continue;
            case '<':
                advance();
                if (currentChar() == '=') {
                    advance();
                    tokens.push_back({TokenKind::LESS_EQUAL, "<=", line, column - 2});
                } else {
                    tokens.push_back({TokenKind::LESS, "<", line, column - 1});
                }
                continue;
            case '>':
                advance();
                if (currentChar() == '=') {
                    advance();
                    tokens.push_back({TokenKind::GREATER_EQUAL, ">=", line, column - 2});
                } else {
                    tokens.push_back({TokenKind::GREATER, ">", line, column - 1});
                }
                continue;
            case '+': advance(); tokens.push_back({TokenKind::PLUS, "+", line, column - 1}); continue;
            case '-': advance(); tokens.push_back({TokenKind::MINUS, "-", line, column - 1}); continue;
            case '*': advance(); tokens.push_back({TokenKind::ASTERISK, "*", line, column - 1}); continue;
            case '/': advance(); tokens.push_back({TokenKind::SLASH, "/", line, column - 1}); continue;
            case '%': advance(); tokens.push_back({TokenKind::PERCENT, "%", line, column - 1}); continue;
            case '(': advance(); tokens.push_back({TokenKind::LPAREN, "(", line, column - 1}); continue;
            case ')': advance(); tokens.push_back({TokenKind::RPAREN, ")", line, column - 1}); continue;
            case '{': advance(); tokens.push_back({TokenKind::LBRACE, "{", line, column - 1}); continue;
            case '}': advance(); tokens.push_back({TokenKind::RBRACE, "}", line, column - 1}); continue;
            case '[': advance(); tokens.push_back({TokenKind::LBRACKET, "[", line, column - 1}); continue;
            case ']': advance(); tokens.push_back({TokenKind::RBRACKET, "]", line, column - 1}); continue;
            case ',': advance(); tokens.push_back({TokenKind::COMMA, ",", line, column - 1}); continue;
            case ';': advance(); tokens.push_back({TokenKind::SEMICOLON, ";", line, column - 1}); continue;
            case ':': advance(); tokens.push_back({TokenKind::COLON, ":", line, column - 1}); continue;
            default:
                advance();
                tokens.push_back({TokenKind::UNKNOWN, std::string(1, c), line, column - 1});
        }
    }

    tokens.push_back({TokenKind::END_OF_FILE, "", line, column});
    return tokens;
}
