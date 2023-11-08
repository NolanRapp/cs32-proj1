#include "New_Parser.h"


// Function to "consume" current token, moving nextToken to next value on the queue
void New_Parser::scanToken(std::deque<Token>& tokenizedQ) {

    if (!tokenizedQ.empty()) {
        nextToken = tokenizedQ.front().text;
        tokenType = tokenizedQ.front().type;

        currentLine = tokenizedQ.front().line;
        currentColumn = tokenizedQ.front().column;

        if (nextToken == "END") {
            lookahead = "END";
        }
        else {
            lookahead = tokenizedQ[1].text;
            lookaheadType = tokenizedQ[1].type;
        }

        tokenizedQ.pop_front();
    }

    else {
        throw ParseError(currentLine, currentColumn, nextToken);
    }
}



// Function specifically to be called in calc.cpp, Track A of Scrypt 03
TreeNode* New_Parser::parseForCalc(std::deque<Token>& tokenizedQ) {
    std::unique_ptr<TreeNode> ast;
    ast.reset(parse(tokenizedQ));

    if (nextToken != "END") {
        throw ParseError(currentLine, currentColumn, nextToken);;
    }
    return (ast.release());
}



TreeNode* New_Parser::parseForState(std::deque<Token>& tokenizedQ) {
    std::unique_ptr<TreeNode> ast;
    ast.reset(parse(tokenizedQ));

    Token pushTok(currentLine, currentColumn, nextToken, tokenType);

    tokenizedQ.push_front(pushTok);

    return (ast.release());
}



// MAIN Parser. Begining of Recursive Descent.
TreeNode* New_Parser::parse(std::deque<Token>& tokenizedQ) {

    scanToken(tokenizedQ); // Initial consumption
    std::unique_ptr<TreeNode> rootTree;

    if ((tokenType == Type::ID) && (lookahead == "=")) {
        rootTree.reset(parseA(tokenizedQ));
        if (rootTree == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
    }
    else if ((tokenType == Type::ID) && (lookahead != "=")) {
        rootTree.reset(parseInclusive(tokenizedQ));
    }
    else {
        rootTree.reset(parseInclusive(tokenizedQ));
        if (rootTree == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
    }
    return rootTree.release();
}



// Inclusive operator parser "|". Lowest precedence operator.
TreeNode* New_Parser::parseInclusive(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> left(parseExclusive(tokenizedQ));

    while (nextToken == "|" ) {
        std::string op = nextToken;
        scanToken(tokenizedQ);

        std::unique_ptr<TreeNode> right(parseExclusive(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        std::unique_ptr<TreeOperator> nodeOperator(new TreeOperator(op));

        nodeOperator->addChild(left.release());
        nodeOperator->addChild(right.release());

        left.reset(nodeOperator.release());
    }
    return left.release();
}



// Exclusive operator parser "^".
TreeNode* New_Parser::parseExclusive(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> left(parseAnd(tokenizedQ));

    while (nextToken == "^") {
        std::string op = nextToken;
        scanToken(tokenizedQ);

        std::unique_ptr<TreeNode> right(parseAnd(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        std::unique_ptr<TreeOperator> nodeOperator(new TreeOperator(op));
        nodeOperator->addChild(left.release());
        nodeOperator->addChild(right.release());

        left.reset(nodeOperator.release());
    }
    return left.release();
}



// And operator parser "&"
TreeNode* New_Parser::parseAnd(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> left(parseEquality(tokenizedQ));

    while (nextToken == "&") {
        std::string op = nextToken;
        scanToken(tokenizedQ);

        std::unique_ptr<TreeNode> right(parseEquality(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        std::unique_ptr<TreeOperator> nodeOperator(new TreeOperator(op));
        nodeOperator->addChild(left.release());
        nodeOperator->addChild(right.release());

        left.reset(nodeOperator.release());
    }
    return left.release();
}



// Equality operator parser "==" and "!=".
TreeNode* New_Parser::parseEquality(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> left(parseComparison(tokenizedQ));

    while ((nextToken == "==") || (nextToken == "!=")) {

        std::string op = nextToken;
        scanToken(tokenizedQ);
        std::unique_ptr<TreeNode> right(parseComparison(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
        
        std::unique_ptr<TreeOperator> operatorNode(new TreeOperator(op));
        operatorNode->addChild(left.release());
        operatorNode->addChild(right.release());

        left.reset(operatorNode.release());
    }

    return left.release();

}



// Comparison operator parser "<", ">", "<=", ">=".
TreeNode* New_Parser::parseComparison(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> left(parseE(tokenizedQ));

    while (nextToken == "<" || nextToken == ">" || nextToken == "<=" || nextToken == ">=") {
        
        std::string op = nextToken;
        scanToken(tokenizedQ);
        std::unique_ptr<TreeNode> right(parseE(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
        
        std::unique_ptr<TreeOperator> comparisonNode(new TreeOperator(op));
        comparisonNode->addChild(left.release());
        comparisonNode->addChild(right.release());

        left.reset(comparisonNode.release());
    }
    return left.release();
}



// Expression operator parser "+" and "-"
TreeNode* New_Parser::parseE(std::deque<Token>& tokenizedQ) {

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



// Term operator parser "*", "/", "%"
TreeNode* New_Parser::parseT(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> node(parseF(tokenizedQ));

    while (nextToken == "*" || nextToken == "/" || nextToken == "%") {
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



// Factor operator parser (integers, identifiers, and parenthesis)
TreeNode* New_Parser::parseF(std::deque<Token>& tokenizedQ) {

    if (nextToken.empty()) {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    if (tokenType == Type::BOOL) { 
        std::unique_ptr<TreeBoolean> boolVal(new TreeBoolean(nextToken));
        scanToken(tokenizedQ); // Consume true or false
        return boolVal.release();

    }
    else if (tokenType == Type::NUM) {
        std::unique_ptr<TreeLeaf> leaf(new TreeLeaf(std::stold(nextToken)));

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

        if ((tokenType == Type::ID) && (lookahead == "=")) {
            node.reset(parseA(tokenizedQ));
        }
        else {
            node.reset(parseInclusive(tokenizedQ));
        }

        if (nextToken == ")") {
            scanToken(tokenizedQ); // Consume "("
            return node.release();
        }
        else {
            throw ParseError(currentLine, currentColumn, nextToken); // missing closing parenthesis
        }
    }

    else if (tokenType == Type::ID) {
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



// Assignment operator parser "=". Recursively calls itself if assignments are nested.
TreeNode* New_Parser::parseA(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeIdentifier> id(new TreeIdentifier(nextToken));

    scanToken(tokenizedQ); // Consume "=" 
    scanToken(tokenizedQ); // Consumes next

    std::unique_ptr<TreeAssign> assignmentNode(new TreeAssign());
    assignmentNode->addChild(id.release());

    std::unique_ptr<TreeNode> rhs;

    // Dealing with parenthetic nested assignments: (a=(b=3))
    if ((!tokenizedQ.empty()) && (nextToken == "(") && (lookaheadType == Type::ID)) {
        scanToken(tokenizedQ); // Consume "("

        if (lookahead == "=") {
            rhs.reset(parseA(tokenizedQ));
        }
        else {
            rhs.reset(parseInclusive(tokenizedQ));

            if (rhs == nullptr) {
                throw ParseError(currentLine, currentColumn, nextToken);
            }
        }

        if (tokenizedQ.empty() || nextToken != ")") {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
        scanToken(tokenizedQ); // Consume ")"
    }

    // Dealing with simple nested assignments: a = b = 5
    else if ((!tokenizedQ.empty()) && (tokenType == Type::ID) && (lookahead == "=")) {
        rhs.reset(parseA(tokenizedQ));
    }

    else {
        rhs.reset(parseInclusive(tokenizedQ));

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



