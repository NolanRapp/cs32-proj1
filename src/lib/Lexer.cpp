#include "Lexer.h"


// Creates END type token at end of input
void Lexer::createEnd(std::deque<Token>& inputq, int line, int column, Type t) {
    inputq.push_back(Token(line, column, "END", t));
}



// Prints tokens for debugging input
void Lexer::printTokens() {
    while (!lexTokens.empty()) {
        Token printToken = lexTokens.front();
        std::cout << std::setfill(' ')  << std::setw(4) << std::right << printToken.line << std::setfill(' ')
        << std::setw(5) << std::right << printToken.column << "  " << printToken.text << std::endl; 
        
        lexTokens.pop_front();
    }
}



// Retrieves the deque made from the input's tokens
std::deque<Token> Lexer::getLexQueue() {
    return lexTokens;
}



// Reads a string and lexes input into tokens stored in deque
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
        '%',
        '^',
        '|',
        '&'
    };

    while (stream.get(i)) { 

        if (i == '\n') {
            line++;
            column = 1;
            continue;
        }

        if (!isspace(i)) {

            // handling two character operators like "<=", ">=", "==", "!="
            if (i == '<') {
                if(stream.peek() == '='){
                    lexTokens.push_back(Token(line, column, "<=", Type::OP));
                    stream.get(i);
                    column++;
                }
                else{
                    lexTokens.push_back(Token(line, column, "<", Type::OP));
                }
            }
            else if (i == '>') {
                if(stream.peek() == '='){
                    lexTokens.push_back(Token(line, column, ">=", Type::OP));
                    stream.get(i);
                    column++;
                }
                else{
                    lexTokens.push_back(Token(line, column, ">", Type::OP));
                }
            }
            else if (i == '=') {
                if(stream.peek() == '='){
                    lexTokens.push_back(Token(line, column, "==", Type::OP));
                    stream.get(i);
                    column++;
                }
                else{
                    lexTokens.push_back(Token(line, column, "=", Type::OP));
                }
            }

            else if (i == '!' && stream.peek() == '=') {
                lexTokens.push_back(Token(line, column, "!=", Type::OP));
                stream.get(i);
                column++;
            }

            //if valid operator/parenthesis input, add to output deque:
            else if (valid.find(i) != valid.end()) {
                lexTokens.push_back(Token(line,column, std::string(1, i), Type::OP));
            }

            //if is an identifier (starts with alphanum or _)
            else if (isalpha(i) || i == '_') {
                std::string placeholder(1, i); // this converts a char (1) to a string
                int startingColumn = column;
                while((isalnum(stream.peek()) || stream.peek() == '_')){
                    char nextChar;
                    stream.get(nextChar);
                    placeholder += nextChar;
                    column++;
                }

                // checking if identifier is a boolean
                if (placeholder == "true" || placeholder == "false") {
                    lexTokens.push_back(Token(line, startingColumn, placeholder, Type::BOOL));
                }
                else if (placeholder == "if" || placeholder == "else" || placeholder == "while" || placeholder == "print"
                      || placeholder == "return" || placeholder == "def") {
                    lexTokens.push_back(Token(line, startingColumn, placeholder, Type::STATE));
                }
                else if (placeholder == "null"){
                    lexTokens.push_back(Token(line, startingColumn, placeholder, Type::MISC));
                }
                else {
                    lexTokens.push_back(Token(line, startingColumn, placeholder, Type::ID));
                }

            }
            

            //if number, check validity:
            else if (isdigit(i)) {
                bool decimal = false; 
                std::string placeholder(1, i); // this converts a char (1) to a string
                int startingColumn = column;

                // if number is a decimal:
                while ((isdigit(stream.peek()) || stream.peek() == '.')) {
                    if (stream.peek() == '.') {
                        // checking for multiple decimals
                        if (decimal) {
                            throw LexError(line, column + 1);
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
                }

                lexTokens.push_back(Token(line,startingColumn, placeholder, Type::NUM));
            }

            // store brackets as Type::END tokens for uniqueness
            else if (i == '{' || i == '}' || i == ',' || i == ';'){
                std::string c = "";
                c += i;
                lexTokens.push_back(Token(line, column, c, Type::MISC));
            }

            else if (i == '[' || i == ']'){
                std::string c = "";
                c += i;
                lexTokens.push_back(Token(line, column, c, Type::ARRAY));
            }


            // if not space, valid operator, or valid number, print error:
            else { 
                throw LexError(line, column);
            }
        }
        column++;
    }
    createEnd(lexTokens, line, column, Type::END);
}

