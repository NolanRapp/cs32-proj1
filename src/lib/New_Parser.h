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
    Each helper function is for a different operator precendence expression - Recursive Descent Parsin
    */

    public:
                  New_Parser(){};
                  ~New_Parser(){};
        
        TreeNode* parse(std::deque<Token>& tokenizedQ);                  // Semi-redundant parse of infix expression
        TreeNode* parseForCalc(std::deque<Token>& tokenizedQ);           // Calls parse with certain end conditions - main parse for calc.cpp
        TreeNode* parseForState(std::deque<Token>& tokenizedQ);          // Calls parse with certain end conditions - main parse for scrypt.cpp and format.cpp

        TreeNode* parseA(std::deque<Token>& tokenizedQ);                 // Parses assignment operators "="
        TreeNode* parseInclusive(std::deque<Token>& tokenizedQ);         // Parses inclusive or operators "|" 
        TreeNode* parseExclusive(std::deque<Token>& tokenizedQ);         // Parses exclusive or operators "^" 
        TreeNode* parseAnd(std::deque<Token>& tokenizedQ);               // Parses and operators "&"
        TreeNode* parseEquality(std::deque<Token>& tokenizedQ);          // Parses equality operators "==" and "!="
        TreeNode* parseComparison(std::deque<Token>& tokenizedQ);        // Parses comparison operators "<" or ">="
        TreeNode* parseE(std::deque<Token>& tokenizedQ);                 // Parses expression operators "+" "-"
        TreeNode* parseT(std::deque<Token>& tokenizedQ);                 // Parses term operators "*" "/" "%"
        TreeNode* parseCall(std::deque<Token>& tokenizedQ);              // Parses function calls
        TreeNode* parseF(std::deque<Token>& tokenizedQ);                 // Parses a factor (integer, ID, parenthesis, null)
        std::vector<TreeNode*> parseArgs(std::deque<Token>& tokenizedQ); // Parses arguments as expressions between bounding tokens ("()" or "[]")
        TreeNode* parseArray(std::deque<Token>& tokenizedQ);             // Parses an Array (Array Lookup: "expression [expression]") (Array Literal: "[0+ expressions]")


    private:
        std::string nextToken;                                           // Current token from the Token deque.
        std::string lookahead;                                           // Token ahead of nextToken
        Type        lookaheadType;                                       // lookahead's token type 
        Type        tokenType;                                           // Current token's token type

        void        scanToken(std::deque<Token>& tokenizedQ);            // Function to consume current token, moving nextToken to next value on the queue
        int         currentLine;                                         // Current token's line
        int         currentColumn;                                       // Current token's column
};


#endif
