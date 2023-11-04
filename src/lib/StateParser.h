#ifndef STATEPARSER_H
#define STATEPARSER_H

#include "New_Parser.h"
#include "Lexer.h"
#include "Token.h"
#include "AST.h"
#include "Errors.h"

#include <dequeue>

class StateParser {

public:
	StateParser(){};
	~StateParser(){};

	void                    createForest(std::deque<Token> oInput);
	std::vector<TreeNode*>  createBlock(std::deque<Token>& input);
	TreeNode*               createTree(std::deque<Token>& input);
	TreeNode*               popHead();
	bool                    isEmpty() const;

private:
	std::deque<TreeNode*>   mHeads;
	bool                    isExp(Token& token) const;

};


#endif
