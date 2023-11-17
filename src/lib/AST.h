#ifndef AST_H
#define AST_H


#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <memory>
#include <variant>


// For predicting output of node
enum class ReturnType {
    BOOL, // Booleans
    NUM,  // Doubles
    NUL,  // null
    FUNC, // Functions
    NONE  // Undefined var
};


class TreeNode;
struct variableVal {
    struct Func {
        std::shared_ptr<std::vector<TreeNode*>> mForest;
        std::vector<std::string>                mParams;

        Func(std::shared_ptr<std::vector<TreeNode*>> forest, std::vector<std::string> params){
            mForest = forest;
            mParams = params;
        }
    };

    ReturnType                                          type;
    std::variant<double, bool, std::shared_ptr<Func>>   value;

    variableVal()                               : type(ReturnType::NONE) {}
    variableVal(std::nullptr_t)                 : type(ReturnType::NUL)  {} // used specifically for return statement
    variableVal(double val)                     : type(ReturnType::NUM)  { value = val; }
    variableVal(bool val)                       : type(ReturnType::BOOL) { value = val; }
    variableVal(std::shared_ptr<Func> val)      : type(ReturnType::FUNC) { value = val; }
};



class ReturnVal: public std::runtime_error {
        variableVal mVal;
    public:
        ReturnVal(variableVal reVal): std::runtime_error("Runtime error: unexpected return."){
            mVal = reVal;
        }
        variableVal getVal() const{ return mVal; }
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

        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const = 0;
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
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;
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
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;
        variableVal         evalOp(std::unordered_map<std::string, variableVal>& vars) const;
        variableVal         evalComp(std::unordered_map<std::string, variableVal>& vars) const;
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
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;
        virtual void        printInfix(int depth) const;
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
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;
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
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;
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



class TreeCall : public TreeNode {

    public:
                            TreeCall(TreeNode* func);
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;
        virtual void        printInfix(int depth) const;
                void        setArgs(std::vector<TreeNode*> args);
        virtual std::string getID() {
            throw std::runtime_error("Runtime error: invalid assignee.");
        }; 

    private:
        TreeNode*               func;
        std::vector<TreeNode*>  args;
};



class TreeDefinition : public TreeNode {

    public:
                            TreeDefinition(std::string name);
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;
        virtual void        printInfix(int depth) const; 
        virtual std::string getID() { 
            throw std::runtime_error("Runtime error: invalid assignee.");
        }; 
        ~TreeDefinition() {

        }

        std::string               funcName;
        std::vector<std::string>  params;
        std::shared_ptr<std::vector<TreeNode*>> forest;
};



class TreeStatement : public TreeNode {
    /*
    This class is used to store statement commands: "if", "while", and "print".
    This class has a special structure as it is not meant to return any specific
    value and instead evaluates forests and uses their values.
    */    

    public:
                            TreeStatement(std::string statement);
                void        evaluateIf(std::unordered_map<std::string, variableVal>& vars) const;
                void        evaluateWhile(std::unordered_map<std::string, variableVal>& vars) const;
                void        evaluatePrint(std::unordered_map<std::string, variableVal>& vars) const;
                void        evaluateReturn(std::unordered_map<std::string, variableVal>& vars) const;
                void        evaluateExp(std::unordered_map<std::string, variableVal>& vars) const;
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;
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

        std::string             stateStr;   // stores type of statement
        TreeNode*               condition = nullptr;  // Tree for condition
        std::vector<TreeNode*>  truths; // Forest for evaluation when condition is true
        std::vector<TreeNode*>  falses; // Forest for evaluation when condition is true
};


#endif
