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
        newParseError (currentLine, currentColumn, nextToken);
    }

    while (nextToken == "+" || nextToken == "-") {
        std::string errorToken = nextToken; 
        int errorColumn = currentColumn - 1;


        TreeOperator* operatorNode = new TreeOperator(nextToken.at(0));
        operatorNode->addChild(node);
        scanToken(tokenizedQ);

        if (nextToken == "END") {
            newParseError(currentLine, errorColumn, errorToken);
        }

        TreeNode* right = parseT(tokenizedQ);

        if (right == nullptr) {
            newParseError(currentLine, errorColumn, errorToken);
        }

        operatorNode->addChild(right);
        node = operatorNode;
    }

    return node;
}



TreeNode* New_Parser::parseT(std::queue<Token>& tokenizedQ) {
    // function to process 2nd order operations (terms): "*" and "/"
    
    TreeNode* node = parseF(tokenizedQ);

    while (nextToken == "*" || nextToken == "/") {
        std::string errorToken = nextToken; 
        int errorColumn = currentColumn;

        TreeOperator* operatorNode = new TreeOperator(nextToken.at(0));
        operatorNode->addChild(node);
        scanToken(tokenizedQ);

        if (nextToken == "END") {
            newParseError(currentLine, errorColumn, errorToken);
        }

        TreeNode* right = parseF(tokenizedQ);

        if (right == nullptr) {
            newParseError(currentLine, errorColumn, errorToken);
        }

        operatorNode->addChild(right);
        node = operatorNode;
    }

    return node;
}



TreeNode* New_Parser::parseF(std::queue<Token>& tokenizedQ) {
    // function to process 4th order operators (factors): integer, identifier
    std::string errorToken = nextToken; 
    int errorColumn = currentColumn;

    if (nextToken.empty()) {
        newParseError(currentLine, currentColumn, nextToken);
    }
    // ^^ might not need this if previous checks work as intended

    if (isdigit(nextToken.at(0)) || nextToken.at(0) == '_') {

        TreeLeaf* leaf = new TreeLeaf(std::stod(nextToken));

        if (leaf == nullptr) {
            newParseError(currentLine, errorColumn, errorToken);
        }

        scanToken(tokenizedQ);
        return leaf;
    }

    else if (isalpha(nextToken.at(0))) {
        TreeIdentifier* ID = new TreeIdentifier(nextToken);

        if (ID == nullptr) {
            newParseError(currentLine, errorColumn, errorToken);
        }

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
            newParseError(currentLine, currentColumn, nextToken); // throws error corresponding to "END" token, missing closing parenthesis
        }
    }

    else {
        newParseError(currentLine, errorColumn, errorToken);
    }

    return nullptr; // should never reach here
}



TreeNode* New_Parser::parseA(TreeIdentifier* id, std::queue<Token>& tokenizedQ) {
    // function to parse assignments
    // recursively calls itself if there are many instances of "="

    std::string errorToken = nextToken; 
    int errorColumn = currentColumn;

    TreeOperator* assignmentNode = new TreeOperator('=');
    assignmentNode->addChild(id);
    scanToken(tokenizedQ); // consume the "=" here

    if (nextToken == "END") {
        delete assignmentNode;
        newParseError(currentLine, errorColumn, errorToken);
    }

    if (isalpha(nextToken.at(0))) {
        TreeIdentifier* nextID = new TreeIdentifier(nextToken);
        scanToken(tokenizedQ); // moving to the next token

        if (nextToken == "=") {
            assignmentNode->addChild(parseA(nextID, tokenizedQ)); // recursively calling parseA for assignment operators
        }
        else {
            delete assignmentNode;
            newParseError(currentLine, errorColumn, errorToken);
        }
    }

    else {
        TreeNode* expressionTree = parseE(tokenizedQ);

        if (expressionTree == nullptr) {
            delete assignmentNode;
            newParseError(currentLine, errorColumn, errorToken);
        }
        assignmentNode->addChild(expressionTree);
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

    while (!tokenizedQ.empty()) {
        std::string errorToken = nextToken; 
        int errorColumn = currentColumn;

        if (isalpha(nextToken.at(0))) {
            TreeIdentifier* ID = new TreeIdentifier(nextToken);
            scanToken(tokenizedQ);

            if (nextToken == "=") {
                rootTree = parseA(ID, tokenizedQ);
            }
            else {
                delete rootTree;
                newParseError(currentLine, errorColumn, errorToken);
            }
        }
        else {
            rootTree = parseE(tokenizedQ);
            
            if (rootTree == nullptr) {
                newParseError(currentLine, errorColumn, errorToken);
            }
        }
    }
    return rootTree;
}



void New_Parser::newParseError(int line, int col, std::string text) const {
	throw std::runtime_error("Unexpected token at line " +
    std::to_string(line) + " column " + std::to_string(col) + ": " + text);
}