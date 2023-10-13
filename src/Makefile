CXXFLAGS = -Wall -Wextra -std=c++17 -Wno-unused-parameter

# Won't make new files
.PHONY: clean

# # https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
# #  $@ is the name of the target
# #  $+ is a list of all the dependencies
# #  $< is the first dependency


output: parse.o lex.o
	g++ $(CXXFLAGS) parse.o lex.o -o output

parse.o: parse.cpp lib/parse.h
	g++ $(CXXFLAGS) -c parse.cpp

lex.o: lex.cpp lib/lex.h
	g++ $(CXXFLAGS) -c lex.cpp

clean:
	rm *.o output
