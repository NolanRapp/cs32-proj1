# cs32
<<<<<<< HEAD

Part 1: Calculator
    The purpose of the first checkpoint of this project is to build a calculator. The calculator is split into two parts: the lexer (which reads in raw text and translates it to a sequence of tokens), and the parser (which builds an abstract syntax tree, and calculates the input S-Expression).


    The Lexer:
        ADD INFO ABOUT LEXER HERE


    The Parser:
        The parser collects a queue of tokens (token line, token column, and token text) from the Lexer, and builds an abstract syntax tree using the Parser::parse() function. It is then able to easily calculate the desired S-Expression using the virtual function evaluateNode(). It also constructs and prints the original S-Expression in infix form using the virtual function printInfix(). In main, we run a try-catch block for runtime errors (exit code 3). Otherwise, parse errors (exit code 2) are thrown as they come up in AST building and calculating.
            
            Important Functions:
                Parser::Parser(): 
                    Constructor that initializes the token vector. It checks for initial Parse Errors, then calls createTree() to begin constructing the AST. The head of this tree is assinged to a variable.
                Parser::createTree():
                    Utilizes recursion to create AST. As tree is constructed, items are popped off the queue. Checks for Parse Errors along the way.
                    Returns an AST of type TreeNode*.
                Parser::parseError():
                    Prints a parse error with corresponding line, column, and text. Exits with exit code 2. 
                
                evaluateNode():
                    Virtual function that's base class is TreeNode, and is inherited by TreeLeaf and TreeOperator. 
                    Returns the evaluated S-Expression.
                
                TreeOperator::printInfix():
                    Virtual function that prints the input S-Expression as infix form. Implements recursion to iterate through children vector to aquire values. Keeps parenthesis in original places.
                    Returns if children vector is empty.
                
                
=======
>>>>>>> 41320212d03411d4604b587788af617baf0471a1
