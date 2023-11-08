#include "lib/Lexer.h"
#include "lib/Token.h"


int main() {

    // Reads all standard input
    char inChar;
    std::string totalString = "\0";
    while(std::cin.get(inChar)){
        totalString += inChar;
    }

    // Creates Lexer with standard input and outputs in requested format
    Lexer lexer;
    try {
        lexer.lex(totalString);
    }
    catch(const LexError& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    lexer.printTokens();

    return 0;
}
