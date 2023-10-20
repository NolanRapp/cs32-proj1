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
        virtual double eval() const = 0;
        virtual void print() const = 0;
        virtual ~TreeNode();
        
    private:
};

class Add : public TreeNode {

    public:
        Add(std::unique_ptr<TreeNode> left, std::unique_ptr<TreeNode> right);
        virtual double eval() const;
        virtual void print() const;

    private:
        std::unique_ptr<TreeNode> left;
        std::unique_ptr<TreeNode> right;
};

class Subtract : public TreeNode {

    public:
        Subtract(std::unique_ptr<TreeNode> left, std::unique_ptr<TreeNode> right);
        virtual double eval() const;
        virtual void print() const;

    private:
        std::unique_ptr<TreeNode> left;
        std::unique_ptr<TreeNode> right;
};

class Mult : public TreeNode {

    public:
        Mult(std::unique_ptr<TreeNode> left, std::unique_ptr<TreeNode> right);
        virtual double eval() const;
        virtual void print() const;

    private:
        std::unique_ptr<TreeNode> left;
        std::unique_ptr<TreeNode> right;
};

class Div : public TreeNode {

    public:
        Div(std::unique_ptr<TreeNode> left, std::unique_ptr<TreeNode> right);
        virtual double eval() const;
        virtual void print() const;

    private:
        std::unique_ptr<TreeNode> left;
        std::unique_ptr<TreeNode> right;
};

class Negate : public TreeNode {

    public:
        Negate(std::unique_ptr<TreeNode> arg);
        virtual double eval() const; // return -(arg.eval())
        virtual void print() const;
    private:
        std::unique_ptr<TreeNode> arg;
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
        ~New_Parser(); // TODO: DESTRUCTOR
        
        std::unique_ptr<TreeNode> parseE(); // parsing and expression
        std::unique_ptr<TreeNode> parseT(); // parsing a term
        std::unique_ptr<TreeNode> parseF(); // parsing a factor

        void scanToken(); // sets nextToken to point to the newly scanned token

        std::unique_ptr<TreeNode> getHead();

    private:
        std::string nextToken; // this will be, at any point, the next unscanned token from the Token Queue.
        std::unique_ptr<TreeNode> resultTree;
        std::queue<Token> lexQueue;
};

#endif
