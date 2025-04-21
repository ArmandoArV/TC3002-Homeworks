#include "./ACT_3_3/Parser/Parser.h"
#include "./Lexer/Lexer.h"
#include <iostream>
#include <vector>

void runTest(const string& description, const string& sourceCode) {
    cout << "Testing: " << description << endl;

    // Step 1: Tokenize the source code using the Lexer
    Lexer lexer(sourceCode);
    vector<Token> tokens = lexer.tokenize();

    // Step 2: Convert Lexer tokens to Parser::TokenInfo format
    vector<Parser::TokenInfo> tokenInfos;
    for (const Token& token : tokens) {
        int type;
        switch (token.kind) {
            case TokenKind::PLUS: type = Parser::MAS; break;
            case TokenKind::MINUS: type = Parser::MENOS; break;
            case TokenKind::ASTERISK: type = Parser::POR; break;
            case TokenKind::SLASH: type = Parser::DIV; break;
            case TokenKind::IDENTIFIER: type = Parser::ID; break;
            case TokenKind::LIT_INT: type = Parser::NUM; break;
            case TokenKind::LPAREN: type = Parser::PIZQ; break;
            case TokenKind::RPAREN: type = Parser::PDER; break;
            case TokenKind::END_OF_FILE: type = Parser::PESO; break;
            default: type = -1; break; // Unknown token
        }
        if (type != -1) {
            tokenInfos.push_back({type, token.value, token.line, token.column});
        }
    }

    // Step 3: Parse the tokens using the Parser
    try {
        Parser parser;
        parser.parse(tokenInfos);
    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
    }

    cout << "-------------------------" << endl;
}

int main() {
    // Valid expressions
    runTest("( 3 + 4 )", "( 3 + 4 )");
    runTest("3 * ( 4 + 5 )", "3 * ( 4 + 5 )");
    runTest("14 + 5 / 7 * ( 8 + 9 )", "14 + 5 / 7 * ( 8 + 9 )");

    // Invalid expressions
    runTest("9++5 (invalid - consecutive operators)", "9++5");
    runTest("* ( 6 + 7) (invalid - starts with operator)", "* ( 6 + 7)");

    return 0;
}