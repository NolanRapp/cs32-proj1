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
	/*
	This class is used to construct a queue using standard input. This
	queue will be made of Tokens which are readable by the Parser class.
	*/
    public: 
        void createEnd(std::queue<Token>& inputq, int line, int column);
        void printTokens();
        void lex();
        std::queue<Token> getLexQueue();

    private:
        std::queue<Token> lexTokens;

};


#endif
