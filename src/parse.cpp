#include "lib/Parser.h"
#include "lib/Lexer.h"


int main() {
	
	std::queue<Token> tokens;

	Lexer lexer;
	lexer.lex();
	Parser parser(lexer.getLexQueue());

	TreeNode* ASThead = parser.getHead();
	ASThead->printInfix();
	std::cout << std::endl;

	try {
		double calculation = ASThead->evaluateNode();
		delete ASThead;
		std::cout << calculation << std::endl;
	}

	catch (const std::runtime_error& e) {
		delete ASThead;
		std::cout << e.what() << std::endl;
		return 3;
	}

   return 0;
};

