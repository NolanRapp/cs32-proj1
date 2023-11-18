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
        void createEnd(std::deque<Token>& inputq, int line, int column, Type t); // Creates END type token at end of input
        void printTokens();                                                      // Prints tokens for debugging input
        std::deque<Token> getLexQueue();                                         // Retrieves the deque made from the input's tokens
        void lex(std::string& inputString);                                      // Reads a string and lexes input into tokens stored in deque

    private:
        std::deque<Token> lexTokens;                                             // Deque of input in token form

};


#endif
