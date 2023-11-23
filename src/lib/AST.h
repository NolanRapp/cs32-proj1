#ifndef AST_H
#define AST_H


#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <memory>
#include <variant>
#include <math.h>


// For predicting output of node
enum class ReturnType {
    BOOL, // Booleans
    NUM,  // Doubles
    NUL,  // null
    FUNC, // Functions
    ARRAY, // Arrays
    NONE  // Undefined var
};



//Forward declaration for variableVal
class TreeNode;
struct variableVal {
    /*
    A struct to properly represent the return value of a given node
    as well as identifiers stored in variable maps.
    */

    struct Func {
        /*
        A struct to properly store all information pretaining to a
        function so a function can be assigned the variables and
        easily called from those variables
        */

        std::shared_ptr<std::vector<TreeNode*>>         mForest; // A ptr to a forest to be evaluated when the function is called
        std::vector<std::string>                        mParams; // A vector of parameters that will be assigned values when called
        std::unordered_map<std::string, variableVal>*   mVars;   // A map that stores a function scope which is limited to when it was declared and allows parameters to be stored locally

        Func(std::shared_ptr<std::vector<TreeNode*>> forest, 
                std::vector<std::string> params, 
                std::unordered_map<std::string, variableVal> vars){
            mForest = forest;
            mParams = params;
            mVars   = new std::unordered_map<std::string, variableVal>(vars);
        }
        ~Func(); 
    };

    struct Array {
        /*
        A struct to properly store all information pretaining to arrays
        so a arrays can be assigned variables, reassigned, and properly handeled.
        */

        std::vector<variableVal> elements;
        
        Array(const std::vector<variableVal>& e) : elements(e) {}
        //~Array();
            
    };

    ReturnType                                          type;    // Stores current ReturnType so the right value can be returned
    std::variant<double, bool, std::shared_ptr<Func>, std::shared_ptr<Array>>   value;   // Stores actual value in a std::variant (a union)

    bool operator == (const variableVal& lVal) const;            // Evaluates equality between values
    bool operator != (const variableVal& lVal) const;            // Evaluates inequality between values

    variableVal()                               : type(ReturnType::NONE) {}
    variableVal(std::nullptr_t)                 : type(ReturnType::NUL)  {} 
    variableVal(double val)                     : type(ReturnType::NUM)  { value = val; }
    variableVal(bool val)                       : type(ReturnType::BOOL) { value = val; }
    variableVal(std::shared_ptr<Func> val)      : type(ReturnType::FUNC) { value = val; }
    variableVal(std::shared_ptr<Array> val)     : type(ReturnType::ARRAY) { value = val;}
};



// Special error that is meant the be caught by function calls to convey a return value
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
    */

    public:
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const = 0; // Evaluates value based on type of node
        virtual void        printInfix(int depth) const = 0;                                        // Prints node is desired format with depth (indentation)
        virtual std::string getID() = 0;                                                            // Will error on all nodes except TreeIdentifiers
        virtual ~TreeNode() {};
};



class TreeLeaf : public TreeNode {
    /* 
    This class is used to store, evaluate (return), and print and return the leaf node of the AST.
    This is always ONE numerical operator. 
    */

    public:
                            TreeLeaf(double val);                                                   // Stores a double inside node
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;     // Returns a variableVal holding the the node's double value
        virtual void        printInfix(int depth) const;                                            // Prints double value
        virtual std::string getID() {                                                               // Will only work on TreeIdentifiers
            throw std::runtime_error("Runtime error: invalid assignee.");
        };
    private:
        double value;                                                                               // Stored double value
};



class TreeOperator : public TreeNode {
    /*
    This class is used to store, evaluate, and print operators of the AST.
    The operator nodes vector of type TreeNode* storing their operands.
    ("+", "-", "/", "*", "%", ">", ">=", "<", "<=", "==", "!=", "&", "|", "^")
    */

    public:
                            TreeOperator(std::string operation);                                    // Stores operator as string inside node
                void        addChild(TreeNode* child);                                              // Adds children to node (left and right child)
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;     // Returns either a variableVal based on operator and its children
        variableVal         evalOp(std::unordered_map<std::string, variableVal>& vars) const;       // Called by evaluate to return a double variableVal determined by children
        variableVal         evalComp(std::unordered_map<std::string, variableVal>& vars) const;     // Called by evaluate to return a bool variableVal determined by children
        virtual void        printInfix(int depth) const;                                            // Print in format "([left child] [operator] [right child])"
        virtual std::string getID(){                                                                // Will only work on TreeIdentifiers
            throw std::runtime_error("Runtime error: invalid assignee.");
        };
        ~TreeOperator() {
            for (TreeNode* child : children) {
                delete child;
            }
        }

    private:
        std::string op;                                                                             // Stored operator as string
        std::vector<TreeNode*> children;                                                            // Children stored in vector (first index is left child and second index is right child)
};



class TreeIdentifier : public TreeNode {
    /*
    This class is used to store, evaluate, and print an identifier.
    The identifier points to its value so it can be used across multiple AST trees.
    Valid identifiers can be mapped to a variableVal holding any type.
    */

    public:
                            TreeIdentifier(std::string name);                                       // Stores identifier name as string inside node
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;     // Return variableVal that the identifiers name maps to in the variable map
        virtual void        printInfix(int depth) const;                                            // Prints identifier name
        virtual std::string getID();                                                                // Returns the variable name, also used to see if a node is a TreeIdentifier

    private:
        std::string idName;                                                                         // Identifier name as string

};



class TreeBoolean : public TreeNode {
    /*
    This class is used to store, evaluate, and print booleans.
    It prints booleans at "True" or "False".
    */

    public:
                            TreeBoolean(std::string value);                                         // Stores "true" or "false" as string inside node
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;     // Returns variableVal True or False depending on node's value string
        virtual void        printInfix(int depth) const;                                            // Prints "True" or "False" depending on node's value string
        virtual std::string getID() {                                                               // Will only work on TreeIdentifiers
            throw std::runtime_error("Runtime error: invalid assignee.");
        };

    private:
        std::string value;                                                                          // Node's boolean value stored as "true" or "false"
};



class TreeAssign : public TreeNode {
    /*
    This class is used to store, evaluate, and print assignment operations only.
    It handles assignments of variableVal which can store any type..
    */

    public:
                            TreeAssign(){}                                                          // No constructor needed
                void        addChild(TreeNode* child);                                              // Adds children to node (left and right child)
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;     // Attempts to assign left child (identifier or error) the value of its right child
        virtual void        printInfix(int depth) const;                                            // Print in format "([left child] = [right child])"
        virtual std::string getID() {                                                               // Will only work on TreeIdentifiers
            throw std::runtime_error("Runtime error: invalid assignee.");
        };
        ~TreeAssign() {
            for (TreeNode* child : children) {
                delete child;
            }
        }

    private:
        std::vector<TreeNode*> children;                                                            // Children stored in vector (first index is left child and second index is right child)
};



class TreeCall : public TreeNode {
    /*
    This class stores a function call. Can be evaluated to return
    the return value of a Func stored in the variable map.
    */

    public:
                            TreeCall(TreeNode* func);                                               // Stores a ptr to a node as the name of the TreeCall (runtime error when name isn't an identifier)
                void        setArgs(std::vector<TreeNode*> args);                                   // Sets args vector given a vector of expressions
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;     // Evaluates a Func from the variable map corresponding to name of TreeCall
        virtual void        printInfix(int depth) const;                                            // Prints in format "[func]([args])"
        virtual std::string getID() {                                                               // Will only work on TreeIdentifiers
            throw std::runtime_error("Runtime error: invalid assignee.");
        };
        ~TreeCall() {
            delete func;
            for (TreeNode* child : args) {
                delete child;
            }
        }

    private:
        TreeNode*               func;                                                               // Stored function name (TreeIdentifier or will error when called)
        std::vector<TreeNode*>  args;                                                               // Vector of expression that will return values for the parameters of the Func called
};



class TreeDefinition : public TreeNode {
    /*
    This class stores a function definition. When evaluated it stores
    its information as a Func object in the variable map.
    */

    public:
                            TreeDefinition(std::string name);                                       // Stored a string as the name of the function defined
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;     // Makes a Func object from member variables and stores in variable map under funcName
        virtual void        printInfix(int depth) const;                                            // Prints in format "def [funcName]([params]){[forest]}" with proper newlines and indentation
        virtual std::string getID() {                                                               // Will only work on TreeIdentifiers
            throw std::runtime_error("Runtime error: invalid assignee.");
        }; 

        ~TreeDefinition() {
            // Only deleted forest if it is the only object using the forest
            if(forest.use_count() == 1){
                for (TreeNode* child : *forest) {
                    delete child;
                }
            }
            forest.reset();
        }

        std::string               funcName;                                                         // Name of function defined
        std::vector<std::string>  params;                                                           // Vector of identifier names
        std::shared_ptr<std::vector<TreeNode*>> forest;                                             // shared_ptr to a forest to be based to a Func
};



class TreeStatement : public TreeNode {
    /*
    This class is used to store statement commands: "if", "while", "print", "return", "null", 
    and standalone expressions. This class has a special structure as it is not meant to 
    return any specific value (except for "null") and instead evaluates forests.
    */    

    public:
                            TreeStatement(std::string statement);                                       // Stores command string in node
        virtual variableVal evaluate(std::unordered_map<std::string, variableVal>& vars) const;         // Will call a function based on stateStr or return a NUL type variableVal
                void        evaluateIf(std::unordered_map<std::string, variableVal>& vars) const;       // Evaluates an "if" statement
                void        evaluateWhile(std::unordered_map<std::string, variableVal>& vars) const;    // Evaluates a "while" statement
                void        evaluatePrint(std::unordered_map<std::string, variableVal>& vars) const;    // Evaluates a "print" statement
                void        evaluateReturn(std::unordered_map<std::string, variableVal>& vars) const;   // Evaluates a "return" statement
                void        evaluateExp(std::unordered_map<std::string, variableVal>& vars) const;      // Evaluates a standalone expression
        virtual void        printInfix(int depth) const;                                                // Complicated print to print statement based on stateStr
        virtual std::string getID() {                                                                   // Will only work on TreeIdentifiers
            throw std::runtime_error("Runtime error: invalid assignee.");
        }; 

        ~TreeStatement() {
            delete condition;
            for (TreeNode* child : truths) {
                delete child;
            }
            for (TreeNode* child : falses) {
                delete child;
            }
        }

        std::string             stateStr;                                                           // Stored type of statement as string
        TreeNode*               condition = nullptr;                                                // Tree for condition
        std::vector<TreeNode*>  truths;                                                             // Forest for evaluation when condition is true
        std::vector<TreeNode*>  falses;                                                             // Forest for evaluation when condition is false
};



class TreeArray : public TreeNode {
    /*
    This class is used to store Array Literals.
    It evaluates each individual element in an array, and prints infix.
    */

    public:
                                TreeArray(TreeNode* array, const std::vector<TreeNode*>& elements) : array(array), arrayElements(elements) {};      // Stores an array name (should be nullptr, if array literal) and array elements in vector
        virtual variableVal     evaluate(std::unordered_map<std::string, variableVal>& vars) const;                                                 // Evaluates Tree Array Literal Contents 
        virtual void            printInfix(int depth) const;                                                                                        // Prints Tree Array Literal Infix
        virtual std::string     getID() {                                                                                                           // Will only work on TreeIdentifiers
            throw std::runtime_error("Runtime error: invalid assignee.");
        };
        ~TreeArray() {
            for (auto e : arrayElements) {
                delete e;
            }
            delete array;
        }

        TreeNode* array;                        // name of array (should be nullptr, if array literal)
        std::vector<TreeNode*> arrayElements;   // vector of elements within the array

};



class TreeArrayCall : public TreeNode {
    /*
    This class is used to store and evaluate Array Lookups (variable[array], or [array][index])
    and Array Lookup Assignments. The parser determines which is which, and TreeArrayCall has an optional 
    "assigned" member variable to handle Array Lookup Assignments. 
    This class also has several helper functions used to aid in array indexing and array lookups.
    */

    public:
                                TreeArrayCall(TreeNode* arrayName, TreeNode* index, TreeNode* assigned = nullptr) : arrayName(arrayName), index(index), assigned(assigned) {};  // Stores an array (TreeArray), and its index (TreeIdentifier)
        virtual variableVal     evaluate(std::unordered_map<std::string, variableVal>& vars) const;                             // Evaluates Tree Array Lookups, Checks for Errors
        virtual void            printInfix(int depth) const;                                                                    // Prints Tree Array Lookup Infix
        virtual std::string     getID() {                                                                                       // Will only work on TreeIdentifiers
            throw std::runtime_error("Runtime error: invalid assignee.");
        }; 
        TreeNode* getArrayName() const {
            return arrayName;
        }
        TreeNode* getArrayIndex() const {
            return index;
        }
        void setAssign(const variableVal& value) {
            this->assignValue = value;
        }
        ~TreeArrayCall() {
            delete arrayName;
            delete index;
            delete assigned;
        }

        TreeNode* arrayName; // name of array
        TreeNode* index; // index, in order to access value in array
        TreeNode* assigned; // assigned value, only used if parsing an array lookup + assignment. Default nullptr
        variableVal assignValue; // assigned value used for array lookup assignments 
};


// Helper function to recursively print a nested array
void printArray(const std::shared_ptr<variableVal::Array>& array);


#endif
