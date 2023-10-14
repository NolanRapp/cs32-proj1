#ifndef PARSE_H
#define PARSE_H

#include "token.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>

class TreeNode {
    /* 
    This class is the BASE of the inheritance implemented in Class TreeLeaf and 
    Class TreeOperator
    It initializes the function evaluateNode(), our virtual function
    TreeNode also contains a destructor, which gets inherited with evaluateNode().
        If a variable goes out of evaluateNode() scope, ~TreeNode will be called 
    */

    public:
        virtual double evaluateNode() const;
        virtual ~TreeNode() {};
};



class TreeLeaf : public TreeNode {
    /* 
    This class is used to store and return ONE numerical value. 
    The evaluateNode() function will just return this value, to be used 
    in conjunction with TreeOperator
    */

    public:
        TreeLeaf(char val);
        virtual double evaluateNode() const;

    private:
        double value;
};



class TreeOperator : public TreeNode {
    /*
    This class assigns a vector of operands to an operator. This will 
    indicate which values the operation is to be preformed on.

    UPDATE FUNCTION DEF
    */

    public:
        TreeOperator(char operation);
        virtual double evaluateNode() const;
        void addChild(TreeNode* child);
		//~TreeOperator();

    private:
        char operation;
        std::vector<TreeNode*> children;
};



class Parser {
    /*
    This class reads in a token vector from the Lexer.
    It parses the vector to create an Abstract Syntax Tree.
    It uses the TreeNode, TreeLeaf, and TreeOperator classes.
    */

    public:
        Parser(std::queue<token> originalInput);
        TreeNode* createTree(std::queue<token>& input); // recursively constructs AST
	
	private:
		TreeNode* mHead;
        bool isNum(std::string tokenValue); // helper function to check if token is a number

};


#endif
