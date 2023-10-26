#include "lib/New_Parser.h"
#include "lib/Lexer.h"


int main() {

	std::queue<Token> tokens; // holds all standard input
	std::unordered_map<std::string, double> variables; // holds all currently assigned variables
	std::string line;
	New_Parser infix;

	while(getline(std::cin, line)) {
		Lexer lexer;
		lexer.lex(line);

		infix.addTree(lexer.getLexQueue());
	}

	while(!infix.isEmpty()){
		TreeNode* ASThead = infix.popHead();
		ASThead->printInfix();
		std::cout << std::endl;

		try{
			double result = ASThead->evaluateNode(variables);
			std::cout << result << std::endl;
			delete ASThead;
		}
		catch(const std::exception& e){
			delete ASThead;
			std::cout << e.what() << std::endl;
			return 3;
		}
	}

	return 0;
}
