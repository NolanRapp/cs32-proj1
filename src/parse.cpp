#include "lib/parse.h"
#include "lib/lex.h"

TreeLeaf::TreeLeaf (double val) {
    value = val; // initializing one numerical
}

double TreeLeaf::evaluateNode() const{
    return value;
}



TreeOperator::~TreeOperator{   
	for(int i = 0; i < children.size(); i++){
		TreeNode* tempPtr = children[i];
		children [i] = nullptr;

		delete tempPtr;
	}
}

TreeOperator::TreeOperator(char operation) {
    operation = operation;
    childrenQ = new vector<TreeNode*>; 

    // initializing operation used and vector for its operands and child operators
}

void TreeOperator::addChild(TreeNode* child){
	childrenQ.push(child);
}

double TreeOperator::evaluateNode() const{
    // the purpose of this function is to return the evaluated operation
    // from all children of this operation in the AST
	
	// we can assume children vector is nonempty
	
	double result = children[0]->evaluateNode();

	switch(operation){
	case '*':
		for(int i = 1; i < children.size(); i++){
			result *= children[i]->evaluateNode();
		}
		break;
	case "/":
		for(int i = 1; i < children.size(); i++){
			if (children[i]->evaluateNode() == 0){
				throw std::runtime_error("Runtime error: division by zero.");	
			}

			result /= children[i]->evaluateNode();
		}
		break;
	case "+":
		for(int i = 1; i < children.size(); i++){
			result += children[i]->evaluateNode();
		}
		break;
	case "-":
		for(int i = 1; i < children.size(); i++){
			result -= children[i]->evaluateNode();
		}
		break;
	}

    return result;
}



Parser::Parser() {
    // nothing to do here yet
    // eventually: we want this constructor to initialize the vector
    //              of tokens created by lexer
}

TreeNode* Parser::parse(const std::vector<token>& input) {
    /*
    todo:   function is supposed to parse tokens / construct AST
            function will call above functions to then execute the operation
            ** use recursion? 
    */ 
    
	

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
                variable of type TreeNode? TreeNode*?
            print AST in in "infix form" (maybe write another function for this?) 
            print the value of the expression 
            
            otherwise, print error messages
                - lexer error: print the same error message as the lex program
                - parse error: print an error message with exit code 2
                - division by zero error while evaluating AST: print "Runtime Error: division by zero" and exit with exit code 3
            delete AST TreeNode* variable to keep memory clean
    
    */

   return 0;
};
