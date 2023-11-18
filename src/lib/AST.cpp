#include "AST.h"


bool variableVal::operator == (const variableVal& rVal) const{ 
    if(type != rVal.type){
        return false;
    }

    switch(type){
        case ReturnType::NONE:
            throw std::runtime_error("Compared NONE type, programmed wrong");
        case ReturnType::NUL:
            return true;
        case ReturnType::NUM:
            return (std::get<double>(value) == std::get<double>(rVal.value));
        case ReturnType::BOOL:
            return (std::get<bool>(value) == std::get<bool>(rVal.value));
        case ReturnType::FUNC:
            return (std::get<std::shared_ptr<Func>>(value)->mForest == std::get<std::shared_ptr<Func>>(rVal.value)->mForest);
        default:
            throw std::runtime_error("Compared type not in variableVals, programmed wrong");
    }
}



bool variableVal::operator != (const variableVal& rVal) const{
    return !(*this == rVal);
}










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
    double result = std::get<double>(lVal.value);

    if (op == "*") { 
        variableVal rVal(children[1]->evaluate(vars));
        if(rVal.type != ReturnType::NUM){
            throw std::runtime_error("Runtime error: invalid operand type.");
            // Needs only numbers
        }
        result *= std::get<double>(rVal.value);
    }
    else if (op == "/") {
        variableVal rVal(children[1]->evaluate(vars));
        if(rVal.type != ReturnType::NUM){
            throw std::runtime_error("Runtime error: invalid operand type.");
            // Needs only numbers
        }

        // Special case of dividing by 0
        if (std::get<double>(rVal.value) == 0){
            throw std::runtime_error("Runtime error: division by zero.");   
        }

        result /= std::get<double>(rVal.value);
    }
    else if (op == "%") {
        variableVal rVal(children[1]->evaluate(vars));
        if(rVal.type != ReturnType::NUM){
            throw std::runtime_error("Runtime error: invalid operand type.");
            // Needs only numbers
        }

        // Special case of dividing by 0
        if (std::get<double>(rVal.value) == 0){
            throw std::runtime_error("Runtime error: division by zero.");   
        }

        result = fmod(result, std::get<double>(rVal.value));
    }
    else if (op == "+") {
        variableVal rVal(children[1]->evaluate(vars));
        if(rVal.type != ReturnType::NUM){
            throw std::runtime_error("Runtime error: invalid operand type.");
            // Needs only numbers
        }
        result += std::get<double>(rVal.value);
    }
    else if (op == "-") { // Redundant "if" for readability
        variableVal rVal(children[1]->evaluate(vars));
        if(rVal.type != ReturnType::NUM){
            throw std::runtime_error("Runtime error: invalid operand type.");
            // Needs only numbers
        }
        result -= std::get<double>(rVal.value);
    }

    return variableVal(result);
}



// Evaluates boolean Operator and children (recursive)
variableVal TreeOperator::evalComp(std::unordered_map<std::string, variableVal>& vars) const{

    variableVal lVal(children[0]->evaluate(vars));
    variableVal rVal(children[1]->evaluate(vars));

    // This is only required if compariosn of non-number and non-bool values are allowed
    if(op == "=="){
        return (lVal == rVal);
    }
    else if(op == "!="){
        return (lVal != rVal);
    }

    // Order Comparison (Only numbers)
    if (lVal.type == ReturnType::NUM){
        if (op == "<") {
            return variableVal(std::get<double>(lVal.value) < std::get<double>(rVal.value));
        }
        else if (op == ">") {
            return variableVal(std::get<double>(lVal.value) > std::get<double>(rVal.value));
        }
        else if (op == "<=") {
            return variableVal(std::get<double>(lVal.value) <= std::get<double>(rVal.value));
        }
        else if (op == ">=") {
            return variableVal(std::get<double>(lVal.value) >= std::get<double>(rVal.value));
        }
    }
    // Logical comparison (Only bools)
    else if(lVal.type == ReturnType::BOOL){
        if (op == "|"){
            return variableVal(std::get<bool>(lVal.value) || std::get<bool>(rVal.value));
        }
        else if (op == "^"){
            return variableVal((std::get<bool>(lVal.value) && !std::get<bool>(rVal.value)) || (!std::get<bool>(lVal.value) && std::get<bool>(rVal.value)));
        }
        else if (op == "&"){
            return variableVal(std::get<bool>(lVal.value) && std::get<bool>(rVal.value));
        }
    }

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









//
TreeCall::TreeCall(TreeNode* func){
    this->func = func;
}



//
variableVal TreeCall::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    std::string funcName;

    try{ 
        // Will throw unless func is an identifier
        funcName = func->getID();
    }
    catch (const std::runtime_error& e) {
        // Gives error message new syntax
        throw std::runtime_error("Runtime error: not a function.");
    }
    // Will throw if identifier doesn't have a value or its not a function
    if(vars.find(funcName) == vars.end() || vars[funcName].type != ReturnType::FUNC){
        throw std::runtime_error("Runtime error: not a function.");
    }

    std::shared_ptr<variableVal::Func> function = std::get<std::shared_ptr<variableVal::Func>>(vars[funcName].value);
    if(args.size() != function->mParams.size()){
        throw std::runtime_error("Runtime error: incorrect argument count.");
    }

    std::unordered_map<std::string, variableVal> tempVars = *function->mVars;
    for(size_t i = 0; i < args.size(); i++){
        tempVars[function->mParams[i]] = args[i]->evaluate(vars);
    }

    try{
        for(TreeNode* tree : *function->mForest){
            tree->evaluate(tempVars);
        }
    }
    catch(const ReturnVal& returned){
        return variableVal(returned.getVal());
    }

    std::cout << "here\n";
    return variableVal(nullptr);
}



// Prints variable assignment infix with corresponding depth
void TreeCall::printInfix(int depth) const {

    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    func->printInfix(0);
    std::cout << "(";
    for (size_t i = 0; i < args.size(); i++) {
        args[i]->printInfix(0);
        if(i != (args.size() - 1)){
            std::cout << ", ";
        }
    }
    std::cout << ")";
}



//
void TreeCall::setArgs(std::vector<TreeNode*> args){
    this->args = args;
}










//
TreeDefinition::TreeDefinition(std::string name){
    funcName = name;
}



//
variableVal TreeDefinition::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    // Makes function object
    std::shared_ptr<variableVal::Func> func(new variableVal::Func(forest, params, vars));

    // Makes value out of function
    variableVal funcVal(func);

    // Passed value into variable map
    vars[funcName] = funcVal;

    return variableVal(); // Dummy val
}



// Prints variable assignment infix with corresponding depth
void TreeDefinition::printInfix(int depth) const {

    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    std::cout << "def " << funcName << "(";
    for(size_t i = 0; i < params.size(); i++){
        std::cout << params[i];
        if(i != (params.size() - 1)){
            std::cout << ", ";
        }
    }
    std::cout << ") {";

    for(TreeNode* tree : *forest){
        std::cout << '\n';
        tree->printInfix(depth + 1);
    }
    
    std::cout << '\n';
    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }
    std::cout << "}"; 
}










// stores either "if", "while", "print" in stateStr
TreeStatement::TreeStatement(std::string statement){
    stateStr = statement;
}



// Calls the corresponding evaluate function and return dummy value that will never be used
variableVal TreeStatement::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    if(stateStr == "if"){
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
    else{ // For "null" statement
        return variableVal(nullptr);
    }
    return variableVal();
}



// Evaluates the "if" statement 
void TreeStatement::evaluateIf(std::unordered_map<std::string, variableVal>& vars) const{

    variableVal cVal(condition->evaluate(vars));

    // Special case for non boolean return type for condition
    if(cVal.type != ReturnType::BOOL){
        throw std::runtime_error("Runtime error: condition is not a bool.");   
    }

    // evaluates truths forest if condition evaluates true
    if(std::get<bool>(cVal.value)){
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
    while(std::get<bool>(cVal.value)){
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
        double value = std::get<double>(cVal.value);
        std::cout << value << '\n';
        return;
    }

    if(cVal.type == ReturnType::BOOL){
        if(std::get<bool>(cVal.value)){
            std::cout << "True\n";
            return;
        }
        std::cout << "False\n";
        return;
    }

    if(cVal.type == ReturnType::NUL){
        std::cout << "null\n";
        return;
    }

    throw std::runtime_error("Called print on non-number or non-bool"); // May need to change to specific syntax
}



// Evaluates the condition of a "return" statement and throws it to be caught by function
void TreeStatement::evaluateReturn(std::unordered_map<std::string, variableVal>& vars) const{
    if(condition == nullptr){
        throw ReturnVal(variableVal(nullptr));
    }

    variableVal cVal(condition->evaluate(vars));
    throw ReturnVal(cVal);
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

    if(stateStr == "if"){ 
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
        std::cout << "return";
        if(condition != nullptr){
            std::cout << " ";
            condition->printInfix(0);
        }
        std::cout << ";";
    }
    else if(stateStr == "expression"){
        condition->printInfix(0);
        std::cout << ";";
    }
    else{ // "null" statement
        std::cout << "null";
    }
}



