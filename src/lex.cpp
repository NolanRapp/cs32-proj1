#include "lib/lex.h"

void createend(std::vector<token>& inputvec){

}
void printtokens(std::vector<token>& inputvec){

}
token maketoken(std::string i, int line, int collumn){

}

std::vector<token> lex() {
    char i;
    std::vector<token> output;
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

            if (valid.find(i)!=valid.end()){
                placeholder = i;
                output.push_back(maketoken(placeholder,line,column));
            }

            else if(isdigit(i)){
                placeholder = "/0";
                bool decimal = false;
                placeholder += i;
                while (std::cin >> i && (isdigit(i) || i == '.')){
                    if (i == '.') {
                        if (decimal){
                            //error
                        }
                        else {
                            decimal = true;
                        }
                    }
                    placeholder += i;
                }
                output.push_back(maketoken(placeholder,line,column));
            }

        }
    }

    createend(output);
    printtokens(output);
}