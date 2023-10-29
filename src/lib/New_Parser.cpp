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



TreeNode* New_Parser::parseE(std::queue<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // function to process 3rd order operations (expressions): "+" and "-" 
    TreeNode* node = parseT(tokenizedQ, variables);
    
    if (node == nullptr) {
        newParseError(currentLine, currentColumn, nextToken);
    }

    while (nextToken == "+" || nextToken == "-") {
        std::string errorToken = nextToken; 
        int errorColumn = currentColumn - 1;


        TreeOperator* operatorNode = new TreeOperator(nextToken.at(0));
        operatorNode->addChild(node);
        scanToken(tokenizedQ);

        if (nextToken == "END") {
            delete operatorNode;
            newParseError(currentLine, errorColumn, errorToken);
        }

        TreeNode* right = parseT(tokenizedQ, variables);

        if (right == nullptr) {
            delete operatorNode;
            newParseError(currentLine, errorColumn, errorToken);
        }

        operatorNode->addChild(right);
        node = operatorNode;
    }

    return node;
}



TreeNode* New_Parser::parseT(std::queue<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // function to process 2nd order operations (terms): "*" and "/"
    TreeNode* node = parseF(tokenizedQ, variables);

    while (nextToken == "*" || nextToken == "/") {
        std::string errorToken = nextToken; 
        int errorColumn = currentColumn;

        TreeOperator* operatorNode = new TreeOperator(nextToken.at(0));
        operatorNode->addChild(node);
        scanToken(tokenizedQ);

        if (nextToken == "END") {
            newParseError(currentLine, errorColumn, errorToken);
        }

        TreeNode* right = parseF(tokenizedQ, variables);

        if (right == nullptr) {
            newParseError(currentLine, errorColumn, errorToken);
        }

        operatorNode->addChild(right);
        node = operatorNode;
    }

    return node;
}



TreeNode* New_Parser::parseF(std::queue<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
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
            delete leaf;
            newParseError(currentLine, errorColumn, errorToken);
        }

        scanToken(tokenizedQ);
        return leaf;
    }

    else if (isalpha(nextToken.at(0)) && (variables.find(varName) != variables.end())) {
        double val = variables[varName];
        TreeLeaf* leaf = new TreeLeaf(val);
        scanToken(tokenizedQ);
        return leaf;
    }

    else if (nextToken == "(") {
        scanToken(tokenizedQ); // consume open parenthesis, move onto next
        TreeNode* node = nullptr;

        if (isalpha(nextToken.at(0))) {
            TreeIdentifier* ID = new TreeIdentifier(nextToken);
            node = parseA(ID, tokenizedQ, variables); // if next token is a variable assignment in parenthesis
        }
        else {
            node = parseE(tokenizedQ, variables);
        }

        if (nextToken == ")") {
            scanToken(tokenizedQ); // consume closing parenthesis
            return node;
        }
        else {
            newParseError(currentLine, currentColumn, nextToken); // throws error corresponding to "END" token, missing closing parenthesis
        }
    }

    else {
        newParseError(currentLine, errorColumn, errorToken);
    }

    return nullptr; // should never reach here
}



TreeNode* New_Parser::parseA(TreeIdentifier* id, std::queue<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // function to parse assignments
    // recursively calls itself if there are many instances of "="

    varName = nextToken;
    scanToken(tokenizedQ);

    if (nextToken != "=") {
        /* TODO:
        okay what's going on here is that the input "x" gets consumed above ^^
        so nextToken is set to "END". This is fine but when going through parseF, 
        the parser has no way to tell what the variable was to properly find it in 
        the variable map... 
        I was thinking of storing a memeber variable "varName" and setting it equal to
        the token before consuming it above? Not sure if there's a better way though
        */
        return parseE(tokenizedQ, variables);
    }

    scanToken(tokenizedQ); // consume the "="

    TreeOperator* assignmentNode = new TreeOperator('=');

    assignmentNode->addChild(id);

    TreeNode* assignVal = parseE(tokenizedQ, variables);
    assignmentNode->addChild(assignVal);

    double val = assignmentNode->evaluateNode(variables);
    variables[varName] = val;

    return assignmentNode;
}



TreeNode* New_Parser::parse(std::queue<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // function to parse entire input, and end when "END" token is reached
    scanToken(tokenizedQ); // initial consumption
    TreeNode* rootTree = nullptr;

    if (nextToken.empty()) {
        newParseError(currentLine, currentColumn, nextToken);
    }

    std::string errorToken = nextToken; 
    int errorColumn = currentColumn;

    if (isalpha(nextToken.at(0))) {
        TreeIdentifier* ID = new TreeIdentifier(nextToken);
        rootTree = parseA(ID, tokenizedQ, variables);
        if (rootTree == nullptr) {
            newParseError(currentLine, errorColumn, errorToken);
        }
    }
    else {
        rootTree = parseE(tokenizedQ, variables);

        if (rootTree == nullptr) {
            newParseError(currentLine, errorColumn, errorToken);
        }
    }
    return rootTree;
}



void New_Parser::newParseError(int line, int col, std::string text) const {
	throw std::runtime_error("Unexpected token at line " +
    std::to_string(line) + " column " + std::to_string(col) + ": " + text);
}