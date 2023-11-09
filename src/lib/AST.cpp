#include "AST.h"


// Stores single number in Leaf
TreeLeaf::TreeLeaf(double val) {
    value = val;
}



// Returns number in Leaf
double TreeLeaf::evalDouble(std::unordered_map<std::string, variableVal>& vars) const{
    return value;
}



// TreeLeaf should error if evaluated as a boolean
bool TreeLeaf::evalBool(std::unordered_map<std::string, variableVal>& vars) const{
    throw std::runtime_error("Runtime error: invalid operand type.");   
}



// Prints number in Leaf with corresponding depth
void TreeLeaf::printInfix(int depth) const {

    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    std::cout << value;
}










// Stores the operation string ("+", "-", "/", "*", "%", ">", ">=", "<", "<=", "==", "!=", "&", "|", "^")
TreeOperator::TreeOperator(std::string operation) {
    op = operation;
}



// Adds a right-most child for the sake of order of operations
void TreeOperator::addChild(TreeNode* child){
    children.push_back(child);
}



// Evaluates double Operators and children (recursive)
double TreeOperator::evalDouble(std::unordered_map<std::string, variableVal>& vars) const{

    // Check to see if return is valid
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
    else if (op == "-") { // Redundant "if" for readability
        for (unsigned int i = 1; i < children.size(); i++) {
            result -= children[i]->evalDouble(vars);
        }
    }

    return result;
}



// Evaluates boolean Operator and children (recursive)
bool TreeOperator::evalBool(std::unordered_map<std::string, variableVal>& vars) const{

    // Check to see if return is valid
    if(type(vars) != ReturnType::BOOL){
        throw std::runtime_error("Runtime error: invalid operand type.");   
    }
    
    ReturnType lReturn = children[0]->type(vars);
    ReturnType rReturn = children[1]->type(vars);
    
    // Order Comparison (Only numbers)
    if (lReturn == ReturnType::NUM){

        // Special case
        if(rReturn != ReturnType::NUM){
            if(op == "=="){
                return false;
            }
            else if(op == "!="){
                return true;
            }
        } 

        // Evaluates children
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
    else if (lReturn == ReturnType::BOOL){

        // Special case
        if(rReturn != ReturnType::BOOL){
            if(op == "=="){
                return false;
            }
            else if(op == "!="){
                return true;
            }
        } 

        // Evaluates children
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
        else if (op == "!="){ // Redundant "if" for readability
            return (left != right);
        } 
    }

    // Will run if right operand is not a number or boolean
    throw std::runtime_error("Runtime error: invalid operand type.");   
}



// Prints expression using all children of current Operation (recursive) using corresponding depth
void TreeOperator::printInfix(int depth) const {

    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    std::cout << '(';
    children[0]->printInfix(0);
    for (unsigned int i = 1; i < children.size(); i++) {
        std::cout << ' ' << op << ' ';
        children[i]->printInfix(0);
    }
    std::cout << ')';
}










// Initializes with name of ID used for assigning and returning values
TreeIdentifier::TreeIdentifier(std::string name) {
    idName = name;
}



// Attempts to find double and return, if no correct value exists throws error
double TreeIdentifier::evalDouble(std::unordered_map<std::string, variableVal>& vars) const{
    if (type(vars) != ReturnType::NUM){
        throw std::runtime_error("Runtime error: unknown identifier " + idName);
    }
    return vars[idName].doubleVal;
}


// Attempts to find bool and return, if no correct value exists throws error
bool TreeIdentifier::evalBool(std::unordered_map<std::string, variableVal>& vars) const{
    if (type(vars) != ReturnType::BOOL){ 
        throw std::runtime_error("Runtime error: unknown identifier " + idName);
    }
    return vars[idName].boolVal;
}


// Prints ID name with corresponding depth
void TreeIdentifier::printInfix(int depth) const{

    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    std::cout << idName;
}



// Important for assigning a value to the ID on the variables table
std::string TreeIdentifier::getID(){
    return idName;  
}










// Places either "true" or "false" into the value member variable
TreeBoolean::TreeBoolean(std::string value) {
    this->value = value;
}



// Boolean nodes should throw errors if ever asked to return a double
double TreeBoolean::evalDouble(std::unordered_map<std::string, variableVal>& vars) const {
    throw std::runtime_error("Runtime error: invalid operand type.");   
}



// Returns boolean value corresponding to the value member variable
bool TreeBoolean::evalBool(std::unordered_map<std::string, variableVal>& vars) const{
    return (value == "true");
}



// Prints the boolean with corresponding depth
void TreeBoolean::printInfix(int depth) const {
    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    std::cout << value;
}











// Variable assignment evaluation for doubles
double TreeAssign::evalDouble(std::unordered_map<std::string, variableVal>& vars) const {

    // Confirms that right-most child can be evaluated as double
    if(children[children.size()-1]->type(vars) != ReturnType::NUM){
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



// Variable assignment evaluation for booleans
bool TreeAssign::evalBool(std::unordered_map<std::string, variableVal>& vars) const {

    // Confirms that right-most child can be evaluated as bool
    if(children[children.size()-1]->type(vars) != ReturnType::BOOL){
        throw std::runtime_error("Runtime error: invalid operand type.");   
    }

    // Sets result to final bool/id which will return bool or error
    bool result = children[children.size() - 1]->evalBool(vars);
    variableVal val(result);

    // Changes all child identifiers to the value of the final bool/id
    for (unsigned int i = 0; i < children.size() - 1; i++) {
        vars[children[i]->getID()] = val;
    }
    return result;
}



// Prints variable assignment infix with corresponding depth
void TreeAssign::printInfix(int depth) const {

    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    std::cout << '(';
    children[0]->printInfix(0);
    for (unsigned int i = 1; i < children.size(); i++) {
        std::cout << " = ";
        children[i]->printInfix(0);
    }
    std::cout << ')';
}


// Adds a right-most child node
void TreeAssign::addChild(TreeNode* child){
    children.push_back(child);
}









// stores either "if", "while", "print" in stateStr
TreeStatement::TreeStatement(std::string statement){
    stateStr = statement;
}



// Evaluates the condition of a "print" statement and prints it
void TreeStatement::evaluateExp(std::unordered_map<std::string, variableVal>& vars) const{
    if(condition->type(vars) == ReturnType::NUM){
        condition->evalDouble(vars);
    }

    condition->evalBool(vars);
    return;
}



// Evaluates the condition of a "print" statement and prints it
void TreeStatement::evaluatePrint(std::unordered_map<std::string, variableVal>& vars) const{
    if(condition->type(vars) == ReturnType::NUM){
        double value = condition->evalDouble(vars);
        std::cout << value << '\n';
        return;
    }

    if(condition->evalBool(vars)){
        std::cout << "True\n";
        return;
    }
    std::cout << "False\n";
    return;
}



// Evaluates the "while" statement 
void TreeStatement::evaluateWhile(std::unordered_map<std::string, variableVal>& vars) const{
    
    // Special case for non boolean return type for condition
    if(condition->type(vars) != ReturnType::BOOL){
        throw std::runtime_error("Runtime error: condition is not a bool.");   
    }

    // evaluates truths forest until condition tree evaluates to false
    while(condition->evalBool(vars)){ 
        for(TreeNode* tree : truths){
            if(tree->type(vars) == ReturnType::NUM){
                tree->evalDouble(vars);
            }
            else{
                tree->evalBool(vars);
            }
        }

        // Rechecks special case above every loop 
        if(condition->type(vars) != ReturnType::BOOL){
            throw std::runtime_error("Runtime error: condition is not a bool.");   
        }
    }
    return;
}


// Evaluates the "if" statement 
void TreeStatement::evaluateIf(std::unordered_map<std::string, variableVal>& vars) const{

    // Special case for non boolean return type for condition
    if(condition->type(vars) != ReturnType::BOOL){
        throw std::runtime_error("Runtime error: condition is not a bool.");   
    }

    // evaluates truths forest if condition evaluates true
    if(condition->evalBool(vars)){
        for(TreeNode* tree : truths){
            if(tree->type(vars) == ReturnType::NUM){
                tree->evalDouble(vars);
            }
            else{
                tree->evalBool(vars);
            }
        }
        return;
    }
    
    // evaluates falses forest if condition evaluates false
    for(TreeNode* tree : falses){
        if(tree->type(vars) == ReturnType::NUM){
            tree->evalDouble(vars);
        }
        else{
            tree->evalBool(vars);
        }
    }
    return;
}



// Calls the corresponding evaluate function and return dummy value that will never be used
double TreeStatement::evalDouble(std::unordered_map<std::string, variableVal>& vars) const{
    if(stateStr == "expression"){
        evaluateExp(vars);
    }
    else if(stateStr == "print"){
        evaluatePrint(vars);
    }
    else if(stateStr == "while"){
        evaluateWhile(vars);
    }
    else{
        evaluateIf(vars);
    }

    return 0.0; // Dummy return will never be used
}



// For predictability statements are not evaluated as bools despite their return type not mattering
bool TreeStatement::evalBool(std::unordered_map<std::string, variableVal>& vars) const{
    throw std::runtime_error("Statement called evalBool, programmed wrong"); // should never run   
}



// Prints corresponding statement with depth and hands its depth off to its children to format
void TreeStatement::printInfix(int depth) const{

    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    if(stateStr == "expression"){
        condition->printInfix(0);
        std::cout << ";";
    }
    else if(stateStr == "print"){
        std::cout << "print ";
        condition->printInfix(0);
        std::cout << ";";
    }
    else if(stateStr == "while"){
        std::cout << "while ";
        condition->printInfix(0);
        std::cout << " {";

        for(TreeNode* tree : truths){
            std::cout << '\n';
            tree->printInfix(depth + 1);
        }
        
        std::cout << '\n';
        for(int i = 1; i <= depth; i++){
            std::cout << "    ";
        }
        std::cout << "}";
    }
    else{ 
        std::cout << "if ";
        condition->printInfix(0);
        std::cout << " {";

        for(TreeNode* tree : truths){
            std::cout << '\n';
            tree->printInfix(depth + 1);
        }
        
        std::cout << '\n';
        for(int i = 1; i <= depth; i++){
            std::cout << "    ";
        }
        std::cout << "}";
        
        if(falses.size() > 0){
            std::cout << '\n';
            for(int i = 1; i <= depth; i++){

                std::cout << "    ";
            }
            std::cout << "else {";
    
            for(TreeNode* tree : falses){
                std::cout << '\n';
                tree->printInfix(depth + 1);
            }
            
            std::cout << '\n';
            for(int i = 1; i <= depth; i++){

                std::cout << "    ";
            }
            std::cout << "}";
        }
    }
}



