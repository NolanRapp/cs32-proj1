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
        tokenType = tokenizedQ.front().type;

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



TreeNode* New_Parser::parse(std::deque<Token>& tokenizedQ) {
    // Function to parse entire input, and end when "END" token is reached

    scanToken(tokenizedQ); // Initial consumption
    std::unique_ptr<TreeNode> rootTree;

    if (nextToken.empty()) {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    if ((isalpha(nextToken.at(0))) && (lookahead == "=")) {
        rootTree.reset(parseA(tokenizedQ));
        if (rootTree == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
    }

    else if ((isalpha(nextToken.at(0))) && (lookahead != "=")) {
        rootTree.reset(parseLogical(tokenizedQ));
    }

    else {
        rootTree.reset(parseLogical(tokenizedQ));

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



TreeNode* New_Parser::parseLogical(std::deque<Token>& tokenizedQ) {
    // Function to process logical operations: "&", "^", "|".
    // Ensure both operands are of boolean type, handle the operations accordingly.

    std::unique_ptr<TreeNode> left(parseEquality(tokenizedQ));

    while (nextToken == "&" || nextToken == "|" || nextToken == "^") {
        std::string op = nextToken;
        scanToken(tokenizedQ);

        std::unique_ptr<TreeNode> right(parseEquality(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        std::unique_ptr<TreeBoolean> nodeLogical(new TreeBoolean(op));
        nodeLogical->addChild(left.release());
        nodeLogical->addChild(right.release());

        left.reset(nodeLogical.release());
    }
    return left.release();
}



TreeNode* New_Parser::parseEquality(std::deque<Token>& tokenizedQ) {
    // Function to process equality and inequality operations: "==", "!="
    // Works on either two numbers or two bools, and return bools

    std::unique_ptr<TreeNode> left(parseComparison(tokenizedQ));

    while ((nextToken == "==") || (nextToken == "!=")) {

        std::string op = nextToken;
        scanToken(tokenizedQ);
        std::unique_ptr<TreeNode> right(parseComparison(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
        
        std::unique_ptr<TreeBoolean> operatorNode(new TreeBoolean(op));
        operatorNode->addChild(left.release());
        operatorNode->addChild(right.release());

        left.reset(operatorNode.release());
    }

    return left.release();

}



TreeNode* New_Parser::parseComparison(std::deque<Token>& tokenizedQ) {
    // Function to parse comparison operations 
    // Only works on numbers, and return bools

    std::unique_ptr<TreeNode> left(parseE(tokenizedQ));

    while (nextToken == "<" || nextToken == ">" || nextToken == "<=" || nextToken == ">=") {
        
        std::string op = nextToken;
        scanToken(tokenizedQ);
        std::unique_ptr<TreeNode> right(parseE(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
        
        std::unique_ptr<TreeBoolean> comparisonNode(new TreeBoolean(op));
        comparisonNode->addChild(left.release());
        comparisonNode->addChild(right.release());

        left.reset(comparisonNode.release());
    }
    return left.release();
}



TreeNode* New_Parser::parseE(std::deque<Token>& tokenizedQ) {
    // Function to process 3rd order operations (expressions): "+" and "-"

    std::unique_ptr<TreeNode> node(parseT(tokenizedQ));

    if (node == nullptr) {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    while (nextToken == "+" || nextToken == "-") {
        std::unique_ptr<TreeOperator> operatorNode(new TreeOperator(nextToken));
        operatorNode->addChild(node.release());
        scanToken(tokenizedQ);

        std::unique_ptr<TreeNode> right(parseT(tokenizedQ));

        if (right == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        operatorNode->addChild(right.release());
        node.reset(operatorNode.release());
    }
    return node.release();
}



TreeNode* New_Parser::parseT(std::deque<Token>& tokenizedQ) {
    // Function to process 2nd order operations (terms): "*" and "/"

    std::unique_ptr<TreeNode> node(parseF(tokenizedQ));

    while (nextToken == "*" || nextToken == "/") {

        std::unique_ptr<TreeOperator> operatorNode(new TreeOperator(nextToken));
        operatorNode->addChild(node.release());
        scanToken(tokenizedQ);

        if (nextToken == "END") {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        std::unique_ptr<TreeNode> right(parseF(tokenizedQ));

        if (right == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        operatorNode->addChild(right.release());
        node.reset(operatorNode.release());

    }

    return node.release();

}



TreeNode* New_Parser::parseF(std::deque<Token>& tokenizedQ) {
    // Function to process 4th order operators (factors): integer, identifier

    if (nextToken.empty()) {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    if (nextToken == "true" || nextToken == "false") {
        bool TrueOrFalse = (nextToken == "true");
        scanToken(tokenizedQ); // Consume true or false
        if (TrueOrFalse) {
            std::unique_ptr<TreeBooleanText> boolVal(new TreeBooleanText(TrueOrFalse));
            return boolVal.release();
        }
        else {
            std::unique_ptr<TreeBooleanText> boolVal(new TreeBooleanText(TrueOrFalse));
            return boolVal.release();
        }

    }

    else if (isdigit(nextToken.at(0)) || nextToken.at(0) == '_') {
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
            throw ParseError(currentLine, currentColumn, nextToken); // Missing closing parenthesis
        }

        if (isalpha(nextToken.at(0)) && (lookahead == "=")) {
            node.reset(parseA(tokenizedQ));
        }
        else {
            node.reset(parseComparison(tokenizedQ));
        }

        if ((nextToken == ")")) {
            scanToken(tokenizedQ); // Consume "("
            return node.release();
        }
        else {
            throw ParseError(currentLine, currentColumn, nextToken); // Throws error corresponding to "END" token, missing closing parenthesis
        }
    }

    else if (isalpha(nextToken.at(0))) {
        std::unique_ptr<TreeIdentifier> leaf(new TreeIdentifier(nextToken));
        scanToken(tokenizedQ); // Consume variable 

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



TreeNode* New_Parser::parseA(std::deque<Token>& tokenizedQ) {
    // Function to parse assignments
    // Recursively calls itself if there are many instances of "="

    std::unique_ptr<TreeIdentifier> id(new TreeIdentifier(nextToken));

    scanToken(tokenizedQ); // Consume the variable 
    scanToken(tokenizedQ); // Consume the "="

    std::unique_ptr<TreeOperator> assignmentNode(new TreeOperator("="));
    assignmentNode->addChild(id.release());

    std::unique_ptr<TreeNode> rhs;

    // Dealing with nested assignments: (a=(b=3))
    if ((!tokenizedQ.empty()) && (nextToken == "(") && (isalpha(lookahead.at(0)))) {
        scanToken(tokenizedQ); // Consume the "("

        if (lookahead == "=") {
            rhs.reset(parseA(tokenizedQ));
        }
        else {
            rhs.reset(parseLogical(tokenizedQ));

            if (rhs == nullptr) {
                throw ParseError(currentLine, currentColumn, nextToken);
            }

        }

        if (tokenizedQ.empty() || nextToken != ")") {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
        scanToken(tokenizedQ); // Consuming ")"
    }

    // Dealing with simple nested assignments like: a = b = 5
    else if ((!tokenizedQ.empty()) && (isalpha(nextToken.at(0))) && (lookahead == "=")) {
        rhs.reset(parseA(tokenizedQ));
    }

    // If none of the above conditions are true, then parse it as an expression
    else {
        rhs.reset(parseLogical(tokenizedQ));

        if (rhs == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
    }
    
    if (rhs == nullptr) {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    assignmentNode->addChild(rhs.release());
    return assignmentNode.release();
}

