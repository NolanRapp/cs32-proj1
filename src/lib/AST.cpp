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






// Stores single char holding one of the 5 operators (+,-,/,*,=) in Operator
TreeOperator::TreeOperator(std::string operation) {
    this->operation = operation;
}



// Adds a right-most child for the sake of order of operations
void TreeOperator::addChild(TreeNode* child){
	children.push_back(child);
}



// Evaluates Operator and children (recursive)
double TreeOperator::evaluateNode(std::unordered_map<std::string, double>& vars) const{
    // the purpose of this function is to return the evaluated S-Expression	
	// we can assume children vector is nonempty by Parser logic
	
	double result;

	if(operation == "="){
		// Sets result to final num/id which will return double or error
		result = children[children.size() - 1]->evaluateNode(vars);

		// Changes all child identifiers to the value of the final num/id
		for(unsigned int i = 0; i < children.size() - 1; i++) {
			vars[children[i]->getID()] = result;
		}

		return result;
	}

	result = children[0]->evaluateNode(vars);

	if (operation == "*") { 
		for (unsigned int i = 1; i < children.size(); i++) {
			result *= children[i]->evaluateNode(vars);
		}
	}
	else if (operation == "/") {
		for (unsigned int i = 1; i < children.size(); i++) {
			// Special case of dividing by 0
			if (children[i]->evaluateNode(vars) == 0){
				throw std::runtime_error("Runtime error: division by zero.");	
			}

			result /= children[i]->evaluateNode(vars);
		}
	}
	else if (operation == "+") {
		for (unsigned int i = 1; i < children.size(); i++) {
			result += children[i]->evaluateNode(vars);
		}
	}
	else if (operation == "-") {
		for (unsigned int i = 1; i < children.size(); i++) {
			result -= children[i]->evaluateNode(vars);
		}
	}
	else if (operation == "<") {
		for (unsigned int i = 1; i < children.size(); i++) {
			result = children[i]->evaluateNode(vars) < children[i + 1]->evaluateNode(vars);
		}
	}
	else if (operation == ">") {
		for (unsigned int i = 1; i < children.size(); i++) {
			result = children[i]->evaluateNode(vars) > children[i + 1]->evaluateNode(vars);
		}
	}
	else if (operation == "<=") {
		for (unsigned int i = 1; i < children.size(); i++) {
			result = children[i]->evaluateNode(vars) <= children[i + 1]->evaluateNode(vars);
		}
	}
	else if (operation == ">=") {
		for (unsigned int i = 1; i < children.size(); i++) {
			result = children[i]->evaluateNode(vars) >= children[i + 1]->evaluateNode(vars);
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






// Initializes with name of ID used for assigning and returning values
TreeIdentifier::TreeIdentifier(std::string name) {
	idName = name;
}



// Attempts to find value and return, if no value exists throws error
double TreeIdentifier::evaluateNode(std::unordered_map<std::string, double>& vars) const{
	if(vars.find(idName) == vars.end()){
		throw std::runtime_error("Runtime error: unknown identifier " + idName);
	}

	return vars[idName];
}



// Prints ID name
void TreeIdentifier::printInfix() const{
	std::cout << idName;
}



// Important for assigning a value to the ID on the variables table
std::string TreeIdentifier::getID(){
	return idName;	
}



double TreeBoolean::evaluateNode(std::unordered_map<std::string, double>& vars) const {
	if (boolVal) {
		return 1.0;
	}
	else {
		return 0.0;
	}
}

void TreeBoolean::printInfix() const {
	if (boolVal) {
		std::cout << "true";
	}
	else {
		std::cout << "false";
	}
}
