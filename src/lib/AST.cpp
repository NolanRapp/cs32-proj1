#include "AST.h"


// Stores single number in Leaf
TreeLeaf::TreeLeaf(double val) {
    value = val;
}



// Returns number in Leaf
double TreeLeaf::evalDouble(std::unordered_map<std::string, variableVal>& vars) const{
    return value;
}



// Should never return a bool value
bool TreeLeaf::evalBool(std::unordered_map<std::string, variableVal>& vars) const{
    throw std::runtime_error("Runtime error: invalid operand type.");   
}



// Prints number in Leaf
void TreeLeaf::printInfix() const {
    std::cout << value;
}










// Stores single char holding one of the 5 operators (+,-,/,*,=) in Operator
TreeOperator::TreeOperator(std::string operation) {
    op = operation;
}



// Adds a right-most child for the sake of order of operations
void TreeOperator::addChild(TreeNode* child){
    children.push_back(child);
}



// Evaluates double Operators and children (recursive)
double TreeOperator::evalDouble(std::unordered_map<std::string, variableVal>& vars) const{

    if(type(vars) != ReturnType::NUM){
        throw std::runtime_error("Runtime error: invalid operand type.");   
    }

    double result = children[0]->evalDouble(vars);

    if (op == "*") { 
        for (unsigned int i = 1; i < children.size(); i++) {
            result *= children[i]->evalDouble(vars);
        }
    }
    else if (op == "/") {
        for (unsigned int i = 1; i < children.size(); i++) {
            // Special case of dividing by 0
            if (children[i]->evalDouble(vars) == 0){
                throw std::runtime_error("Runtime error: division by zero.");   
            }

            result /= children[i]->evalDouble(vars);
        }
    }
    else if (op == "%") {
        for (unsigned int i = 1; i < children.size(); i++) {
            // Special case of dividing by 0
            if (children[i]->evalDouble(vars) == 0){
                throw std::runtime_error("Runtime error: division by zero.");   
            }

            result = fmod(result, children[i]->evalDouble(vars));
        }
    }

    else if (op == "+") {
        for (unsigned int i = 1; i < children.size(); i++) {
            result += children[i]->evalDouble(vars);
        }
    }
    else if (op == "-") {
        for (unsigned int i = 1; i < children.size(); i++) {
            result -= children[i]->evalDouble(vars);
        }
    }

    return result;
}



// Evaluates boolean Operator and children (recursive)
bool TreeOperator::evalBool(std::unordered_map<std::string, variableVal>& vars) const{

    if(type(vars) != ReturnType::BOOL){
        throw std::runtime_error("Runtime error: invalid operand type.");   
    }

    // Order Comparison (Only numbers)
    if (children[0]->type(vars) == ReturnType::NUM){
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

    return false; // Should never reach here
}



// Prints expression using all children of current Operation (recursive)
void TreeOperator::printInfix() const {
    if (children.empty()) {
        return;
    }

    std::cout << '(';
    children[0]->printInfix();
    for (unsigned int i = 1; i < children.size(); i++) {
        std::cout << ' ' << op << ' ';
        children[i]->printInfix();
    }
    std::cout << ')';
}










// Initializes with name of ID used for assigning and returning values
TreeIdentifier::TreeIdentifier(std::string name) {
    idName = name;
}



// Attempts to find value and return, if no value exists throws error
double TreeIdentifier::evalDouble(std::unordered_map<std::string, variableVal>& vars) const{
    if (type(vars) != ReturnType::NUM){
        throw std::runtime_error("Runtime error: unknown identifier " + idName);
    }
    return vars[idName].doubleVal;
}


// Variable assignment to boolean will throw error if enters here, implementation happens in TreeAssign
bool TreeIdentifier::evalBool(std::unordered_map<std::string, variableVal>& vars) const{
    if (type(vars) != ReturnType::BOOL){ 
        throw std::runtime_error("Runtime error: unknown identifier " + idName);
    }
    return vars[idName].boolVal;
}


// Prints ID name
void TreeIdentifier::printInfix() const{
    std::cout << idName;
}



// Important for assigning a value to the ID on the variables table
std::string TreeIdentifier::getID(){
    return idName;  
}










TreeBoolean::TreeBoolean(std::string value) {
    this->value = value;
}



// This should always throw an error
double TreeBoolean::evalDouble(std::unordered_map<std::string, variableVal>& vars) const {
    throw std::runtime_error("Runtime error: invalid operand type.");   
}



// Returns "true" 
bool TreeBoolean::evalBool(std::unordered_map<std::string, variableVal>& vars) const{
    return (value == "true");
}



// Prints the boolean
void TreeBoolean::printInfix() const {
    std::cout << value;
}









// Nothing to do here
TreeAssign::TreeAssign() {
}



// Variable assignment evaluation for doubles (will throw an error if Type::BOOL)
double TreeAssign::evalDouble(std::unordered_map<std::string, variableVal>& vars) const {
    if(children[0]->type(vars) == ReturnType::BOOL
    || children[1]->type(vars) != ReturnType::NUM){
        throw std::runtime_error("Runtime error: invalid operand type.");   
    }

    // Sets result to final num/id which will return double or error
    double result = children[children.size() - 1]->evalDouble(vars);
    variableVal val(result);

    // Changes all child identifiers to the value of the final num/id
    for (unsigned int i = 0; i < children.size() - 1; i++) {
        vars[children[i]->getID()] = val;
    }
    return result;
}



// Variable assignment for booleans (will throw an error if Type::NUM)
bool TreeAssign::evalBool(std::unordered_map<std::string, variableVal>& vars) const {
    if(children[0]->type(vars) == ReturnType::NUM 
    || children[1]->type(vars) != ReturnType::BOOL){
        throw std::runtime_error("Runtime error: invalid operand type.");   
    }

    // Sets result to final bool/id which will return double or error
    bool result = children[children.size() - 1]->evalBool(vars);
    variableVal val(result);

    // Changes all child identifiers to the value of the final num/id
    for (unsigned int i = 0; i < children.size() - 1; i++) {
        vars[children[i]->getID()] = val;
    }
    return result;
}



// Prints variable assignment infix
void TreeAssign::printInfix() const {
    if (children.empty()) {
        return;
    }

    std::cout << '(';
    children[0]->printInfix();
    for (unsigned int i = 1; i < children.size(); i++) {
        std::cout << " = ";
        children[i]->printInfix();
    }
    std::cout << ')';
}


// Adds a right-most child node
void TreeAssign::addChild(TreeNode* child){
    children.push_back(child);
}



// Incoming logic of TreeStatements
