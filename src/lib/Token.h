#ifndef TOKEN_H
#define TOKEN_H

#include <string>


// Type system for New_Parser
enum class Type {
    BOOL,
    NUM,
    OP,
    ID,
    END,
    STATE,
    ARRAY,
    MISC
};



// this is the token struct containing information about one token
struct Token {
    int         line;   // the line number of the first character of the token
    int         column; // the column number of the first character of the token
    std::string text;   // the text of the token exactly as it appears in the input
    Type        type;   // the type of the token

    Token(int l, int c, const std::string &t, Type type) : line(l), column(c), text(t), type(type) {} // token constructor for testing, type default is num
};


#endif
