#include "New_Parser.h"

New_Parser::New_Parser() {
    // Nothing to see here...
}
New_Parser::~New_Parser() {
    // Nothing to see here...
    
}



void New_Parser::scanToken(std::deque<Token>& tokenizedQ) {
    // Function to "consume" current token, moving nextToken to next value on the queue

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
    // Function to process 3rd order operations (expressions): "+" and "-"

    TreeNode* node = parseT(tokenizedQ, variables);

    if (node == nullptr) {
        newParseError(currentLine, currentColumn, nextToken);
    }

    while (nextToken == "+" || nextToken == "-") {
        TreeOperator* operatorNode = new TreeOperator(nextToken.at(0));
        operatorNode->addChild(node);
        scanToken(tokenizedQ);

        if (nextToken == "END") {
            delete operatorNode;
            newParseError(currentLine, currentColumn, nextToken);
        }

        TreeNode* right = parseT(tokenizedQ, variables);

        if (right == nullptr) {
            delete operatorNode;
            newParseError(currentLine, currentColumn, nextToken);
        }

        operatorNode->addChild(right);
        node = operatorNode;
    }

    return node;
}



TreeNode* New_Parser::parseT(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // Function to process 2nd order operations (terms): "*" and "/"

    TreeNode* node = parseF(tokenizedQ, variables);

    while (nextToken == "*" || nextToken == "/") {

        TreeOperator* operatorNode = new TreeOperator(nextToken.at(0));
        operatorNode->addChild(node);
        scanToken(tokenizedQ);

        if (nextToken == "END") {
            newParseError(currentLine, currentColumn, nextToken);
        }

        TreeNode* right = parseF(tokenizedQ, variables);

        if (right == nullptr) {
            newParseError(currentLine, currentColumn, nextToken);
        }

        operatorNode->addChild(right);
        node = operatorNode;
    }

    return node;
}



TreeNode* New_Parser::parseF(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // Function to process 4th order operators (factors): integer, identifier

    if (nextToken.empty()) {
        newParseError(currentLine, currentColumn, nextToken);
    }

    if (isdigit(nextToken.at(0)) || nextToken.at(0) == '_') {
        TreeLeaf* leaf = new TreeLeaf(std::stod(nextToken));

        if (leaf == nullptr) {
            delete leaf;
            newParseError(currentLine, currentColumn, nextToken);
        }

        scanToken(tokenizedQ);
        return leaf;
    }

    else if (nextToken == "(") {
        scanToken(tokenizedQ); // Consume open parenthesis, move onto next
        TreeNode* node = nullptr;

        if (nextToken == "END") {
            newParseError(currentLine, currentColumn, nextToken);
        }

        if (isalpha(nextToken.at(0)) && (lookahead == "=")) {
            node = parseA(tokenizedQ, variables);
        }
        else {
            node = parseE(tokenizedQ, variables);
        }

        if (nextToken == ")") {
            scanToken(tokenizedQ); // Consume closing parenthesis
            return node;
        }
        else {
            newParseError(currentLine, currentColumn, nextToken); // Throws error corresponding to "END" token, missing closing parenthesis
        }
    }

    else if (isalpha(nextToken.at(0))) {
        //if ((variables.find(nextToken) != variables.end())) {
            TreeIdentifier* leaf = new TreeIdentifier(nextToken);
            scanToken(tokenizedQ); // Consume the variable 
            return leaf;
        /*}
        else {
            // TODO: have to print wrong expression infix still
            throw std::runtime_error("Runtime error: unknown identifier " + nextToken);
        }*/
    }

    else {
        newParseError(currentLine, currentColumn, nextToken);
    }

    return nullptr; // Should never reach here
}

TreeNode* New_Parser::parseA(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // Function to parse assignments
    // Recursively calls itself if there are many instances of "="

    TreeIdentifier* id = new TreeIdentifier(nextToken);

    scanToken(tokenizedQ); // consume the variable 
    scanToken(tokenizedQ); // consume the "="

    TreeOperator* assignmentNode = new TreeOperator('=');
    assignmentNode->addChild(id);

    TreeNode* rhs = nullptr;  // Node for the right-hand side of the assignment

    // Dealing with nested assignments: (a=(b=3))
    if ((!tokenizedQ.empty()) && (nextToken == "(") && (isalpha(lookahead.at(0)))) {
        scanToken(tokenizedQ); // consume the "("

        if (lookahead == "=") {
            rhs = parseA(tokenizedQ, variables);
        }
        else {
            rhs = parseE(tokenizedQ, variables);

            if (rhs == nullptr) {
                delete assignmentNode;
                newParseError(currentLine, currentColumn, nextToken);
            }

            assignmentNode->addChild(rhs);
            return assignmentNode;
            // Returns because we don't want to double evaluateNode
        }

        if (tokenizedQ.empty() || nextToken != ")") {
            newParseError(currentLine, currentColumn, nextToken);
        }
        scanToken(tokenizedQ); // Consuming ")"
    }

    // Dealing with simple nested assignments like: a = b = 5
    else if ((!tokenizedQ.empty()) && (isalpha(nextToken.at(0))) && (lookahead == "=")) {
        rhs = parseA(tokenizedQ, variables);
    }

    // If none of the above conditions are true, then parse it as an expression
    // Returns because we don't want to double-evaluateNode 
    else {
        rhs = parseE(tokenizedQ, variables);

        if (rhs == nullptr) {
            delete assignmentNode;
            newParseError(currentLine, currentColumn, nextToken);
        }

        assignmentNode->addChild(rhs);
        return assignmentNode;
    }
    
    if (rhs == nullptr) {
        delete assignmentNode;
        newParseError(currentLine, currentColumn, nextToken);
    }

    assignmentNode->addChild(rhs);
    
    // Evaluate the expression and assign the value to the variable
    double val = assignmentNode->evaluateNode(variables);
    variables[id->getID()] = val;

    return assignmentNode;
}



TreeNode* New_Parser::parse(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // Function to parse entire input, and end when "END" token is reached

    scanToken(tokenizedQ); // Initial consumption
    TreeNode* rootTree = nullptr;

    if (nextToken.empty()) {
        newParseError(currentLine, currentColumn, nextToken);
    }

    if ((isalpha(nextToken.at(0))) && (lookahead == "=")) {
        rootTree = parseA(tokenizedQ, variables);

        if (rootTree == nullptr) {
            newParseError(currentLine, currentColumn, nextToken);
        }

    }

    else if ((isalpha(nextToken.at(0))) && (lookahead != "=")) {
        rootTree = parseE(tokenizedQ, variables);
    }

    else {
        rootTree = parseE(tokenizedQ, variables);

        if (rootTree == nullptr) {
            newParseError(currentLine, currentColumn, nextToken);
        }
    }
    return rootTree;
}



void New_Parser::newParseError(int line, int col, std::string text) const {
	throw std::runtime_error("Unexpected token at line " +
    std::to_string(line) + " column " + std::to_string(col) + ": " + text);
}