# cs32


#### Authors:
> Jesse Cobb, Chloe Andersen, Nolan Rapp, Christy Yu

### Programs:
Using a `[Program]`:
- Please run all program commands in `src/` and replace `[Program]` with one of the program names.
- Compiling: `cd src/ && [Program]`
- Running: `./[Program]`

`Scrypt`:
> This program takes in user input in the form of a program using "if", "while", and "print" statements along with infix expressions then evaluates the program.

`Format`
> This program takes in user input in the form of a program using "if", "while", and "print" statements along with infix expressions then prints the program in standard format.

`New_Parser`
> This program takes user input line-by-line and outputs the expression and evaluates it. Errors will not interrupt the program but will ask for new input.

`Parser` [OUTDATED]
> This program no longer compiles but used to take all input and parses a single S-expression.

`Lexer`
> This program reads input and saves all valid inputs as Token objects used by parsers. Will exit if there is an invalid input.

### Files:
    cs32-scrypt
    ├── README.md   ------------- Project Documentation
    ├── .gitignore
    └── src         ------------- All main source files
        ├── Makefile
        ├── calc.cpp ------------ Parses expression by line
        ├── format.cpp ---------- Parses program and prints
        ├── lex.cpp ------------- Outputs tokens from input
        ├── parse.cpp ----------- [OUTDATED] Parses S-expression using input
        ├── scrypt.cpp ---------- Parses program and evaluates
        └── lib     ------------- Library files used by main files
            ├── AST.cpp
            ├── AST.h ----------- Class for nodes of ASTs (includes statements)
            ├── Errors.h -------- Custom exceptions for intuitive errors
            ├── Lexer.cpp
            ├── Lexer.h --------- Class for converting input to Tokens
            ├── New_Parser.cpp
            ├── New_Parser.h ---- Parses infix expression
            ├── Parser.cpp
            ├── Parser.h -------- [OUTDATED] Parses S-expressions
            ├── StateParser.cpp
            ├── StateParser.h --- Parses full programs with statements
            └── Token.h --------- Fundamental object of Lexer and used for parsing

## Timeline
Calculator (Checkpoint 1)
> The Lexer (Track A)
>
>> Test
>
> The Parser (Track B)

Checkpoint 2


Checkpoint 3








