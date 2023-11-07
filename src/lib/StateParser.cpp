#include "StateParser.h"


// Reads entire input to create complete program
void StateParser::createForest(std::dequeue<Token> oInput){
    while(!input.empty()){
        // if "END" read and continue
        // if "{" or "}" error
        mHeads.push_back(createTree(oInput));
    }
}



// Reads single expression or statement
TreeNode* StateParser::createTree(std::dequeue<Token>& input){
    if(isExp(input.front())){
        New_Parser parser;

        return parse(input); // Only ends on "END", "{", or "}"
    }

    return createStatement(input);
}


// When given "if" or "while" creates a node with a condition and a true and false tree
TreeNode* StateParser::createStatement(std::dequeue<Token>& input){
    unique_ptr<TreeStatement> head(new TreeStatement());

    if(!isExp(input.front())){
        throw ParseError(input.front().line, input.front().column, input.front().text);
        // Expects expression
    }
    head->condition = parseForState(input); 

    // Check for "{"
    // createBlock into True Tree
    while(/*Not "}"*/){
        head->truth.push_back(/**/);
    }

    
    // Return if "while"

    // Return if not "else"
    
    // Read "else"
    // If "if" call createStatement into False Tree
    // If "{" call createForest into False Tree
}


// Creates vector of trees in block between "{}"
std::vector<TreeNode*> StateParser::createBlock(std::dequeue<Token>& input){
    std::vector<TreeNode*> forest;

    // read "{"

    while(input.front().text != "}"){
        // if "END" read (then check if empty() and throw for missing "}") and continue
        // if "{" error
        forest.push_back(createTree(input));
    }

    // read "}"

    return forest;
}


// Checks if next character indicates the beginning of an expression
bool StateParser::isExp(Token& token){
    return (token.type != STATEMENT);
}


