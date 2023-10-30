#include "lib/New_Parser.h"
#include "lib/Lexer.h"


int main() {

	std::deque<Token> tokens; // holds all standard input
	std::unordered_map<std::string, double> variables; // holds all currently assigned variables
	std::string line;
	New_Parser infix;
		
	while(getline(std::cin, line)) {
		try {
			Lexer lexer;
			lexer.lex(line);
			std::deque<Token> tokenizedQ = lexer.getLexQueue();
			TreeNode* rootTree = infix.parse(tokenizedQ, variables);

			rootTree->printInfix();
			std::cout << std::endl;
			std::cout << rootTree->evaluateNode(variables) << std::endl;
			delete rootTree;
		}
		catch(const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
		}
	}
};