#ifndef STATEPARSER_H
#define STATEPARSER_H

#include "New_Parser.h"
#include "Lexer.h"
#include "Token.h"
#include "AST.h"
#include "Errors.h"

#include <deque>
#include <memory>


class StateParser {

public:
    /*
    This class is an extension of New_Parser that parses statement (including expression)
    that give commands rather than simply evaluating to a value. This class allows for a
    program to utilize the TreeStatement node type in "AST.h" that dramatically extends
    the abilities of the New_Parser
    */

                            StateParser(){};
                            ~StateParser(){                             // Incase evaluation is interupted
                                while(!isEmpty()){
                                    TreeNode* currTree;
                                    currTree = popHead();
                                    delete currTree;
                                }
                            }
    void                    createForest(std::deque<Token> oInput);     // Creates a deque of statements as trees
    TreeNode*               popHead();                                  // Returns and pops first tree in forest
    bool                    isEmpty() const;                            // Checks if forest is empty

private:
    std::deque<TreeNode*>   mHeads;                                     // Stores statements as trees
    bool                    isExp(Token& token) const;                  // Checks if next character indicates the beginning of an expression
    TreeNode*               createStatement(std::deque<Token>& input);  // Creates single expression as a tree
    std::vector<TreeNode*>  createBlock(std::deque<Token>& input);      // Makes forest of statements between "{}"
    TreeNode*               createDef(std::deque<Token>& input);        // Parses and creates a "def" statement (a function definition)
    TreeNode*               createIf(std::deque<Token>& input);         // Parses and creates an "if" statement (including "if-else")
    TreeNode*               createWhile(std::deque<Token>& input);      // Parses and creates a "while" statement
    TreeNode*               createPrint(std::deque<Token>& input);      // Parses and creates a "print" statement
    TreeNode*               createReturn(std::deque<Token>& input);     // Parses and creates a "return" statement
    TreeNode*               createExp(std::deque<Token>& input);        // Parses and creates an empty "expression" statement and stores an expression in it
};


#endif
