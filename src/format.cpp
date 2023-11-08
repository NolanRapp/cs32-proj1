
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

    // Parses all input to make a forest of the program
    StateParser parser;
    try { // Attempts to parse queue
        parser.createForest(lexer.getLexQueue());
    }
    catch(const ParseError& e){
        std::cout << e.what() << std::endl;
        return 2;
    }
    
    // Evaluates forest
    while(!parser.isEmpty()){
        TreeNode* ASThead = parser.popHead();

        ASThead->printInfix(0);
        std::cout << '\n';
        delete ASThead;
    }

    return 0;
};
