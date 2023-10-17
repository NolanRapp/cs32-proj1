#include "lib/Parser.h"
#include "lib/Lexer.h"


int main() {
	
	TreeNode* ASThead;
 
	try {
		std::queue<Token> tokens;
		
		Lexer lexer;
		lexer.lex();
		Parser parser(lexer.getLexQueue());

		ASThead = parser.getHead();
		ASThead->printInfix();
		std::cout << std::endl;
		double calculation = ASThead->evaluateNode();
		delete ASThead;
		std::cout << calculation << std::endl;
	}

	catch (const std::runtime_error& e) {
		delete ASThead;
		std::cout << e.what() << std::endl;
		exit (3);
	}

   return 0;
};

