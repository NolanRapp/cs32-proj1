#ifndef AST_H
#define AST_H


#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>


// For predicting output of node
enum class ReturnType {
    BOOL, // Booleans
    NUM,  // Doubles
    NUL,  // null
    FUNC, // Functions
    NONE  // Undefined var
};



class TreeNode {
    /* 
    This class is the BASE of the Tree Forest inheritance.
    It initializes our virtual functions to be utilized in their respective classes.
    TreeNode also contains a destructor, which gets inherited as well.
        If a variable goes out of the inherited functions scope, ~TreeNode will be called 
    */

    public:
        // Used to store both doubles and booleans in the variable map
        struct variableVal {
            struct Func {
                std::string name;
                TreeNode*   tree;
                std::unordered_map<std::string, variableVal> funcVars;
            };

            union Value {
                double  d;
                bool    b;
                Func*   f;
                // Can add arrays later
            };

            ReturnType  type;
            Value       value;

            variableVal()           : type(ReturnType::NUL)   {}
            variableVal(double val) : type(ReturnType::NUM)   { value.d = val; }
            variableVal(bool val)   : type(ReturnType::BOOL)  { value.b = val; }
            variableVal(Func* val)  : type(ReturnType::FUNC) { value.f = val; }
        };


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
        virtual std::string getID() { 
            throw std::runtime_error("Runtime error: invalid assignee.");
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
            // ReturnType depends on which operator is being held
            if(op == "+" || op == "-" || op == "*" ||
               op == "/" || op == "%"){
                return ReturnType::NUM;
            }
            return ReturnType::BOOL;
        }
        virtual void        printInfix(int depth) const; 
                void        addChild(TreeNode* child);
        virtual std::string getID() {
            throw std::runtime_error("Runtime error: invalid assignee.");
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
    Valid identifiers can hold values of Type::NUM or Type::BOOL (will use Type::NONE for empty id)
    */

    public:
                            TreeIdentifier(std::string name);
        virtual double      evalDouble(std::unordered_map<std::string, variableVal>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, variableVal>& vars) const;
        virtual ReturnType  type(std::unordered_map<std::string, variableVal>& vars) const {
            if (vars.find(idName) != vars.end()) {
                return vars[idName].type;
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
        void                addChild(TreeNode* child);
        virtual double      evalDouble(std::unordered_map<std::string, variableVal>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, variableVal>& vars) const;
        virtual ReturnType  type(std::unordered_map<std::string, variableVal>& vars) const {return ReturnType::BOOL;}
        virtual void        printInfix(int depth) const;
        virtual std::string getID() {
            throw std::runtime_error("Runtime error: invalid assignee.");
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
                            TreeAssign(){} // No constructor needed
        virtual double      evalDouble(std::unordered_map<std::string, variableVal>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, variableVal>& vars) const;
        virtual ReturnType  type(std::unordered_map<std::string, variableVal>& vars) const{ return children[children.size()-1]->type(vars); }
        virtual void        printInfix(int depth) const; 
                void        addChild(TreeNode* child);
        virtual std::string getID() {
            throw std::runtime_error("Runtime error: invalid assignee.");
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
    /*
    This class is used to store statement commands: "if", "while", and "print".
    This class has a special structure as it is not meant to return any specific
    value and instead evaluates forests and uses their values.
    */    

    public:
                            TreeStatement(std::string statement);
                void        evaluateReturn(std::unordered_map<std::string, variableVal>& vars) const;
                void        evaluateExp(std::unordered_map<std::string, variableVal>& vars) const;
                void        evaluatePrint(std::unordered_map<std::string, variableVal>& vars) const;
                void        evaluateWhile(std::unordered_map<std::string, variableVal>& vars) const;
                void        evaluateIf(std::unordered_map<std::string, variableVal>& vars) const;
        virtual double      evalDouble(std::unordered_map<std::string, variableVal>& vars) const;
        virtual bool        evalBool(std::unordered_map<std::string, variableVal>& vars) const;
        virtual ReturnType  type(std::unordered_map<std::string, variableVal>& vars) const { return ReturnType::NUM; } // Dummy return type, doesn't matter
        virtual void        printInfix(int depth) const; 
        virtual std::string getID() { 
            throw std::runtime_error("Runtime error: invalid assignee.");
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

        std::string               stateStr;   // stores type of statement
        TreeNode*                 condition;  // Tree for condition
        std::vector<TreeNode*>    truths;     // Forest for evaluation when condition is true
        std::vector<TreeNode*>    falses;     // Forest for evaluation when condition is false
        std::vector<std::string>  params;     // Forest of parameters (only used in "def" statement)
};


#endif
