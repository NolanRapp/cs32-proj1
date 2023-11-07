#ifndef AST_H
#define AST_H


#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>


enum class ReturnType {
    BOOL,
    NUM,
    NONE,
    INVALID
};



struct variableVal {
    double  doubleVal;
    bool    boolVal;
    bool    isBool;

    variableVal()               : doubleVal(0), boolVal(false), isBool(false) {}
    variableVal(double number)  : doubleVal(number), boolVal(false), isBool(false) {} // num constructor
    variableVal(bool boolean)   : doubleVal(0), boolVal(boolean), isBool(true) {} // bool constructor
};




class TreeNode {
    /* 
    This class is the BASE of the Tree Forest inheritance.
    It initializes our virtual functions to be utilized in their respective classes.
    TreeNode also contains a destructor, which gets inherited as well.
        If a variable goes out of the inherited functions scope, ~TreeNode will be called 
    */

    public:
        virtual double      evalDouble(std::unordered_map<std::string, variableVal>& vars) const = 0;
        virtual bool        evalBool(std::unordered_map<std::string, variableVal>& vars) const = 0;
        virtual ReturnType  type(std::unordered_map<std::string, variableVal>& vars) const = 0; 
        virtual void        printInfix(int depth) const = 0;
        virtual std::string getID() = 0; // Should only be called on TreeIdentifiers
        virtual ~TreeNode() {};
};



class TreeLeaf : public TreeNode {
    /* 
    This class is used to store, evaluate (return), and print and return the leaf node of the AST.
    This is always ONE numerical operator. 
    */

    public:
        TreeLeaf(double val);
        virtual double      evalDouble(std::unordered_map<std::string, variableVal>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, variableVal>& vars) const;
        virtual ReturnType  type(std::unordered_map<std::string, variableVal>& vars) const {return ReturnType::NUM;}
        virtual void        printInfix(int depth) const;
        virtual std::string getID() { // Should only be called on TreeIdentifiers
            throw std::runtime_error("Runtime error: getID() called on Leaf");
            return "";
        } 

    private:
        double value;
};



class TreeOperator : public TreeNode {
    /*
    This class is used to store, evaluate, and print operators of the AST.
    The operator nodes vector of type TreeNode* storing their operands.
    */

    public:
        TreeOperator(std::string operation);
        virtual double      evalDouble(std::unordered_map<std::string, variableVal>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, variableVal>& vars) const;
        virtual ReturnType  type(std::unordered_map<std::string, variableVal>& vars) const { 
            if(op == "+" || op == "-" || op == "*" ||
               op == "/" || op == "%"){
                return ReturnType::NUM;
            }
            return ReturnType::BOOL;
        }

        virtual void        printInfix(int depth) const; 
                void        addChild(TreeNode* child);
        virtual std::string getID() { // Should only be called on TreeIdentifiers
            throw std::runtime_error("Runtime error: getID() called on Operator");
            return "";
        } 
        ~TreeOperator() {
            for (auto child : children) {
                delete child;
            }
            children.clear();
        }

    private:
        std::string op;
        std::vector<TreeNode*> children;
};



class TreeIdentifier : public TreeNode {
    /*
    This class is used to store, evaluate, and print an identifier.
    The identifier points to its value so it can be used across multiple AST trees.
    Valid identifiers can hold values of Type::NUM or Type::BOOL. 
    */

    public:
        TreeIdentifier(std::string name);
        virtual double      evalDouble(std::unordered_map<std::string, variableVal>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, variableVal>& vars) const;
        virtual ReturnType  type(std::unordered_map<std::string, variableVal>& vars) const {
            if (vars.find(idName) != vars.end()) {
                if (vars[idName].isBool) {
                    return ReturnType::BOOL;
                }
                else {
                    return ReturnType::NUM;
                }
            }
            else {
                return ReturnType::NONE;
            }
        }

        virtual void        printInfix(int depth) const;
                void        addChild(TreeNode* child);
        virtual std::string getID(); 

    private:
        std::string idName;
};



class TreeBoolean : public TreeNode {
    /*
    This class is used to store, evaluate, and print booleans.
    It prints booleans at "true" or "false".
    */

    public:
        TreeBoolean(std::string value);
        void addChild(TreeNode* child);
        virtual double      evalDouble(std::unordered_map<std::string, variableVal>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, variableVal>& vars) const;
        virtual ReturnType  type(std::unordered_map<std::string, variableVal>& vars) const {return ReturnType::BOOL;}
        virtual void        printInfix(int depth) const;
        virtual std::string getID() { // Should only be called on TreeIdentifiers
            throw std::runtime_error("Runtime error: getID() called on Bool");
            return "";
        };

    private:
        std::string value;
};



class TreeAssign : public TreeNode {
    /*
    This class is used to store, evaluate, and print assignment operations only.
    It handles both double and boolean assignments.
    */

   public:
        TreeAssign(){}
        virtual double      evalDouble(std::unordered_map<std::string, variableVal>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, variableVal>& vars) const;
        virtual ReturnType  type(std::unordered_map<std::string, variableVal>& vars) const;  // return children[children.size()-1]->type(vars);
        virtual void        printInfix(int depth) const; 
                void        addChild(TreeNode* child);
        virtual std::string getID() { // Should only be called on TreeIdentifiers
            throw std::runtime_error("Runtime error: getID() called on Assign");
            return "";
        }; 
        ~TreeAssign() {
            for (auto child : children) {
                delete child;
            }
            children.clear();
        }

    private:
        std::vector<TreeNode*> children;
};




class TreeStatement : public TreeNode {

   public:
        TreeStatement(std::string statement);
                void        evaluatePrint(std::unordered_map<std::string, variableVal>& vars) const;
                void        evaluateWhile(std::unordered_map<std::string, variableVal>& vars) const;
                void        evaluateIf(std::unordered_map<std::string, variableVal>& vars) const;
        virtual double      evalDouble(std::unordered_map<std::string, variableVal>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, variableVal>& vars) const;
        virtual ReturnType  type(std::unordered_map<std::string, variableVal>& vars) const { return ReturnType::NUM; } // Dummy return type, doesn't matter
        virtual void        printInfix(int depth) const; 
        virtual std::string getID() { // Should only be called on TreeIdentifiers
            throw std::runtime_error("Runtime error: getID() called on Statement");
            return "";
        }; 
        ~TreeStatement() {
            delete condition;
            for (auto child : truths) {
                delete child;
            }
            for (auto child : falses) {
                delete child;
            }
            truths.clear();
            falses.clear();
        }

        std::string stateStr;
        TreeNode* condition;
        std::vector<TreeNode*> truths;
        std::vector<TreeNode*> falses;
};


#endif
