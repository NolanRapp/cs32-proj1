#include "StateParser.h"


//
TreeNode* StateParser::popHead(){
	TreeNode* tempHead = mHeads.front();
	mHeads.pop_front();

	return tempHead;
}



//
bool StateParser::isEmpty() const{
    return mHeads.empty();
}



// Reads entire input to create complete program
void StateParser::createForest(std::deque<Token> oInput){
    while(oInput.front().text != "END"){
        if(oInput.front().type == Type::END){ // for "{" and "}"
            throw ParseError(oInput.front().line, oInput.front().column, oInput.front().text);
        }

        mHeads.push_back(createTree(oInput));
    }
}



// Reads single expression or statement
TreeNode* StateParser::createTree(std::deque<Token>& input){
    if(isExp(input.front())){
        New_Parser parser;
        return parser.parseForState(input);
    }

    return createStatement(input);
}


// When given "if" or "while" creates a node with a condition and a true and false tree
TreeNode* StateParser::createStatement(std::deque<Token>& input){
    std::string stateStr = input.front().text;
    input.pop_front(); // Reads statement command
    std::unique_ptr<TreeStatement> stateHead(new TreeStatement(stateStr));

    if(!isExp(input.front())){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects expression
    }
    New_Parser parser;
    stateHead->condition = parser.parseForState(input); 

    if(stateStr == "print"){
        return stateHead.release();
    }

    if(input.front().text != "{"){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects "{"
    }
    stateHead->truths = createBlock(input); // adds trees until "}"

    if(stateStr == "while" || input.front().text != "else"){
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


// Creates vector of trees in block between "{}"
std::vector<TreeNode*> StateParser::createBlock(std::deque<Token>& input){
    std::vector<TreeNode*> forest;

    input.pop_front(); // Reads "{"

    while(input.front().text != "}"){
        if(input.front().text == "{" || input.front().text == "END"){ 
            throw ParseError(input.front().line, input.front().column, input.front().text);
            // Doesn't expect "{" or "END"
        }
        forest.push_back(createTree(input));
    }

    input.pop_front(); // Reads "}"

    return forest;
}


// Checks if next character indicates the beginning of an expression
bool StateParser::isExp(Token& token) const{
    return (token.type != Type::STATE && token.type != Type::END);
}



