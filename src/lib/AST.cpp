#include "AST.h"


// Evaluates equality between values
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
        case ReturnType::ARRAY: {
            // Check if one variableVal is NOT an array
            // using std::holds_alternative, https://en.cppreference.com/w/cpp/utility/variant/holds_alternative
            if (!std::holds_alternative<std::shared_ptr<Array>>(value) ||
                !std::holds_alternative<std::shared_ptr<Array>>(rVal.value)) {
                return false;
            }

            auto& left = std::get<std::shared_ptr<Array>>(value);
            auto& right = std::get<std::shared_ptr<Array>>(rVal.value);

            // Check if both shared pointers point to the same array
            if (left == right) {
                return true;
            }

            // Check if either array is null
            if (!left || !right) {
                return false;
            }

            // Check if array sizes are different
            if (left->elements.size() != right->elements.size()) {
                return false;
            }

            // Check if any elements in array are different
            for (size_t i = 0; i < std::get<std::shared_ptr<Array>>(value)->elements.size(); i++) {
                if (!(left->elements[i] == right->elements[i])) {
                    return false;
                }
            }

            // If everything equal, return true
            return true;
    }
        default:
            throw std::runtime_error("Compared type not in variableVals, programmed wrong");
    }
}



// Evaluates inequality between values
bool variableVal::operator != (const variableVal& rVal) const{
    return !(*this == rVal);
}



// Makes sure nothing else is using this forest
variableVal::Func::~Func(){
    delete mVars;
    if(mForest.use_count() == 1){
        for (TreeNode* child : *mForest) {
            delete child;
        }
    }
    mForest.reset();
}

/*variableVal::Array::~Array(){
    // implement?
}*/










// Stores a double inside node
TreeLeaf::TreeLeaf(double val) {
    value = val;
}



// Returns a variableVal holding the the node's double value
variableVal TreeLeaf::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    return variableVal(value);
}



// Prints double value
void TreeLeaf::printInfix(int depth) const {
    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    std::cout << value;
}










// Stores operator as string inside node
TreeOperator::TreeOperator(std::string operation) {
    op = operation;
}



// Adds children to node (left and right child)
void TreeOperator::addChild(TreeNode* child){
    children.push_back(child);
}



// Returns either a variableVal based on operator and its children
variableVal TreeOperator::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    if(op == "+" || op == "-" || op == "*" ||
       op == "/" || op == "%"){
        return evalOp(vars);
    }
    return evalComp(vars);
}



// Called by evaluate to return a double variableVal determined by children
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



// Called by evaluate to return a bool variableVal determined by children
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
    if (lVal.type == ReturnType::NUM && rVal.type == ReturnType::NUM){
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
    else if(lVal.type == ReturnType::BOOL && rVal.type == ReturnType::BOOL){
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



// Print in format "([left child] [operator] [right child])"
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










// Stores identifier name as string inside node
TreeIdentifier::TreeIdentifier(std::string name) {
    idName = name;
}



// Return variableVal that the identifiers name maps to in the variable map
variableVal TreeIdentifier::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    if(vars.find(idName) == vars.end()){ 
        throw std::runtime_error("Runtime error: unknown identifier " + idName);
    }

    return vars[idName];
}



// Prints identifier name
void TreeIdentifier::printInfix(int depth) const{
    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    std::cout << idName;
}



// Returns the variable name, also used to see if a node is a TreeIdentifier
std::string TreeIdentifier::getID(){
    return idName;  
}










// Stores "true" or "false" as string inside node
TreeBoolean::TreeBoolean(std::string value) {
    this->value = value;
}



// Returns variableVal True or False depending on node's value string
variableVal TreeBoolean::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    return variableVal(value == "true");
}



// Prints "True" or "False" depending on node's value string
void TreeBoolean::printInfix(int depth) const {
    for(int i = 1; i <= depth; i++){
        std::cout << "    ";
    }

    std::cout << value;
}










// Adds children to node (left and right child)
void TreeAssign::addChild(TreeNode* child){
    children.push_back(child);
}



// Attempts to assign left child (identifier or error) the value of its right child
variableVal TreeAssign::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    variableVal rVal(children[children.size()-1]->evaluate(vars));

    for (size_t i = 0; i < children.size() - 1; i++) {
        TreeNode* lVal = children[i];

        // If left child is a TreeArrayCall type, set equal to arrayC
        // using dynamic_cast https://en.cppreference.com/w/cpp/language/dynamic_cast
        if (TreeArrayCall* arrayC = dynamic_cast<TreeArrayCall*>(lVal)) {
            std::string arrayId;

            // Array element assignment
            try {
                arrayId = arrayC->getArrayName()->getID();
            }
            catch (const std::runtime_error& e) {
                // Gives error message with specific syntax
                throw std::runtime_error("Runtime error: not an array.");
            }
            
            // Check if array exists
            auto i = vars.find(arrayId);
            if (i == vars.end() || i->second.type != ReturnType::ARRAY) {
                throw std::runtime_error("Runtime error: not an array.");
            }


            // Array index
            variableVal index = arrayC->getArrayIndex()->evaluate(vars);
            if (index.type != ReturnType::NUM) {
                throw std::runtime_error("Runtime error: index is not a number.");
            }

            double idx = static_cast<double>(std::get<double>(index.value));
            auto& arrayElements = std::get<std::shared_ptr<variableVal::Array>>(i->second.value)->elements;

            // Check if index is positive and in bounds 
            if (idx < 0 || idx >= static_cast<int>(arrayElements.size())) {
                throw std::runtime_error("Runtime error: index out of bounds.");
            }

            // Check if index is an integer
            double integral;
            double fraction = modf(idx, &integral);
            if (fraction != 0.0) {
                throw std::runtime_error("Runtime error: index is not an integer.");
            }

            // Assign value
            arrayElements[idx] = rVal;
        } 

        // Otherwise, regular assignment
        else {
            vars[lVal->getID()] = rVal;
        }
    }

    return rVal;
}



// Print in format "([left child] = [right child])"
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










// Stores a ptr to a node as the name of the TreeCall (runtime error when name isn't an identifier)
TreeCall::TreeCall(TreeNode* func){
    this->func = func;
}



// Sets args vector given a vector of expressions
void TreeCall::setArgs(std::vector<TreeNode*> args){
    this->args = args;
}



// Evaluates a Func from the variable map corresponding to name of TreeCall
variableVal TreeCall::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    std::string funcName;

    try { 
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

    return variableVal(nullptr);
}



// Prints in format "[func]([args])"
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










// Stored a string as the name of the function defined
TreeDefinition::TreeDefinition(std::string name){
    funcName = name;
}



// Makes a Func object from member variables and stores in variable map under funcName
variableVal TreeDefinition::evaluate(std::unordered_map<std::string, variableVal>& vars) const{
    // Makes function object
    std::shared_ptr<variableVal::Func> func(new variableVal::Func(forest, params, vars));

    // Makes value out of function
    variableVal funcVal(func);

    // Passed value into variable map
    vars[funcName] = funcVal;

    return variableVal(); // Dummy val
}



// Prints in format "def [funcName]([params]){[forest]}" with proper newlines and indentation
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










// Stores command string in node
TreeStatement::TreeStatement(std::string statement){
    stateStr = statement;
}



// Will call a function based on stateStr or return a NUL type variableVal
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



// Evaluates an "if" statement
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



// Evaluates a "while" statement
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



// Evaluates a "print" statement
void TreeStatement::evaluatePrint(std::unordered_map<std::string, variableVal>& vars) const{
    variableVal cVal(condition->evaluate(vars));

    if(cVal.type == ReturnType::NUM){
        double value = std::get<double>(cVal.value);
        std::cout << value << '\n';
        return;
    }

    if(cVal.type == ReturnType::BOOL){
        if(std::get<bool>(cVal.value)){
            std::cout << "true\n";
            return;
        }
        std::cout << "false\n";
        return;
    }

    if(cVal.type == ReturnType::NUL){
        std::cout << "null\n";
        return;
    }

    if (cVal.type == ReturnType::ARRAY) {
        auto array = std::get<std::shared_ptr<variableVal::Array>>(cVal.value);
        printArray(array);
        std::cout << std::endl;
        return;
    }

    throw std::runtime_error("Called print on non-number or non-bool"); // May need to change to specific syntax
}



// Evaluates a "return" statement
void TreeStatement::evaluateReturn(std::unordered_map<std::string, variableVal>& vars) const{
    if(condition == nullptr){
        throw ReturnVal(variableVal(nullptr));
    }

    variableVal cVal(condition->evaluate(vars));
    throw ReturnVal(cVal);
}



// Evaluates a standalone expression
void TreeStatement::evaluateExp(std::unordered_map<std::string, variableVal>& vars) const{
    condition->evaluate(vars);

    return;
}



// Complicated print to print statement based on stateStr
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







// Evaluates Tree Array Literal Contents 
variableVal TreeArray::evaluate(std::unordered_map<std::string, variableVal>& vars) const {
    std::vector<variableVal> arrayContents;
    for (auto& e : arrayElements) {
        arrayContents.push_back(e->evaluate(vars));
    }
    return variableVal(std::make_shared<variableVal::Array>(arrayContents));
}



// Prints Tree Array Literal Infix
void TreeArray::printInfix(int depth) const {
    std::cout << "[";
    for (size_t i = 0; i < arrayElements.size(); ++i) {
        arrayElements[i]->printInfix(0);
        if (i < arrayElements.size()-1) {
            std::cout << ", ";
        }
    }
    std::cout << "]";
}






// Evaluates Tree Array Lookups, Checks for Errors
variableVal TreeArrayCall::evaluate(std::unordered_map<std::string, variableVal>& vars) const {
    variableVal arrayId = arrayName->evaluate(vars);

    // Check if arrayId is an array
    if (arrayId.type != ReturnType::ARRAY) {
        throw std::runtime_error("Runtime error: not an array.");
    }

    // Getting array elements
    auto& arrayElements = std::get<std::shared_ptr<variableVal::Array>>(arrayId.value)->elements;

    // Evaluating index of array expression
    variableVal arrayIndex = index->evaluate(vars);

    // Check if index is a number
    if (arrayIndex.type != ReturnType::NUM) {
        throw std::runtime_error("Runtime error: index is not a number.");
    }

    // Converting index of array to an integer
    double index = static_cast<double>(std::get<double>(arrayIndex.value));

    // Check if index is out of bounds
    if (index >= static_cast<int>(arrayElements.size()) || index < 0.0) {
        throw std::runtime_error("Runtime error: index out of bounds.");
    }

    // Check if index is not an integer
    double integral;
    double fraction = modf(index, &integral);
    if (fraction != 0.0) {
        throw std::runtime_error("Runtime error: index is not an integer.");
    }

    // Return value of indexed array
    return arrayElements[index];
}



// Prints Tree Array Lookup Infix
void TreeArrayCall::printInfix(int depth) const {
    arrayName->printInfix(depth);
    std::cout << "[";
    index->printInfix(depth);
    std::cout << "]";
}



void printArray(const std::shared_ptr<variableVal::Array>& array) {
    std::cout << "[";
    for (size_t i = 0; i < array->elements.size(); ++i) {

        if (array->elements[i].type == ReturnType::NUM) {
            std::cout << std::get<double>(array->elements[i].value);
        }
        else if (array->elements[i].type == ReturnType::BOOL) {
            std::cout << (std::get<bool>(array->elements[i].value) ? "true" : "false");
        }
        else if (array->elements[i].type == ReturnType::NUL) {
            std::cout << "null";
        }
        else if (array->elements[i].type == ReturnType::ARRAY) {
            printArray(std::get<std::shared_ptr<variableVal::Array>>(array->elements[i].value));
        }

        if (i < array->elements.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]";
}