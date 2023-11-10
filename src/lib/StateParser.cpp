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

        mHeads.push_back(createTree(oInput));
    }
}



// creates single tree (either statement or expression tree)
TreeNode* StateParser::createTree(std::deque<Token>& input){
    if(isExp(input.front())){
        New_Parser parser;
        std::unique_ptr<TreeNode> expression(parser.parseForState(input));

        if(input.front().text != ";"){ 
            throw ParseError(input.front().line, input.front().column, input.front().text);
            // Expects ";" after stand-alone expressions
        }
        input.pop_front(); // Reads ";"

        // Places expression into statement
        std::unique_ptr<TreeStatement> exHolder(new TreeStatement("expression"));
        exHolder-> condition = expression.release();

        return exHolder.release();
    }

    return createStatement(input);
}


// takes an input token that is confirmed as statement and turns it into a tree with required parts
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

    // "print" statement should only have condition
    if(stateStr == "print"){
        if(input.front().text != ";"){
            throw ParseError(input.front().line, input.front().column, input.front().text);
            // Expects ";" after stand-alone expressions
        }
        input.pop_front(); // Reads ";"

        return stateHead.release();
    }


    if(input.front().text != "{"){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects "{"
    }

    stateHead->truths = createBlock(input); // adds trees until "}" token is found

    // only continues if the statement is an if statement with an else following it
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



