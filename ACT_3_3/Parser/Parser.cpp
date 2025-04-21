#include "Parser.h"
#include <iostream>

using namespace std;
int Parser::getToken() {
    return current < tokens.size() ? tokens[current++] : PESO;
}

void Parser::move() {
    token = getToken();
}

void Parser::consume(int t) {
    if (t == token) {
        move();
    } else {
        throw ::runtime_error("Error: Expected token " + ::to_string(t) + 
                               ", but found " + ::to_string(token));
    }
}

void Parser::error(const ::string& msg) {
    throw ::runtime_error(msg);
}

void Parser::E() {
    if (token == PIZQ || token == NUM || token == ID) {
        T();
        Ep();
    } else {
        error("Expected '(', NUM or ID at expression start (E)");
    }
}

void Parser::Ep() {
    switch (token) {
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
            return;
        default:
            error("Expected '+', '-', ')' or end of input in Ep");
    }
}

void Parser::T() {
    if (token == PIZQ || token == NUM || token == ID) {
        F();
        Tp();
    } else {
        error("Expected '(', NUM or ID at term start (T)");
    }
}

void Parser::Tp() {
    switch (token) {
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
            return;
        default:
            error("Expected '*', '/', '+', '-', ')' or end of input in Tp");
    }
}

void Parser::F() {
    switch (token) {
        case PIZQ:
            consume(PIZQ);
            E();
            consume(PDER);
            break;
        case NUM:
            consume(NUM);
            break;
        case ID:
            consume(ID);
            break;
        default:
            error("Expected '(', NUM or ID in F");
    }
}

void Parser::parse(const ::vector<int>& inputTokens) {
    if (inputTokens.empty() || inputTokens.back() != PESO) {
        error("Error: Input must end with end token ($) → 9.");
    }

    tokens = inputTokens;
    current = 0;
    move();
    E();

    if (token != PESO) {
        error("Error: Expected end of input, but there are more tokens to analyze.");
    }

    ::cout << "SUCCESS: Parsing successful." << ::endl;
}