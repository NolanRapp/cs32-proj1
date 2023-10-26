#include "lib/New_Parser.h"
#include "lib/Lexer.h"


int main() {

	std::queue<Token> tokens; // holds all standard input
	std::unordered_map<std::string, double> variables; // holds all currently assigned variables
	std::string line;
	New_Parser infix;
	

try {	
	while(getline(std::cin, line)) {
		Lexer lexer;
		lexer.lex(line);
		std::queue<Token> tokenizedQ = lexer.getLexQueue();
		TreeNode* rootTree = infix.parse(tokenizedQ);

		rootTree->printInfix();
		std::cout << std::endl;

		double result = rootTree->evaluateNode(variables);
		std::cout << result << std::endl;
	}
}
catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
}

};
