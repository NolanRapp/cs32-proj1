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
        throw ParseError(currentLine, currentColumn, nextToken);
    }
}



TreeNode* New_Parser::parseE(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // Function to process 3rd order operations (expressions): "+" and "-"

    std::unique_ptr<TreeNode> node(parseT(tokenizedQ, variables));

    if (node == nullptr) {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    while (nextToken == "+" || nextToken == "-") {
        std::unique_ptr<TreeOperator> operatorNode(new TreeOperator(nextToken.at(0)));
        operatorNode->addChild(node.release());
        scanToken(tokenizedQ);

        if (nextToken == "END") {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        std::unique_ptr<TreeNode> right(parseT(tokenizedQ, variables));

        if (right == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        operatorNode->addChild(right.release());
        node.reset(operatorNode.release());
    }

    return node.release();
}



TreeNode* New_Parser::parseT(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // Function to process 2nd order operations (terms): "*" and "/"

    std::unique_ptr<TreeNode> node(parseF(tokenizedQ, variables));

    while (nextToken == "*" || nextToken == "/") {

        std::unique_ptr<TreeOperator> operatorNode(new TreeOperator(nextToken.at(0)));
        operatorNode->addChild(node.release());
        scanToken(tokenizedQ);

        if (nextToken == "END") {
            //delete operatorNode;
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        std::unique_ptr<TreeNode> right(parseF(tokenizedQ, variables));

        if (right == nullptr) {
            //delete operatorNode;
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        operatorNode->addChild(right.release());
        node.reset(operatorNode.release());
    }

    return node.release();
}



TreeNode* New_Parser::parseF(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // Function to process 4th order operators (factors): integer, identifier

    if (nextToken.empty()) {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    if (isdigit(nextToken.at(0)) || nextToken.at(0) == '_') {
        std::unique_ptr<TreeLeaf> leaf(new TreeLeaf(std::stod(nextToken)));

        if (leaf == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        scanToken(tokenizedQ); // Consume digit
        return leaf.release();
    }

    else if (nextToken == "(") {
        scanToken(tokenizedQ); // Consume open parenthesis
        std::unique_ptr<TreeNode> node;

        if (nextToken == "END") {
            throw ParseError(currentLine, currentColumn, nextToken); // missing closing parenthesis
        }

        if (isalpha(nextToken.at(0)) && (lookahead == "=")) {
            node.reset(parseA(tokenizedQ, variables));
        }
        else {
            node.reset(parseE(tokenizedQ, variables));
        }

        if ((nextToken == ")")) {
            scanToken(tokenizedQ); // consume closing parenthesis
            return node.release();
        }
        else {
            throw ParseError(currentLine, currentColumn, nextToken); // Throws error corresponding to "END" token, missing closing parenthesis
        }
    }

    else if (isalpha(nextToken.at(0))) {
        std::unique_ptr<TreeIdentifier> leaf(new TreeIdentifier(nextToken));
        scanToken(tokenizedQ); // Consume the variable 

        if (leaf == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        return leaf.release();
    }

    else {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    return nullptr; // Should never reach here
}

TreeNode* New_Parser::parseA(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // Function to parse assignments
    // Recursively calls itself if there are many instances of "="

    std::unique_ptr<TreeIdentifier> id(new TreeIdentifier(nextToken));

    scanToken(tokenizedQ); // consume the variable 
    scanToken(tokenizedQ); // consume the "="

    std::unique_ptr<TreeOperator> assignmentNode(new TreeOperator('='));
    assignmentNode->addChild(id.release());

    std::unique_ptr<TreeNode> rhs;  // Node for the right-hand side of the assignment

    // Dealing with nested assignments: (a=(b=3))
    if ((!tokenizedQ.empty()) && (nextToken == "(") && (isalpha(lookahead.at(0)))) {
        scanToken(tokenizedQ); // consume the "("

        if (lookahead == "=") {
            rhs.reset(parseA(tokenizedQ, variables));
        }
        else {
            rhs.reset(parseE(tokenizedQ, variables));

            if (rhs == nullptr) {
                throw ParseError(currentLine, currentColumn, nextToken);
            }

            assignmentNode->addChild(rhs.release());
            return assignmentNode.release();
            // Returns because we don't want to double evaluateNode
        }

        if (tokenizedQ.empty() || nextToken != ")") {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
        scanToken(tokenizedQ); // Consuming ")"
    }

    // Dealing with simple nested assignments like: a = b = 5
    else if ((!tokenizedQ.empty()) && (isalpha(nextToken.at(0))) && (lookahead == "=")) {
        rhs.reset(parseA(tokenizedQ, variables));
    }

    // If none of the above conditions are true, then parse it as an expression
    // Returns because we don't want to double-evaluateNode 
    else {
        rhs.reset(parseE(tokenizedQ, variables));

        if (rhs == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        assignmentNode->addChild(rhs.release());
        return assignmentNode.release();
    }
    
    if (rhs == nullptr) {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    assignmentNode->addChild(rhs.release());
    return assignmentNode.release();
}



TreeNode* New_Parser::parse(std::deque<Token>& tokenizedQ, std::unordered_map<std::string, double>& variables) {
    // Function to parse entire input, and end when "END" token is reached

    scanToken(tokenizedQ); // Initial consumption
    std::unique_ptr<TreeNode> rootTree;

    if (nextToken.empty()) {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    if ((isalpha(nextToken.at(0))) && (lookahead == "=")) {
        rootTree.reset(parseA(tokenizedQ, variables));

        if (rootTree == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

    }

    else if ((isalpha(nextToken.at(0))) && (lookahead != "=")) {
        rootTree.reset(parseE(tokenizedQ, variables));
    }

    else {
        rootTree.reset(parseE(tokenizedQ, variables));

        if (rootTree == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
    }

    if (nextToken == "END" && lookahead == "END") {
        return rootTree.release();
    }
    else {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    return nullptr;
}