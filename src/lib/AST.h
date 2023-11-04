#ifndef AST_H
#define AST_H


#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>


enum class ReturnType {
    BOOL,
    NUM
};






class TreeNode {
    /* 
    This class is the BASE of the inheritance implemented in Class TreeLeaf and 
    Class TreeOperator
    It initializes the function evaluateNode(), our virtual function
    TreeNode also contains a destructor, which gets inherited with evaluateNode().
        If a variable goes out of evaluateNode() scope, ~TreeNode will be called 
    */

    public:
        virtual double      evalDouble(std::unordered_map<std::string, double>& vars) const = 0;
        virtual bool        evalBool(std::unordered_map<std::string, double>& vars) const = 0;
        virtual ReturnType  type() const = 0; 
        virtual void        printInfix() const = 0;
        virtual std::string getID() = 0; // Should only be called on TreeIdentifiers
        virtual ~TreeNode() {};
        virtual ReturnType  rType() = 0;
};



class TreeLeaf : public TreeNode {
    /* 
    This class is used to store and return ONE numerical value. 
    The evaluateNode() function will just return this value, to be used 
    in conjunction with TreeOperator
    */

    public:
        TreeLeaf(double val);
        virtual double      evalDouble(std::unordered_map<std::string, double>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, double>& vars) const;
        virtual ReturnType  type() const {return ReturnType::NUM;}
        virtual void        printInfix() const;
        virtual std::string getID() { // Should only be called on TreeIdentifiers
            throw std::runtime_error("Runtime error: getID() called on Leaf");
            return "";
        }; 

    private:
        double value;
        ReturnType          rType(){ return ReturnType::NUM; };
};



class TreeOperator : public TreeNode {
    /*
    This class assigns a vector of operands to an operator. This will 
    indicate which values the operation is to be preformed on.
    */

    public:
        TreeOperator(std::string operation);
        virtual double      evalDouble(std::unordered_map<std::string, double>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, double>& vars) const;
        virtual ReturnType  type() const {return ReturnType::NUM;}
        virtual void        printInfix() const; 
                void        addChild(TreeNode* child);
        virtual std::string getID() { // Should only be called on TreeIdentifiers
            throw std::runtime_error("Runtime error: getID() called on Operator");
            return "";
        }; 
        ~TreeOperator() {
            for (auto child : children) {
                delete child;
            }
            children.clear();
        };

    private:
        std::string operation;
        std::vector<TreeNode*> children;
        ReturnType          rType(){ return ReturnType::NUM; };
};



class TreeIdentifier : public TreeNode {
    /*
    This class is used to store an identifier which will point
    to its value so it can be used across multiple AST trees
    */

    public:
        TreeIdentifier(std::string name);
        virtual double      evalDouble(std::unordered_map<std::string, double>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, double>& vars) const;
        virtual ReturnType  type() const {return ReturnType::NUM;}
        virtual void        printInfix() const;
        virtual std::string getID(); 

    private:
        std::string idName;
        ReturnType          rType(){ return ReturnType::NUM; };
};



class TreeBoolean : public TreeNode {
    /*
    This class is used to handle booleans, 
    and print them as "true" and "false"*/

    public:
        TreeBoolean(std::string op);
        void addChild(TreeNode* child);
        virtual double      evalDouble(std::unordered_map<std::string, double>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, double>& vars) const;
        virtual ReturnType  type() const {return ReturnType::BOOL;}
        virtual void        printInfix() const;
        virtual std::string getID() { // Should only be called on TreeIdentifiers
            throw std::runtime_error("Runtime error: getID() called on Leaf");
            return "";
        };
        ~TreeBoolean() {
            for (auto child : children) {
                delete child;
            }
            children.clear();
        };

    private:
        std::string op;
        std::vector<TreeNode*> children;
        ReturnType          rType(){ return ReturnType::BOOL; };
};



class TreeBooleanText : public TreeNode {
    /*
    This class is used to handle boolean text
    input like "true" and "false"
    */

   public:
        TreeBooleanText(bool TrueFalse);
        virtual double      evalDouble(std::unordered_map<std::string, double>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, double>& vars) const;
        virtual ReturnType  type() const {return ReturnType::BOOL;}
        virtual void        printInfix() const;
        virtual std::string getID() { // Should only be called on TreeIdentifiers
            throw std::runtime_error("Runtime error: getID() called on Leaf");
            return "";
        };
    
    private:
        bool TrueFalse;
        ReturnType          rType(){ return ReturnType::BOOL; };
};



// Incoming declaration of TreeStatment


#endif
