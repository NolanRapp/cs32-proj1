#include "Parser.h"
#include "AST.h" // redundant


// 
Parser::Parser(std::queue<Token> oInput){
	while(oInput.front().text != "END"){
		createTree(oInput);
	}
}



// Creates AST from the queue of Tokens that will be supplied by the Lexer
void Parser::createTree(std::queue<Token>& input) {
	TreeNode* head;

	if(input.front().text == "("){
		head = closedTree(input);
	}
	else if(isdigit(input.front().text.at(0))){
		head = numTree(input);
	}
	else{
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (First element should not be ")" or "END" or operation)
	}

	if(input.front().text != "END"){
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Initial expression should always end in "END")
	}

	mHeads.push(head);
}



// Evaluates an S expression in the context of "(" and ")", make sure it starts with operator
TreeNode* Parser::closedTree(std::queue<Token>& input){
	input.pop();
	TreeNode* head;

	if(isOp(input.front().text)){
		head = opTree(input);
	}
	else if(input.front().text == "="){
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
	input.pop();

	return head;
}



// Evaluates an S expression from its operator by adding valid children to it on the AST
TreeNode* Parser::opTree(std::queue<Token>& input){

	TreeOperator* op = new TreeOperator(input.front().text.at(0));
	TreeNode* tempExp;
	TreeLeaf* tempLeaf;
	int childNum = 0; // Counter for operands
	input.pop();

	while(isdigit(input.front().text.at(0)) || input.front().text == "("){
		if(input.front().text == "("){
			// Makes a child expression
			tempExp = closedTree(input);
			op->addChild(tempExp);
		}
		else{
			// Creates and adds a child number of the operator
			tempLeaf = new TreeLeaf(std::stold(input.front().text));
			op->addChild(tempLeaf);

			input.pop();
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



//
TreeNode* Parser::assignTree(std::queue<Token>& input){
	
	TreeOperator* assign = new TreeOperator(input.front().text.at(0));
	TreeIdentifier* tempID;
	int childNum = 0;
	input.pop();

	while(isalpha(input.front().text.at(0)) || input.front().text.at(0) == '_'){
		tempID = new TreeIdentifier(input.front());
		assign->addChild(tempID);
		input.pop();

		childNum++;
	}

	if(childNum < 1){
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Assign tree needs atleast 1 identifier)	
	}

	if(isdigit(input.front().text.at(0))){ // Checks for trailing number
		TreeLeaf* tempLeaf = new TreeLeaf(std::stold(input.front().text));
		assign->addChild(tempLeaf);
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



// Makes sure any expression starting with a number only has 1 number
TreeNode* Parser::numTree(std::queue<Token>& input){

	TreeLeaf* leaf = new TreeLeaf(std::stold(input.front().text));
	input.pop();

	if(isdigit(input.front().text.at(0))){
		parseError(input.front().line, input.front().column, input.front().text);
		// Parse Error (Num tree can only have one number)
	}

	return leaf;
}



// Returns head of the AST
TreeNode* Parser::popHead() {
	TreeNode* tempHead = mHeads.front();
	mHeads.pop();

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



