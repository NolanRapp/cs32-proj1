#ifndef PARSE_H
#define PARSE_H


#include "Token.h"
#include "AST.h"
#include <deque>
#include <set>


class Parser {
    /*
    This class reads in a token queue from the Lexer.
    It parses the queue to create an Abstract Syntax Tree.
    It uses the TreeNode, TreeLeaf, and TreeOperator classes.
    */

    public:
        Parser(std::deque<Token> oInput);
		void createTree(std::deque<Token>& input);
        TreeNode* popHead();
		bool isEmpty() const;
	
	private:
		std::deque<TreeNode*> mHeads;
		std::set<std::string> operators = {
			"*",
			"/",
			"+",
			"-"
		};
        void parseError(int line, int col, std::string text) const;
		bool isOp(std::string str) const; // Used to make syntax more intuitive to the reader

		TreeNode* closedTree(std::deque<Token>& input);
		TreeNode* opTree(std::deque<Token>& input);
		TreeNode* assignTree(std::deque<Token>& input);
};


#endif
