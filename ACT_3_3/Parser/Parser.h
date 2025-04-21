#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stdexcept>
#include <string>
#include <utility> // for pair
using namespace std;

class Parser {
public:
    // Token constants for easy reference
    static const int MAS = 1, MENOS = 2, POR = 3, DIV = 4, ID = 5;
    static const int NUM = 6, PIZQ = 7, PDER = 8, PESO = 9;

    // TokenInfo structure to hold information about tokens
    struct TokenInfo {
        int type;          // Type of the token (e.g., MAS, NUM, etc.)
        string value;      // Value of the token (e.g., "1", "+", etc.)
        size_t line;       // Line number where the token appeared
        size_t position;   // Position of the token in the line
    };

    // Public method to start parsing
    void parse(const vector<TokenInfo>& inputTokens);

private:
    // Current token being processed
    TokenInfo currentToken;

    // List of tokens to parse
    vector<TokenInfo> tokens;

    // Index of the current token in the token list
    size_t current = 0;

    // Last error message (if any)
    string lastError;

    // Basic parsing functions
    TokenInfo getToken();
    void move();
    void consume(int expectedType);
    void error(const string& msg);

    // Grammar rule functions for expression parsing
    void E();
    void Ep();
    void T();
    void Tp();
    void F();

    // Helper function to convert token types to strings
    string tokenToString(int tokenType) const;
};

#endif // PARSER_H
