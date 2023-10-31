#ifndef LEXER_H
#define LEXER_H


#include "Token.h"
#include "Errors.h"
#include <vector>
#include <deque>
#include <set>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>


class Lexer {
	/*
	This class is used to construct a de-queue using standard input. This
	de queue will be made of Tokens which are readable by the Parser class.
	*/
    public: 
        void createEnd(std::deque<Token>& inputq, int line, int column);
        void printTokens();
        void lex(std::string& inputString);
        std::deque<Token> getLexQueue();

    private:
        std::deque<Token> lexTokens;

};


#endif
