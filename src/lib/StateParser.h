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
                            ~StateParser(){};
    void                    createForest(std::deque<Token> oInput);
    TreeNode*               popHead();
    bool                    isEmpty() const;

private:
    std::deque<TreeNode*>   mHeads;
    bool                    isExp(Token& token) const;
    TreeNode*               createStatement(std::deque<Token>& input);
    std::vector<TreeNode*>  createBlock(std::deque<Token>& input);
    TreeNode*               createDef(std::deque<Token>& input);
    TreeNode*               createIf(std::deque<Token>& input);
    TreeNode*               createWhile(std::deque<Token>& input);
    TreeNode*               createPrint(std::deque<Token>& input);
    TreeNode*               createReturn(std::deque<Token>& input);
    TreeNode*               createExp(std::deque<Token>& input);

};


#endif
