#ifndef LEX_H
#define LEX_H

#include <vector>
#include <queue>
#include <set>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "token.h"

class Lexer {
    public: 
        void createEnd(std::queue<token>& inputq, int line, int column);
        void printTokens();
        token makeToken(int line, int column, std::string text);
        void lex();
        std::queue<token> getLexQueue();

    private:
        std::queue<token> lexTokens;

};

#endif