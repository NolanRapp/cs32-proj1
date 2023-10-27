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
        currentLine = tokenizedQ.front().line;
        currentColumn = tokenizedQ.front().column;
        tokenizedQ.pop();
    }

    else {
        newParseError(currentLine, currentColumn, nextToken);
    }
}



TreeNode* New_Parser::parseE(std::queue<Token>& tokenizedQ) {
    // function to process 3rd order operations (expressions): "+" and "-" 

    TreeNode* node = parseT(tokenizedQ);
    if (node == nullptr) {
        delete node;
        newParseError(currentLine, currentColumn, nextToken);
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
        newParseError(currentLine, currentColumn, nextToken);
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
            delete node;
            newParseError(currentLine, currentColumn, nextToken);
        }
    }
    else {
        std::cout << "HERE!" << nextToken << std::endl;
        newParseError(currentLine, currentColumn, nextToken);
    }
    return 0; // should never reach here
}



TreeNode* New_Parser::parseA(TreeIdentifier* id, std::queue<Token>& tokenizedQ) {
    // function to parse assignments
    // recursively calls itself if there are many instances of "="

    scanToken(tokenizedQ); // consuming the "="
    TreeOperator* assignmentNode = new TreeOperator('=');
    assignmentNode->addChild(id);

    // checking next token type 
    if (nextToken == "=") {
        TreeIdentifier* nextID = nullptr;
        scanToken(tokenizedQ);
        if (isalpha(nextToken.at(0))) {
            nextID = new TreeIdentifier(nextToken);
             
        }
        else {
            newParseError(currentLine, currentColumn, nextToken);
        }

        assignmentNode->addChild(parseA(nextID, tokenizedQ));
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
        delete rootTree;
        newParseError(currentLine, currentColumn, nextToken);
    }

    std::string errorToken = nextToken;

    while (!tokenizedQ.empty()) {
        if (isalpha(nextToken.at(0))) {
            TreeIdentifier* ID = new TreeIdentifier(nextToken);
            scanToken(tokenizedQ);

            if (nextToken == "=") {
                rootTree = parseA(ID, tokenizedQ);
            }
            else {
                delete rootTree;
                newParseError(currentLine, currentColumn, errorToken);
            }
        }
        else {
            std::cout << "parse nextToken: " << nextToken << std::endl;
            rootTree = parseE(tokenizedQ);
            
            if (rootTree == nullptr) {
                newParseError(currentLine, currentColumn, errorToken);
            }
        }
    }
    return rootTree;
}



void New_Parser::newParseError(int line, int col, std::string text) const {
	throw std::runtime_error("Unexpected token at line " +
    std::to_string(currentLine) + " and column " + std::to_string(currentColumn) + ": " + text);
}