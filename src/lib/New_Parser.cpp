#include "New_Parser.h"

New_Parser::New_Parser() {
    // nothing to see here...
}
New_Parser::~New_Parser() {
    // nothing to see here...
    
}



void New_Parser::scanToken(std::deque<Token>& tokenizedQ) {
    // function to "consume" current token, moving nextToken to next value on the queue

    if (!tokenizedQ.empty()) {
        nextToken = tokenizedQ.front().text;
        if (nextToken == "END") {
            lookahead = "END";
        }
        else {
            lookahead = tokenizedQ[1].text;
        }

        currentLine = tokenizedQ.front().line;
        currentColumn = tokenizedQ.front().column;
        tokenizedQ.pop_front();
    }

    else {
        newParseError(currentLine, currentColumn, nextToken);
    }
}



TreeNode* New_Parser::parseE(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
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



TreeNode* New_Parser::parseT(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
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



TreeNode* New_Parser::parseF(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
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

    else if (isalpha(nextToken.at(0)) && (variables.find(nextToken) != variables.end())) {
        double val = variables[nextToken];
        TreeLeaf* leaf = new TreeLeaf(val);

        scanToken(tokenizedQ); // consuming the variable 

        return leaf;
    }

    else if (nextToken == "(") {
        scanToken(tokenizedQ); // consume open parenthesis, move onto next
        TreeNode* node = nullptr;

        if (isalpha(nextToken.at(0))) {
            node = parseA(tokenizedQ, variables); // if next token is a variable assignment in parenthesis
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



TreeNode* New_Parser::parseA(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // function to parse assignments
    // recursively calls itself if there are many instances of "="

    /*if ((isalpha(nextToken.at(0))) && (lookahead != "=")) {
        parseE(tokenizedQ, variables);
    }*/

    TreeIdentifier* id = new TreeIdentifier(nextToken);

    if (lookahead != "=") {
        newParseError(currentLine, currentColumn, nextToken);
    }

    scanToken(tokenizedQ); // consume the variable 
    scanToken(tokenizedQ); // consume the "="

    TreeOperator* assignmentNode = new TreeOperator('=');
    assignmentNode->addChild(id);


    if ((!tokenizedQ.empty()) && (nextToken == "(")) {
        scanToken(tokenizedQ); // consume the "("
        TreeNode* nestedParenthesis = parseA(tokenizedQ, variables);

        if (tokenizedQ.empty() || nextToken != ")") {
            newParseError(currentLine, currentColumn, nextToken);
        }
        scanToken(tokenizedQ); // consuming ")"
        assignmentNode->addChild(nestedParenthesis);
    }

    else if ((!tokenizedQ.empty()) && (isalpha(nextToken.at(0)))) {
        TreeNode* nestedAssignment = parseA(tokenizedQ, variables);
        assignmentNode->addChild(nestedAssignment);
    }

    else {
        TreeNode* expressionNode = parseE(tokenizedQ, variables);
        assignmentNode->addChild(expressionNode);

    }
    
    double val = assignmentNode->evaluateNode(variables);
    variables[id->getID()] = val;
    return assignmentNode;

}



TreeNode* New_Parser::parse(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // function to parse entire input, and end when "END" token is reached
    scanToken(tokenizedQ); // initial consumption
    TreeNode* rootTree = nullptr;

    if (nextToken.empty()) {
        newParseError(currentLine, currentColumn, nextToken);
    }

    std::string errorToken = nextToken; 
    int errorColumn = currentColumn;

    if ((isalpha(nextToken.at(0))) && (lookahead == "=")) {
        rootTree = parseA(tokenizedQ, variables);

        if (rootTree == nullptr) {
            newParseError(currentLine, errorColumn, errorToken);
        }

    }

    else if ((isalpha(nextToken.at(0))) && (lookahead != "=")) {
        //TreeIdentifier* ID = new TreeIdentifier(nextToken);
        rootTree = parseE(tokenizedQ, variables);
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