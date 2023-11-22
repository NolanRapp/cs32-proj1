#include "New_Parser.h"


// Function to consume current token, moving nextToken to next value on the queue
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



// Calls parse with certain end conditions - main parse for calc.cpp
TreeNode* New_Parser::parseForCalc(std::deque<Token>& tokenizedQ) {
    std::unique_ptr<TreeNode> ast;
    ast.reset(parse(tokenizedQ));

    if (nextToken != "END") {
        throw ParseError(currentLine, currentColumn, nextToken);;
    }
    return ast.release();
}



// Calls parse with certain end conditions - main parse for scrypt.cpp and format.cpp
TreeNode* New_Parser::parseForState(std::deque<Token>& tokenizedQ) {
    std::unique_ptr<TreeNode> ast;
    ast.reset(parse(tokenizedQ));

    // Reattaches interrupting token to queue for check in state parser
    Token pushTok(currentLine, currentColumn, nextToken, tokenType);
    tokenizedQ.push_front(pushTok);

    return ast.release();
}



// Semi-redundant parse of infix expression
TreeNode* New_Parser::parse(std::deque<Token>& tokenizedQ) {

    scanToken(tokenizedQ); // Initial consumption
    std::unique_ptr<TreeNode> rootTree(parseA(tokenizedQ));

    return rootTree.release();
}



// Parses assignment operators "="
TreeNode* New_Parser::parseA(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> left(parseInclusive(tokenizedQ));

    while (nextToken == "=" ) {
        scanToken(tokenizedQ);

        std::unique_ptr<TreeNode> right(parseA(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        std::unique_ptr<TreeAssign> assignNode(new TreeAssign());

        assignNode->addChild(left.release());
        assignNode->addChild(right.release());

        left.reset(assignNode.release());
    }
    return left.release();
}



// Parses inclusive or operators "|" 
TreeNode* New_Parser::parseInclusive(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> left(parseExclusive(tokenizedQ));

    while (nextToken == "|" ) {
        std::string op = nextToken;
        scanToken(tokenizedQ);

        std::unique_ptr<TreeNode> right(parseExclusive(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        std::unique_ptr<TreeOperator> opNode(new TreeOperator(op));

        opNode->addChild(left.release());
        opNode->addChild(right.release());

        left.reset(opNode.release());
    }
    return left.release();
}



// Parses exclusive or operators "^" 
TreeNode* New_Parser::parseExclusive(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> left(parseAnd(tokenizedQ));

    while (nextToken == "^") {
        std::string op = nextToken;
        scanToken(tokenizedQ);

        std::unique_ptr<TreeNode> right(parseAnd(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        std::unique_ptr<TreeOperator> opNode(new TreeOperator(op));
        opNode->addChild(left.release());
        opNode->addChild(right.release());

        left.reset(opNode.release());
    }
    return left.release();
}



// Parses and operators "&"
TreeNode* New_Parser::parseAnd(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> left(parseEquality(tokenizedQ));

    while (nextToken == "&") {
        std::string op = nextToken;
        scanToken(tokenizedQ);

        std::unique_ptr<TreeNode> right(parseEquality(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }

        std::unique_ptr<TreeOperator> opNode(new TreeOperator(op));
        opNode->addChild(left.release());
        opNode->addChild(right.release());

        left.reset(opNode.release());
    }
    return left.release();
}



// Parses equality operators "==" and "!="
TreeNode* New_Parser::parseEquality(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> left(parseComparison(tokenizedQ));

    while ((nextToken == "==") || (nextToken == "!=")) {
        std::string op = nextToken;
        scanToken(tokenizedQ);

        std::unique_ptr<TreeNode> right(parseComparison(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
        
        std::unique_ptr<TreeOperator> opNode(new TreeOperator(op));
        opNode->addChild(left.release());
        opNode->addChild(right.release());

        left.reset(opNode.release());
    }

    return left.release();

}



// Parses comparison operators "<" or ">="
TreeNode* New_Parser::parseComparison(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> left(parseE(tokenizedQ));

    while (nextToken == "<" || nextToken == ">" || nextToken == "<=" || nextToken == ">=") {
        
        std::string op = nextToken;
        scanToken(tokenizedQ);
        std::unique_ptr<TreeNode> right(parseE(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
        
        std::unique_ptr<TreeOperator> opNode(new TreeOperator(op));
        opNode->addChild(left.release());
        opNode->addChild(right.release());

        left.reset(opNode.release());
    }
    return left.release();
}



// Parses expression operators "+" "-"
TreeNode* New_Parser::parseE(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> left(parseT(tokenizedQ));

    while (nextToken == "+" || nextToken == "-") {
        
        std::string op = nextToken;
        scanToken(tokenizedQ);
        std::unique_ptr<TreeNode> right(parseT(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
        
        std::unique_ptr<TreeOperator> opNode(new TreeOperator(op));
        opNode->addChild(left.release());
        opNode->addChild(right.release());

        left.reset(opNode.release());
    }
    return left.release();
}



// Parses term operators "*" "/" "%"
TreeNode* New_Parser::parseT(std::deque<Token>& tokenizedQ) {

    std::unique_ptr<TreeNode> left(parseCall(tokenizedQ));

    while (nextToken == "*" || nextToken == "/" || nextToken == "%") {
        
        std::string op = nextToken;
        scanToken(tokenizedQ);
        std::unique_ptr<TreeNode> right(parseCall(tokenizedQ));

        if (right == nullptr || left == nullptr) {
            throw ParseError(currentLine, currentColumn, nextToken);
        }
        
        std::unique_ptr<TreeOperator> opNode(new TreeOperator(op));
        opNode->addChild(left.release());
        opNode->addChild(right.release());

        left.reset(opNode.release());
    }
    return left.release();
}



// Parses function calls
TreeNode* New_Parser::parseCall(std::deque<Token>& tokenizedQ) {
    std::unique_ptr<TreeNode> left(parseArray(tokenizedQ));

    while (nextToken == "("){
        std::unique_ptr<TreeCall> callNode(new TreeCall(left.release()));
        callNode->setArgs(parseArgs(tokenizedQ));

        left.reset(callNode.release());
    }
    return left.release();
}


// Parses an Array Literal OR an Array Lookup
TreeNode* New_Parser::parseArray(std::deque<Token>& tokenizedQ) { 
    std::unique_ptr<TreeNode> left;

    // Parsing an Array Literal
    if (nextToken == "[") {
        std::vector<TreeNode*> elements = parseArgs(tokenizedQ);
        std::unique_ptr<TreeArray> arrayLiteralNode(new TreeArray(nullptr, elements));
        left = std::move(arrayLiteralNode);
    }

    // Parsing all other expressions
    else {
        std::unique_ptr<TreeNode> node(parseF(tokenizedQ));
        left.reset(node.release());
    }

    // Parsing array lookups, in the format of variable[array], or [array][index]
    while (nextToken == "[") {
        std::unique_ptr<TreeNode> idxNode(parseIdx(tokenizedQ));
        std::unique_ptr<TreeArrayCall> arrayCallNode(new TreeArrayCall(left.release(), idxNode.release()));
        left = std::move(arrayCallNode);
    }
    return left.release();
}



// Parses a factor (integer, ID, parenthesis, null)
TreeNode* New_Parser::parseF(std::deque<Token>& tokenizedQ) {

    if (nextToken.empty()) {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    // Parsing Boolean
    else if (tokenType == Type::BOOL) { 
        std::unique_ptr<TreeBoolean> boolVal(new TreeBoolean(nextToken));
        scanToken(tokenizedQ); // Consume true or false
        return boolVal.release();
    }

    // Parsing Number
    else if (tokenType == Type::NUM) {
        std::unique_ptr<TreeLeaf> leaf(new TreeLeaf(std::stold(nextToken)));
        scanToken(tokenizedQ); // Consume digit
        return leaf.release();
    }

    // Parsing Parenthesis
    else if (nextToken == "(") {
        scanToken(tokenizedQ); // Consume open parenthesis
        std::unique_ptr<TreeNode> node(parseA(tokenizedQ));

        if (nextToken == ")") {
            scanToken(tokenizedQ); // Consume ")"
            return node.release();
        }
        else {
            throw ParseError(currentLine, currentColumn, nextToken);
            // Expects ")"
        }
    }
    
    // Parsing Identifiers
    else if (tokenType == Type::ID) {
        std::unique_ptr<TreeIdentifier> leaf(new TreeIdentifier(nextToken));
        scanToken(tokenizedQ); // Consume variable 
        return leaf.release();
    }

    // Parsing Null
    else if (nextToken == "null"){
        std::unique_ptr<TreeStatement> nullPtr(new TreeStatement("null"));
        scanToken(tokenizedQ); // Consume "null"
        return nullPtr.release();
    }

    else {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    return nullptr; // Should never reach here
}



// Parses arguments as expressions between bounding tokens ("()" or "[]")
std::vector<TreeNode*> New_Parser::parseArgs(std::deque<Token>& tokenizedQ) {
    std::string boundL = nextToken; // Either "(" or "[" depending on where its called
    scanToken(tokenizedQ); // Reads left bound
    std::string boundR = (boundL == "(") ? ")" : "]"; // Finds what pair bound should be
    std::vector<TreeNode*> argForest;

    // No args case
    if(nextToken == boundR){
        scanToken(tokenizedQ); // Reads right bound
        return argForest;
    }

    argForest.push_back(parseA(tokenizedQ));
    if(nextToken != "," && nextToken != boundR){
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    // Non-first args are led by a ","
    while(nextToken != boundR){
        scanToken(tokenizedQ); // Reads ","
        argForest.push_back(parseA(tokenizedQ));

        if(nextToken == boundR){
            break;
        }
        if(nextToken != ","){
            throw ParseError(currentLine, currentColumn, nextToken);
        }
    }
    scanToken(tokenizedQ); // Reads right bound

    return argForest;
}


// Helper function to parse index for array lookup
TreeNode* New_Parser::parseIdx(std::deque<Token>& tokenizedQ) {
    scanToken(tokenizedQ); // Consume '['

    // If index is an expression, parse:
    std::unique_ptr<TreeNode> idxNode(parseA(tokenizedQ)); 
    
    // Check for closing square bracket
    if (nextToken != "]") {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    scanToken(tokenizedQ); // Consume ']'
    return (idxNode.release());
}
