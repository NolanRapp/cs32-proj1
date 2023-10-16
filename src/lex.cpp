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
    int line = 1;
    int column = 1;
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
                //concat the string of numbers
                while (std::cin >> i && (isdigit(i) || i == '.')){
                    if (i == '.') {
                        if (decimal){
                            //print error
                            std::cout << "Syntax error on line " << line << "column " << column << ".";
                            throw std::runtime_error("Syntax error");
                        }
                        else {
                            decimal = true;
                        }
                    }
                    placeholder += i;
                    column++;
                }
                output.push(maketoken(placeholder,line,column));
                //if next token valid make another token
                if (valid.find(i)!=valid.end()){
                    placeholder = i;
                    output.push(maketoken(placeholder,line,column));
                }
                //if space break
                else if (isspace(i)){
                    column++;
                    break;
                }
            }
            //not valid!
            else {
                std::cout << "Syntax error on line " << line << "column " << column << ".";
                throw std::runtime_error("Syntax error");
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

int main(){
    try{

    }
    catch(const std::runtime_error& e){
        return 1;
    }
    return 0;
}
