#include "lib/lex.h"
#include "lib/token.h"


void Lexer::createEnd(std::queue<token>& inputq, int line, int column) {
    inputq.push(token(line, column, "END"));
}



void Lexer::printTokens() {
    while (!lexTokens.empty()) {
        token printToken = lexTokens.front();
        std::cout << std::setw(4) << printToken.line << std::setw(5) << 
        printToken.column << std::setw(5) << printToken.text << std::endl;
        // using example from  https://cplusplus.com/reference/iomanip/setw/ 
        
        lexTokens.pop();
    }
}


std::queue<token> Lexer::getLexQueue() {
    return lexTokens;
}



token Lexer::makeToken(int line, int column, std::string text) {
  return token(line, column, text);
}



void Lexer::lex() {
    int line = 1;
    int column = 1;
    char i;
    std::string placeholder;

    std::set<char> valid {
        ')',
        '(',
        '+',
        '-',
        '*',
        '/'
    };

    while (std::cin.get(i)) { 
        //std::cin.get(i) reads input, whitespace, and newlines
        // https://cplusplus.com/reference/istream/istream/get/

        if (i == '\n') {
            line++;
            column = 0;
        }

        if (!isspace(i)) {

            //if valid operator/parenthesis input, add to output queue:
            if (valid.find(i) != valid.end()) {
                lexTokens.push(makeToken(line,column, std::string(1, i)));
            }

            //if number input, check validity:
            else if (isdigit(i)) {
                bool decimal = false;
                std::string placeholder(1, i); // this converts a char (1) to a string

                // if number is a decimal:
                while ((isdigit(std::cin.peek()) || std::cin.peek() == '.')) {
                    // peek returns next character in the input sequence, without extracting it
                    // https://cplusplus.com/reference/istream/istream/peek/

                    if (std::cin.peek() == '.') {
                        if (decimal) {
                            //print error
                            std::cout << "Syntax error on line " << line << " column " << column << ".";
                            exit(1);
                        }
                        else {
                            decimal = true;
                        }
                    }
                    char nextChar;
                    std::cin.get(nextChar);
                    placeholder += nextChar;

                    column++;
                }
                lexTokens.push(makeToken(line,column, placeholder));
            }

            // if not space, valid operator, or valid number, print error:
            else {
                std::cout << "Syntax error on line " << line << " column " << column << ".";
                exit(1);
            }
        }
        // if input is space, add column:
        column++;
    } 
    createEnd(lexTokens, line, column);
}



int main() {
    try {
        Lexer lexer;
        lexer.lex();
        lexer.printTokens();
    }

    catch(const std::exception& e) {
        return 1;
    }

    return 0;
}
