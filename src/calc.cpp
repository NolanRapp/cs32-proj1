#include "lib/New_Parser.h"
#include "lib/Lexer.h"


int main() {

	std::deque<Token> tokens; // holds all standard input
	std::unordered_map<std::string, double> variables; // holds all currently assigned variables
	std::unordered_map<std::string, double> tempVars; // Temporary map to store variables in (will be deleted if runtime_error)
	std::string line;
	New_Parser infix;
		
	while(getline(std::cin, line)) {
		std::unique_ptr<TreeNode> rootTree;
		tempVars = variables;

		try {
			Lexer lexer;
			lexer.lex(line);
			std::deque<Token> tokenizedQ = lexer.getLexQueue();

			rootTree.reset(infix.parse(tokenizedQ, tempVars));

			rootTree->printInfix();
			std::cout << std::endl;

			std::cout << rootTree->evaluateNode(tempVars) << std::endl;
			
			variables = tempVars;
		}
		catch(const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
		}
	}
};
