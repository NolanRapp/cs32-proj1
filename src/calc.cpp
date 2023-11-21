#include "lib/New_Parser.h"
#include "lib/Lexer.h"


int main(){ 

    std::deque<Token>                            tokens;    // holds all standard input
    std::unordered_map<std::string, variableVal> variables; // holds all currently assigned variables
    std::unordered_map<std::string, variableVal> tempVars;  // Temporary map to store variables in (will be deleted if runtime_error)
    
    std::string line;       // string to hold a line of user input
    New_Parser  infix;      // parser object for parsing lines
        
    // Parses line by line (errors will not interrupt program)
    while(getline(std::cin, line)) {
        std::unique_ptr<TreeNode> rootTree;
        tempVars = variables;

        try {
            Lexer lexer;
            lexer.lex(line);
            std::deque<Token> tokenizedQ = lexer.getLexQueue();

            rootTree.reset(infix.parseForCalc(tokenizedQ));

            rootTree->printInfix(0); // prints parsed expression
            std::cout << std::endl;

            // attempts to evaluate expression
            variableVal treeVal(rootTree->evaluate(tempVars));
            if (treeVal.type == ReturnType::NUM) {
                std::cout << std::get<double>(treeVal.value) << std::endl;
            }
            else if (treeVal.type == ReturnType::BOOL) {
                if (std::get<bool>(treeVal.value)) {
                    std::cout << "true" << std::endl;
                }
                else {
                    std::cout << "false" << std::endl;
                }
            }
            else if (treeVal.type == ReturnType::ARRAY) {
                auto array = std::get<std::shared_ptr<variableVal::Array>>(treeVal.value);
                std::cout << "[";
                for (size_t i = 0; i < array->elements.size(); ++i) {
                    if (array->elements[i].type == ReturnType::NUM) {
                        std::cout << std::get<double>(array->elements[i].value);
                    } 
                    else if (array->elements[i].type == ReturnType::BOOL) {
                        if (std::get<bool>(treeVal.value)) {
                            std::cout << "true" << std::endl;
                        }
                        else {
                            std::cout << "false" << std::endl;
                        }
                    } 
                    if (i < array->elements.size() - 1) {
                        std::cout << ", ";
                    }
                }
                std::cout << "]" << std::endl;
            }
            
            variables = tempVars; // if evaluated correctly the variable map is updated
        }
        catch(const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
    }
}



