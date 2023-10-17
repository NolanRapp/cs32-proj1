#include "Parser.h"
#include "AST.h" // redundant


// Creates AST from the queue of Tokens that will be supplied by the Lexer
Parser::Parser(std::queue<Token> oInput) {
	if(oInput.front().text == "("){
		mHead = closedTree(oInput);
	}
	else if(isdigit(oInput.front().text.at(0))){
		mHead = numTree(oInput);
	}
	else{
		parseError(oInput.front().line, oInput.front().column, oInput.front().text);
		// Parse Error (First element should not be ")" or "END" or operation)
	}

	if(oInput.front().text != "END"){
		parseError(oInput.front().line, oInput.front().column, oInput.front().text);
		// Parse Error (Initial expression should always end in "END")
	}
}



// Evaluates an S expression in the context of "(" and ")", make sure it starts with operator
TreeNode* Parser::closedTree(std::queue<Token>& input){
	input.pop();
	TreeNode* head;

	if(isOp(input.front().text)){
		head = opTree(input);
	}
	else{	
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Invalid start for closed tree: 0,"(",")", and "END")
	}

	if(input.front().text != ")"){	
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Closed trees should end in ")")
	}
	input.pop();

	return head;
}



// Evaluates an S expression from its operator by adding valid children to it on the AST
TreeNode* Parser::opTree(std::queue<Token>& input){

	TreeOperator* op = new TreeOperator(input.front().text.at(0));
	TreeNode* tempExp;
	TreeLeaf* tempLeaf;
	int childNum = 0; // Counter for operands
	input.pop();

	while(isdigit(input.front().text.at(0)) || input.front().text == "("){
		if(input.front().text == "("){
			// Makes a child expression
			tempExp = closedTree(input);
			op->addChild(tempExp);
		}
		else{
			// Creates and adds a child number of the operator
			tempLeaf = new TreeLeaf(std::stold(input.front().text));
			op->addChild(tempLeaf);

			input.pop();
		}

		if(input.front().text == "END"){
			break;
		}

		childNum++;
	}

	if(childNum < 1){
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Operation tree needs atleast 1 children)
	}

	return op;
}



// Makes sure any expression starting with a number only has 1 number
TreeNode* Parser::numTree(std::queue<Token>& input){

	TreeLeaf* leaf = new TreeLeaf(std::stold(input.front().text));
	input.pop();

	if(isdigit(input.front().text.at(0))){
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Num tree can only have one number)
	}

	return leaf;
}



// Returns head of the AST
TreeNode* Parser::getHead() {
	return mHead;
}



// Helper for printing a specific Parse Error
void Parser::parseError(int line, int col, std::string text) const {
	std::cout << "Unexpected token at line " << line << " column "
	<< col << ": " << text << std::endl;
	exit(2);
}



// Helper to make code more readable
bool Parser::isOp(std::string str) const{
	return (operators.find(str) != operators.end());
}



