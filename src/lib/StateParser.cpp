#include "StateParser.h"


// Pops the first item from the mHeads deque and returns it
TreeNode* StateParser::popHead(){
    TreeNode* tempHead = mHeads.front();
    mHeads.pop_front();

    return tempHead;
}



// checks to see if there are any ASTs remaining in the mHeads deque
bool StateParser::isEmpty() const{
    return mHeads.empty();
}



// makes forest of user input until "END" token
void StateParser::createForest(std::deque<Token> oInput){

    while(oInput.front().text != "END"){

        // "{" and "}" should only be seen after statements 
        if(oInput.front().type == Type::END){ 
            throw ParseError(oInput.front().line, oInput.front().column, oInput.front().text);
        }

        mHeads.push_back(createStatement(oInput));
    }
}



// takes an input token that is confirmed as statement and turns it into a tree with required parts
TreeNode* StateParser::createStatement(std::deque<Token>& input){
    if(isExp(input.front())){
        return createExp(input);
    }

    std::string stateStr = input.front().text;
    input.pop_front(); // Reads statement command
    std::unique_ptr<TreeNode> originalHead(nullptr);

    if(stateStr == "def"){
        originalHead.reset(createDef(input));
    }
    else if(stateStr == "if"){
        originalHead.reset(createIf(input));
    }
    else if(stateStr == "while"){
        originalHead.reset(createWhile(input));
    }
    else if(stateStr == "print"){
        originalHead.reset(createPrint(input));
    }
    else if(stateStr == "return"){ // Redundant for readability
        originalHead.reset(createReturn(input));
    }
    else{
        throw std::runtime_error("Called createStatement on invalid statement."); // should never run
    }

    return originalHead.release();
}


// within a statement, turns all the commants between "{" and "}" tokens
std::vector<TreeNode*> StateParser::createBlock(std::deque<Token>& input){
    std::vector<TreeNode*> forest;

    input.pop_front(); // Reads "{"

    while(input.front().text != "}"){
        if(input.front().text == "{" || input.front().text == "END"){ 
            throw ParseError(input.front().line, input.front().column, input.front().text);
            // Doesn't expect "{" or "END"
        }
        forest.push_back(createStatement(input));
    }

    input.pop_front(); // Reads "}"

    return forest;
}


// Checks if next character indicates the beginning of an expression
bool StateParser::isExp(Token& token) const{
    return (token.type != Type::STATE && token.type != Type::END);
}



// Parses a function (the "def" statement)
TreeNode* StateParser::createDef(std::deque<Token>& input){ 
    if(input.front().type != Type::ID){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects identifier
    }

    std::unique_ptr<TreeDefinition> defHead(new TreeDefinition(input.front().text));
    input.pop_front(); // Reads name of function

    if(input.front().text != "("){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects "("
    }
    input.pop_front(); // Reads "("

    while(input.front().text != ")"){ 
        if(input.front().type != Type::ID){
            throw ParseError(input.front().line, input.front().column, input.front().text);
            // Expects identifier for parameter
        }
        defHead->params.push_back(input.front().text);
        input.pop_front(); // Reads parameter

        if(input.front().text == ")"){
            break;
        }

        if(input.front().text != ","){
            throw ParseError(input.front().line, input.front().column, input.front().text);
            // Expects identifier for parameter
        }
        input.pop_front(); // Reads ","

        if(input.front().type != Type::ID){
            throw ParseError(input.front().line, input.front().column, input.front().text);
            // Expects identifier after ","
        }
    }
    input.pop_front(); // Reads ")"

    if(input.front().text != "{"){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects "{"
    }

    std::shared_ptr<std::vector<TreeNode*>> tmpForest = std::make_shared<std::vector<TreeNode*>>(createBlock(input));
    defHead->forest = tmpForest; // adds trees until "}" token is found

    return defHead.release();
}



// Parses "if" and "if-else" statements
TreeNode* StateParser::createIf(std::deque<Token>& input){
    std::unique_ptr<TreeStatement> stateHead(new TreeStatement("if"));
 
    New_Parser parser;
    stateHead->condition = parser.parseForState(input);

    if(input.front().text != "{"){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects "{"
    }
    
    stateHead->truths = createBlock(input); // adds trees until "}" token is found

    if(input.front().text != "else"){
        return stateHead.release();
    }
    input.pop_front(); // Reads "else"

    // "else" is followed by "if" statement
    if(input.front().text == "if"){
        stateHead->falses.push_back(createStatement(input));
    }
    // "else" is followed by block
    else if(input.front().text == "{"){
        stateHead->falses = createBlock(input);
    }
    // "else" isn't followed by "{" or "if"
    else{
        throw ParseError(input.front().line, input.front().column, input.front().text);
    }

    return stateHead.release();
}



// Parses "while" statement
TreeNode* StateParser::createWhile(std::deque<Token>& input){
    std::unique_ptr<TreeStatement> stateHead(new TreeStatement("while"));
 
    New_Parser parser;
    stateHead->condition = parser.parseForState(input);

    if(input.front().text != "{"){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects "{"
    }

    stateHead->truths = createBlock(input); // adds trees until "}" token is found

    return stateHead.release();
}



// Parses the "print" statement
TreeNode* StateParser::createPrint(std::deque<Token>& input){
    std::unique_ptr<TreeStatement> stateHead(new TreeStatement("print"));
 
    New_Parser parser;
    stateHead->condition = parser.parseForState(input);

    if(input.front().text != ";"){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects ";" after stand-alone expressions
    }
    input.pop_front(); // Reads ";"

    return stateHead.release();
}



// Parses the "return" statement
TreeNode* StateParser::createReturn(std::deque<Token>& input){
    std::unique_ptr<TreeStatement> stateHead(new TreeStatement("return"));
 
    if(input.front().text == ";"){
        input.pop_front(); // Reads ";"
        stateHead->condition = nullptr;
        return stateHead.release();
    }
    
    if(input.front().text == "null"){
        input.pop_front(); // Reads "null"

        if(input.front().text == ";"){
            input.pop_front(); // Reads ";"
            std::unique_ptr<TreeStatement> nullPtr(new TreeStatement("null"));
            stateHead->condition = nullPtr.release();
            return stateHead.release();
        }
        else{
            throw ParseError(input.front().line, input.front().column, input.front().text);
            // Expects ";"
        }
    }

    New_Parser parser;
    stateHead->condition = parser.parseForState(input);

    if(input.front().text != ";"){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects ";" after stand-alone expressions
    }
    input.pop_front(); // Reads ";"

    return stateHead.release();
}



// Parses single expression as statement
TreeNode* StateParser::createExp(std::deque<Token>& input){ 
    std::unique_ptr<TreeStatement> stateHead(new TreeStatement("expression"));

    New_Parser parser;
    stateHead->condition = parser.parseForState(input);

    if(input.front().text != ";"){ 
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects ";" after stand-alone expressions
    }
    input.pop_front(); // Reads ";"

    return stateHead.release();
}



