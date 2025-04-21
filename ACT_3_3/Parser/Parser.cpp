#include "Parser.h"
#include <iostream>
#include <sstream>
using namespace std;

string Parser::tokenToString(int tokenType) const {
    switch (tokenType) {
        case MAS: return "'+'";
        case MENOS: return "'-'";
        case POR: return "'*'";
        case DIV: return "'/'";
        case NUM: return "number";
        case PIZQ: return "'('";
        case PDER: return "')'";
        case PESO: return "end of input";
        default: return "unknown token";
    }
}

Parser::TokenInfo Parser::getToken() {
    return current < tokens.size() ? tokens[current++] : TokenInfo{PESO, "$", 0, 0};
}

void Parser::move() {
    currentToken = getToken();
}

void Parser::consume(int expectedType) {
    if (expectedType == currentToken.type) {
        move();
    } else {
        ostringstream oss;
        oss << "Error at line " << currentToken.line << ", position " << currentToken.position
            << ": Expected " << tokenToString(expectedType)
            << ", but found " << tokenToString(currentToken.type)
            << " (" << currentToken.value << ")";
        error(oss.str());
    }
}

void Parser::error(const string& msg) {
    lastError = "Line: " + to_string(currentToken.line) + ", Position: " +
                to_string(currentToken.position) + ": " + msg;
}

void Parser::E() {
    T();  // Parse the first term
    Ep(); // Parse the rest of the expression
}

void Parser::T() {
    F();  // Parse the first factor
    Tp(); // Parse the rest of the term
}

void Parser::Ep() {
    switch (currentToken.type) {
        case MAS:
        case MENOS:
            consume(currentToken.type); // Consume '+' or '-'
            T();                        // Parse the next term
            Ep();                       // Continue parsing the rest of the expression
            break;
        case PDER:
        case PESO:
            return; // Valid end of expression
        default:
            ostringstream oss;
            oss << "Error at line " << currentToken.line << ", position " << currentToken.position
                << ": Expected '+', '-', ')' or end of input, but found "
                << tokenToString(currentToken.type);
            error(oss.str());
    }
}

void Parser::Tp() {
    switch (currentToken.type) {
        case POR:
        case DIV:
            consume(currentToken.type); // Consume '*' or '/'
            F();                        // Parse the next factor
            Tp();                       // Continue parsing the rest of the term
            break;
        case MAS:
        case MENOS:
        case PDER:
        case PESO:
            return; // Valid end of term
        default:
            ostringstream oss;
            oss << "Error at line " << currentToken.line << ", position " << currentToken.position
                << ": Expected '*', '/', '+', '-', ')' or end of input, but found "
                << tokenToString(currentToken.type);
            error(oss.str());
    }
}

void Parser::F() {
    switch (currentToken.type) {
        case PIZQ:
            consume(PIZQ); // Consume '('
            E();           // Parse the inner expression
            consume(PDER); // Consume ')'
            break;
        case NUM:
            consume(NUM); // Consume a number
            break;
        default:
            ostringstream oss;
            oss << "Error at line " << currentToken.line << ", position " << currentToken.position
                << ": Expected '(', number, but found " << tokenToString(currentToken.type);
            error(oss.str());
    }
}

void Parser::parse(const vector<TokenInfo>& inputTokens) {
    lastError.clear();

    if (inputTokens.empty() || inputTokens.back().type != PESO) {
        lastError = "Line: 1, Position: 0: Input must end with end token ($)";
        cout << lastError << endl;
        return;
    }

    tokens = inputTokens;
    current = 0;
    move();

    E();

    if (lastError.empty() && currentToken.type != PESO) {
        ostringstream oss;
        oss << "Error at line " << currentToken.line << ", position " << currentToken.position
            << ": Expected " << tokenToString(PESO) << ", but found "
            << tokenToString(currentToken.type);
        lastError = "Line: " + to_string(currentToken.line) + ", Position: " +
                    to_string(currentToken.position) + ": " + oss.str();
    }

    if (!lastError.empty()) {
        cout << lastError << endl;
    } else {
        cout << "Parsing successful!" << endl;
    }
}