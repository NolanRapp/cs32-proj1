#ifndef PARSE_H
#define PARSE_H

#include "token.h"
#include <iostream>
#include <string>
#include <vector>

class TreeNode {
    /* 
    This class is the BASE of the inheritance implemented in Class TreeLeaf and 
    Class TreeOperator
    It initializes the function evaluateNode() and assigns it to a temporary value of 0.0
    TreeNode also contains a destructor, which gets inherited with evaluateNode().
        If a variable goes out of evaluateNode() scope, ~TreeNode will be called 
    */

    public:
        virtual double evaluateNode() = 0;
        virtual ~TreeNode() {};
};



class TreeLeaf : public TreeNode {
    /* 
    This class Is used to store and return ONE numerical value. 
    The evaluateNode() function will just return this value, to be used 
    in conjunction with TreeOperator
    */

    public:
        TreeLeaf(double val);
        virtual double evaluateNode();

    private:
        double value;
};



class TreeOperator : public TreeNode {
    /*
    This class assigns right and left children to an operator. This will 
    indicate which values the operation is to be preformed on.
    Eventually, this will allow for linkage and sorting of nodes
    */

    public:
        TreeOperator(char operation, TreeNode* right, TreeNode* left);
        virtual double evaluateNode();
        ~TreeOperator() {
            delete right;
            delete left;
        };

    private:
        char operation;
        TreeNode* right;
        TreeNode* left;
};



class Parser {
    /*
    This class reads in a token vector from the Lexer.
    It parses the vector to create an Abstract Syntax Tree.
    It then executes the desired operation using TreeNode and TreeLeaf,
    and TreeOperator classes.
    */

    public:
        Parser();
        TreeNode* parse();
    };


#endif
