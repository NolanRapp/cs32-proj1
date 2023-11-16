#include "AST.h"


// Stores single number in Leaf
TreeLeaf::TreeLeaf(double val) {
    value = val;
}



// Returns a double value in Leaf
variableVal TreeLeaf::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    return variableVal(value);
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



//
variableVal TreeOperator::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    if(op == "+" || op == "-" || op == "*" ||
       op == "/" || op == "%"){
        return evalOp(vars);
    }
    return evalComp(vars);
}



// Evaluates double Operators and children (recursive)
variableVal TreeOperator::evalOp(std::unordered_map<std::string, variableVal>& vars) const{

    variableVal lVal(children[0]->evaluate(vars));
    if(lVal.type != ReturnType::NUM){
        throw std::runtime_error("Runtime error: invalid operand type.");
        // Needs only numbers
    }
    double result = lVal.value.d;

    if (op == "*") { 
        variableVal rVal(children[1]->evaluate(vars));
        if(rVal.type != ReturnType::NUM){
            throw std::runtime_error("Runtime error: invalid operand type.");
            // Needs only numbers
        }
        result *= rVal.value.d;
    }
    else if (op == "/") {
        variableVal rVal(children[1]->evaluate(vars));
        if(rVal.type != ReturnType::NUM){
            throw std::runtime_error("Runtime error: invalid operand type.");
            // Needs only numbers
        }

        // Special case of dividing by 0
        if (rVal.value.d == 0){
            throw std::runtime_error("Runtime error: division by zero.");   
        }

        result /= rVal.value.d;
    }
    else if (op == "%") {
        variableVal rVal(children[1]->evaluate(vars));
        if(rVal.type != ReturnType::NUM){
            throw std::runtime_error("Runtime error: invalid operand type.");
            // Needs only numbers
        }

        // Special case of dividing by 0
        if (rVal.value.d == 0){
            throw std::runtime_error("Runtime error: division by zero.");   
        }

        result = fmod(result, rVal.value.d);
    }
    else if (op == "+") {
        variableVal rVal(children[1]->evaluate(vars));
        if(rVal.type != ReturnType::NUM){
            throw std::runtime_error("Runtime error: invalid operand type.");
            // Needs only numbers
        }
        result += rVal.value.d;
    }
    else if (op == "-") { // Redundant "if" for readability
        variableVal rVal(children[1]->evaluate(vars));
        if(rVal.type != ReturnType::NUM){
            throw std::runtime_error("Runtime error: invalid operand type.");
            // Needs only numbers
        }
        result -= rVal.value.d;
    }

    return variableVal(result);
}



// Evaluates boolean Operator and children (recursive)
variableVal TreeOperator::evalComp(std::unordered_map<std::string, variableVal>& vars) const{

    variableVal lVal(children[0]->evaluate(vars));
    variableVal rVal(children[1]->evaluate(vars));

    ReturnType lReturn = lVal.type;
    ReturnType rReturn = rVal.type;
    
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
            throw std::runtime_error("Runtime error: invalid operand type.");   
        } 

        if (op == "<") {
            return variableVal(lVal.value.d < rVal.value.d);
        }
        else if (op == ">") {
            return variableVal(lVal.value.d > rVal.value.d);
        }
        else if (op == "<=") {
            return variableVal(lVal.value.d <= rVal.value.d);
        }
        else if (op == ">=") {
            return variableVal(lVal.value.d >= rVal.value.d);
        }
        else if (op == "=="){
            return variableVal(lVal.value.d == rVal.value.d);
        }
        else if (op == "!="){
            return variableVal(lVal.value.d != rVal.value.d);
        }
    }
    // Logical comparison (Only bools)
    else if(lReturn == ReturnType::BOOL){
        // Special case
        if(rReturn != ReturnType::BOOL){
            if(op == "=="){
                return false;
            }
            else if(op == "!="){
                return true;
            }
            throw std::runtime_error("Runtime error: invalid operand type.");   
        } 

        if (op == "|"){
            return variableVal(lVal.value.b || rVal.value.b);
        }
        else if (op == "^"){
            return variableVal((lVal.value.b && !rVal.value.b) || (!lVal.value.b && rVal.value.b));
        }
        else if (op == "&"){
            return variableVal(lVal.value.b && rVal.value.b);
        }
        else if (op == "=="){
            return variableVal(lVal.value.b == rVal.value.b);
        }
        else if (op == "!="){ // Redundant "if" for readability
            return variableVal(lVal.value.b != rVal.value.b);
        }
    }

    // This is only required if compariosn of non-number and non-bool values are allowed
    if(op == "=="){
        return false;
    }
    else if(op == "!="){
        return true;
    }

    // Fails if non-number or non-bool exists without equality comparison
    throw std::runtime_error("Runtime error: invalid operand type.");   
}



// Prints expression using all children of current Operation (recursive) using corresponding depth
void TreeOperator::printInfix(int depth) const {

    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    std::cout << '(';
    children[0]->printInfix(0);
    for (size_t i = 1; i < children.size(); i++) {
        std::cout << ' ' << op << ' ';
        children[i]->printInfix(0);
    }
    std::cout << ')';
}










// Initializes with name of ID used for assigning and returning values
TreeIdentifier::TreeIdentifier(std::string name) {
    idName = name;
}



//
variableVal TreeIdentifier::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    if(vars.find(idName) == vars.end()){ 
        throw std::runtime_error("Runtime error: unknown identifier " + idName);
    }

    return vars[idName];
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



//
variableVal TreeBoolean::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    return variableVal(value == "true");
}



// Prints the boolean with corresponding depth
void TreeBoolean::printInfix(int depth) const {
    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    std::cout << value;
}











//
variableVal TreeAssign::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    variableVal rVal(children[children.size()-1]->evaluate(vars));

    for (size_t i = 0; i < children.size() - 1; i++) {
        vars[children[i]->getID()] = rVal;
    }
    return rVal;
}



// Prints variable assignment infix with corresponding depth
void TreeAssign::printInfix(int depth) const {

    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    std::cout << '(';
    children[0]->printInfix(0);
    for (size_t i = 1; i < children.size(); i++) {
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



// Calls the corresponding evaluate function and return dummy value that will never be used
variableVal TreeStatement::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    if(stateStr == "def"){
        evaluateDef(vars);
    }
    else if(stateStr == "if"){
        evaluateIf(vars);
    }
    else if(stateStr == "while"){
        evaluateWhile(vars);
    }
    else if(stateStr == "print"){
        evaluatePrint(vars);
    }
    else if(stateStr == "return"){
        evaluateReturn(vars);
    }
    else if(stateStr == "expression"){ // Redundant for readability
        evaluateExp(vars);
    }

    return variableVal(); // Dummy return will never be used
}



// Evaluates the condition of a "def" statement (functions)
void TreeStatement::evaluateDef(std::unordered_map<std::string, variableVal>& vars) const{
    // Implement function eval
}



// Evaluates the "if" statement 
void TreeStatement::evaluateIf(std::unordered_map<std::string, variableVal>& vars) const{

    variableVal cVal(condition->evaluate(vars));

    // Special case for non boolean return type for condition
    if(cVal.type != ReturnType::BOOL){
        throw std::runtime_error("Runtime error: condition is not a bool.");   
    }

    // evaluates truths forest if condition evaluates true
    if(cVal.value.b){
        for(TreeNode* tree : truths){
            tree->evaluate(vars);
        }
        return;
    }
    
    // evaluates falses forest if condition evaluates false
    for(TreeNode* tree : falses){
        tree->evaluate(vars);
    }
    return;
}



// Evaluates the "while" statement 
void TreeStatement::evaluateWhile(std::unordered_map<std::string, variableVal>& vars) const{
    
    variableVal cVal(condition->evaluate(vars));

    // Special case for non boolean return type for condition
    if(cVal.type != ReturnType::BOOL){
        throw std::runtime_error("Runtime error: condition is not a bool.");   
    }

    // evaluates truths forest until condition tree evaluates to false
    while(cVal.value.b){
        for(TreeNode* tree : truths){
            tree->evaluate(vars);
        }

        cVal = variableVal(condition->evaluate(vars));

        // Special case for non boolean return type for condition
        if(cVal.type != ReturnType::BOOL){
            throw std::runtime_error("Runtime error: condition is not a bool.");   
        }
    }
    return;
}



// Evaluates the condition of a "print" statement and prints it
void TreeStatement::evaluatePrint(std::unordered_map<std::string, variableVal>& vars) const{
    variableVal cVal(condition->evaluate(vars));

    if(cVal.type == ReturnType::NUM){
        double value = cVal.value.d;
        std::cout << value << '\n';
        return;
    }

    if(cVal.type == ReturnType::BOOL){
        if(cVal.value.b){
            std::cout << "True\n";
            return;
        }
        std::cout << "False\n";
        return;
    }

    throw std::runtime_error("Called print on non-number or non-bool"); // May need to change to specific syntax
}



// Evaluates the condition of a "return" statement and throws it to be caught by function
void TreeStatement::evaluateReturn(std::unordered_map<std::string, variableVal>& vars) const{
    if(condition == nullptr){
        // Throw NUL type
    }

    variableVal cVal(condition->evaluate(vars));
    if(cVal.type == ReturnType::NUM){
        // Throw NUM type
    }

    // Throw BOOL type
}



// Evaluates the condition of an expression 
void TreeStatement::evaluateExp(std::unordered_map<std::string, variableVal>& vars) const{
    condition->evaluate(vars);

    return;
}



// Prints corresponding statement with depth and hands its depth off to its children to format
void TreeStatement::printInfix(int depth) const{

    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    if(stateStr == "def"){
        std::cout << "def " << params[0] << "(";
        for(size_t i = 1; i < params.size(); i++){
            std::cout << params[i];
            if(i != params.size()){
                std::cout << ", ";
            }
        }
        std::cout << ") {";

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
    else if(stateStr == "if"){ 
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
    else if(stateStr == "print"){
        std::cout << "print ";
        condition->printInfix(0);
        std::cout << ";";
    }
    else if(stateStr == "return"){
        std::cout << "return ";
        if(condition == nullptr){
            std::cout << "null";
        }
        else{
            condition->printInfix(0);
        }
        std::cout << ";";
    }
    else if(stateStr == "expression"){ // Redundant for readability
        condition->printInfix(0);
        std::cout << ";";
    }
}



