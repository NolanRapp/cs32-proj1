#include "lib/Lexer.h"
#include "lib/token.h"


int main() {

    Lexer lexer;
    lexer.lex();
    lexer.printTokens();

    return 0;
}
