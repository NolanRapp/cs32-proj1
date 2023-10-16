#include "lib/Parser.h"
#include "lib/Lexer.h"


int main() {
    
	try {
		std::queue<token> tokens;
		
		Lexer lexer;
		lexer.lex();
		Parser parser(lexer.getLexQueue());

		TreeNode* ASThead = parser.getHead();
		ASThead->printInfix();
		std::cout << std::endl;
		double calculation = ASThead->evaluateNode();
		delete ASThead;
		std::cout << calculation << std::endl;

	}

	catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
		exit (3);
	}

   return 0;
};

