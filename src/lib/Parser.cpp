#include "Parser.h"


// Stores single number in Leaf
TreeLeaf::TreeLeaf (double val) {
	value = val;
}



// Returns number in Leaf
double TreeLeaf::evaluateNode() const{
    return value;
}



// Prints number in Leaf
void TreeLeaf::printInfix() const {
	std::cout << value;
}






// Stores single char holding one of the 4 operators (+,-,/,*) in Operator
TreeOperator::TreeOperator(char operation) {
    this->operation = operation;

    // initializing operation used and vector for its operands and child operators
}



// Adds a right-most child for the sake of order of operations
void TreeOperator::addChild(TreeNode* child){
	children.push_back(child);
}



// Evaluates Operator and children (recursive)
double TreeOperator::evaluateNode() const{
    // the purpose of this function is to return the evaluated S-Expression	
	// we can assume children vector is nonempty
	
	double result = children[0]->evaluateNode();

	switch(operation) {
	case '*':
		for (unsigned int i = 1; i < children.size(); i++) {
			result *= children[i]->evaluateNode();
		}
		break;
	case '/':
		for (unsigned int i = 1; i < children.size(); i++) {
			// Special case of dividing by 0
			if (children[i]->evaluateNode() == 0){
				throw std::runtime_error("Runtime error: division by zero.");	
			}

			result /= children[i]->evaluateNode();
		}
		break;
	case '+':
		for (unsigned int i = 1; i < children.size(); i++) {
			result += children[i]->evaluateNode();
		}
		break;
	case '-':
		for (unsigned int i = 1; i < children.size(); i++) {
			result -= children[i]->evaluateNode();
		}
		break;
	}

    return result;
}



// Prints expression using all children of current Operation (recursive)
void TreeOperator::printInfix() const {
	if (children.empty()) {
		return;
	}

	std::cout << '(';
	children[0]->printInfix();
	for (unsigned int i = 1; i < children.size(); i++) {
		std::cout << ' ' << operation << ' ';
		children[i]->printInfix();
	}
	std::cout << ')';
}






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



