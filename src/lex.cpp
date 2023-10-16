#include "lib/lex.h"

void createend(std::queue<token>& inputq){
    //TODO: make END token
    token end;
    //end.line = line; 
    //end.column = column; 
    end.text = "END";
}
void printtokens(std::queue<token>& inputq){
//TODO print token line, column, value
}
token maketoken(std::string text, int line, int column){
  token t;
  t.line = line;
  t.column = column;
  t.text = text;
}

std::queue<token> lex() {
    char i;
    std::queue<token> output;
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
                output.push(maketoken(placeholder,line,column));
            }

            //if number input check validity:
            else if(isdigit(i)){
                placeholder = "/0";
                bool decimal = false;
                placeholder += i;
                column++;
                while (std::cin >> i && (isdigit(i) || i == '.')){
                    if (i == '.') {
                        if (decimal){
                            //print error
                            std::cout << "Syntax error on line " << line << "column " << column << ".";
                            return; //TODO: throw a standard library error here and then in main we catch it by returning 1
                        }
                        else {
                            decimal = true;
                        }
                    }
                    placeholder += i;
                    column++;
                }

                output.push(maketoken(placeholder,line,column));

                if (valid.find(i)!=valid.end()){
                    placeholder = i;
                    output.push(maketoken(placeholder,line,column));
                }
                else if (isspace(i)){
                    break;
                }
            }
            //not valid!
            else {
                std::cout << "Syntax error on line " << line << "column " << column << ".";
                return; //TODO: throw a standard library error here and then in main we catch it by returning 1
            }
        }

        if (i == '/n') {
            line++;
            column = 0;
        }
        column++;
    }

    createend(output);
    printtokens(output);
}
