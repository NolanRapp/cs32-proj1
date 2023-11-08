#include "lib/StateParser.h"
#include "lib/New_Parser.h"
#include "lib/Lexer.h"


int main() {

    // Reads all standard input
    char inChar;
    std::string totalString = "\0";
    while(std::cin.get(inChar)){
        totalString += inChar;
    }

    // Creates deque by reading user input and feeds into a parser
    Lexer lexer;
    try { // Attempts to lex input
        lexer.lex(totalString);
    }
    catch(const LexError& e){
        std::cout << e.what() << std::endl;
        return 1;
    }

    StateParser parser;
    try { // Attempts to parse queue
        parser.createForest(lexer.getLexQueue());
    }
    catch(const ParseError& e){
        std::cout << e.what() << std::endl;
        return 2;
    }

    std::unordered_map<std::string, variableVal> vars; // holds all currently assigned variables
        
    while(!parser.isEmpty()){
        // Evaluates current AST
        TreeNode* ASThead = parser.popHead();

        try { // Attempts to evaluate AST in forest
            if(ASThead->type(vars) == ReturnType::NUM){
                ASThead->evalDouble(vars);
            }
            else{
                ASThead->evalBool(vars);
            }
            delete ASThead;
        }
        catch (const std::runtime_error& e) {
            delete ASThead;
            std::cout << e.what() << std::endl;
            return 3;
        }
    }


    return 0;
};
