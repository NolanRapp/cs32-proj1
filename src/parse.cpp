#include "lib/parse.h"
#include "lib/lex.h"

TreeLeaf::TreeLeaf (double val) {
    value = val; // initializing one numerical
}

double TreeLeaf::evaluateNode() const{
    return value;
}



/*~TreeOperator {   
	for (TreeNode* child : children) {
		delete child;
	}
	children.clear();
}*/

TreeOperator::TreeOperator(char operation) {
    this->operation = operation;

    // initializing operation used and vector for its operands and child operators
}

void TreeOperator::addChild(TreeNode* child){
	children.push_back(child);
}

double TreeOperator::evaluateNode() const{
    // the purpose of this function is to return the evaluated operation
    // from all children of this operation in the AST
	
	// we can assume children vector is nonempty

	double result = children[0]->evaluateNode();

	switch(operation) {
	case '*':
		for (unsigned int i = 1; i < children.size(); i++) {
			result *= children[i]->evaluateNode();
		}
		break;
	case '/':
		for (unsigned int i = 1; i < children.size(); i++) {
			if (children[i]->evaluateNode() == 0){
				throw std::runtime_error("Runtime error: division by zero.");	
			}

			result /= children[i]->evaluateNode();
		}
		break;
	case '+':
		for (unsigned int i = 1; i < children.size(); i++) {
			result += children[i]->evaluateNode();
		}
		break;
	case '-':
		for (unsigned int i = 1; i < children.size(); i++) {
			result -= children[i]->evaluateNode();
		}
		break;
	}

    return result;
}



Parser::Parser(std::queue<token> originalInput) {
    // initialize and return the head of the created AST 

    if (originalInput.front().text != "(") {
        std::cout << "Unexpected token at line" << originalInput.front().line << "column"
		<< originalInput.front().column << ": " << originalInput.front().text << std::endl;
		exit(2);
		// Parse Error (Expects "()" for S expression)
    }
    else {
        originalInput.pop();
		if(operators.find(originalInput.front().text) == operators.end()){
			std::cout << "Unexpected token at line" << originalInput.front().line << "column"
			<< originalInput.front().column << ": " << originalInput.front().text << std::endl;
			exit(2);
			// Parse Error (Expects Operator)
		}
        mHead = createTree(originalInput);
    }

	// ADD Parse Error here to check input doesn't contain multiple (or no) top-level S expressions
}

TreeNode* Parser::createTree(std::queue<token>& input) {
    /*
    The createTree function parses input tokens in a queue and constructs AST
    Recursion is used to simplify the tree build
    */

    TreeOperator* head;
	TreeLeaf* leaf;

    if (operators.find(input.front().text) != operators.end()) {
       	head = new TreeOperator(input.front().text.at(0));
        input.pop();

        while (input.front().text != ")") {
			
			// Makes sure only numbers or new expressions are registered
	        if(operators.find(input.front().text) != operators.end() || input.front().text == "END"){
				std::cout << "Unexpected token at line" << input.front().line << "column"
				<< input.front().column << ": " << input.front().text << std::endl;
				exit(2);
				// Parse Error (Expects only numbers and new S expressions)
			}

			// Checks for new expression
			if(input.front().text == "(") {
				input.pop();
				if(operators.find(input.front().text) == operators.end()){
					std::cout << "Unexpected token at line" << input.front().line << "column"
					<< input.front().column << ": " << input.front().text << std::endl;
					exit(2);
					// Parse Error (Expects Operator)
				}
			}

			head->addChild(createTree(input));
        }

        input.pop();
		return head;
	}
	// Ideally speaking the only alternative case is a number
    else {
        leaf = new TreeLeaf(std::stold(input.front().text));
        input.pop();
		return leaf;
    }

    return nullptr;
}

TreeNode* Parser::getHead() {
	return mHead;
}



int main() {
    
	try {
		/*
		TO IMPLEMENT WHEN LEXER IS READY: 

		std::string sExpression;
		char ch;
		while (std::cin.get(ch) && ch != '\n') {
			sExpression.push_back(ch);
		}
		Lexer lexer(sExpression);
		*/

		std::queue<token> tokens;
			// eventually want to SET TOKENS EQUAL TO lexer function that fills this queue

		// manual token push for parser testing:
			tokens.push(token(1, 1, "("));
        	tokens.push(token(1, 2, "*"));
			tokens.push(token(1, 3, "("));
			tokens.push(token(1, 4, "+"));
			tokens.push(token(1, 5, "1"));
			tokens.push(token(1, 6, "2"));
			tokens.push(token(1, 7, ")"));
			tokens.push(token(1, 1, "3"));
			tokens.push(token(1, 2, "("));
			tokens.push(token(1, 3, "/"));
			tokens.push(token(1, 4, "4"));
			tokens.push(token(1, 5, "5"));
			tokens.push(token(1, 6, "("));
			tokens.push(token(1, 7, "-"));
			tokens.push(token(1, 8, "6"));
			tokens.push(token(1, 9, "7"));
			tokens.push(token(1, 10, ")"));
			tokens.push(token(1, 11, ")"));
			tokens.push(token(1, 12, ")"));
		
		Parser parser(tokens);
		TreeNode* ASThead = parser.getHead();
		// Parse Error here? if "no / multiple top level s-expressions" found
		// TODO: print in infix form
		double calculation = ASThead->evaluateNode();
		std::cout << "calculation: " << calculation << std::endl;

	}

	catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
		exit(3);
	}

   return 0;
};


 /* 
TODO:
	- lexer error: print the same error message as the lex program
	- delete AST TreeNode* variable to keep memory clean
	- TreeOperator destructor? getting weird errors with that
	- run tests and make sure we handle cases properly
*/
