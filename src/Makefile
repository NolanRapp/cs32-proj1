output: parse.o lex.o
	g++ -Wall -Wextra -std=c++17 parse.o lex.o -o output

parse.o: parse.cpp lib/parse.h
	g++ -Wall -Wextra -std=c++17 -c parse.cpp

lex.o: lex.cpp lib/lex.h
	g++ -Wall -Wextra -std=c++17 -c lex.cpp

clean:
	rm *.o output