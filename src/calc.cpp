#include "lib/New_Parser.h"
#include "lib/Lexer.h"


int main(){ 

    std::deque<Token>                            tokens;    // holds all standard input
    std::unordered_map<std::string, TreeNode::variableVal> variables; // holds all currently assigned variables
    std::unordered_map<std::string, TreeNode::variableVal> tempVars;  // Temporary map to store variables in (will be deleted if runtime_error)
    
    std::string line;       // string to hold a line of user input
    New_Parser  infix;      // parser object for parsing lines
    double      boolDouble; // stores evaluated value (will store bools as 0 or 1)
        
    // Parses line by line (errors will not interrupt program)
    while(getline(std::cin, line)) {
        std::unique_ptr<TreeNode> rootTree;
        tempVars = variables;

        try {
            Lexer lexer;
            lexer.lex(line);
            std::deque<Token> tokenizedQ = lexer.getLexQueue();

            rootTree.reset(infix.parseForCalc(tokenizedQ));
            ReturnType returnType = rootTree->type(tempVars); // determines the evaluation function to use

            rootTree->printInfix(0); // prints parsed expression
            std::cout << std::endl;

            // attempts to evaluate expression
            if (returnType == ReturnType::NUM) {
                std::cout << rootTree->evalDouble(tempVars) << std::endl;
            }
            else if (returnType == ReturnType::BOOL) {
                boolDouble = rootTree->evalBool(tempVars);
                if (boolDouble) {
                    std::cout << "true" << std::endl;
                }
                else {
                    std::cout << "false" << std::endl;
                }
            }
            
            variables = tempVars; // if evaluated correctly the variable map is updated
        }
        catch(const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
    }
};
