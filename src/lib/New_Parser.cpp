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



TreeNode* New_Parser::parseT() {
// this function parses TERMS: expressions seperated by multiplication or division
    
    TreeNode* a = parseF();
    while (true) {
        if (nextToken == "*") {
            scanToken();
            TreeNode* b = parseF();
            a = new Mult(a,b);
            // TODO: check for errors here
        }
        else if (nextToken == "/") {
            scanToken();
            TreeNode* b = parseF();
            a = new Div(a,b);
            // TODO: check for errors here
        }
        else {
            return a;
        }
    }
    // needs to return something here

}



TreeNode* New_Parser::parseF() {
// this function parses a FACTOR: an integer, identifier, or entire expression

    if (isdigit(nextToken.at(0))) { // does this correctly check if token is an integer?
        TreeNode* integer;
        integer = new Integer(std::stold(nextToken));
        scanToken();
        return integer;
        // we want nextToken to be the OBJECT that represents the ID or the integer
    }
    /*else if (isalpha(nextToken.at(0))) {
        
        TreeNode* ID;
        ID = std::make_unique<ID>(nextToken);
        scanToken();
        return ID;
        // we want nextToken to be the OBJECT that represents the ID/variable
    }*/
    else if (nextToken == "(") {
        scanToken(); // consume non-terminal token
        TreeNode* a = parseE();
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
        TreeNode* negate;
        negate = new Negate (parseF()); 
        scanToken();
        return negate;
        // building a new negate object who points to the object we just parsed
    }
    else {
        return NULL;
        // TODO: CORRECT ERROR
    }
}



TreeNode* New_Parser::parseE() {
    TreeNode* a = parseT();
    while (true) { 
        if (nextToken == "+") {
            scanToken();
            TreeNode* b = parseT();
            a = new Add(a, b);
            // check for errors here
        }
        else if (nextToken == "-") {
            scanToken();
            TreeNode* b = parseT();
            a = new Subtract(a, b);
            // check for errors here
        }
        else {
            return a;
        }
    }
    // TODO: make sure to handle while loop errors
}



TreeNode* New_Parser::getHead() {
    return std::move(resultTree);
    // this totally transfers ownership of resultTree
    // we could alternativly return a "raw pointer" to resultTree, and deal w memory with that
}

void New_Parser::printInfix() {
    resultTree->print();
}



Add::Add(TreeNode* left, TreeNode* right) {
    this->left = std::move(left);
    this->right = std::move(right);
}
void Add::print() const {
    std::cout << "(";
    left->print();
    std::cout << " + ";
    right->print(); 
    std::cout << ")";
}

Add::~Add() {
    delete right;
    delete left;
}



Subtract::Subtract(TreeNode* left, TreeNode* right) {
    this->left = std::move(left);
    this->right = std::move(right);
}
void Subtract::print() const {
    std::cout << "(";
    left->print();
    std::cout << " - ";
    right->print(); 
    std::cout << ")";
}

Subtract::~Subtract() {
    delete right;
    delete left;
}



Mult::Mult(TreeNode* left, TreeNode* right) {
    this->left = std::move(left);
    this->right = std::move(right);
}
void Mult::print() const {
    std::cout << "(";
    left->print();
    std::cout << " * ";
    right->print(); 
    std::cout << ")";
}

Mult::~Mult() {
    delete right;
    delete left;
}



Div::Div(TreeNode* left, TreeNode* right) {
    this->left = std::move(left);
    this->right = std::move(right);
}
void Div::print() const {
    std::cout << "(";
    left->print();
    std::cout << " / ";
    right->print(); 
    std::cout << ")";
}
Div::~Div() {
    delete right;
    delete left;
}



Negate::Negate(TreeNode* arg) {
    this->arg = std::move(arg);
}
void Negate::print() const {
    std::cout << "(-";
    arg->print();
    std::cout << ")";
}
Negate::~Negate() {
    delete arg;
}

/*ID::ID(std::string variable) {
    this->variable = variable;
}*/

Integer::Integer(double val) {
    this->val = val;
}
void Integer::print() const {
    std::cout << val;
}
