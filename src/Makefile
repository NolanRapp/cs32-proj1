CXXFLAGS = -Wall -Wextra -std=c++17 -Wno-unused-parameter

# Won't make new files
.PHONY: clean

# # https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
# #  $@ is the name of the target
# #  $+ is a list of all the dependencies
# #  $< is the first dependency


Parser: parse.cpp Parser.o Lexer.o
	g++ $(CXXFLAGS) $+ -o $@

Lexer: lex.cpp Lexer.o
	g++ $(CXXFLAGS) $+ -o $@

Parser.o: lib/Parser.h lib/Parser.cpp
	g++ $(CXXFLAGS) -c $+

Lexer.o: lib/Lexer.h lib/Lexer.cpp
	g++ $(CXXFLAGS) -c $+

clean:
	rm *.o lib/*.gch Parser Lexer
