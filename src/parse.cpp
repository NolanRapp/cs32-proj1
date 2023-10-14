#include "lib/parse.h"
#include "lib/lex.h"

TreeLeaf::TreeLeaf (char val) {
    value = val; // initializing one numerical
}

double TreeLeaf::evaluateNode() const{
    return value;
}



/*TreeOperator::~TreeOperator {   
	for (int i = 0; i < children.size(); i++) {
		TreeNode* tempPtr = children[i];
		children[i] = nullptr;

		delete tempPtr;
	}
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

	switch(operation){
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
        // Parse Error (Expects "()" for S expression)
    }
    else {
        originalInput.pop();
		if(operators.find(originalInput.front().text) == operators.end()){
			// Parse Error (Expects Operator)
		}
        mHead = createTree(originalInput);
    }
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
				// Parse Error (Expects only numbers and new S expressions)
			}

			// Checks for new expression
			if(input.front().text == "(") {
				input.pop();
				if(operators.find(input.front().text) == operators.end()){
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



int main() {
    std::string input;
    std::getline(std::cin, input);
    // reading input into a string
    
    /* 
    Lexer lexer(input);
    not sure the implementation of lexer yet
    but place here: lexer and input as tokens in a vector
    std::vector<Token> tokens;

    Parser parser(tokens);


   TODO:    build AST using parser.parse() function 
            print AST in in "infix form" (maybe write another function for this?) 
            print the value of the expression 
            
            otherwise, print error messages
                - lexer error: print the same error message as the lex program
                - parse error: print an error message with exit code 2
                - division by zero error while evaluating AST: print "Runtime Error: division by zero" and exit with exit code 3
            delete AST TreeNode* variable to keep memory clean

            ERROR MESSAGES ... implement 
    
    */

   return 0;
};
