#include "AST.h"


// Stores single number in Leaf
TreeLeaf::TreeLeaf(double val) {
	value = val;
}



// Returns number in Leaf
double TreeLeaf::evaluateNode(std::unordered_map<std::string, double>& vars) const{
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
double TreeOperator::evaluateNode(std::unordered_map<std::string, double>& vars) const{
    // the purpose of this function is to return the evaluated S-Expression	
	// we can assume children vector is nonempty
	
	double result;

	if(operation == '='){
		result = children[children.size() - 1]->evaluateNode(vars);

		// Changes all child identifiers to the value of the final num/id
		for(unsigned int i = 0; i < children.size() - 1; i++) {
			vars[children[i]->getID()] = result;
		}

		return result;
	}

	result = children[0]->evaluateNode(vars);

	switch(operation) {
	case '*':
		for (unsigned int i = 1; i < children.size(); i++) {
			result *= children[i]->evaluateNode(vars);
		}
		break;
	case '/':
		for (unsigned int i = 1; i < children.size(); i++) {
			// Special case of dividing by 0
			if (children[i]->evaluateNode(vars) == 0){
				throw std::runtime_error("Runtime error: division by zero.");	
			}

			result /= children[i]->evaluateNode(vars);
		}
		break;
	case '+':
		for (unsigned int i = 1; i < children.size(); i++) {
			result += children[i]->evaluateNode(vars);
		}
		break;
	case '-':
		for (unsigned int i = 1; i < children.size(); i++) {
			result -= children[i]->evaluateNode(vars);
		}
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






//
TreeIdentifier::TreeIdentifier(std::string name) {
	idName = name;
}



//
double TreeIdentifier::evaluateNode(std::unordered_map<std::string, double>& vars) const{
	if(vars.find(idName) == vars.end()){
		throw std::runtime_error("Runtime error: unknown identifier " + idName);
	}

	return vars[idName];
}



//
void TreeIdentifier::printInfix() const{
	std::cout << idName;
}



//
std::string TreeIdentifier::getID(){
	return idName;	
}



