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
    char inChar;
	std::string totalString = "\0";
	while(std::cin.get(inChar)){
		totalString += inChar;
	}
	
	std::queue<Token> tokens;

	// Creates queue by reading user input
	Lexer lexer;
	lexer.lex(totalString);
	New_Parser new_parser(lexer.getLexQueue());

    std::unique_ptr<TreeNode> ast = new_parser.getHead();


    // std::unique_ptr<TreeNode> TreeHead = new_parser.getHead(); ----> haven't implemented this helper function yet


};
