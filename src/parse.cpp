#include "lib/Parser.h"
#include "lib/Lexer.h"


int main() {

	// Reads all standard input
	char inChar;
	std::string totalString = "\0";
	while(std::cin.get(inChar)){
		totalString += inChar;
	}
	
	std::queue<Token> tokens; // Holds all standard input
	std::unordered_map<std::string, double> variables; // Holds all currently assigned variables

	// Creates queue by reading user input and feeds into a parser
	Lexer lexer;
	lexer.lex(totalString);
	Parser parser(lexer.getLexQueue());

	while(!parser.isEmpty()){
		// Evaluates current AST
		TreeNode* ASThead = parser.popHead();
		ASThead->printInfix();
		std::cout << std::endl;

		try {
			double calculation = ASThead->evaluateNode(variables);
			delete ASThead;
			std::cout << calculation << std::endl;
		}
		catch (const std::runtime_error& e) {
			delete ASThead;
			std::cout << e.what() << std::endl;
			return 3;
		}
	}
	
	return 0;
};



