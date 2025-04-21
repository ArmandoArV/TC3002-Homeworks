#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stdexcept>
#include <string>
using namespace std;
class Parser {
public:
    // Token constants
    static const int MAS = 1, MENOS = 2, POR = 3, DIV = 4, ID = 5;
    static const int NUM = 6, PIZQ = 7, PDER = 8, PESO = 9;

    void parse(const ::vector<int>& inputTokens);

private:
    int token;
    ::vector<int> tokens;
    size_t current = 0;

    // Basic functions
    int getToken();
    void move();
    void consume(int t);
    void error(const ::string& msg);

    // Grammar rules
    void E();
    void Ep();
    void T();
    void Tp();
    void F();
};

#endif // PARSER_H