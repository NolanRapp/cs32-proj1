#include "AST.h"


// Stores single number in Leaf
TreeLeaf::TreeLeaf(double val) {
    value = val;
}



// Returns number in Leaf
double TreeLeaf::evalDouble(std::unordered_map<std::string, double>& vars) const{
    return value;
}



// Should never return a bool value
bool TreeLeaf::evalBool(std::unordered_map<std::string, double>& vars) const{
    throw std::runtime_error("Runtime error: invalid opearand type.");   
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
double TreeOperator::evalDouble(std::unordered_map<std::string, double>& vars) const{
    // the purpose of this function is to return the evaluated S-Expression 
    // we can assume children vector is nonempty by Parser logic
    
    double result;

    if(operation == "="){
        // Sets result to final num/id which will return double or error
        result = children[children.size() - 1]->evalDouble(vars);

        // Changes all child identifiers to the value of the final num/id
        for(unsigned int i = 0; i < children.size() - 1; i++) {
            vars[children[i]->getID()] = result;
        }

        return result;
    }

    result = children[0]->evalDouble(vars);

    if (operation == "*") { 
        for (unsigned int i = 1; i < children.size(); i++) {
            result *= children[i]->evalDouble(vars);
        }
    }
    else if (operation == "/") {
        for (unsigned int i = 1; i < children.size(); i++) {
            // Special case of dividing by 0
            if (children[i]->evalDouble(vars) == 0){
                throw std::runtime_error("Runtime error: division by zero.");   
            }

            result /= children[i]->evalDouble(vars);
        }
    }
    /*else if (operation == "%") {
        for (unsigned int i = 1; i < children.size(); i++) {
            result %= children[i]->evalDouble(vars);
        }
    }*/

    else if (operation == "+") {
        for (unsigned int i = 1; i < children.size(); i++) {
            result += children[i]->evalDouble(vars);
        }
    }
    else if (operation == "-") {
        for (unsigned int i = 1; i < children.size(); i++) {
            result -= children[i]->evalDouble(vars);
        }
    }

    return result;
}



// Should never return a bool value
bool TreeOperator::evalBool(std::unordered_map<std::string, double>& vars) const{
    throw std::runtime_error("Runtime error: invalid opearand type.");   
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
double TreeIdentifier::evalDouble(std::unordered_map<std::string, double>& vars) const{
    if(vars.find(idName) == vars.end()){
        throw std::runtime_error("Runtime error: unknown identifier " + idName);
    }

    return vars[idName];
}



// Variables can't be assigned bools so it will always error
bool TreeIdentifier::evalBool(std::unordered_map<std::string, double>& vars) const{
    throw std::runtime_error("Runtime error: invalid opearand type.");   
}



// Prints ID name
void TreeIdentifier::printInfix() const{
    std::cout << idName;
}



// Important for assigning a value to the ID on the variables table
std::string TreeIdentifier::getID(){
    return idName;  
}










TreeBoolean::TreeBoolean(std::string op) {
    this->op = op;
}



void TreeBoolean::addChild(TreeNode* child){
    children.push_back(child);
}



// This should always throw an error
double TreeBoolean::evalDouble(std::unordered_map<std::string, double>& vars) const {
    throw std::runtime_error("Runtime error: invalid opearand type.");   
}



//
bool TreeBoolean::evalBool(std::unordered_map<std::string, double>& vars) const{
    if (children.size() != 2) {
        throw std::runtime_error("Logical operator has to have two children");
    }

    // Order Comparison (Only numbers)
    if (children[0]->rType() == ReturnType::NUM){
        double left  = children[0]->evalDouble(vars);
        double right = children[1]->evalDouble(vars);

        if (op == "<") {
            return (left < right);
        }
        else if (op == ">") {
            return (left > right);
        }
        else if (op == "<=") {
            return (left <= right);
        }
        else if (op == ">=") {
            return (left >= right);
        }
        else if (op == "=="){
            return (left == right);
        }
        else if (op == "!="){
            return (left != right);
        }
    }

    // Logical comparison (Only bools)
    bool left  = children[0]->evalBool(vars);
    bool right = children[1]->evalBool(vars);

    if (op == "|"){
        return (left || right);
    }
    else if (op == "^"){
        return ((left && !right) || (!left && right));
    }
    else if (op == "&"){
        return (left && right);
    }
    else if (op == "=="){
        return (left == right);
    }
    else if (op == "!="){
        return (left != right);
    } 

    // This should not run if Parser runs correctly
    throw std::runtime_error("TreeBoolean is holding wrong operator.");
}



//
void TreeBoolean::printInfix() const {
    if (children.size() != 2) {
        throw std::runtime_error("Logical operator has to have two children");
    }

    std::cout << '(';
    children[0]->printInfix();
    std::cout << ' ' << op << ' ';
    children[1]->printInfix();
    std::cout << ')';
}










TreeBooleanText::TreeBooleanText(bool TrueFalse) {
    this->TrueFalse = TrueFalse;
}



// This should always throw an error
double TreeBooleanText::evalDouble(std::unordered_map<std::string, double>& vars) const {
    throw std::runtime_error("Runtime error: invalid opearand type.");   
}



bool TreeBooleanText::evalBool(std::unordered_map<std::string, double>& vars) const {
    return TrueFalse;
}



void TreeBooleanText::printInfix() const {
    if (TrueFalse) {
        std::cout << "true";
    }
    else {
        std::cout << "false";
    }
}




// Incoming logic of TreeStatements
