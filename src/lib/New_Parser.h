#ifndef NEW_PARSER_H
#define NEW_PARSER_H

#include "Lexer.h"
#include "Token.h"

/* New_Parser implements Recursive Descent Parsing - a top down AST parsing method
        sources: https://en.wikipedia.org/wiki/Recursive_descent_parser
                https://www.youtube.com/watch?v=SToUyjAsaFk
*/

class TreeNode {
    // this is our base class for our new AST - parsing infix expressions

    public:
        virtual double eval() const = 0;
        virtual void print() const = 0;
        
    private:
};

class Add : public TreeNode {

    public:
        Add(TreeNode* left, TreeNode* right);
        virtual double eval() const;
        virtual void print() const;

    private:
        TreeNode* left;
        TreeNode* right;
};

class Subtract : public TreeNode {

    public:
        Subtract(TreeNode* left, TreeNode* right);
        virtual double eval() const;
        virtual void print() const;

    private:
        TreeNode* left;
        TreeNode* right;
};

class Mult : public TreeNode {

    public:
        Mult(TreeNode* left, TreeNode* right);
        virtual double eval() const;
        virtual void print() const;

    private:
        TreeNode* left;
        TreeNode* right;
};

class Div : public TreeNode {

    public:
        Div(TreeNode* left, TreeNode* right);
        virtual double eval() const;
        virtual void print() const;

    private:
        TreeNode* left;
        TreeNode* right;
};

class Negate : public TreeNode {

    public:
        Negate(TreeNode* arg);
        virtual double eval() const; // return -(arg.eval())
        virtual void print() const;
    private:
        TreeNode* arg;
};

class ID : public TreeNode {
    
    public:
        virtual double eval() const; // return variable .. not sure about this one yet
        virtual void print() const;

    private:

};

class Integer : public TreeNode {

    public:
        Integer(double val);
        virtual double eval() const; // return val;
        virtual void print() const;

    private:
        double val;
};

class New_Parser {
    /* parser implentation for infix expressions.
    Each helper function is for a different non-terminal grammar symbol
    Helper functions return a pointer to the tree that it builds
    */

    public:
        New_Parser(std::queue<Token> tokenizedQ); // constructor that begins parsing
        
        TreeNode* parseE(); // parsing and expression
        TreeNode* parseT(); // parsing a term
        TreeNode* parseF(); // parsing a factor

        void scanToken(std::string tokenText); // sets nextToken to point to the newly scanned token

    private:
        std::string nextToken; // this will be, at any point, the next unscanned token from the Token Queue.
        TreeNode* resultTree;
};

#endif
