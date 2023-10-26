#include "New_Parser.h"

New_Parser::New_Parser() {
    // nothing to see here...
}
New_Parser::~New_Parser() {
    // nothing to see here... 
}

void New_Parser::addTree(std::queue<Token> oInput){
	mHeads.push(parse(oInput));
}

TreeNode* New_Parser::popHead(){
	TreeNode* tempHead = mHeads.front();
	mHeads.pop();

	return tempHead;
}

bool New_Parser::isEmpty(){
	return mHeads.empty();
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
        delete node;
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

    if (nextToken.empty()) {
    std::cerr << "Error: Unexpected end of expression." << std::endl;
    exit(3);
    }  

    if (isdigit(nextToken.at(0)) || nextToken.at(0) == '_') {
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
            delete node;
            exit(3);
        }
    }

    else {
        std::cerr << "Error: Unexpected Token" << std::endl;
        exit(3);
    }
}



TreeNode* New_Parser::parseA(TreeIdentifier* id, std::queue<Token>& tokenizedQ) {
    // function to parse assignments
    // recursively calls itself if there are many instances of "="

    scanToken(tokenizedQ); // consuming the "="
    TreeOperator* assignmentNode = new TreeOperator('=');
    assignmentNode->addChild(id);

    if (nextToken == "=") {
        TreeIdentifier* ID = new TreeIdentifier(nextToken);
        assignmentNode->addChild(parseA(ID, tokenizedQ));
    }

    else {
        assignmentNode->addChild(parseE(tokenizedQ));
    }

    return assignmentNode;
}



TreeNode* New_Parser::parse(std::queue<Token>& tokenizedQ) {
    // function to parse entire input, and end when "END" token is reached
    scanToken(tokenizedQ); // initial consumption

    TreeNode* rootTree = nullptr;
    
    if (nextToken.empty()) {
    std::cerr << "Error: Unexpected end of expression." << std::endl;
    delete rootTree;
    exit(3);
    }

    if (isalpha(nextToken.at(0))) {
        TreeIdentifier* ID = new TreeIdentifier(nextToken);
        scanToken(tokenizedQ); // consume token
        if (nextToken == "=") {
            rootTree = parseA(ID, tokenizedQ);
        }
        else {
            std::cerr << "Error: Expects equal sign after identifier" << std::endl;
            delete rootTree;
            exit(3);
        }
    }
    
    else {
        rootTree = parseE(tokenizedQ);
    }

    return rootTree;

    /*if (nextToken != "END") {
        std::cerr << "Error: Unexpected end token. Recieved this instead: " << nextToken << "." << std::endl;
        exit(3);
    }
    return rootTree;*/
}
