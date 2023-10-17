#include "lib/Lexer.h"
#include "lib/Token.h"


int main() {

    Lexer lexer;
    lexer.lex();
    lexer.printTokens();

    return 0;
}
