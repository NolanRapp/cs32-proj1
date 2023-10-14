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
        // throw error 
    }
    else {
        originalInput.pop();
        mHead = createTree(originalInput);
    }

    // note: doesn't handle case of first token being just a number 
}

TreeNode* Parser::createTree(std::queue<token>& input) {
    /*
    The createTree function parses input tokens in a queue and constructs AST
    Recursion is used to simplify the tree build
    */

    if (input.empty()) {
        // inital check if input is empty
        // throw error here
    }

    TreeOperator* head; 

    if (input.front().text == "+" || input.front().text == "-" || input.front().text == "*" || input.front().text == "/") {
        head = new TreeOperator((input.front().text).at(0));
        input.pop();

        while (input.front().text != ")") {
            // is this the right sign parenthesis?
            head->addChild(createTree(input));
        }
        input.pop();
        return head;
        }

    else if (isNum(input.front().text)) {
        TreeLeaf* leaf = new TreeLeaf(input.front().text.at(0));
        input.pop();
        return leaf;
        // cast head as a double
        // number
    }

    /*else if ((input.front().text == ")") || (input.front().text == "(") || (input.front().text == "END")) {
        // throw parsing error 
        // add in opening parenthesis to this?
    }*/

    else {
        // commented out above errors, not sure if we need to include them as specific else if statements
        // throw error here 
    }

    return nullptr;
}

bool Parser::isNum(std::string tokenValue) {
    if (isdigit(tokenValue.at(0))) {
        return true;
    }
    else {
        return false;
    }
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
