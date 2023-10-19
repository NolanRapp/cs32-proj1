#include "New_Parser.h"

New_Parser::New_Parser(std::queue<Token> tokenizedQ) {
    scanToken(tokenizedQ.front().text); // i don't think im implementing this right, but we'll see
                                        // might need to be a char
    resultTree = parseE();

    /*if (nextToken != '\n') {
        // ERROR
    }*/
}

void New_Parser::scanToken(std::string tokenText) {
    nextToken = tokenText;
}

TreeNode* New_Parser::parseT() {
// this is where we set a pointers to individual terms 
// like integers, 
}

TreeNode* New_Parser::parseF() {
    if (nextToken == ID) {
        return nextToken;
        // we want nextToken to be the OBJECT that represents the ID or the integer
    }
    else if (nextToken == INTEGER) {
        return nextToken;
        // we want nextToken to be the OBJECT that represents the ID or the integer
    }
    else if (nextToken == "(") {
        scanToken(); // consume non-terminal token
        TreeNode* a = parseE();
        if (a == NULL) {
            return NULL;
            // ERROR 
        }
        if (nextToken == ")") {
            scanToken();
            return a;
        }
        else {
            return NULL;
            // ERROR 
        }
    }
    else if (nextToken == "-") {
        scanToken();
        return new Negate(parseF()); 
        // building a new negate object who points to the object we just parsed
    }
    else {
        return NULL;
        // ERROR
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
}


Add::Add(TreeNode* left, TreeNode* right) {
    left = left;
    right = right;
}

Subtract::Subtract(TreeNode* left, TreeNode* right) {
    left = left;
    right = right;
}

Mult::Mult(TreeNode* left, TreeNode* right) {
    left = left;
    right = right;
}

Div::Div(TreeNode* left, TreeNode* right) {
    left = left;
    right = right;
}

Negate::Negate(TreeNode* arg) {
    arg = arg;
}

Integer::Integer(double val) {
    val = val;
}