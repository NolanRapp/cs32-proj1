# cs32


#### Authors:
> Jesse Cobb, Chloe Andersen, Nolan Rapp, Christy Yu

### Programs:
Using a `[Program]`:
- Please run all program commands in `src/` and replace `[Program]` with one of the program names.
- Compiling: `cd src/ && [Program]`
- Running: `./[Program]`
- Valid Tokens:
    - Arithmatic Operations: `*`, `/`, `%`, `+`, `-`, `(`, `)`, `=`
    - Booleans Operations: `<`, `<=`, `>`, `>=`, `==`, `!=`, `&`, `^`, `|`
    - Statements: `if`, `else`, `while`, `print`, `def`
    - Misc.: `null`, `{`, `}`, `,`, `;`, `[`, `]`
    - Numbers, Identifiers (Any non-resevred string of letters and underscore), `true`, `false`


`Scrypt`:
> This program takes in user input in the form of a program using statement tokens along with infix expressions then evaluates the program.

`Format`
> This program takes in user input in the form of a program using statement tokens along with infix expressions then prints the program in standard format.

`New_Parser`
> This program takes user input line-by-line and outputs the expression and evaluates it. Errors will not interrupt the program but will ask for new input.
> Doesn't accept Statement Tokens

`Parser` [OUTDATED]
> Takes all input and parses a single S-expression.
> No longer compiles due to changes in ASTs [Refer to New_Parser]

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
>> This makes use of the `Lexer` which takes in user input and allows for standard S-expression notation: `(`, `)`, `+`, `-`, `/`, `*`, and numbers. This program outputs the build queue of Tokens.
>
> The Parser (Track B)
>
>> This makes use of the `Parser` which takes the queue built by the lexer and attempts to parse a single S-expression. Has error handling for parse errors as well as evaluation errors such as dividing by 0.

Persistence (Checkpoint 2)
> Variables (Track A)
>
>> This uses both `Lexer` and `Parser` after altering the valid input of the user. The user can now submit strings of letters and underscores as well as the assignment operator `=`. This is used for assigning double values to these variable. Multiple expressions are now supported.
>
> Infix Notation (Track B)
>
>> This uses `New_Parser` which takes user input and parses a single infix expression per line. No errors interrupt the program but will request a new input. This also supports assignment of variables. This parser was created using Recursive-Descent Parser logic.

Conditionals (Checkpoint 3)
> Booleans (Track A)
>
>> This uses `New_Parser` with many new operations and tokens added: `%`, `<`, `>`, `<=`, `>=`, `==`, `!=`, `&`, `^`, `|`, `true`, and `false`. This requires the implementation of booleans as well as boolean assigning.
>
> Statements (Track B)
>
>> This uses `Scrypt` and `Format` and allows for `if`, `else`, `while`, `print`, `{`, and `}` to allow for an entire program to be written using fundamental statements. It can output the forest of ASTs built in standard formatting and can also evaluate this forest.

Completion (Checkpoint 4)
>
> Support (Track A)
>
>> This uses `Scrypt` and `Format`. Added support for new tokens such as `;` and `,` as well as required most statements to end with `;`. Changed errors and the behaviour of `==` and `!=` to be able to evaluate two children of different types.
>
> Functions (Track B)
>
>> This uses `Scrypt` amd `Format` and implements function definitions and function call. Variables can now store functions, numbers, bools, and null types. The null type was implemented a unique type. Functions use scope based on parameters of function calls and the scope of when the function was defined. Also implemented `shared_ptr`s in order to allow functions to move forests of statement trees around without memory problems.
>
> Arrays (Track C)
>
>> This uses `Scrypt` and `New_Parser` and implements array literals `[expression(s)]`, array lookups `expression[expression(s)]`, and array assignments `[expression(s)][index]=[expression(s)]` or `expression[index] = expression(s)`. Arrays can now be utilized throughout the program with all the nuances of the previous checkpoints. In addition, len(), pop(), and push() array utility functions were implemented as specialty array pre-defined functions.





