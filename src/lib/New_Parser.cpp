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
    return ast.release();
}



TreeNode* New_Parser::parseForState(std::deque<Token>& tokenizedQ) {
    std::unique_ptr<TreeNode> ast;
    ast.reset(parse(tokenizedQ));

    // Reattaches interrupting token to queue for check
    Token pushTok(currentLine, currentColumn, nextToken, tokenType);
    tokenizedQ.push_front(pushTok);

    return ast.release();
}



// MAIN Parser. Begining of Recursive Descent.
TreeNode* New_Parser::parse(std::deque<Token>& tokenizedQ) {

    scanToken(tokenizedQ); // Initial consumption
    std::unique_ptr<TreeNode> rootTree(parseA(tokenizedQ));

    return rootTree.release();
}



// Inclusive operator parser "=". Lowest precedence operator.
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



// Inclusive operator parser "|".
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

        std::unique_ptr<TreeOperator> opNode(new TreeOperator(op));
        opNode->addChild(left.release());
        opNode->addChild(right.release());

        left.reset(opNode.release());
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

        std::unique_ptr<TreeOperator> opNode(new TreeOperator(op));
        opNode->addChild(left.release());
        opNode->addChild(right.release());

        left.reset(opNode.release());
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
        
        std::unique_ptr<TreeOperator> opNode(new TreeOperator(op));
        opNode->addChild(left.release());
        opNode->addChild(right.release());

        left.reset(opNode.release());
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
        
        std::unique_ptr<TreeOperator> opNode(new TreeOperator(op));
        opNode->addChild(left.release());
        opNode->addChild(right.release());

        left.reset(opNode.release());
    }
    return left.release();
}



// Expression operator parser "+" and "-"
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



// Term operator parser "*", "/", "%"
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



//
TreeNode* New_Parser::parseCall(std::deque<Token>& tokenizedQ) {
    std::unique_ptr<TreeNode> left(parseF(tokenizedQ));

    while (nextToken == "("){
        std::unique_ptr<TreeCall> callNode(new TreeCall(left.release()));
        callNode->setArgs(parseArgs(tokenizedQ));

        left.reset(callNode.release());
    }
    return left.release();
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
        scanToken(tokenizedQ); // Consume digit
        return leaf.release();
    }
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
    else if (tokenType == Type::ID) {
        std::unique_ptr<TreeIdentifier> leaf(new TreeIdentifier(nextToken));
        scanToken(tokenizedQ); // Consume variable 
        return leaf.release();
    }
    else {
        throw ParseError(currentLine, currentColumn, nextToken);
    }

    return nullptr; // Should never reach here
}



// Parses arguments for arrays and function calls
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






