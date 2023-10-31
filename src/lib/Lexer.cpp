#include "Lexer.h"


// Creates "END" token, prevents the queue from being empty
void Lexer::createEnd(std::deque<Token>& inputq, int line, int column) {
    inputq.push_back(Token(line, column, "END"));
}



// Prints the queue, for debugging purposes
void Lexer::printTokens() {
    while (!lexTokens.empty()) {
        Token printToken = lexTokens.front();
        std::cout << std::setfill(' ')  << std::setw(4) << std::right << printToken.line << std::setfill(' ')
        << std::setw(5) << std::right << printToken.column << "  " << printToken.text << std::endl;
        // using example from  https://cplusplus.com/reference/iomanip/setw/ 
        
        lexTokens.pop_front();
    }
}



// Used to retrieve deque for classes using the Tokens
std::deque<Token> Lexer::getLexQueue() {
    return lexTokens;
}



// Constructs the deque based on standard output, stores it in member variable
void Lexer::lex(std::string& inputString) {
	std::istringstream stream(inputString);
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
        '/',
        '='
    };

    while (stream.get(i)) { 
        //std::stream.get(i) reads input, whitespace, and newlines
        // https://cplusplus.com/reference/istream/istream/get/

        if (i == '\n') {
            line++;
            column = 1;
            continue;
        }

        if (!isspace(i)) {

            //if valid operator/parenthesis input, add to output deque:
            if (valid.find(i) != valid.end()) {
                lexTokens.push_back(Token(line,column, std::string(1, i)));
            }

            //if is an identifier (starts with alphanum or _)
            else if (isalpha(i) || i == '_'){
                std::string placeholder(1, i); // this converts a char (1) to a string
                int startingColumn = column;
                while((isalnum(stream.peek()) || stream.peek() == '_')){
                    char nextChar;
                    stream.get(nextChar);
                    placeholder += nextChar;
                    column++;
                }
                lexTokens.push_back(Token(line,startingColumn, placeholder));
            }
            

            //if number, check validity:
            else if (isdigit(i)) {
                bool decimal = false; 
                std::string placeholder(1, i); // this converts a char (1) to a string
                int startingColumn = column;

                // if number is a decimal:
                while ((isdigit(stream.peek()) || stream.peek() == '.')) {
                    // peek returns next character in the input sequence, without extracting it
                    // https://cplusplus.com/reference/istream/istream/peek/

                    if (stream.peek() == '.') {
                        // checking for multiple decimals
                        if (decimal) {
							throw LexError(line, column + 1);
                            //  std::cout << "Syntax error on line " << line << " column " << column + 1 << ".\n";
                            //  exit(1);
                        }
                        else {
							decimal = true;
                        }
                    }
                    char nextChar;
                    stream.get(nextChar);
                    placeholder += nextChar;
                    column++;
                }

                // checking for trailing decimal
                if (placeholder.back() == '.') {
					throw LexError(line, column + 1);
                    //  std::cout << "Syntax error on line " << line << " column " << column + 1 << ".\n";
                    //  exit(1);
                }

                lexTokens.push_back(Token(line,startingColumn, placeholder));
            }
            // if not space, valid operator, or valid number, print error:
            else {
				throw LexError(line, column);
                //  std::cout << "Syntax error on line " << line << " column " << column << ".\n";
                //  exit(1);
            }
        }
        // if input is space, add column:
        column++;
    } 
    createEnd(lexTokens, line, column);
}

