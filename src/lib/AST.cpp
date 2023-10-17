#include "AST.h"


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



