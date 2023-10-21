#include "lib/New_Parser.h"
#include "lib/Lexer.h"


int main() {

	/* use parse.cpp as an example 

    FROM DIRECTIONS:
    
    Then create a new main() function in src/calc.cpp:

This program should read standard input one line at a time. Each line should contain exactly one infix expression.
After reading a line, your program should lex it into tokens.
It should then parse those tokens into an AST.
It should then print the AST in infix form, as in Part 1 (print parentheses around every operation).
It should then evaluate the AST and print the result.
When it reaches the end of its input, it should exit with exit code zero.

*/

try{
	std::string line;
	
	while(getline(std::cin, line)) {
		Lexer lexer;
		lexer.lex(line);
		New_Parser new_parser(lexer.getLexQueue());
		new_parser.getHead();
		if (new_parser.getHead() != nullptr) {
    		new_parser.printInfix();
		} 		
		else {
    		std::cerr << "Tree head is nullptr, something went wrong in parsing." << std::endl;
    		exit(3);
}
		new_parser.printInfix();
	}
}
catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
}

    // TreeNode* TreeHead = new_parser.getHead(); ----> haven't implemented this helper function yet


};
