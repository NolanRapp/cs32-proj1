#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <vector>
#include <queue>
#include <set>
#include <iostream>
#include <iomanip>
#include <cstdlib>


class Lexer {
    public: 
        void createEnd(std::queue<token>& inputq, int line, int column);
        void printTokens();
        void lex();
        std::queue<token> getLexQueue();

    private:
        std::queue<token> lexTokens;

};

#endif
