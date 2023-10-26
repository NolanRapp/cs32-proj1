#ifndef NEW_PARSER_H
#define NEW_PARSER_H

#include "Lexer.h"
#include "Token.h"
#include "AST.h"

#include <queue>
#include <memory>

/* New_Parser implements Recursive Descent Parsing - a top down AST parsing method
        sources: https://en.wikipedia.org/wiki/Recursive_descent_parser
                https://www.youtube.com/watch?v=SToUyjAsaFk
*/

class New_Parser {
    /* 
    Parser implentation for infix expressions.
    Each helper function is for a different operator precendence expression
    */

    public:
        New_Parser();
        ~New_Parser();
        
        TreeNode* parse(std::queue<Token>& tokenizedQ); // begins parse, and identifies when to end parse
        TreeNode* parseE(std::queue<Token>& tokenizedQ); // parsing and expression
        TreeNode* parseT(std::queue<Token>& tokenizedQ); // parsing a term
        TreeNode* parseF(std::queue<Token>& tokenizedQ); // parsing a factor

    private:
        std::string nextToken; // this is, at any point, the next unscanned token from the Token Queue.
        void scanToken(std::queue<Token>& tokenizedQ); // sets nextToken to point to the newly scanned token
};

#endif
