#ifndef AST_H
#define AST_H


#include <string>
#include <iostream>
#include <vector>


class TreeNode {
    /* 
    This class is the BASE of the inheritance implemented in Class TreeLeaf and 
    Class TreeOperator
    It initializes the function evaluateNode(), our virtual function
    TreeNode also contains a destructor, which gets inherited with evaluateNode().
        If a variable goes out of evaluateNode() scope, ~TreeNode will be called 
    */

    public:
        virtual double evaluateNode() const = 0;
        virtual void printInfix() const = 0;
        virtual ~TreeNode() {};
};



class TreeLeaf : public TreeNode {
    /* 
    This class is used to store and return ONE numerical value. 
    The evaluateNode() function will just return this value, to be used 
    in conjunction with TreeOperator
    */

    public:
        TreeLeaf(double val);
        virtual double evaluateNode() const;
        virtual void printInfix() const;

    private:
        double value;
};



class TreeOperator : public TreeNode {
    /*
    This class assigns a vector of operands to an operator. This will 
    indicate which values the operation is to be preformed on.
    */

    public:
        TreeOperator(char operation);
        virtual double evaluateNode() const;
        void addChild(TreeNode* child);
        virtual void printInfix() const;
		~TreeOperator() {
            for (TreeNode* child : children) {
		        delete child;
	        }
	        children.clear();
        };

    private:
        char operation;
        std::vector<TreeNode*> children;
};


class TreeIdentifier : public TreeNode {
    /*
    This class 
    */

   
};


#endif
