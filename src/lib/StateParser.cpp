#include "StateParser.h"


// Returns and pops first tree in forest
TreeNode* StateParser::popHead(){
    TreeNode* tempHead = mHeads.front();
    mHeads.pop_front();

    return tempHead;
}



// Checks if forest is empty
bool StateParser::isEmpty() const{
    return mHeads.empty();
}




// Creates a deque of statements as trees
void StateParser::createForest(std::deque<Token> oInput){

    while(oInput.front().text != "END"){

        // MISC tokens should never start a statement
        if(oInput.front().type == Type::MISC){ 
            throw ParseError(oInput.front().line, oInput.front().column, oInput.front().text);
        }

        mHeads.push_back(createStatement(oInput));
    }
}



// Creates single expression as a tree
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
    else if(stateStr == "return"){
        originalHead.reset(createReturn(input));
    }
    else{ // Redundant, for debugging (should never run)
        throw std::runtime_error("Called createStatement on invalid statement.");
    }

    return originalHead.release();
}



// Makes forest of statements between "{}"
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



// Parses and creates a "def" statement (a function definition)
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

    // Makes block into shared_ptr since this forest may be shared
    std::shared_ptr<std::vector<TreeNode*>> tmpForest = std::make_shared<std::vector<TreeNode*>>(createBlock(input));
    defHead->forest = tmpForest; 

    return defHead.release();
}



// Parses and creates an "if" statement (including "if-else")
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



// Parses and creates a "while" statement
TreeNode* StateParser::createWhile(std::deque<Token>& input){
    std::unique_ptr<TreeStatement> stateHead(new TreeStatement("while"));
 
    New_Parser parser;
    stateHead->condition = parser.parseForState(input);

    if(input.front().text != "{"){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects "{"
    }

    stateHead->truths = createBlock(input);

    return stateHead.release();
}



// Parses and creates a "print" statement
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



// Parses and creates a "return" statement
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



// Parses and creates an empty "expression" statement and stores an expression in it
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



