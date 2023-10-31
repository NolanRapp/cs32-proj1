#ifndef ERRORS_H
#define ERRORS_H

#include <stdexcept>
#include <string>


// Used for invalid characters
class LexError: public std::runtime_error {
	int mLine;
	int mColumn;
public:
	LexError(int line, int col): std::runtime_error("Syntax error on line " + std::to_string(line) + " column " + std::to_string(col) + ".")  {
		mLine = line;
		mColumn = col;
	}
};

// Used for errors when parsing input
class ParseError: public std::runtime_error {
	int mLine;
	int mColumn;
	std::string mToken;
public:
	ParseError(int line, int col, std::string tok): std::runtime_error("Unexpected token at line " + std::to_string(line) + " column " + std::to_string(col) + ": " + tok) {
		mLine = line;
		mColumn = col;
		mToken = tok;
	}
};

#endif
