# cs32

Part 1: Calculator
    The purpose of the first part of this project is to build a calculator. The calculator is split into two parts: the lexer (which reads in raw text and translates it to a sequence of tokens), and the parser (which builds an abstract syntax tree, representing those tokens as the desired operations).


    The Lexer:
        ADD INFO ABOUT LEXER HERE


    The Parser:
        The parser reads in a vector of tokens from the Lexer, and builds an abstract syntax tree using the Parser::parse() function. It then is able to easily preform the operations desired by the input!
            
            Functions:
                (STILL WORKING..) Parser::Parser(): constructor that initializes the token vector
                
                (STILL WORKING..) TreeNode* Parser::parse(): parses the token vector, and constructs an abstract syntax tree. It then uses other functions within the Parse.cpp file to preform the desired calculation.

                (STILL WORKING..) TreeNode::evaluateNode(): of the class TreeNode. BASE Virtual function (utilizing inheritance) to initialize the input to 0.0, before getting later sorted in other classes.

                (STILL WORKING..) TreeLeaf::evaluateNode(): assigns value collected by TreeNode::evaluateNode()

                (STILL WORKING..) TreeOperator::TreeOperator: assigns the proper left and right children (values) to the operator.

                (STILL WORKING..) TreeOperator::evaluateNode(): returns the value of the operation between the left child, operator, and right child. 
                
                (STILL WORKING..) TreeOperator::~TreeOperator(): deletes the right and left variables, as these are type TreeNode* and could mess with memory.


    Error Messages: