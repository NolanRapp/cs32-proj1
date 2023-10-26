#include "New_Parser.h"

New_Parser::New_Parser() {
    // nothing to see here...
}
New_Parser::~New_Parser() {
    // nothing to see here...
}



void New_Parser::scanToken(std::queue<Token>& tokenizedQ) {
    // function to "consume" current token, moving nextToken to next value on the queue

    if (!tokenizedQ.empty()) {
        nextToken = tokenizedQ.front().text;
        tokenizedQ.pop();
    }

    else {
        std::cerr << "Error: Unexpected end of expression. scanToken" << std::endl;
        exit(3);
    }
}



TreeNode* New_Parser::parseE(std::queue<Token>& tokenizedQ) {
    // function to process 3rd order operations (expressions): "+" and "-" 

    TreeNode* node = parseT(tokenizedQ);
    if (node == nullptr) {
        throw std::runtime_error("Invalid expression");
        exit(3);
    }

    while (nextToken == "+" || nextToken == "-") {
        TreeOperator* operatorNode = new TreeOperator(nextToken.at(0));
        operatorNode->addChild(node);
        scanToken(tokenizedQ);
        TreeNode* right = parseT(tokenizedQ);
        operatorNode->addChild(right);
        node = operatorNode;
    }
    return node;
}



TreeNode* New_Parser::parseT(std::queue<Token>& tokenizedQ) {
    // function to process 2nd order operations (terms): "*" and "/"
    
    TreeNode* node = parseF(tokenizedQ);
    while (nextToken == "*" || nextToken == "/") {
        TreeOperator* operatorNode = new TreeOperator(nextToken.at(0));
        operatorNode->addChild(node);
        scanToken(tokenizedQ);
        TreeNode* right = parseF(tokenizedQ);
        operatorNode->addChild(right);
        node = operatorNode;
    }
    return node;
}



TreeNode* New_Parser::parseF(std::queue<Token>& tokenizedQ) {
    // function to process 4th order operators (factors): integer, identifier
    // todo: handle identifiers and assignments

    if (isdigit(nextToken.at(0)) || nextToken.at(0 == '_')) {
        TreeLeaf* leaf = new TreeLeaf(std::stod(nextToken));
        scanToken(tokenizedQ);
        return leaf;
    }
    else if (isalpha(nextToken.at(0))) {
        TreeIdentifier* ID = new TreeIdentifier(nextToken);
        scanToken(tokenizedQ);
        return ID;
    }

    else if (nextToken == "(") {
        scanToken(tokenizedQ); // consume open parenthesis, move onto next
        TreeNode* node = parseE(tokenizedQ); // parse expression, until closing parenthesis reached
        if (nextToken == ")") {
            scanToken(tokenizedQ); // consume closing parenthesis
            return node;
        }
        else {
            std::cerr << "Error: Missing closing parenthesis" << std::endl;
            exit(3);
        }
    }
    // HERE: handle identifiers and other assignments?
}

TreeNode* New_Parser::parse(std::queue<Token>& tokenizedQ) {
    // function to parse entire input, and end when "END" token is reached

    scanToken(tokenizedQ); //consume first token
    TreeNode* rootTree = parseE(tokenizedQ);
    if (nextToken != "END") {
        std::cerr << "Error: Unexpected end token. Recieved this instead: " << nextToken << "." << std::endl;
        exit(3);
    }
    return rootTree;
}