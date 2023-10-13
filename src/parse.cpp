#include "lib/parse.h"
#include "lib/lex.h"

TreeLeaf::TreeLeaf (double val) {
    value = val; // initializing one numerical
}

double TreeLeaf::evaluateNode() {
    return value;
}



TreeOperator::TreeOperator(char operation, TreeNode* right, TreeNode* left) {
    operation = operation;
    right = right;
    left = left; 

    // initializing operation used, right child, and left child for class
}

double TreeOperator::evaluateNode() {
    //TODO: implement... call TreeLeaf's evaulateNode to get values

    // the purpose of this function is to return the evaluated operation
    // between the left child, operator, and right child

    // maybe use "switch"


    return 0.0;
}



Parser::Parser() {
    // nothing to do here yet
    // eventually: we want this constructor to initialize the vector
    //              of tokens created by lexer
}

TreeNode* Parser::parse() {
    /*
    todo:   function is supposed to parse tokens / construct AST
            function will call above functions to then execute the operation
            ** use recursion? 
    */ 
    
    return nullptr;
};



int main() {
    std::string input;
    std::getline(std::cin, input);
    // reading input into a string
    
    /* 
    Lexer lexer(input);
    not sure the implementation of lexer yet
    but place here: lexer and input as tokens in a vector
    std::vector<Token> tokens;

   Parser parser(tokens);


   TODO:    build AST using parser.parse() function 
                variable of type TreeNode? TreeNode*?
            print AST in in "infix form" (maybe write another function for this?) 
            print the value of the expression 
            
            otherwise, print error messages
                - lexer error: print the same error message as the lex program
                - parse error: print an error message with exit code 2
                - division by zero error while evaluating AST: print "Runtime Error: division by zero" and exit with exit code 3
            delete AST TreeNode* variable to keep memory clean
    
    */

   return 0;
};