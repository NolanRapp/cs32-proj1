#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <vector>
#include <queue>
#include <set>
#include <iostream>
#include <iomanip>
#include <cstdlib>


class Lexer {
    public: 
        void createEnd(std::queue<Token>& inputq, int line, int column);
        void printTokens();
        void lex();
        std::queue<Token> getLexQueue();

    private:
        std::queue<Token> lexTokens;

};

#endif
