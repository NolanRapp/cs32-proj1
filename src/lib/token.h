#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// this is the token struct containing information about one token

struct token {
    int line; // the line number of the first character of the token
    int column; // the column number of the first character of the token
    std::string text; // the text of the token exactly as it appears in the input

    token(int l, int c, const std::string &t) : line(l), column(c), text(t) {} // token constructor for testing
};

#endif