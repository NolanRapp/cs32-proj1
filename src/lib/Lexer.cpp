#include "Lexer.h"


void Lexer::createEnd(std::queue<Token>& inputq, int line, int column) {
    inputq.push(Token(line, column, "END"));
}



void Lexer::printTokens() {
    while (!lexTokens.empty()) {
        Token printToken = lexTokens.front();
        std::cout << std::setfill(' ')  << std::setw(4) << std::right << printToken.line << std::setfill(' ')
        << std::setw(5) << std::right << printToken.column << "  " << printToken.text << std::endl;
        // using example from  https://cplusplus.com/reference/iomanip/setw/ 
        
        lexTokens.pop();
    }
}



std::queue<Token> Lexer::getLexQueue() {
    return lexTokens;
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
            column = 1;
            continue;
        }

        if (!isspace(i)) {

            //if valid operator/parenthesis input, add to output queue:
            if (valid.find(i) != valid.end()) {
                lexTokens.push(Token(line,column, std::string(1, i)));
            }

            //if number, check validity:
            else if (isdigit(i)) {
                bool decimal = false; 
                std::string placeholder(1, i); // this converts a char (1) to a string
                int startingColumn = column;

                // if number is a decimal:
                while ((isdigit(std::cin.peek()) || std::cin.peek() == '.')) {
                    // peek returns next character in the input sequence, without extracting it
                    // https://cplusplus.com/reference/istream/istream/peek/

                    if (std::cin.peek() == '.') {
                        // checking for multiple decimals
                        if (decimal) {
                            std::cout << "Syntax error on line " << line << " column " << column + 1 << ".\n";
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

                // checking for trailing decimal
                if (placeholder.back() == '.') {
                    std::cout << "Syntax error on line " << line << " column " << column + 1 << ".\n";
                    exit(1);
                }

                lexTokens.push(Token(line,startingColumn, placeholder));
            }
            // if not space, valid operator, or valid number, print error:
            else {
                std::cout << "Syntax error on line " << line << " column " << column << ".\n";
                exit(1);
            }
        }
        // if input is space, add column:
        column++;
    } 
    createEnd(lexTokens, line, column);
}


