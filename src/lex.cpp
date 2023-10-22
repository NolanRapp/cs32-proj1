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
    lexer.lex(totalString);
    lexer.printTokens();

    return 0;
}
