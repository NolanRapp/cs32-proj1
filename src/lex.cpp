#include "lib/Lexer.h"
#include "lib/Token.h"


int main() {

	char tempChar;
	std::string totalString = "\0";
	while(std::cin.get(tempChar)){
		totalString += tempChar;
	}

    Lexer lexer;
    lexer.lex(totalString);
    lexer.printTokens();

    return 0;
}
