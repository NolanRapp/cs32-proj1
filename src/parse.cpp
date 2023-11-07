#include "lib/Parser.h"
#include "lib/Lexer.h"


int main() {

	// Reads all standard input
	char inChar;
	std::string totalString = "\0";
	while(std::cin.get(inChar)){
		totalString += inChar;
	}
	
	std::deque<Token> tokens; // Holds all standard input
	std::unordered_map<std::string, double> variables; // Holds all currently assigned variables

	// Creates deque by reading user input and feeds into a parser
	Lexer lexer;

	try { // Attempts to lex input
		lexer.lex(totalString);
	}
	catch(const LexError& e){
		std::cout << e.what() << std::endl;
		return 1;
	}

	Parser parser;
	try { // Attempts to parse queue
		parser.createForest(lexer.getLexQueue());
	}
	catch(const ParseError& e){
		std::cout << e.what() << std::endl;
		return 2;
	}

	while(!parser.isEmpty()){
		// Evaluates current AST
		TreeNode* ASThead = parser.popHead();
		ASThead->printInfix();
		std::cout << std::endl;

		try { // Attempts to evaluate AST in forest
			double calculation = ASThead->evalDouble(variables);
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



