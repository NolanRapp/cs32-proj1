#include "Parser.h"
#include "AST.h" // redundant


// Feeds an entire queue made from a single Lexer and outputs multiple ASTs
Parser::Parser(std::deque<Token> oInput){
	while(oInput.front().text != "END"){
		createTree(oInput);
	}
}



// Creates AST from the queue of Tokens that will be supplied by the Lexer
void Parser::createTree(std::deque<Token>& input) {
	TreeNode* head;

	if(input.front().text == "("){ // Checks for expression
		head = closedTree(input);
	}
	else if(isdigit(input.front().text.at(0))){ // Checks for single number
		TreeLeaf* numTree = new TreeLeaf(std::stold(input.front().text));
		head = numTree;

		input.pop_front();
	}
	else if(isalpha(input.front().text.at(0)) || input.front().text.at(0) == '_'){ // Checks for single ID
		TreeIdentifier* idTree = new TreeIdentifier(input.front().text);
		head = idTree;

		input.pop_front();
	}
	else{
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (First element should not be ")" or "END" or operation)
	}

	mHeads.push_back(head);
}



// Evaluates an S expression in the context of "(" and ")", make sure it starts with operator
TreeNode* Parser::closedTree(std::deque<Token>& input){
	input.pop_front();
	TreeNode* head;

	if(isOp(input.front().text)){ // Checks for conventional operator tree
		head = opTree(input);
	}
	else if(input.front().text == "="){ // Checks for assignment tree
		head = assignTree(input);
	}
	else{	
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Invalid start for closed tree: 0,"(",")", and "END")
	}

	if(input.front().text != ")"){
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Closed trees should end in ")")
	}
	input.pop_front();

	return head;
}



// Evaluates an S expression from its operator by adding valid children to it on the AST
TreeNode* Parser::opTree(std::deque<Token>& input){

	TreeOperator* op = new TreeOperator(input.front().text.at(0));
	TreeNode* 		tempExp;
	TreeLeaf* 		tempLeaf;
	TreeIdentifier* tempID;
	int childNum = 0; // Counter for operands
	input.pop_front();

	while(isdigit(input.front().text.at(0)) || input.front().text == "(" || isalpha(input.front().text.at(0)) || input.front().text.at(0) == '_'){
		if(input.front().text == "("){
			// Makes a child expression
			tempExp = closedTree(input);
			op->addChild(tempExp);
		}
		else if (isalpha(input.front().text.at(0)) || input.front().text.at(0) == '_'){
			tempID = new TreeIdentifier(input.front().text);
			op->addChild(tempID);

			input.pop_front();
		}
		else{
			// Creates and adds a child number of the operator
			tempLeaf = new TreeLeaf(std::stold(input.front().text));
			op->addChild(tempLeaf);

			input.pop_front();
		}

		if(input.front().text == "END"){
			break;
		}

		childNum++;
	}

	if(childNum < 1){
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Operation tree needs atleast 1 children)
	}

	return op;
}



// Evaluates an assignment expression, confirms conditions for valid expression
TreeNode* Parser::assignTree(std::deque<Token>& input){
	
	TreeOperator* assign = new TreeOperator(input.front().text.at(0));
	TreeIdentifier* tempID;
	int childNum = 0;
	input.pop_front();

	// Adds all IDs
	while(isalpha(input.front().text.at(0)) || input.front().text.at(0) == '_'){
		tempID = new TreeIdentifier(input.front().text);
		assign->addChild(tempID);
		input.pop_front();

		childNum++;
	}

	if(childNum < 1){
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Assign tree needs atleast 1 identifier)	
	}

	if(isdigit(input.front().text.at(0))){ // Checks for trailing number
		TreeLeaf* tempLeaf = new TreeLeaf(std::stold(input.front().text));
		assign->addChild(tempLeaf);

		input.pop_front();
	}
	else if(input.front().text == "("){ // Checks for tailing expression
		TreeNode* tempExp = closedTree(input);
		assign->addChild(tempExp);
	}
	else if(childNum < 2){
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (If no trailing number or expression there should be atleast 2 identifiers)
	}

	if(input.front().text != ")"){
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Assign tree should only have ")" after number or expression)
	}

	return assign;
}



// Returns head of the AST
TreeNode* Parser::popHead() {
	TreeNode* tempHead = mHeads.front();
	mHeads.pop_front();

	return tempHead;
}



// Helper for printing a specific Parse Error
void Parser::parseError(int line, int col, std::string text) const {
	std::cout << "Unexpected token at line " << line << " column "
	<< col << ": " << text << std::endl;
	exit(2);
}



// Helper to make code more readable
bool Parser::isOp(std::string str) const{
	return (operators.find(str) != operators.end());
}



// Returns true when there are no ASTs in the Parser
bool Parser::isEmpty() const{
	return mHeads.empty();
}



