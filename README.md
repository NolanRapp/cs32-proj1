# cs32

Part 1: Calculator
    The purpose of the first checkpoint of this project is to build a calculator. The calculator is split into two parts: the lexer (which reads in raw text and translates it to a sequence of tokens), and the parser (which builds an abstract syntax tree, and calculates the input S-Expression).


    The Lexer:
        The Lexer collects standard input in its construction and forms a queue of Token objects that are restricted to being any of the following strings: "(", ")", "+", "-", "/", "*", and a valid number. These are collected to later be used by a class such as the Parser that will evaluate them.
            
            Important Functions:
                Lexer::createEnd():
                    Creates an "END" token for formatting.
                Lexer::printTokens():
                    Prints all tokens from the queue in easy to read column format.
                Lexer::Lexer():
                    Creates a queue of Tokens from standard input. 

    The Parser:
        The Parser collects a queue of tokens (token line, token column, and token text) from the Lexer, and builds an abstract syntax tree using the Parser::parse() function. It is then able to easily calculate the desired S-Expression using the virtual function evaluateNode(). It also constructs and prints the original S-Expression in infix form using the virtual function printInfix(). In main, we run a try-catch block for runtime errors (exit code 3). Otherwise, parse errors (exit code 2) are thrown as they come up in AST building and calculating.
            
            Important Functions:
                Parser::Parser(): 
                    Constructor that initializes the token vector. It checks for initial Parse Errors, then calls createTree() to begin constructing the AST. The head of this tree is assinged to a variable.
                Parser::closedTree():
                    A function that evaluates a given S expression in the context of opening and closing parenthesis.
                Parser::opTree():
                    A function that evaluates a given S expression from the head of it (the Operator).
                Parser::numTree():
                    A function that confirms an S expression starting with a number only consists of that number.
                Parser::parseError():
                    Prints a parse error with corresponding line, column, and text. Exits with exit code 2. 
                
                evaluateNode():
                    Virtual function that's base class is TreeNode, and is inherited by TreeLeaf and TreeOperator. 
                    Returns the evaluated S-Expression.
                
                TreeOperator::printInfix():
                    Virtual function that prints the input S-Expression as infix form. Implements recursion to iterate through children vector to aquire values. Keeps parenthesis in original places.
                    Returns if children vector is empty.
