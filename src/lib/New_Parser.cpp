#include "New_Parser.h"

New_Parser::New_Parser(std::queue<Token> tokenizedQ) {
    lexQueue = tokenizedQ;
    resultTree = parseE();
    // account for errors here
}



void New_Parser::scanToken() {
    nextToken = lexQueue.front().text;
    if (!lexQueue.empty()) {
        lexQueue.pop();
    }
    else {
        // TODO: EMPTY QUEUE ERROR?
    }
}



std::unique_ptr<TreeNode> New_Parser::parseT() {
// this function parses TERMS: expressions seperated by multiplication or division
    
    std::unique_ptr<TreeNode> a = parseF();
    while (true) {
        if (nextToken == "*") {
            scanToken();
            std::unique_ptr<TreeNode> b = parseF();
            a = std::make_unique<Mult>(a,b);
            // TODO: check for errors here
        }
        else if (nextToken == "/") {
            scanToken();
            std::unique_ptr<TreeNode> b = parseF();
            a = std::make_unique<Div>(a,b);
            // TODO: check for errors here
        }
        else {
            return a;
        }
    }
    // needs to return something here

}



std::unique_ptr<TreeNode> New_Parser::parseF() {
// this function parses a FACTOR: an integer, identifier, or entire expression

    if (isdigit(nextToken.at(0))) { // does this correctly check if token is an integer?
        std::unique_ptr<TreeNode> integer;
        integer = std::make_unique<Integer>(std::stold(nextToken));
        scanToken();
        return integer;
        // we want nextToken to be the OBJECT that represents the ID or the integer
    }
    /*else if (isalpha(nextToken.at(0))) {
        
        std::unique_ptr<TreeNode> ID;
        ID = std::make_unique<ID>(nextToken);
        scanToken();
        return ID;
        // we want nextToken to be the OBJECT that represents the ID/variable
    }*/
    else if (nextToken == "(") {
        scanToken(); // consume non-terminal token
        std::unique_ptr<TreeNode> a = parseE();
        if (a == NULL) {
            return NULL;
            // TODO: CORRECT ERROR 
        }
        if (nextToken == ")") {
            scanToken();
            return a;
        }
        else {
            return NULL;
            // TODO: CORRECT ERROR
        }
    }
    else if (nextToken == "-") {
        std::unique_ptr<TreeNode> negate;
        negate = std::make_unique<Negate>(parseF()); 
        scanToken();
        return negate;
        // building a new negate object who points to the object we just parsed
    }
    else {
        return NULL;
        // TODO: CORRECT ERROR
    }
}



std::unique_ptr<TreeNode> New_Parser::parseE() {
    std::unique_ptr<TreeNode> a = parseT();
    while (true) { 
        if (nextToken == "+") {
            scanToken();
            std::unique_ptr<TreeNode> b = parseT();
            a = std::make_unique<Add>(a, b);
            // check for errors here
        }
        else if (nextToken == "-") {
            scanToken();
            std::unique_ptr<TreeNode> b = parseT();
            a = std::make_unique<Subtract>(a, b);
            // check for errors here
        }
        else {
            return a;
        }
    }
    // TODO: make sure to handle while loop errors
}



std::unique_ptr<TreeNode> New_Parser::getHead() {
    return std::move(resultTree);
    // this totally transfers ownership of resultTree
    // we could alternativly return a "raw pointer" to resultTree, and deal w memory with that
}



Add::Add(std::unique_ptr<TreeNode> left, std::unique_ptr<TreeNode> right) {
    this->left = std::move(left);
    this->right = std::move(right);
}

Subtract::Subtract(std::unique_ptr<TreeNode> left, std::unique_ptr<TreeNode> right) {
    this->left = std::move(left);
    this->right = std::move(right);
}

Mult::Mult(std::unique_ptr<TreeNode> left, std::unique_ptr<TreeNode> right) {
    this->left = std::move(left);
    this->right = std::move(right);
}

Div::Div(std::unique_ptr<TreeNode> left, std::unique_ptr<TreeNode> right) {
    this->left = std::move(left);
    this->right = std::move(right);
}

Negate::Negate(std::unique_ptr<TreeNode> arg) {
    this->arg = std::move(arg);
}

/*ID::ID(std::string variable) {
    this->variable = variable;
}*/

Integer::Integer(double val) {
    this->val = val;
}