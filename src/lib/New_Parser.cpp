#include "New_Parser.h"

New_Parser::New_Parser(std::queue<Token> tokenizedQ) {

    // TODO: account for END character 


    if (!tokenizedQ.empty()) {
        nextToken = tokenizedQ.front().text; // initializing nextToken
    } 
    else {
        std::cerr << "Error: Unexpected end of expression. New Parser constructor" << std::endl;
        exit(3);
    }
    resultTree = parseE(tokenizedQ);
}
New_Parser::~New_Parser() {
    delete resultTree;
}


void New_Parser::scanToken(std::queue<Token>& tokenizedQ) {
    if (!tokenizedQ.empty()) {
        nextToken = tokenizedQ.front().text;
        tokenizedQ.pop();
    }
    else {
        std::cerr << "Error: Unexpected end of expression. scanToken" << std::endl;
        exit(3);
    }
}



TreeNode* New_Parser::parseT(std::queue<Token>& tokenizedQ) {
// this function parses TERMS: expressions seperated by multiplication or division
    
    TreeNode* a = parseF(tokenizedQ);
    while (true) {
        if (!tokenizedQ.empty() && nextToken == "*") {
            scanToken(tokenizedQ);
            TreeNode* b = parseF(tokenizedQ);
            a = new Mult(a,b);

            if (a == nullptr || b == nullptr) {
                std::cerr << "Error: Invalid operands for the operation." << std::endl;
                exit(3);
            }
        }
        else if (!tokenizedQ.empty() && nextToken == "/") {
            scanToken(tokenizedQ);
            TreeNode* b = parseF(tokenizedQ);
            a = new Div(a,b);

            if (a == nullptr || b == nullptr) {
                std::cerr << "Error: Invalid operands for the operation." << std::endl;
                exit(3);
            }
        }
        else {
            return a;
        }
    }
    // needs to return something here?

}



TreeNode* New_Parser::parseF(std::queue<Token>& tokenizedQ) {
// this function parses a FACTOR: an integer, identifier, or entire expression

    if ((!nextToken.empty()) && (isdigit(nextToken.at(0)))) { // does this correctly check if token is an integer?
        TreeNode* integer;
        integer = new Integer(std::stold(nextToken));
        scanToken(tokenizedQ);
        return integer;
    }
    /*else if (isalpha(nextToken.at(0))) {
        
        TreeNode* ID;
        ID = std::make_unique<ID>(nextToken);
        scanToken();
        return ID;
        // we want nextToken to be the OBJECT that represents the ID/variable
    }*/
    else if (!tokenizedQ.empty() && nextToken == "(") {
        scanToken(tokenizedQ); // consume non-terminal token
        TreeNode* a = parseE(tokenizedQ);
        if (a == nullptr) {
            throw std::runtime_error("Invalid expression");
            exit(3);
        }
        if (nextToken == ")") {
            scanToken(tokenizedQ);
            return a;
        }
        else {
            std::cerr << "Error: Missing closing parenthesis." << std::endl;
            exit(3);
        }
    }
    else if (!tokenizedQ.empty() && nextToken == "-") {
        TreeNode* negate;
        negate = new Negate (parseF(tokenizedQ)); 
        scanToken(tokenizedQ);
        return negate;
        // building a new negate object who points to the object we just parsed
    }
    else {
        std::cerr << "Error: Unexpected token encountered while parsing a factor." << std::endl;
        exit(3);
    }
}



TreeNode* New_Parser::parseE(std::queue<Token>& tokenizedQ) {
    TreeNode* a = parseT(tokenizedQ);
    if (a == nullptr) {
        throw std::runtime_error("Invalid expression");
        exit(3);
    }

    while (true) { 
        if (!tokenizedQ.empty() && nextToken == "+") {
            scanToken(tokenizedQ);
            TreeNode* b = parseT(tokenizedQ);
            a = new Add(a, b);

            if (a == nullptr || b == nullptr) {
                std::cerr << "Error: Invalid operands for the operation." << std::endl;
                exit(3);
            }
        }
        else if (!tokenizedQ.empty() && nextToken == "-") {
            scanToken(tokenizedQ);
            TreeNode* b = parseT(tokenizedQ);
            a = new Subtract(a, b);

            if (a == nullptr || b == nullptr) {
                std::cerr << "Error: Invalid operands for the operation." << std::endl;
                exit(3);
            }
        }
        else {
            return a;
        }
    }
}



TreeNode* New_Parser::getHead() {
    return (resultTree);
}



void New_Parser::printInfix() {
    resultTree->print();
}



Add::Add(TreeNode* left, TreeNode* right) {
    this->left = (left);
    this->right = (right);
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
    this->left = (left);
    this->right = (right);
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
    this->left = (left);
    this->right = (right);
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
    this->left = (left);
    this->right = (right);
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
    this->arg = (arg);
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
