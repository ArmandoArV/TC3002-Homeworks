#include "ACT_3_3/Parser/Parser.h"
#include <iostream>
#include <vector>

void runTest(const std::string& description, const std::vector<int>& tokens) {
    std::cout << "Testing: " << description << std::endl;
    try {
        Parser parser;
        parser.parse(tokens);
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    std::cout << "-------------------------" << std::endl;
}

int main() {
    // Valid expressions
    runTest("( 3 + 4 )",
            {Parser::PIZQ, Parser::NUM, Parser::MAS, Parser::NUM, Parser::PDER, Parser::PESO});

    runTest("3 * ( 4 + 5 )",
            {Parser::NUM, Parser::POR, Parser::PIZQ, Parser::NUM, Parser::MAS, Parser::NUM,
             Parser::PDER, Parser::PESO});

    runTest("14 + 5 / 7 * ( 8 + 9 )",
            {Parser::NUM, Parser::MAS, Parser::NUM, Parser::DIV, Parser::NUM, Parser::POR,
             Parser::PIZQ, Parser::NUM, Parser::MAS, Parser::NUM, Parser::PDER, Parser::PESO});

    // Invalid expressions
    runTest("9++5 (invalid - consecutive operators)",
            {Parser::NUM, Parser::MAS, Parser::MAS, Parser::NUM, Parser::PESO});

    runTest("* ( 6 + 7) (invalid - starts with operator)",
            {Parser::POR, Parser::PIZQ, Parser::NUM, Parser::MAS, Parser::NUM,
             Parser::PDER, Parser::PESO});

    return 0;
}