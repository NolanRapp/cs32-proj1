# cs32

Authors:
    Jesse Cobb, Chloe Andersen, Nolan Rapp, Christy Yu

Make Commands:
    Lexer Parser New_Parser clean


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


Part 2: Persistence
    The purpose of the second checkpoint for this project is to integrate variables and infix input for the previously constructed calculator. This will allow for assignment and use of variables as well as the capability for either S expressions or infix expressions to be deciphered by the program. This checkpoint will also tackle the ability to evaluate multiple expression at once.

    Variables:
        The Lexer now supports a new operation, "=", which when incorporated into ASTs will assign a numerical value to variables. Variables are also supported in the Parser and Lexer with the only restriction that the variable names are limited to letters or underscores ("_"). The ability to now create multiple expressions and remember variables across them has also been integrated.

            Important Additions:
                TreeIdentifier:
                    This new class inherits from TreeNode similar to TreeOperator and TreeLeaf but carries an important new version of evaluateNode() which will check if a value has been assigned to the current ID.
                evaluateNode():
                    This function now takes a hash map of variables so that variables can easily be assigned values through this map or checked to see if an ID has been mapped to a value.
                Parser::assignTree():
                    This function works similarly to Parser::opTree() with some unique logic requiring all IDs aside from the final child which can be an expression, ID, or number.
                Parser::numTree() removed:
                    This function was only required when only single expression were allowed. Now with multiple expressions the logic is simpler for handling single numbers so it was removed.           

    Infix Notation:
        The New_Parser now parses expressions in infix notation, supporting equations in order of operations. The program uses the Recursive Descent Parser logic to parse the expression, building the AST top-down. New_Parser also handles variable assignment and usage within expressions.
            
            Important Additions:
                Parse:
                    This is the base function for parsing an expression or assignment. It looks at the current token and the token ahead of it to decide which parsing function to enter. It throws parse errors from Error.h.
                    
                    ParseE and ParseT:
                        Parses an expression (+ or -), or parses a term (* or /)
                    ParseF:
                        Parses factors (integers and identifiers)
                    ParseA:
                        Parses assignments, and recursively calls itself if there are many / nested instances of assignments.
                
                scanToken:
                    This implements std::deque to assign the current token to a variable called nextToken, and peek ahead at the next token.


Part 3: Conditionals
    The purpose of the third checkpoint is to implement booleans into our current parsing and evaluating logic in order to support statements in our language. The goal is to be able to support statements like "if" and "while" that evaluate to boolean true or false.

        Booleans:
            A new data type, booleans, are now supported in our program. Boolean-returning or boolean evaluating operations, like equality/inequality, comparison, logical and/exclusive/inclusive, and boolean assignment are now supported. This required updating New_Parser.cpp, AST.cpp, Token.h and Lexer.cpp to recognize these new operators and be able to handle boolean evaluations.

                Important Additions: 
                    New_Parser:
                        New operators were added to the Recursive Descent Parser to agree with the precedence order defined on the F23 Github page. These included comparison, equality, and logical operators. Assignments were also updated to handle variable assignments to booleans as well as numbers. 
                        parseForCalc() function was added to handle parsing for specifically calc.cpp, for Track A of Scrypt 03.
                    AST:
                        The AST TreeNodes were modified slightly to handle boolean and double evaluations seperately. The type of token is determined when constructing the deque in the lexer, and this type determines which evaluation to enter when constructing the AST (double or bool). Logic for evaulating these new operators was added. Additionally, a new TreeAssign class was created to specially handle assignment nodes, seperately from operator nodes.
                    Token:
                        The token class was expanded to include the type of token (enum class Type) when the constructor is called from the lexer. This made AST and New_Parser more readable and convenient.


        Statements: 
                    


        
