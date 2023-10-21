#ifndef PARSE_H
#define PARSE_H


#include "Token.h"
#include "AST.h"
#include <queue>
#include <set>


class Parser {
    /*
    This class reads in a token queue from the Lexer.
    It parses the queue to create an Abstract Syntax Tree.
    It uses the TreeNode, TreeLeaf, and TreeOperator classes.
    */

    public:
        Parser(std::queue<Token> oInput);
		void createTree(std::queue<Token>& input);
        TreeNode* popHead();
	
	private:
		std::queue<TreeNode*> mHeads;
		std::set<std::string> operators = {
			"*",
			"/",
			"+",
			"-"
		};
        void parseError(int line, int col, std::string text) const;
		bool isOp(std::string str) const; // Used to make syntax more intuitive to the reader
		
		TreeNode* closedTree(std::queue<Token>& input);
		TreeNode* opTree(std::queue<Token>& input);
		TreeNode* assignTree(std::queue<Token>& input);
		TreeNode* numTree(std::queue<Token>& input);
};


#endif
