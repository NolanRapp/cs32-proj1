#ifndef NEW_PARSER_H
#define NEW_PARSER_H

#include "Lexer.h"
#include "Token.h"
#include "AST.h"
#include "Errors.h"

#include <deque>
#include <memory>

/*
 New_Parser implements Recursive Descent Parsing - a top down AST parsing method

        sources:    https://en.wikipedia.org/wiki/Recursive_descent_parser
                    https://www.youtube.com/watch?v=SToUyjAsaFk
                    https://cplusplus.com/reference/istream/istream/peek/ 
                    https://cplusplus.com/reference/iomanip/setw/
                    https://cplusplus.com/reference/istream/istream/get/
                
*/

class New_Parser {
    /* 
    Parser implentation for infix expressions.
    Each helper function is for a different operator precendence expression- Recursive Descent Parsin
    */

    public:
        New_Parser();
        ~New_Parser();
        
        TreeNode* parse(std::deque<Token>& tokenizedQ); // begins parse
        TreeNode* parseForCalc(std::deque<Token>& tokenizedQ); // begins and ends parse calc.cpp

        TreeNode* parseE(std::deque<Token>& tokenizedQ); // parsing expression operators "+" "-"
        TreeNode* parseT(std::deque<Token>& tokenizedQ); // parsing term operators "*" "/" "%"
        TreeNode* parseF(std::deque<Token>& tokenizedQ); // parsing a factor (integer, ID, parenthesis)
        TreeNode* parseA(std::deque<Token>& tokenizedQ); // parsing assignment operators "="
        TreeNode* parseComparison(std::deque<Token>& tokenizedQ); // parsing comparison operators "<" or ">="
        TreeNode* parseEquality(std::deque<Token>& tokenizedQ); // parsing an equality operators "==" and "!="
        TreeNode* parseInclusive(std::deque<Token>& tokenizedQ); // parsing inclusive operators "|" 
        TreeNode* parseExclusive(std::deque<Token>& tokenizedQ); // parsing exclusive operators  "^" 
        TreeNode* parseAnd(std::deque<Token>& tokenizedQ); // parsing and operators "&"


    private:
        std::string nextToken; // the current token from the Token deque.
        std::string lookahead; // the token ahead of nextToken
        Type lookaheadType; // lookahead's token type 
        Type tokenType; // current token's token type

        void scanToken(std::deque<Token>& tokenizedQ); // sets nextToken and lookahead, pops front of deque
        int currentLine;
        int currentColumn;
};

#endif