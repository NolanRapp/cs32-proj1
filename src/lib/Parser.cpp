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



Parser::Parser(std::queue<Token> originalInput) {
    // initialize the head of the created AST 

	contained = (originalInput.front().text == "("); // used to see if base expression was contianed by "()"	
	
	if(contained){
		// Pops off "(" and begins reading	
		originalInput.pop();
		if(originalInput.front().text == ")" || originalInput.front().text == "END"){
			parseError(originalInput.front().line, originalInput.front().column, originalInput.front().text);
			// Parse Error (Expects Operator or Number)
		}
	}

	if(operators.find(originalInput.front().text) == operators.end()){
		Token tempTok = originalInput.front();
		originalInput.pop();
		
		if(!isdigit(tempTok.text.at(0))){
			parseError(tempTok.line, tempTok.column, tempTok.text);
		}
		if((originalInput.front().text != ")" && contained) || (originalInput.front().text != "END" && !contained)){
			parseError(originalInput.front().line, originalInput.front().column, originalInput.front().text);
			// Parse Error (Expects ")" after number is first in S expression)
		}
		
		mHead = new TreeLeaf(std::stold(tempTok.text));
		originalInput.pop();
	}
	else{
		mHead = createTree(originalInput);
	}

	if ((originalInput.front().text != "END" && contained) || (!originalInput.empty() && !contained)) {
		parseError(originalInput.front().line, originalInput.front().column, originalInput.front().text);
		// Parse Error (checking input doesn't contain multiple (or no) top-level S expressions)
	}
}

TreeNode* Parser::createTree(std::queue<Token>& input) {
    /*
    The createTree function parses input tokens in a queue and constructs AST
    Recursion is used to simplify the tree build
    */

    TreeOperator* head;
	TreeLeaf* leaf;

    if (operators.find(input.front().text) != operators.end()) {
       	head = new TreeOperator(input.front().text.at(0));
        input.pop();

        while (input.front().text != ")" && input.front().text != "END") {
			
			// Makes sure only numbers or new expressions are registered
	        if (operators.find(input.front().text) != operators.end()) {
				parseError(input.front().line, input.front().column, input.front().text);
				// Parse Error (Expects only numbers and new S expressions)
			}

			// Checks for new expression
			if(input.front().text == "(") {
				input.pop();

				if(operators.find(input.front().text) == operators.end()){
					Token tempTok = input.front();
					input.pop();
					
					if(!isdigit(tempTok.text.at(0))){
						parseError(tempTok.line, tempTok.column, tempTok.text);
					}
					if(input.front().text != ")"){
						parseError(input.front().line, input.front().column, input.front().text);
					}
					
					TreeLeaf* tempLeaf = new TreeLeaf(std::stold(tempTok.text));
					head->addChild(tempLeaf);
					
					input.pop();
					continue;	
				}
			}
			head->addChild(createTree(input));
		}

		// Checks for special case of no "()"
		if(input.front().text == "END"){
			if(contained){
				parseError(input.front().line, input.front().column, input.front().text);
			}
		}

        input.pop();
		return head;
	}
	// (ideally) the only alternative case is a number
    else {	
        leaf = new TreeLeaf(std::stold(input.front().text));
        input.pop();
		return leaf;
    }

    return nullptr;
}

TreeNode* Parser::getHead() {
	return mHead;
}

void Parser::parseError(int line, int col, std::string text) {
	std::cout << "Unexpected token at line " << line << " column "
	<< col << ": " << text << std::endl;
	exit(2);
}



