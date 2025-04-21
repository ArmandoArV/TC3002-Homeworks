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
    if (currentToken.type == PIZQ || currentToken.type == NUM || currentToken.type == ID) {
        T();
        Ep();
    } else {
        error("Expected '(', number, or identifier at the start of an expression.");
    }
}
void Parser::T() {
    if (currentToken.type == PIZQ || currentToken.type == NUM || currentToken.type == ID) {
        F();
        Tp();
    } else {
        error("Expected '(', number, or identifier at the start of a term.");
    }
}

void Parser::Ep() {
    switch (currentToken.type) {
        case MAS:
            consume(MAS);
            T();
            Ep();
            break;
        case MENOS:
            consume(MENOS);
            T();
            Ep();
            break;
        case PDER:
        case PESO:
            return; // Valid end of expression
        default:
            error("Expected '+', '-', ')', or end of input.");
    }
}

void Parser::Tp() {
    switch (currentToken.type) {
        case POR:
            consume(POR);
            F();
            Tp();
            break;
        case DIV:
            consume(DIV);
            F();
            Tp();
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
        case NUM:
            consume(NUM);
            break;
        case ID:
            consume(ID);
            break;
        case PIZQ:
            consume(PIZQ);
            E();
            consume(PDER);
            break;
        default:
            ostringstream oss;
            oss << "Error at line " << currentToken.line << ", position " << currentToken.position
                << ": Expected '(', number, or identifier, but found " << tokenToString(currentToken.type);
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