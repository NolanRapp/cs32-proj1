#include "lib/lex.h"

void createend(std::vector<token>& inputvec){
    //TODO: make END token
    token end;
    //end.line = line; 
    //end.column = column; 
    end.text = "END";
}
void printtokens(std::vector<token>& inputvec){
//TODO print token line, column, value
}
token maketoken(std::string text, int line, int column){
  token t;
  t.line = line;
  t.column = column;
  t.text = text;
}

std::vector<token> lex() {
    char i;
    std::vector<token> output;
    int line = 1; // TODO: iterate this at every new line
    int column = 1; // TODO: iterate this for each new char
    std::string placeholder = "/0";
    std::set<char> valid {
        ')',
        '(',
        '+',
        '-',
        '*',
        '/'
    };
    while (std::cin >> i){
        if (!isspace(i)){

            //if valid non-number input:
            if (valid.find(i)!=valid.end()){
                placeholder = i;
                output.push_back(maketoken(placeholder,line,column));
            }

            //if number input check validity:
            else if(isdigit(i)){
                placeholder = "/0";
                bool decimal = false;
                placeholder += i;
                while (std::cin >> i && (isdigit(i) || i == '.')){
                    if (i == '.') {
                        if (decimal){
                            //print error
                            std::cout << "Syntax error on line " << line << "column " << column << ".";
                            return 1; //apperently you return a 1 but its not a queue so idk
                        }
                        else {
                            decimal = true;
                        }
                    }
                    placeholder += i;
                }
                output.push_back(maketoken(placeholder,line,column));
            }
            //not valid!
            else {
                std::cout << "Syntax error on line " << line << "column " << column << ".";
                return 1; //apperently you return a 1 but its not a queue so idk
            }
        }
    }

    createend(output);
    printtokens(output);
}
