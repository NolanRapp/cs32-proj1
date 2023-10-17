#include "Parser.h"


TreeLeaf::TreeLeaf (double val) {
    // initializing one numerical value
	value = val;
}



double TreeLeaf::evaluateNode() const{
    return value;
}



void TreeLeaf::printInfix() const {
	std::cout << value;
}






TreeOperator::TreeOperator(char operation) {
    this->operation = operation;

    // initializing operation used and vector for its operands and child operators
}



void TreeOperator::addChild(TreeNode* child){
	children.push_back(child);
}



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






Parser::Parser(std::queue<Token> oInput) {
	if(oInput.front().text == "("){
		mHead = closedTree(oInput);
	}
	else if(isOp(oInput.front().text)){
		mHead = opTree(oInput);
	}
	else if(isdigit(oInput.front().text.at(0))){
		mHead = numTree(oInput);
	}
	else{
		parseError(oInput.front().line, oInput.front().column, oInput.front().text);
		// Parse Error (First element should not be ")" or "END")
	}

	if(oInput.front().text != "END"){
		parseError(oInput.front().line, oInput.front().column, oInput.front().text);
		// Parse Error (Initial expression should always end in "END")
	}
}



TreeNode* Parser::closedTree(std::queue<Token>& input){
	input.pop();
	TreeNode* head;

	/*if(input.front().text == "("){ // comment out if "(())" is not allowed
		head = closedTree(input);
	}*/
	if(isOp(input.front().text)){
		head = opTree(input);
	}
	else if(isdigit(input.front().text.at(0))){
		head = numTree(input);
	}
	else{	
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Invalid start for closed tree)
	}

	if(input.front().text != ")"){	
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Closed trees should end in ")")
	}
	input.pop();

	return head;
}



TreeNode* Parser::opTree(std::queue<Token>& input){

	TreeOperator* op = new TreeOperator(input.front().text.at(0));
	TreeNode* tempExp;
	TreeLeaf* tempLeaf;
	Token tempTok = input.front();
	int childNum = 0;
	input.pop();

	while(isdigit(input.front().text.at(0)) || input.front().text == "("){
		std::cout << "Tried\n";
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

		childNum = 0;
	}

	if(childNum < 2){
		parseError(tempTok.line, tempTok.column, tempTok.text);
		// Parse Error (Operation tree needs atleast 2 children)
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



TreeNode* Parser::getHead() {
	return mHead;
}



void Parser::parseError(int line, int col, std::string text) const {
	std::cout << "Unexpected token at line " << line << " column "
	<< col << ": " << text << std::endl;
	exit(2);
}



bool Parser::isOp(std::string str) const{
	return (operators.find(str) != operators.end());
}



