CXXFLAGS = -Wall -Wextra -std=c++17 -Wno-unused-parameter

# Won't make new files
.PHONY: clean

# # https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
# #  $@ is the name of the target
# #  $+ is a list of all the dependencies
# #  $< is the first dependency



Scrypt: scrypt.cpp StateParser.o New_Parser.o AST.o Lexer.o
	g++ $(CXXFLAGS) $+ -o $@

Format: format.cpp StateParser.o New_Parser.o AST.o Lexer.o
	g++ $(CXXFLAGS) $+ -o $@

New_Parser: calc.cpp New_Parser.o AST.o Lexer.o
	g++ $(CXXFLAGS) $+ -o $@

Parser: parse.cpp Parser.o AST.o Lexer.o
	g++ $(CXXFLAGS) $+ -o $@

Lexer: lex.cpp Lexer.o
	g++ $(CXXFLAGS) $+ -o $@



StateParser.o: lib/StateParser.cpp lib/StateParser.h lib/Token.h	
	g++ $(CXXFLAGS) -c $<

New_Parser.o: lib/New_Parser.cpp lib/New_Parser.h lib/Token.h
	g++ $(CXXFLAGS) -c $<

Parser.o: lib/Parser.cpp lib/Parser.h lib/Token.h
	g++ $(CXXFLAGS) -c $<

AST.o: lib/AST.cpp lib/AST.h
	g++ $(CXXFLAGS) -c $<

Lexer.o: lib/Lexer.cpp lib/Lexer.h lib/Token.h
	g++ $(CXXFLAGS) -c $<



clean:
	rm *.o lib/*.gch Parser Lexer New_Parser Format Scrypt
