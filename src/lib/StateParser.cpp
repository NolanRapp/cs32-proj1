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



// fills mHeads with ASTs representing each command made by the user that is seperated by an "END" token
void StateParser::createForest(std::deque<Token> oInput){
    while(oInput.front().text != "END"){
        if(oInput.front().type == Type::END){ // "{" and "}" tokens are parsed seperately and their contents are turned into seperate ASTs later in the program, so seeing them now should result in an error
            throw ParseError(oInput.front().line, oInput.front().column, oInput.front().text);
        }

        mHeads.push_back(createTree(oInput));
    }
}



// first checks to see if the statment at the from of the queue is an expression and if so, it is passed to the expression parser and returned as a new tree
// otherwise, it must be an expression, and it is passed to the statement creator and the statement is returned
TreeNode* StateParser::createTree(std::deque<Token>& input){
    if(isExp(input.front())){
        New_Parser parser;
        return parser.parseForState(input);
    }

    return createStatement(input);
}


// takes an input token that is confirmed as an expression statement and turns it into a tree containing the commands that exist within the statement
TreeNode* StateParser::createStatement(std::deque<Token>& input){
    std::string stateStr = input.front().text;
    input.pop_front(); // Reads statement command
    std::unique_ptr<TreeStatement> stateHead(new TreeStatement(stateStr));

    if(!isExp(input.front())){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects expression
    }
    New_Parser parser;
    stateHead->condition = parser.parseForState(input); // evaluates the condition expression in if and while statements to know when to execute the commands within the statement

    if(stateStr == "print"){
        return stateHead.release();
    }

    if(input.front().text != "{"){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects "{"
    }
    stateHead->truths = createBlock(input); // adds trees until "}" token is found

    if(stateStr == "while" || input.front().text != "else"){ // only continues if the statement is an if statement with an else following it
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


// within a statement, turns all the commants between "{" and "}" tokens
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



