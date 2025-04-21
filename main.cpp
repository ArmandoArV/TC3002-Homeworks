#include "./ACT_3_3/Parser/Parser.h"
#include <iostream>
#include <vector>

void runTest(const std::string& description, const std::vector<int>& tokenTypes) {
    std::cout << "Testing: " << description << std::endl;

    vector<Parser::TokenInfo> tokenInfos;
    size_t line = 1;
    size_t position = 1;

    for (int type : tokenTypes) {
        string value;
        switch (type) {
            case Parser::MAS: value = "+"; break;
            case Parser::MENOS: value = "-"; break;
            case Parser::POR: value = "*"; break;
            case Parser::DIV: value = "/"; break;
            case Parser::ID: value = "id"; break;
            case Parser::NUM: value = "num"; break;
            case Parser::PIZQ: value = "("; break;
            case Parser::PDER: value = ")"; break;
            case Parser::PESO: value = "$"; break;
            default: value = "?"; break;
        }
        tokenInfos.push_back({type, value, line, position++});
    }

    try {
        Parser parser;
        parser.parse(tokenInfos);
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;

        // Print formatted line/position again for clarity
        size_t linePos = string(e.what()).find("line ");
        size_t posPos = string(e.what()).find("position ");
        if (linePos != string::npos && posPos != string::npos) {
            string lineNum = string(e.what()).substr(linePos + 5, posPos - linePos - 7);
            string posNum = string(e.what()).substr(posPos + 9);
            size_t colon = posNum.find(":");
            if (colon != string::npos) posNum = posNum.substr(0, colon);
            std::cerr << "Line: " << lineNum << ", Position: " << posNum << ": "
                      << e.what() << std::endl;
        }
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