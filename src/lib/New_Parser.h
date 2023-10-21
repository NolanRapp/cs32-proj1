#ifndef NEW_PARSER_H
#define NEW_PARSER_H

#include "Lexer.h"
#include "Token.h"
#include <memory>

/* New_Parser implements Recursive Descent Parsing - a top down AST parsing method
        sources: https://en.wikipedia.org/wiki/Recursive_descent_parser
                https://www.youtube.com/watch?v=SToUyjAsaFk
*/

class TreeNode {
    // this is our base class for our new AST - parsing infix expressions

    public:
        //virtual double eval() const = 0;
        virtual void print() const = 0;
        virtual ~TreeNode() {}
        
    private:
};

class Add : public TreeNode {

    public:
        Add(TreeNode* left, TreeNode* right);
        ~Add();
        //double eval() const override;
        void print() const override;

    private:
        TreeNode* left;
        TreeNode* right;
};

class Subtract : public TreeNode {

    public:
        Subtract(TreeNode* left, TreeNode* right);
        ~Subtract();
        //double eval() const override;
        void print() const override;

    private:
        TreeNode* left;
        TreeNode* right;
};

class Mult : public TreeNode {

    public:
        Mult(TreeNode* left, TreeNode* right);
        ~Mult();
        //double eval() const override;
        void print() const override;

    private:
        TreeNode* left;
        TreeNode* right;
};

class Div : public TreeNode {

    public:
        Div(TreeNode* left, TreeNode* right);
        ~Div();
        //double eval() const override;
        void print() const override;

    private:
        TreeNode* left;
        TreeNode* right;
};

class Negate : public TreeNode {

    public:
        Negate(TreeNode* arg);
        ~Negate();
        //double eval() const override; // return -(arg.eval())
        void print() const override;
    private:
        TreeNode* arg;
};

/*class ID : public TreeNode {
    
    public:
        ID(std::string variable);
        virtual double eval() const; // return variable .. not sure about this one yet
        virtual void print() const;

    private:
        std::string variable:

};*/

class Integer : public TreeNode {

    public:
        Integer(double val);
        ~Integer() {}
        //double eval() const override; // return val;
        void print() const override;

    private:
        double val;

// how do I clean up memory for this class?
};

class New_Parser {
    /* parser implentation for infix expressions.
    Each helper function is for a different non-terminal grammar symbol
    Helper functions return a pointer to the tree that it builds
    */

    public:
        New_Parser(std::queue<Token> tokenizedQ); // constructor that begins parsing
        //~New_Parser(); // TODO: DESTRUCTOR
        
        TreeNode* parseE(); // parsing and expression
        TreeNode* parseT(); // parsing a term
        TreeNode* parseF(); // parsing a factor

        void scanToken(); // sets nextToken to point to the newly scanned token

        TreeNode* getHead();

        void printInfix();

    private:
        std::string nextToken; // this will be, at any point, the next unscanned token from the Token Queue.
        TreeNode* resultTree;
        std::queue<Token> lexQueue;
};

#endif
